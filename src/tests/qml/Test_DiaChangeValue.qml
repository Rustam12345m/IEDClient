import QtQuick
import QtQuick.Controls
import QtTest

import "qrc:/logical_node/"

TestCase {
    id: tc
    name: "DiaChangeValue"
    when: windowShown

    ApplicationWindow {
        id: testWindow
        visible: true
        width: 640
        height: 480

        DiaChangeValue {
            id: dlg
            doReference: "IED/LN.GGIO1.SPCSO1"
            fc: "SP"
        }
    }

    SignalSpy {
        id: writeSpy
        target: dlg
        signalName: "sigWrite"
    }

    function init() {
        writeSpy.clear()
        dlg.statusMsg = ""
        dlg.lastResultSuccess = true
        if (dlg.visible) {
            dlg.close()
        }
    }

    function test_instantiation() {
        verify(dlg !== null, "DiaChangeValue should instantiate")
        compare(dlg.doReference, "IED/LN.GGIO1.SPCSO1")
        compare(dlg.fc, "SP")
    }

    function test_openWriteSeedsFields() {
        dlg.openWrite("IED/LN.GGIO1.SPCSO2", "CF", "Enter value", "42")
        tryCompare(dlg, "visible", true)
        compare(dlg.doReference, "IED/LN.GGIO1.SPCSO2")
        compare(dlg.fc, "CF")
        compare(dlg.statusMsg, "")
        compare(dlg.lastResultSuccess, true)
    }

    function test_writeButtonEmitsSigWrite() {
        dlg.openWrite("IED/LN.GGIO1.SPCSO1", "SP", "Write me", "7")
        tryCompare(dlg, "visible", true)

        var writeBtn = findChildByText(dlg.contentItem, "Write")
        verify(writeBtn !== null, "Write button must exist")

        writeBtn.clicked()

        tryCompare(writeSpy, "count", 1)
        compare(writeSpy.signalArguments[0][0], "IED/LN.GGIO1.SPCSO1")
        compare(writeSpy.signalArguments[0][1], "SP")
        compare(writeSpy.signalArguments[0][2], "7")
        compare(dlg.statusMsg, "Writing...")
        compare(dlg.lastResultSuccess, true)
    }

    function test_setResultSuccess() {
        dlg.setResult(true, "ok")
        compare(dlg.lastResultSuccess, true)
        compare(dlg.statusMsg, "ok")
    }

    function test_setResultFailure() {
        dlg.setResult(false, "err")
        compare(dlg.lastResultSuccess, false)
        compare(dlg.statusMsg, "err")
    }

    function test_closeButtonDoesNotEmitSigWrite() {
        dlg.openWrite("IED/LN.GGIO1.SPCSO1", "SP", "msg", "0")
        tryCompare(dlg, "visible", true)

        var closeBtn = findChildByText(dlg.contentItem, "Close")
        verify(closeBtn !== null, "Close button must exist")

        closeBtn.clicked()

        compare(writeSpy.count, 0)
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
