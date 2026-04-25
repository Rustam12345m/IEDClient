import QtQuick

// Home page (screen 6) drives appBackend.lastConnList. Earlier screens
// don't touch appBackend, so this mock stays minimal.
QtObject {
    // --- Home page surface ---
    // lastConnList is a Q_PROPERTY exposing a QAbstractItemModel* in
    // production. Home reads it as ListView.model, calls .index()/.data()
    // on it from the delegate, and calls .removeFromHistory(row) on
    // Delete key. We expose it as a nested QtObject with the same call
    // surface; the default stub reports zero rows so the ListView renders
    // empty. No ListModel child (that broke screen 4).
    property QtObject lastConnList: QtObject {
        property int removeFromHistoryCallCount: 0
        property int lastRemoveFromHistoryRow: -1
        // Row count; ListView falls back to 0 when the model exposes none.
        property int rowCount: 0
        // Rows by (row, col); tests override by assigning a JS object.
        property var rows: ({})

        function index(row, col) { return { row: row, column: col } }
        function data(idx) {
            if (!idx) return ""
            var key = idx.row + ":" + idx.column
            return rows[key] !== undefined ? rows[key] : ""
        }
        function removeFromHistory(row) {
            removeFromHistoryCallCount++
            lastRemoveFromHistoryRow = row
        }
    }
}
