import QtQuick
import QtQuick.Controls
import QtTest

import "qrc:/logical_node/"

TestCase {
    id: tc
    name: "LD_SettingsContent"
    when: windowShown

    ApplicationWindow {
        id: testWindow
        visible: true
        width: 800
        height: 600

        LD_SettingsContent {
            id: view
            anchors.fill: parent
        }
    }

    SignalSpy {
        id: writeReqSpy
        target: view
        signalName: "sigWriteRequested"
    }

    function init() {
        iedBackend.nextLD_SettingsModel = []
        iedBackend.nextLD_SettingsItemRef = ""
        iedBackend.nextLD_SettingsItemFC = ""
        iedBackend.nextLD_SettingsItemValue = ""
        iedBackend.getLD_SettingsItemRefCallCount = 0
        iedBackend.getLD_SettingsItemFCCallCount = 0
        iedBackend.getLD_SettingsItemValueCallCount = 0

        iedBackend.getCurrentSettingsLDRefCallCount = 0
        iedBackend.nextCurrentSettingsLDRef = ""

        iedBackend.getSGCBInfoCallCount = 0
        iedBackend.lastGetSGCBInfoLdRef = ""
        iedBackend.nextSGCBInfo = ({})

        writeReqSpy.clear()
    }

    function test_instantiation() {
        verify(view !== null, "LD_SettingsContent should instantiate")
        // Empty JS-array model → TableView.rows === 0.
        tryCompare(view, "tableRows", 0)
    }

    // The LD pane embeds SGCBPanel. With no SGCB info (empty map) the panel
    // must be invisible and its numOfSG must be zero.
    function test_sgcbPanelEmbedded() {
        var panel = findSGCBPanel(view)
        verify(panel !== null, "Embedded SGCBPanel must be locatable")
        compare(panel.numOfSG, 0)
        tryCompare(panel, "visible", false)
        // ldRef is set imperatively from onModelReset which hasn't fired for
        // the empty JS-array default — so it remains the declared default "".
        compare(panel.ldRef, "")
    }

    // onModelReset is the sole path that calls getCurrentSettingsLDRef.
    // With a plain-array default model, no reset fires during construction;
    // the counter must stay at zero. Guards against a regression that would
    // rewire the LDRef lookup into the binding itself.
    function test_emptyModelDoesNotCallCurrentLDRef() {
        compare(iedBackend.getCurrentSettingsLDRefCallCount, 0)
    }

    // Baseline: the outbound signal exists with the agreed shape
    // (ref, fc, val, editSG). The per-row emission path needs a live
    // QAbstractItemModel to populate TableView delegates, which is out of
    // scope for a baseline smoke test — covered indirectly here via a
    // direct emit + SignalSpy readback.
    function test_sigWriteRequestedSignalShape() {
        view.sigWriteRequested("IED1/LD0/LLN0.DO.sp", "SP", "42", 3)

        compare(writeReqSpy.count, 1)
        compare(writeReqSpy.signalArguments[0][0], "IED1/LD0/LLN0.DO.sp")
        compare(writeReqSpy.signalArguments[0][1], "SP")
        compare(writeReqSpy.signalArguments[0][2], "42")
        compare(writeReqSpy.signalArguments[0][3], 3)
    }

    // Walk the tree to find the embedded SGCBPanel — it has no exported id.
    // Identify by the set of properties that only SGCBPanel declares.
    function findSGCBPanel(root) {
        if (!root) return null
        if (root.hasOwnProperty("numOfSG") && root.hasOwnProperty("actSG")
            && root.hasOwnProperty("editSG") && root.hasOwnProperty("ldRef")) {
            return root
        }
        var pools = [root.children || [], root.resources || []]
        for (var p = 0; p < pools.length; ++p) {
            var kids = pools[p]
            for (var i = 0; i < kids.length; ++i) {
                var hit = findSGCBPanel(kids[i])
                if (hit) return hit
            }
        }
        if (root.contentItem && root.contentItem !== root) {
            var h2 = findSGCBPanel(root.contentItem)
            if (h2) return h2
        }
        return null
    }
}
