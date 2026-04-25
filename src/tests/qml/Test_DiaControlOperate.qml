import QtQuick
import QtQuick.Controls
import QtTest

import "qrc:/logical_node/"

TestCase {
    id: tc
    name: "DiaControlOperate"
    when: windowShown

    ApplicationWindow {
        id: testWindow
        visible: true
        width: 640
        height: 480

        DiaControlOperate {
            id: dlg
            objectRef: "IED/LN.CSWI1.Pos"
            ctlModel: 1     // Direct (Normal) — Operate enabled, Select hidden
            ctlValType: 3   // Float
        }
    }

    function init() {
        // Reset mock counters that this test inspects.
        iedBackend.setTestModeCallCount = 0
        iedBackend.setInterlockCheckCallCount = 0
        iedBackend.setSynchroCheckCallCount = 0
        iedBackend.setOriginCallCount = 0
        iedBackend.lastSetTestMode = false
        iedBackend.lastSetInterlockCheck = false
        iedBackend.lastSetSynchroCheck = false
        iedBackend.lastSetOriginIdent = ""
        iedBackend.lastSetOriginCat = -1

        // Dialog state that individual tests depend on.
        dlg.statusMsg = ""
        dlg.lastResultSuccess = true
        if (dlg.visible) {
            dlg.close()
        }
    }

    function test_instantiation() {
        verify(dlg !== null, "DiaControlOperate should instantiate")
        compare(dlg.objectRef, "IED/LN.CSWI1.Pos")
        compare(dlg.ctlModel, 1)
    }

    // Inbound-signal path: mock emits sigCommandTermination, dialog's
    // Connections block updates statusMsg + lastResultSuccess.
    function test_commandTerminationUpdatesStatus() {
        dlg.open()
        tryCompare(dlg, "visible", true)

        iedBackend.sigCommandTermination(dlg.objectRef, true, "done")
        compare(dlg.lastResultSuccess, true)
        compare(dlg.statusMsg, "CommandTermination+: done")

        iedBackend.sigCommandTermination(dlg.objectRef, false, "blocked")
        compare(dlg.lastResultSuccess, false)
        compare(dlg.statusMsg, "CommandTermination-: blocked")
    }

    // Inbound-signal path: mismatched objRef must be ignored.
    function test_commandTerminationIgnoresOtherObjRef() {
        dlg.open()
        tryCompare(dlg, "visible", true)
        dlg.statusMsg = "untouched"
        dlg.lastResultSuccess = true

        iedBackend.sigCommandTermination("SomeOther/Ref", false, "nope")
        compare(dlg.statusMsg, "untouched")
        compare(dlg.lastResultSuccess, true)
    }

    // Outbound path: emitting the Operate button's clicked() signal pushes the
    // control flags into iedBackend. We walk the dialog tree rather than
    // clicking by coordinate — ModalDialog is a Popup, and hit-testing popup
    // children under offscreen QPA is brittle.
    function test_operateButtonPushesFlags() {
        dlg.open()
        tryCompare(dlg, "visible", true)

        var operateBtn = findChildByText(dlg.contentItem, "Operate")
        verify(operateBtn !== null, "Operate button must exist")
        verify(operateBtn.enabled, "Operate must be enabled when ctlModel > 0")

        operateBtn.clicked()

        compare(iedBackend.setTestModeCallCount, 1)
        compare(iedBackend.setInterlockCheckCallCount, 1)
        compare(iedBackend.setSynchroCheckCallCount, 1)
        compare(iedBackend.setOriginCallCount, 1)
    }

    // Walk the item tree looking for a child with matching `text`. Good enough
    // for the handful of Buttons in this dialog.
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
