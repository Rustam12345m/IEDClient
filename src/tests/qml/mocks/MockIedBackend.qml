import QtQuick

QtObject {
    // --- DiaControlOperate surface ---
    property int setTestModeCallCount: 0
    property bool lastSetTestMode: false
    function setTestMode(test) {
        setTestModeCallCount++
        lastSetTestMode = test
    }

    property int setInterlockCheckCallCount: 0
    property bool lastSetInterlockCheck: false
    function setInterlockCheck(check) {
        setInterlockCheckCallCount++
        lastSetInterlockCheck = check
    }

    property int setSynchroCheckCallCount: 0
    property bool lastSetSynchroCheck: false
    function setSynchroCheck(check) {
        setSynchroCheckCallCount++
        lastSetSynchroCheck = check
    }

    property int setOriginCallCount: 0
    property string lastSetOriginIdent: ""
    property int lastSetOriginCat: -1
    function setOrigin(orIdent, orCat) {
        setOriginCallCount++
        lastSetOriginIdent = orIdent
        lastSetOriginCat = orCat
    }

    // --- DiaChangeValue (parent-forwarded) surface ---
    property int writeValueCallCount: 0
    property string lastWriteRef: ""
    property string lastWriteFc: ""
    property string lastWriteValue: ""
    function writeValue(ref, fc, value) {
        writeValueCallCount++
        lastWriteRef = ref
        lastWriteFc = fc
        lastWriteValue = value
    }

    // --- SGCBPanel surface ---
    property int getSGCBInfoCallCount: 0
    property string lastGetSGCBInfoLdRef: ""
    // Default stub: empty map -> panel stays hidden unless test overrides.
    property var nextSGCBInfo: ({})
    function getSGCBInfo(ldRef) {
        getSGCBInfoCallCount++
        lastGetSGCBInfoLdRef = ldRef
        return nextSGCBInfo
    }

    property int setActiveSGCallCount: 0
    property string lastSetActiveSGLdRef: ""
    property int lastSetActiveSG: 0
    function setActiveSG(ldRef, sg) {
        setActiveSGCallCount++
        lastSetActiveSGLdRef = ldRef
        lastSetActiveSG = sg
    }

    property int selectEditSGCallCount: 0
    property string lastSelectEditSGLdRef: ""
    property int lastSelectEditSG: 0
    function selectEditSG(ldRef, sg) {
        selectEditSGCallCount++
        lastSelectEditSGLdRef = ldRef
        lastSelectEditSG = sg
    }

    property int cancelEditSGCallCount: 0
    property string lastCancelEditSGLdRef: ""
    function cancelEditSG(ldRef) {
        cancelEditSGCallCount++
        lastCancelEditSGLdRef = ldRef
    }

    property int confirmEditSGCallCount: 0
    property string lastConfirmEditSGLdRef: ""
    function confirmEditSG(ldRef) {
        confirmEditSGCallCount++
        lastConfirmEditSGLdRef = ldRef
    }

    // --- LN_SettingsView surface ---
    // QtObject can't hold ListModel children, so default models are JS arrays.
    // TableView treats array.length as row count, which is all we need for
    // empty-state tests. Tests needing rows seed these with a ListModel
    // created via Qt.createQmlObject or a populated array of row objects.
    property var nextLN_SettingsModel: []
    function getLN_SettingsModel() { return nextLN_SettingsModel }

    property var nextLD_SettingsModel: []
    function getLD_SettingsModel() { return nextLD_SettingsModel }

    property int getSettingsItemRefCallCount: 0
    property int lastGetSettingsItemRefRow: -1
    property string nextSettingsItemRef: ""
    function getSettingsItemRef(row) {
        getSettingsItemRefCallCount++
        lastGetSettingsItemRefRow = row
        return nextSettingsItemRef
    }

    property int getSettingsItemFCCallCount: 0
    property string nextSettingsItemFC: ""
    function getSettingsItemFC(row) {
        getSettingsItemFCCallCount++
        return nextSettingsItemFC
    }

    property int getSettingsItemValueCallCount: 0
    property string nextSettingsItemValue: ""
    function getSettingsItemValue(row) {
        getSettingsItemValueCallCount++
        return nextSettingsItemValue
    }

    property int getLD_SettingsItemRefCallCount: 0
    property string nextLD_SettingsItemRef: ""
    function getLD_SettingsItemRef(row) {
        getLD_SettingsItemRefCallCount++
        return nextLD_SettingsItemRef
    }

    property int getLD_SettingsItemFCCallCount: 0
    property string nextLD_SettingsItemFC: ""
    function getLD_SettingsItemFC(row) {
        getLD_SettingsItemFCCallCount++
        return nextLD_SettingsItemFC
    }

    property int getLD_SettingsItemValueCallCount: 0
    property string nextLD_SettingsItemValue: ""
    function getLD_SettingsItemValue(row) {
        getLD_SettingsItemValueCallCount++
        return nextLD_SettingsItemValue
    }

    property int getCurrentSettingsLDRefCallCount: 0
    property string nextCurrentSettingsLDRef: ""
    function getCurrentSettingsLDRef() {
        getCurrentSettingsLDRefCallCount++
        return nextCurrentSettingsLDRef
    }

    // --- Home page surface ---
    property int dumpMmsModelCallCount: 0
    property string lastDumpMmsIp: ""
    property int lastDumpMmsPort: 0
    function dumpMmsModel(ip, port) {
        dumpMmsModelCallCount++
        lastDumpMmsIp = ip
        lastDumpMmsPort = port
    }

    // --- Dataset page surface ---
    // getDS_ComModel() / getDS_ItemModel() return QAbstractItemModel* in
    // production. We can't subclass that from QML, but the QML bindings
    // (`ListView.model`, `TableView.model`, `Connections { target: model }`)
    // tolerate any QObject — they just render no rows. We expose nested
    // QtObjects so `Connections.target` is valid and member calls
    // (setSelectedDS, getItemDetail) don't blow up if the page ever
    // dispatches them. JS arrays would break `Connections.target`.
    property QtObject nextDSComModel: QtObject {
        property int setSelectedDSCallCount: 0
        property int lastSetSelectedDS: -1
        property int count: 0
        signal sigSelectRow(int row)
        function setSelectedDS(ds) {
            setSelectedDSCallCount++
            lastSetSelectedDS = ds
        }
    }
    function getDS_ComModel() { return nextDSComModel }

    property QtObject nextDSItemModel: QtObject {
        property int getItemDetailCallCount: 0
        property int lastGetItemDetailRow: -1
        property var nextItemDetail: []
        property int rows: 0
        property int columns: 0
        function getItemDetail(row) {
            getItemDetailCallCount++
            lastGetItemDetailRow = row
            return nextItemDetail
        }
        function index(row, col) { return { row: row, column: col } }
        function data(idx) { return "" }
        function rowCount() { return rows }
        function columnCount() { return columns }
    }
    function getDS_ItemModel() { return nextDSItemModel }

    property int selectDataSetByRefCallCount: 0
    property string lastSelectDataSetByRef: ""
    function selectDataSetByRef(ref) {
        selectDataSetByRefCallCount++
        lastSelectDataSetByRef = ref
    }

    property int getDataSetRefsCallCount: 0
    property var nextDataSetRefs: []
    function getDataSetRefs() {
        getDataSetRefsCallCount++
        return nextDataSetRefs
    }

    property int updateDS_ValuesCallCount: 0
    function updateDS_Values() { updateDS_ValuesCallCount++ }

    // --- Reports page surface ---
    // RCB / GOOSE / SV overview models. Like the dataset models, production
    // returns QAbstractTableModel*. We expose nested QtObjects so member
    // calls (setSelectedRCB / setSelectedGOOSE / setSelectedSV) don't
    // throw; tests that need rows seed `count`.
    property QtObject nextBRCB_ComModel: QtObject {
        property int setSelectedRCBCallCount: 0
        property int lastSetSelectedRCB: -1
        property int count: 0
        function setSelectedRCB(row) {
            setSelectedRCBCallCount++
            lastSetSelectedRCB = row
        }
    }
    function getBRCB_ComModel() { return nextBRCB_ComModel }

    property QtObject nextURCB_ComModel: QtObject {
        property int setSelectedRCBCallCount: 0
        property int lastSetSelectedRCB: -1
        property int count: 0
        function setSelectedRCB(row) {
            setSelectedRCBCallCount++
            lastSetSelectedRCB = row
        }
    }
    function getURCB_ComModel() { return nextURCB_ComModel }

    property QtObject nextGOSE_ComModel: QtObject {
        property int setSelectedGOOSECallCount: 0
        property int lastSetSelectedGOOSE: -1
        property int count: 0
        function setSelectedGOOSE(row) {
            setSelectedGOOSECallCount++
            lastSetSelectedGOOSE = row
        }
    }
    function getGOSE_ComModel() { return nextGOSE_ComModel }

    property QtObject nextSV_ComModel: QtObject {
        property int setSelectedSVCallCount: 0
        property int lastSetSelectedSV: -1
        property int count: 0
        function setSelectedSV(row) {
            setSelectedSVCallCount++
            lastSetSelectedSV = row
        }
    }
    function getSV_ComModel() { return nextSV_ComModel }

    // RCB_ReportsTable's TableView.model = iedBackend.getReportsModel().
    // Same QtObject-shape rule as the dataset models — give it
    // getReportDetail() to absorb double-click / row-change calls.
    property QtObject nextReportsModel: QtObject {
        property int getReportDetailCallCount: 0
        property int lastGetReportDetailRow: -1
        property var nextReportDetail: ({})
        property int rows: 0
        function getReportDetail(row) {
            getReportDetailCallCount++
            lastGetReportDetailRow = row
            return nextReportDetail
        }
    }
    function getReportsModel() { return nextReportsModel }

    // RCB enable/disable. main.qml routes RCB_PropertiesPanel.sigEnable /
    // sigDisable to setRCBEnable(buffered, index, enable, trgOps, bufTm,
    // intgPd, rptId, datSet) — Reports tests drive this from the test
    // scaffold to mimic that wiring.
    property int setRCBEnableCallCount: 0
    property bool lastSetRCBEnableBuffered: false
    property int lastSetRCBEnableIndex: -1
    property bool lastSetRCBEnableEnable: false
    property int lastSetRCBEnableTrgOps: 0
    property int lastSetRCBEnableBufTm: 0
    property int lastSetRCBEnableIntgPd: 0
    property string lastSetRCBEnableRptId: ""
    property string lastSetRCBEnableDatSet: ""
    function setRCBEnable(buffered, index, enable, trgOps, bufTm, intgPd, rptId, datSet) {
        setRCBEnableCallCount++
        lastSetRCBEnableBuffered = buffered
        lastSetRCBEnableIndex = index
        lastSetRCBEnableEnable = enable
        lastSetRCBEnableTrgOps = trgOps
        lastSetRCBEnableBufTm = bufTm
        lastSetRCBEnableIntgPd = intgPd
        lastSetRCBEnableRptId = rptId
        lastSetRCBEnableDatSet = datSet
    }

    property int setGOOSEEnableCallCount: 0
    property int lastSetGOOSEEnableIndex: -1
    property bool lastSetGOOSEEnableEnable: false
    function setGOOSEEnable(index, enable) {
        setGOOSEEnableCallCount++
        lastSetGOOSEEnableIndex = index
        lastSetGOOSEEnableEnable = enable
    }

    property int setSVEnableCallCount: 0
    property int lastSetSVEnableIndex: -1
    property bool lastSetSVEnableEnable: false
    function setSVEnable(index, enable) {
        setSVEnableCallCount++
        lastSetSVEnableIndex = index
        lastSetSVEnableEnable = enable
    }

    // --- IED tree page surface ---
    // getIED_TreeModel() / getWatchlistModel() return QAbstractItemModel* /
    // QAbstractTableModel* in production. As with the dataset/reports pages,
    // QML views accept any QObject — they just render no rows. We expose
    // nested QtObjects so member access from Connections / delegates is valid.
    property QtObject nextIEDTreeModel: QtObject {
        property int rows: 0
        property int columns: 0
        function index(row, col) { return { row: row, column: col } }
        function data(idx) { return "" }
        function rowCount() { return rows }
        function columnCount() { return columns }
        signal modelReset()
    }
    function getIED_TreeModel() { return nextIEDTreeModel }

    property QtObject nextWatchlistModel: QtObject {
        property int rows: 0
        property int columns: 0
        function index(row, col) { return { row: row, column: col } }
        function data(idx) { return "" }
        function rowCount() { return rows }
        function columnCount() { return columns }
        signal dataChanged()
    }
    function getWatchlistModel() { return nextWatchlistModel }

    property int setTreeFilterCallCount: 0
    property string lastSetTreeFilter: ""
    function setTreeFilter(text) {
        setTreeFilterCallCount++
        lastSetTreeFilter = text
    }

    property int addTreeItemToWatchlistCallCount: 0
    property var lastAddTreeItemToWatchlistIndex: null
    function addTreeItemToWatchlist(proxyIndex) {
        addTreeItemToWatchlistCallCount++
        lastAddTreeItemToWatchlistIndex = proxyIndex
    }

    property int removeFromWatchlistCallCount: 0
    property int lastRemoveFromWatchlistRow: -1
    function removeFromWatchlist(row) {
        removeFromWatchlistCallCount++
        lastRemoveFromWatchlistRow = row
    }

    property int clearWatchlistCallCount: 0
    function clearWatchlist() { clearWatchlistCallCount++ }

    property int updateWatchlistValuesCallCount: 0
    function updateWatchlistValues() { updateWatchlistValuesCallCount++ }

    // --- Signals consumed by screens 1–3 ---
    signal sigCommandTermination(string objRef, bool success, string addCause)
    signal sigWriteResult(string ref, bool success, string message)
    signal sigSGCBUpdated(string ldRef)
    // --- Signals consumed by screen 8 (Reports) ---
    signal sigRCBUpdated()
    signal sigGOOSE_SVUpdated()
}
