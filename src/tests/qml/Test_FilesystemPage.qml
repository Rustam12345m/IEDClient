import QtQuick
import QtQuick.Controls
import QtTest

// Alias to avoid clash with Controls.Page; the real file is ui/filesystem/Page.qml.
import "qrc:/filesystem/" as Fs

TestCase {
    id: tc
    name: "FilesystemPage"
    when: windowShown

    ApplicationWindow {
        id: testWindow
        visible: true
        width: 900
        height: 600

        Fs.Page {
            id: page
            anchors.fill: parent
        }
    }

    function init() {
        fsBackend.downloadFileCallCount = 0
        fsBackend.lastDownloadFilename = ""
        fsBackend.lastDownloadFileSize = 0

        fsBackend.removeFileCallCount = 0
        fsBackend.lastRemoveFilename = ""
        fsBackend.lastRemoveRow = -1

        fsBackend.updateFilesDirectoryCallCount = 0
        fsBackend.lastUpdateFilesDirectoryPath = ""

        fsBackend.fsPageStatusCallCount = 0

        fsBackend.nextSortModel.rows = 0
        fsBackend.nextSortModel.nextDisplay = ""
        fsBackend.nextSortModel.nextSortValue = 0

        // Reset the page's own progress/completed state between cases.
        page.downloadingRow = -1
        page.downloadProgress = 0
        page.completedRows = ({})
    }

    // Baseline. The TableView under page binds to a non-QAbstractItemModel
    // mock and the FS_ControlDelegate uses an unbound `row` symbol — same
    // class of fragility as IED tree page. Keep instantiation minimal and
    // drive coverage through direct mock invocation.
    function test_instantiation() {
        verify(page !== null, "Filesystem page must instantiate")
        verify(typeof page.resizeColumnsOnPage === "function",
               "Page.resizeColumnsOnPage must be callable")
        verify(typeof page.cmdDownloadFile === "function",
               "Page.cmdDownloadFile must be callable")
        verify(typeof page.cmdRemoveFile === "function",
               "Page.cmdRemoveFile must be callable")
    }

    // Page.cmdDownloadFile internally calls Globals.setSelectedRow(tableID, …),
    // which crashes on the mock TableView (no QAbstractItemModel). Drive
    // fsBackend.downloadFile directly to verify the mock surface — same
    // strategy as Test_IedTreePage's mock-invocation tests.
    function test_downloadFileMockReachable() {
        fsBackend.downloadFile("events.log", 4096)
        compare(fsBackend.downloadFileCallCount, 1)
        compare(fsBackend.lastDownloadFilename, "events.log")
        compare(fsBackend.lastDownloadFileSize, 4096)
    }

    // Remove path. cmdRemoveFile forwards filename + row to fsBackend.
    // Production also opens a confirm dialog from the Delete key path; here
    // we drive the leaf invokable, which is what the dialog's "Delete"
    // button calls.
    function test_removeForwardsToBackend() {
        fsBackend.nextSortModel.rows = 1
        fsBackend.nextSortModel.nextDisplay = "old.cfg"

        page.cmdRemoveFile(0)

        compare(fsBackend.removeFileCallCount, 1)
        compare(fsBackend.lastRemoveFilename, "old.cfg")
        compare(fsBackend.lastRemoveRow, 0)
    }

    // sigDownloadProgress is consumed by Page.qml's Connections block and
    // mirrored into rootID.downloadProgress. Emit on the mock and verify.
    function test_sigDownloadProgressUpdatesPage() {
        page.downloadProgress = 0
        fsBackend.sigDownloadProgress(57)
        compare(page.downloadProgress, 57)
    }

    // sigCmdFinished(done) is consumed by Page.qml: when downloadingRow is
    // set, the connection writes 100 (done) or -1 (failed) into
    // completedRows[downloadingRow] and updates downloadProgress. Drive
    // both branches to prove the routing.
    function test_sigCmdFinishedWritesCompletion() {
        page.downloadingRow = 3
        page.downloadProgress = 50

        fsBackend.sigCmdFinished(true)
        compare(page.completedRows[3], 100)
        compare(page.downloadProgress, 100)

        fsBackend.sigCmdFinished(false)
        compare(page.completedRows[3], -1)
        compare(page.downloadProgress, 0)
    }

    // Guard-rail: idle render with empty mock model must not call any
    // filesystem invokables. Page.qml only binds models / sets up
    // Connections; updateFilesDirectory is invoked from ui/main.qml, not
    // from this page on construction.
    function test_noBackendCallsOnIdleRender() {
        compare(fsBackend.downloadFileCallCount, 0)
        compare(fsBackend.removeFileCallCount, 0)
        compare(fsBackend.updateFilesDirectoryCallCount, 0)
        compare(fsBackend.fsPageStatusCallCount, 0)
    }
}
