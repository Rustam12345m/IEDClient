import QtQuick
import QtQuick.Controls
import QtTest

// Alias to avoid clash with Controls.Page; the real file is ui/dataset/Page.qml.
import "qrc:/dataset/" as DS

TestCase {
    id: tc
    name: "DatasetPage"
    when: windowShown

    ApplicationWindow {
        id: testWindow
        visible: true
        width: 900
        height: 600

        DS.Page {
            id: page
            anchors.fill: parent
        }
    }

    function init() {
        iedBackend.selectDataSetByRefCallCount = 0
        iedBackend.lastSelectDataSetByRef = ""
        iedBackend.getDataSetRefsCallCount = 0
        iedBackend.nextDataSetRefs = []
        iedBackend.updateDS_ValuesCallCount = 0

        iedBackend.nextDSComModel.setSelectedDSCallCount = 0
        iedBackend.nextDSComModel.lastSetSelectedDS = -1
        iedBackend.nextDSComModel.count = 0

        iedBackend.nextDSItemModel.getItemDetailCallCount = 0
        iedBackend.nextDSItemModel.lastGetItemDetailRow = -1
        iedBackend.nextDSItemModel.nextItemDetail = []
        iedBackend.nextDSItemModel.rows = 0
        iedBackend.nextDSItemModel.columns = 0

    }

    function test_instantiation() {
        verify(page !== null, "Dataset page must instantiate")
        // Page exposes resizeColumnsOnPage() — it's the only public function.
        verify(typeof page.resizeColumnsOnPage === "function",
               "Page.resizeColumnsOnPage must be callable")
        var lv = findOverviewListView()
        verify(lv !== null, "DS_OverviewTable's ListView must be locatable")
        var tv = findStateTableView()
        verify(tv !== null, "DS_StateTable's TableView must be locatable")
    }

    // Outbound binding: ListView.model binds to iedBackend.getDS_ComModel().
    // ListView accepts non-QAbstractItemModel QObjects, so we can compare
    // the bound reference directly.
    function test_overviewModelBoundToBackendGetter() {
        var lv = findOverviewListView()
        verify(lv !== null, "ListView must exist")
        compare(lv.model, iedBackend.nextDSComModel)
    }

    // TableView in Qt 6.4 strictly demands QAbstractItemModel*; our QML
    // mock can't subclass that, so tv.model ends up null. The best we can
    // prove without a C++ mock is that the TableView was instantiated.
    // Real model-binding regressions will surface as production crashes,
    // not unit-test failures — this is acknowledged baseline coverage.
    function test_stateTableViewExists() {
        var tv = findStateTableView()
        verify(tv !== null, "TableView must exist")
    }

    // Guard-rail: idle render with empty mock models must not call any
    // dataset backend invokables. The page only orchestrates; the actual
    // selectDataSetByRef() lives in main.qml — confirm Page.qml itself
    // never reaches for it on construction.
    function test_noBackendCallsOnIdleRender() {
        compare(iedBackend.selectDataSetByRefCallCount, 0)
        compare(iedBackend.updateDS_ValuesCallCount, 0)
        compare(iedBackend.getDataSetRefsCallCount, 0)
        compare(iedBackend.nextDSComModel.setSelectedDSCallCount, 0)
        compare(iedBackend.nextDSItemModel.getItemDetailCallCount, 0)
    }

    // The DS_OverviewTable's ListView is the only ListView under page.
    function findOverviewListView() {
        return findNodeWithProp(page, "keyNavigationEnabled")
    }

    // The DS_StateTable's TableView is the only TableView under page.
    // Identify by selectionBehavior — distinct from ListView.
    function findStateTableView() {
        return findNodeWithProp(page, "selectionBehavior")
    }

    function findNodeWithProp(root, propName) {
        if (!root) return null
        if (root.hasOwnProperty(propName)) return root
        var pools = [root.children || [], root.resources || []]
        for (var p = 0; p < pools.length; ++p) {
            var kids = pools[p]
            for (var i = 0; i < kids.length; ++i) {
                var hit = findNodeWithProp(kids[i], propName)
                if (hit) return hit
            }
        }
        if (root.contentItem && root.contentItem !== root) {
            var h2 = findNodeWithProp(root.contentItem, propName)
            if (h2) return h2
        }
        return null
    }
}
