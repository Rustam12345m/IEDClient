import QtQuick
import QtQuick.Controls
import QtTest

import "qrc:/logical_node/"

TestCase {
    id: tc
    name: "LN_SettingsView"
    when: windowShown

    ApplicationWindow {
        id: testWindow
        visible: true
        width: 800
        height: 600

        LN_SettingsView {
            id: view
            anchors.fill: parent
        }
    }

    function init() {
        iedBackend.writeValueCallCount = 0
        iedBackend.lastWriteRef = ""
        iedBackend.lastWriteFc = ""
        iedBackend.lastWriteValue = ""

        iedBackend.getSettingsItemRefCallCount = 0
        iedBackend.lastGetSettingsItemRefRow = -1
        iedBackend.nextSettingsItemRef = ""
        iedBackend.nextSettingsItemFC = ""
        iedBackend.nextSettingsItemValue = ""

        iedBackend.getSGCBInfoCallCount = 0
        iedBackend.nextSGCBInfo = ({})

        var dlg = findWriteDialog()
        if (dlg && dlg.visible) {
            dlg.close()
        }
        if (dlg) {
            dlg.statusMsg = ""
            dlg.lastResultSuccess = true
        }
    }

    function test_instantiation() {
        verify(view !== null, "LN_SettingsView should instantiate")
        // Empty models → per-LN pane is visible (ldViewActive is false).
        compare(view.ldViewActive, false)
    }

    // With both models empty, the "No settings" label is shown.
    function test_emptyStateShowsNoSettingsLabel() {
        var label = findChildByText(view, "No settings")
        verify(label !== null, "'No settings' label must exist")
        tryCompare(label, "visible", true)
    }

    // Outbound: DiaChangeValue emits sigWrite → LN_SettingsView forwards to
    // iedBackend.writeValue(ref, fc, value).
    function test_writeDialogForwardsToBackend() {
        var dlg = findWriteDialog()
        verify(dlg !== null, "Embedded DiaChangeValue must exist")

        dlg.sigWrite("IED/LN.DO.sp", "SP", "42")

        compare(iedBackend.writeValueCallCount, 1)
        compare(iedBackend.lastWriteRef, "IED/LN.DO.sp")
        compare(iedBackend.lastWriteFc, "SP")
        compare(iedBackend.lastWriteValue, "42")
    }

    // Inbound: while dialog is open, sigWriteResult flows into setResult().
    function test_sigWriteResultUpdatesOpenDialog() {
        var dlg = findWriteDialog()
        verify(dlg !== null, "Embedded DiaChangeValue must exist")
        dlg.openWrite("IED/LN.DO.sp", "SP", "IED/LN.DO.sp", "1")
        tryCompare(dlg, "visible", true)

        iedBackend.sigWriteResult("IED/LN.DO.sp", false, "failed")
        compare(dlg.lastResultSuccess, false)
        compare(dlg.statusMsg, "failed")

        iedBackend.sigWriteResult("IED/LN.DO.sp", true, "ok")
        compare(dlg.lastResultSuccess, true)
        compare(dlg.statusMsg, "ok")
    }

    // Guardrail: sigWriteResult arriving while the dialog is closed is ignored
    // (isActive() gate in LN_SettingsView).
    function test_sigWriteResultIgnoredWhenDialogClosed() {
        var dlg = findWriteDialog()
        verify(dlg !== null, "Embedded DiaChangeValue must exist")
        verify(!dlg.visible, "Precondition: dialog must start closed")
        dlg.statusMsg = "untouched"

        iedBackend.sigWriteResult("IED/LN.DO.sp", false, "nope")
        compare(dlg.statusMsg, "untouched")
    }

    // Walk the tree for a ModalDialog exposing DiaChangeValue's API (openWrite
    // + sigWrite). The dialog is an internal child — no id exported.
    function findWriteDialog() {
        return findNodeWithFn(view, "openWrite")
    }

    function findNodeWithFn(root, fnName) {
        if (!root) return null
        if (typeof root[fnName] === "function") return root
        // Popups (ModalDialog/DiaChangeValue) live in `resources`, not
        // `children`, so walk both.
        var pools = [root.children || [], root.resources || []]
        for (var p = 0; p < pools.length; ++p) {
            var kids = pools[p]
            for (var i = 0; i < kids.length; ++i) {
                var hit = findNodeWithFn(kids[i], fnName)
                if (hit) return hit
            }
        }
        if (root.contentItem && root.contentItem !== root) {
            var h2 = findNodeWithFn(root.contentItem, fnName)
            if (h2) return h2
        }
        return null
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
