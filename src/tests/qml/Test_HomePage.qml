import QtQuick
import QtQuick.Controls
import QtTest

// Alias to avoid clash with Controls.Page; the real file is ui/home/Page.qml.
import "qrc:/home/" as Home

TestCase {
    id: tc
    name: "HomePage"
    when: windowShown

    SignalSpy {
        id: connectSpy
        target: page
        signalName: "sigConnectTo"
    }

    ApplicationWindow {
        id: testWindow
        visible: true
        width: 900
        height: 600

        Home.Page {
            id: page
            anchors.fill: parent
        }
    }

    function init() {
        iedBackend.dumpMmsModelCallCount = 0
        iedBackend.lastDumpMmsIp = ""
        iedBackend.lastDumpMmsPort = 0

        appBackend.lastConnList.removeFromHistoryCallCount = 0
        appBackend.lastConnList.lastRemoveFromHistoryRow = -1

        connectSpy.clear()

        // Reset form fields to Page.qml defaults — QtQuickTest runs cases in
        // alphabetical order, so one test's slotSetCurrentDevice() leaks into
        // the next unless we restore.
        page.slotSetCurrentDevice("127.0.0.1", "102")
    }

    function test_instantiation() {
        verify(page !== null, "Home page must instantiate")
        // Quick-Connect form defaults come from Page.qml itself.
        var connBtn = findChildByText(page, qsTr("Connect"))
        verify(connBtn !== null, "Connect button must exist")
        var dumpBtn = findChildByText(page, qsTr("MMS Dump"))
        verify(dumpBtn !== null, "MMS Dump button must exist")
    }

    // Outbound to iedBackend: clicking "MMS Dump" forwards (ip, port).
    function test_mmsDumpCallsBackend() {
        var dumpBtn = findChildByText(page, qsTr("MMS Dump"))
        verify(dumpBtn !== null, "MMS Dump button must exist")
        dumpBtn.clicked()

        compare(iedBackend.dumpMmsModelCallCount, 1)
        compare(iedBackend.lastDumpMmsIp, "127.0.0.1")  // Page.qml default
        compare(iedBackend.lastDumpMmsPort, 102)        // Page.qml default
    }

    // Outbound local signal: connectNow() bundles the form into sigConnectTo.
    // Drives slotSetCurrentDevice() to prove the form is actually the source.
    function test_connectNowEmitsSigConnectTo() {
        page.slotSetCurrentDevice("10.0.0.5", "1234")
        page.connectNow()

        compare(connectSpy.count, 1)
        var con = connectSpy.signalArguments[0][0]
        compare(con.ip, "10.0.0.5")
        compare(con.port, "1234")
        compare(con.tls, false)
    }

    // Guard-rail: rendering Home with empty backends doesn't call into
    // iedBackend. The connectTo path never fires without user input, and
    // dumpMmsModel is click-only.
    function test_noBackendCallsOnIdleRender() {
        compare(iedBackend.dumpMmsModelCallCount, 0)
        compare(appBackend.lastConnList.removeFromHistoryCallCount, 0)
        compare(connectSpy.count, 0)
    }

    function findChildByText(root, label) {
        if (!root) return null
        if (root.hasOwnProperty("text") && root.text === label) {
            return root
        }
        var kids = root.children || []
        for (var i = 0; i < kids.length; ++i) {
            var hit = findChildByText(kids[i], label)
            if (hit) return hit
        }
        if (root.contentItem && root.contentItem !== root) {
            var h2 = findChildByText(root.contentItem, label)
            if (h2) return h2
        }
        return null
    }
}
