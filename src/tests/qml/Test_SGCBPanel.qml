import QtQuick
import QtQuick.Controls
import QtTest

import "qrc:/logical_node/"

TestCase {
    id: tc
    name: "SGCBPanel"
    when: windowShown

    ApplicationWindow {
        id: testWindow
        visible: true
        width: 640
        height: 480

        SGCBPanel {
            id: panel
            ldRef: "IED1/LD0"
            width: 500
            height: 120
        }
    }

    function init() {
        iedBackend.getSGCBInfoCallCount = 0
        iedBackend.lastGetSGCBInfoLdRef = ""
        iedBackend.setActiveSGCallCount = 0
        iedBackend.lastSetActiveSGLdRef = ""
        iedBackend.lastSetActiveSG = 0
        iedBackend.selectEditSGCallCount = 0
        iedBackend.lastSelectEditSGLdRef = ""
        iedBackend.lastSelectEditSG = 0
        iedBackend.cancelEditSGCallCount = 0
        iedBackend.confirmEditSGCallCount = 0
        iedBackend.nextSGCBInfo = ({})

        panel.numOfSG = 0
        panel.actSG = 0
        panel.editSG = 0
        panel.statusMsg = ""
        panel.ldRef = "IED1/LD0"
    }

    function test_hiddenWhenNoSGCB() {
        iedBackend.nextSGCBInfo = ({})
        panel.refresh()
        compare(iedBackend.getSGCBInfoCallCount, 1)
        compare(panel.numOfSG, 0)
        tryCompare(panel, "visible", false)
    }

    function test_populatedOnRefresh() {
        iedBackend.nextSGCBInfo = ({numOfSG: 4, actSG: 2, editSG: 1})
        panel.refresh()

        compare(panel.numOfSG, 4)
        compare(panel.actSG, 2)
        compare(panel.editSG, 1)
        tryCompare(panel, "visible", true)

        var actSGCombo = findChildByObjectName(panel, "actSGCombo") || findComboByCurrentIndex(panel, 1)
        verify(actSGCombo !== null, "Active SG combo must exist")
        compare(actSGCombo.model.length, 4)
        compare(actSGCombo.currentIndex, 1)
    }

    function test_applyActiveSG() {
        iedBackend.nextSGCBInfo = ({numOfSG: 3, actSG: 1, editSG: 0})
        panel.refresh()

        var combos = collectCombos(panel)
        verify(combos.length >= 1, "At least one combo")
        combos[0].currentIndex = 2  // SG 3

        var applyBtn = findChildByText(panel, "Apply")
        verify(applyBtn !== null, "Apply button must exist")
        applyBtn.clicked()

        compare(iedBackend.setActiveSGCallCount, 1)
        compare(iedBackend.lastSetActiveSGLdRef, "IED1/LD0")
        compare(iedBackend.lastSetActiveSG, 3)
    }

    function test_startEditSession() {
        iedBackend.nextSGCBInfo = ({numOfSG: 3, actSG: 1, editSG: 0})
        panel.refresh()

        var combos = collectCombos(panel)
        verify(combos.length >= 2, "Active + edit combos")
        combos[1].currentIndex = 1  // edit SG 2

        var startBtn = findChildByText(panel, "Start Edit")
        verify(startBtn !== null, "Start Edit button must exist")
        verify(startBtn.enabled, "Start Edit enabled when no edit session")
        startBtn.clicked()

        compare(iedBackend.selectEditSGCallCount, 1)
        compare(iedBackend.lastSelectEditSGLdRef, "IED1/LD0")
        compare(iedBackend.lastSelectEditSG, 2)
    }

    function test_sigSGCBUpdatedTriggersRefresh() {
        iedBackend.nextSGCBInfo = ({numOfSG: 2, actSG: 1, editSG: 1})
        iedBackend.sigSGCBUpdated("IED1/LD0")

        tryCompare(panel, "numOfSG", 2)
        compare(panel.actSG, 1)
        compare(panel.editSG, 1)
        compare(panel.statusMsg, "Editing group 1")
    }

    function test_sigSGCBUpdatedIgnoresOtherLdRef() {
        iedBackend.nextSGCBInfo = ({numOfSG: 5, actSG: 2, editSG: 0})
        var countBefore = iedBackend.getSGCBInfoCallCount
        iedBackend.sigSGCBUpdated("IED1/LD_OTHER")

        compare(iedBackend.getSGCBInfoCallCount, countBefore)
        compare(panel.numOfSG, 0)
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

    function findChildByObjectName(root, name) {
        if (!root) return null
        if (root.objectName === name) return root
        var kids = root.children || []
        for (var i = 0; i < kids.length; ++i) {
            var hit = findChildByObjectName(kids[i], name)
            if (hit) return hit
        }
        return null
    }

    // Walk tree, gather ComboBoxes in declaration order (layout order).
    function collectCombos(root) {
        var out = []
        function walk(node) {
            if (!node) return
            if (node.hasOwnProperty("currentIndex") && node.hasOwnProperty("model")
                && node.hasOwnProperty("displayText")) {
                out.push(node)
            }
            var kids = node.children || []
            for (var i = 0; i < kids.length; ++i) walk(kids[i])
        }
        walk(root)
        return out
    }

    function findComboByCurrentIndex(root, idx) {
        var combos = collectCombos(root)
        for (var i = 0; i < combos.length; ++i) {
            if (combos[i].currentIndex === idx) return combos[i]
        }
        return combos.length > 0 ? combos[0] : null
    }
}
