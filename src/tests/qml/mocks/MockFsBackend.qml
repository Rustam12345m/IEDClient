import QtQuick

// Filesystem page mock surface. Step 3 / screen 10 — Filesystem view is the
// first screen that consumes fsBackend, so this file was promoted from the
// empty stub used by screens 1–9.
QtObject {
    // --- Filesystem page surface ---
    // Page.qml binds `TableView.model: fsBackend.sortModel`. Production
    // returns QAbstractItemModel*; QML's TableView accepts any QObject and
    // just renders zero rows. Expose a nested QtObject so member access
    // (index/data, used by getFilename/getFileSize) doesn't blow up if the
    // page ever dispatches them. Do NOT use a JS array — we need
    // `model.index(...)` / `model.data(...)` to be callable.
    property QtObject nextSortModel: QtObject {
        property int rows: 0
        property int columns: 5
        property int indexCallCount: 0
        property var nextDisplay: ""
        property var nextSortValue: 0
        function index(row, col) { return { row: row, column: col } }
        // Page.qml passes role string ("display" / "sort_value") as 2nd arg.
        function data(idx, role) {
            if (role === "sort_value") return nextSortValue
            return nextDisplay
        }
        function rowCount() { return rows }
        function columnCount() { return columns }
    }
    property QtObject sortModel: nextSortModel

    // updateFilesDirectory(path) — called from main.qml on connect / nav.
    // Page.qml itself does not invoke it, but the production C++ surface
    // exposes it; included so the mock matches the documented contract.
    property int updateFilesDirectoryCallCount: 0
    property string lastUpdateFilesDirectoryPath: ""
    function updateFilesDirectory(path) {
        updateFilesDirectoryCallCount++
        lastUpdateFilesDirectoryPath = path
    }

    property int downloadFileCallCount: 0
    property string lastDownloadFilename: ""
    property var lastDownloadFileSize: 0
    function downloadFile(filename, fileSize) {
        downloadFileCallCount++
        lastDownloadFilename = filename
        lastDownloadFileSize = fileSize
    }

    property int removeFileCallCount: 0
    property string lastRemoveFilename: ""
    property int lastRemoveRow: -1
    function removeFile(filename, row) {
        removeFileCallCount++
        lastRemoveFilename = filename
        lastRemoveRow = row
    }

    // fsPageStatus() lives on the C++ surface but is invoked from
    // ui/main.qml, not from the Filesystem Page itself. Stubbed for parity
    // with the contract doc.
    property int fsPageStatusCallCount: 0
    property string nextFsPageStatus: ""
    function fsPageStatus() {
        fsPageStatusCallCount++
        return nextFsPageStatus
    }

    // --- Signals consumed by Page.qml's Connections { target: fsBackend } ---
    // sigDownloadProgress lives on IED_FS_Backend; sigCmdFinished is
    // inherited from BackendInterface. Both are wired by Connections in
    // ui/filesystem/Page.qml — so we expose both off the same mock object.
    signal sigDownloadProgress(int perc)
    signal sigCmdFinished(bool done)
    // sigCmdProgress / sigCmdError are wired by main.qml, not Page.qml.
    // Listed for parity with BackendInterface but not exercised by tests.
    signal sigCmdProgress(int perc, string msg)
    signal sigCmdError(string msg)
}
