# Project Invariants

Rules and patterns established during development that must be maintained.

## Build & CI

- **Always use `./ci/build_local.sh --release --check --archive`** for building; it produces an AppImage via Docker. Never build manually with cmake outside the container.
- **Suppress verbose output** during AppImage packaging: `cmake --install` and `linuxdeploy` calls redirect stdout to `/dev/null`.
- **Don't auto-build** after every change. The user will check manually. Only build when explicitly asked.

## Thread Safety

- **libiec61850 callbacks run on an internal network thread**, not the Qt main thread. All `MmsValue` data must be copied to `QString`/Qt types inside the callback before returning (pointers are invalid after callback returns).
- **`ReportStorage::addReport()`** is mutex-guarded (same pattern as `EventStorage`).
- **`sigReportReceived`** must be connected with `Qt::QueuedConnection` so it arrives on the Qt main thread.
- **On disconnect**: uninstall all report handlers BEFORE calling `IedConnection_abort()` to prevent dangling callback pointers.

## Report Handler Lifecycle

- **Install handler BEFORE enabling RCB** (so no reports are missed).
- **Uninstall handler AFTER disabling RCB** (so in-flight reports are still captured).
- **On disconnect**: `uninstallAllHandlers()` called before connection teardown.

## RCB (Report Control Block) Behavior

- **RCB disable must NOT clear fields**: When disabling, only set `RptEna=false`. Do not overwrite DataSet, RptID, TrgOps, BufTm, IntgPd.
- **RCB panel fields (ID, DataSet) must be editable**, not read-only `TextField` elements.
- **On disable in `ied_control_api_impl`**: use only `RCB_ELEMENT_RPT_ENA` mask, not the full field mask.
- **On disable in `set_rcb_values_cmd`**: skip `setTrgOps`, `setRptId`, `setDsRef`, `setBufTm`, `setIntgPd` model updates.

## RCB Selection & Reports Table

- **`slotRCBSelected` uses `qobject_cast<RCB_OverviewTable*>(sender())`** to resolve which overview table (BRCB or URCB) emitted the signal, then calls `getSelectedReportBlock()`.
- **Keyboard arrow navigation must update the reports table** — use `onCurrentRowChanged` handler on the RCB TableView, not only click handlers.

## Data Display — `[object]` for Complex Items

- **Dataset table (DS_SignalsTable)**: Items with children (`getItemCount() > 0`) display `[object]` in the Value column.
- **Reports table (ReportsTable)**: Multi-entry reports show `[object]` in the Values column (not a joined string of first N values).
- **Double-click on `[object]`** opens a `ModalDialog` showing all leaf attributes (name + value) in a scrollable list.
- **`getItemDetail(row)`** is `Q_INVOKABLE` and returns `QVariantList` of `{name, value}` maps by recursively collecting leaves.

## UI Patterns

- **`ModalDialog.qml`** is the standard reusable modal: TUI/DOS-style double white border on `bgColor` background, dark overlay, closes on Escape or click outside. `bgColor` defaults to `VisualStyle.modalColor` (blue) — pass `VisualStyle.errorModalColor` (dark red) for error dialogs.
- **Ctrl+C on any table** copies the selected row to clipboard with `;` delimiter via `Globals.copyRowToClipboard(tableID)`.
- **`copyToClipboard(text)`** on `MainPresenter` is the C++ bridge to `QGuiApplication::clipboard()`.
- **Reports page uses `SplitView { orientation: Qt.Vertical }`**: top pane = RCB overview, bottom pane = received reports table.

## QML Conventions

- **`Globals.qml`** singleton holds shared helper functions (`copyRowToClipboard`, `setSelectedRow`, `columnWidthCalculator`, `resizeColumnsToContent`).
- **`VisualStyle.qml`** singleton holds all style constants.
- **`TextDelegate`** emits `sigClick` and `sigDoubleClick` — use `onSigDoubleClick` in delegates for detail modals.
- **`TableHeader`** is the standard header component for `TableView`.

## Model Notification after Command Execution

- **LN/LD models** are notified automatically: `ModelItem::updateValue()` → `notifyFromChild()` bubbles up through `LogicalNode`/`LogicalDevice`, which emit `sigDataObjectUpdated`. The table models are connected to these signals.
- **Dataset model (`DS_SignalsTable`) has NO automatic notification chain**: `DataSetItem::m_valItem` points into the main data model tree (under LogicalNode), so `notifyFromChild` bubbles up through the LN/LD hierarchy — it never reaches `DataSet`. The dataset model must be notified explicitly after `ModelStateUpdater::update()`.
- **`DataSet::sigItemUpdated` is declared but never emitted** — do not rely on it. Use direct `slotDataUpdated()` calls from the backend instead.
- **RCB models use the direct-call pattern**: connect to `sigCmdEvent`, then call `model->slotDataUpdated()` in the FINISH handler. This is the reliable pattern for models that don't have automatic `notifyFromChild` coverage.

## dataChanged Index Bounds

- **`dataChanged(topLeft, bottomRight)`** — `bottomRight` column must be `COLUMN_COUNT - 1`, not `COLUMN_COUNT`. Using the count (not last index) creates an invalid `QModelIndex` that Qt silently ignores.
- **Guard `rowCount() > 0`** before emitting `dataChanged` — `index(-1, col)` is invalid and the signal is silently dropped.

## Qt6 QML TableView Refresh

- **Qt6 `TableView` may not repaint visible delegates** after `dataChanged` alone. Add a `Connections` block on `tableID.model` with `onDataChanged: tableID.forceLayout()` to force delegate re-evaluation.
- **`forceLayout()`** forces the TableView to re-query `model.display` for visible delegates. Without it, cached delegates may show stale values until scrolled off-screen and back.

## Empty-State Labels

- **Every table view** should display a centered placeholder `Text` (e.g. "No files available", "No data objects") when `tableID.rows === 0`. Use `VisualStyle.textColor`, `font.pixelSize: 14`, `anchors.centerIn: parent`.

## LD Grid Item Display

- **Unselected LD boxes**: border color and header color must match (`VisualStyle.ldGrid.borderColor` == `VisualStyle.ldGrid.color`). Mismatched values look like a visual bug.
- **Status values in LD boxes** (Mod, Beh, Health, Sim, Blk): use a `Led` component (colored dot) + plain black text (`VisualStyle.textColor`), not colored text directly.

## Connection Error Handling

- **`IEC61850_API::connect()` returns `QString`** — empty on success, human-readable error reason on failure. Never discard error codes from libiec61850.
- **`IedClientError` codes are mapped to user-friendly strings** in `libiec61850_adapter.cpp::iedErrorToString()` (timeout, rejected, access denied, etc.).
- **`MainPresenter::sigConnectionError(QString)`** carries the error message to QML. The error dialog (`connectionErrorDialog`) shows a dark-red modal with the reason.
- **Connection failure closes the progress bar immediately** — no timer delay. The error modal replaces the old 3-second silent timeout.

## GOOSE/SV Control Blocks

- **libiec61850 object reference format**: `getGoCBValues` and `ClientSVControlBlock_create` expect `LD/LN.name` (e.g., `LD/LLN0.gcbEvents`). They internally insert `$GO$` or `$MS$`/`$US$`. Never pass `LD/LN.GO.name` — that causes double-prefix (`LLN0$GO$GO$name`) and fails with error 99.
- **libiec61850 v1.6.x lacks MSVCB/USVCB discovery**: `IedConnection_getLogicalNodeDirectory` doesn't handle `ACSI_CLASS_MSVCB`/`ACSI_CLASS_USVCB`. Patched in `ied_connection.c` to add FC codes "MS"/"US".
- **`setGOOSEEnable`/`setSVEnable` return `QString`**: empty on success, error message on failure (using `IedClientError_toString`). Not `bool`.
- **Empty control block lists are not errors**: Update commands treat 0-of-0 as success (`bool ok = (list.size() == 0) || (updated > 0)`).
- **GOOSE panel**: GoID and DataSet are editable (writable via libiec61850 API). ConfRev, MinTime, MaxTime, network params are read-only.
- **SV panel**: Only SvEna is writable. All other fields (SvID, DataSet, ConfRev, SmpRate, NoASDU) are read-only per libiec61850 API.

## RCB Owner Field

- **Owner is an octet string**: 4 bytes = IPv4 address (format as `a.b.c.d`), other lengths = colon-separated hex.
- **Re-read after enable/disable**: `SetRCBValues_Cmd` calls `refreshRCBValues()` after success to get server-set fields (owner, resv, confRev).

## Multi-Row Selection in Tables

- **`selVer` counter pattern**: QML `selectionModel.isSelected()` is not reactive. Use `property int selVer: 0` on the TableView, increment in `onSelectionChanged`, reference in binding: `selected: { tableID.selVer; return tableID.selectionModel.isSelected(...) }`.
- **`multiSelect` flag**: Set `true` before `toggleSelectedRow`, `false` after. Prevents `onCurrentRowChanged` from clearing multi-selection via `setSelectedRow`.
- **Arrow keys clear multi-select**: `onCurrentRowChanged` calls `setSelectedRow` (Clear+Select) when `!multiSelect`.
- **Block operations on multi-select**: Double-click control/detail handlers check `Globals.selectedRowCount(tableID) > 1` and return early.
- **`TextDelegate.sigCtrlClick`**: Emitted on Ctrl+Click, handlers call `Globals.toggleSelectedRow`.

## File Download Progress

- **libiec61850 `IedClientGetFileHandler` callback**: Called per data block. `DownloadContext` struct tracks `bytesTotal`/`fileSize` and emits `sigDownloadProgress(perc)`.
- **Signal chain**: `IED_FS_API::sigDownloadProgress` → `DownloadFileCmd::sigDownloadProgress` → `IED_FS_Backend::sigDownloadProgress` → QML.
- **Per-row progress**: `downloadingRow` tracks active download, `completedRows` map persists 100% for finished downloads.

## Event Log

- **Arrow keys need explicit handlers**: `keyNavigationEnabled` on TableView changes `currentRow` but doesn't update `selectionModel`. Use `Keys.onPressed` with `Globals.setSelectedRow` for Up/Down.
- **Focus in ModalDialog**: Use `FocusScope` as root + `onVisibleChanged: tableID.forceActiveFocus()` to grab keyboard focus from the Popup.

## BackendInterface Error Signal

- **`sigCmdError(QString)`**: Emitted by `BackendInterface::slotCmdEvent` on any `FINISH_EVENT` with `m_result == false`. Connected to `connectionErrorDialog.showError()` in main.qml for both `iedBackend` and `fsBackend`.
- **IP auto-fill**: `slotCmdEvent` fills empty or `"IP?"` IP fields from `m_con.m_cred.ip()` before storing events.

## FC-Aware Data Model Architecture

- **Sub-attributes are filtered by FC during model building**: `recursiveReadAttributes()` uses `IedConnection_getDataDirectoryByFC()` (not `getDataDirectory()`) to only discover sub-attributes for the matching functional constraint. This prevents mixing children from different FCs under the same DataAttribute node.
- **Duplicate DA names per DataObject are expected**: A DataObject like `A` can have `phsA[DC]` and `phsA[MX]` as separate children — both named "phsA" but with different FCs. This is correct per IEC 61850 — each FC has its own attribute hierarchy.
- **`recFindModelItem()` uses backtracking**: When resolving a path like `A.phsA.cVal.mag.f`, if the first `phsA` child (e.g., `phsA[DC]`) doesn't have `cVal`, it tries the next `phsA` child (`phsA[MX]`). This handles FC-duplicate names.
- **`findSubItem(name)` returns the first match only**: Not FC-aware. Safe for unique names (LN, DO level) but ambiguous at the DA level where duplicates exist. Use `recFindModelItem` for FC-safe path resolution.
- **`getReference()` does not include FC**: Two siblings with the same name produce identical path strings. This is mitigated because libiec61850 API always takes FC as a separate parameter alongside the path.
- **Signal matrix correctly filters by FC**: `recursiveFillMatrix()` skips DAs whose FC doesn't match the filter, so no duplicate rows are created.
- **Dataset item resolution is FC-safe**: Uses `getItemByReference()` → `recFindModelItem()` with backtracking. Finds the correct FC variant by following the deepest resolvable path.
- **Value notification is pointer-based**: `updateValue()` → `notifyFromChild()` uses parent pointers, not name lookups. Unaffected by duplicate names.
- **Positional value matching**: `getValuesForItemByMmsValue()` matches MMS structure elements to model children by position. This works correctly because each DA now only has children from its own FC — the element count matches.
- **Never hide values based on tree structure**: `getValue()` is always shown in tree models regardless of `getItemCount()`. The tree displays exactly what the IED provides.

## Crash Reporting (Breakpad)

- **Linux**: Uses `client/linux/handler/exception_handler.h`. Dumps to `~/.local/share/IEDClient/crashes/`.
- **Windows**: Uses `client/windows/handler/exception_handler.h` with `UNICODE`/`_UNICODE` defines for MinGW. Dumps to `%LOCALAPPDATA%\IEDClient\crashes/`.
- **Guarded by defines**: `BREAKPAD_ENABLED` + `BREAKPAD_LINUX` or `BREAKPAD_WINDOWS`. Unsupported platforms get empty INTERFACE target.
- **Initialize before QGuiApplication**: The ExceptionHandler must be constructed before any Qt objects.

## Embedded Font

- **Noto Sans Mono** embedded via Qt resources (`ui/fonts/`). Loaded in `main.cpp` via `QFontDatabase::addApplicationFont`, set as app-wide default with `app.setFont()`.
- **`VisualStyle.fontFamily`** property references the font name. All QML code uses this instead of hardcoded `"Monospace"`.

## Tree Filter

- **`TreeFilterProxy`**: Recursive `QSortFilterProxyModel` for the IED tree. Splits filter text on `.` and matches all parts sequentially against the node's full path (root-to-leaf).
- **`setRecursiveFilteringEnabled(true)`**: Qt built-in recursive filtering keeps parent nodes visible when children match.
- **`layoutChanged()`** for tree refresh: `dataChanged` with flat indices doesn't refresh nested tree rows. Use `layoutChanged()` in `slotDataUpdated()` to force full tree re-read.

## Disconnect Behavior

- **Data preserved on disconnect**: `slotConClosed()` only emits `sigIEDConChanged(false)` — it does NOT create a new empty IED or reset models. The fetched data remains visible after connection loss or user-initiated disconnect.
- **Tabs remain visible after first connection**: `hasConnected` flag (QML) is set `true` on first successful connect. Tabs are hidden only before first connection (`showOnlyFirst: !hasConnected`), never re-hidden on disconnect.
- **Page navigation unrestricted**: No guard on `setActivePage()` — users can browse any tab while disconnected.
- **New connection replaces data**: `slotConnected(true)` on successful connect calls `setActiveIED()` on all models with the new IED, replacing any stale data.

## ScrollBar Overlap Prevention

- **ListView ScrollBar overlaps content by default**: Qt Quick's `ScrollBar.vertical` renders on top of delegate content.
- **Fix pattern**: Give the ScrollBar an `id` (e.g., `vScrollBar`), then set delegate and section delegate `width: listView.width - vScrollBar.width`. This keeps content out from under the scrollbar.
- **Applied to**: `LN_AllOverviewList`, `DS_OverviewTable`. Other ListViews with ScrollBars should follow the same pattern.

## LN Page Status Bar

- **Status shows selected LN reference**: `lnsPageStatus()` returns `"ldName / lnName"` from `LN_AllOverviewTable::getSelectedReference()`, not the LD page's selection.
- **Updates on selection change**: LN Page emits `sigLNSelectionChanged()` on every LN click, connected to `setPageStatusText(iedBackend.lnsPageStatus())` in main.qml.
- **`m_selectedRow`** in `LN_AllOverviewTable` tracks the last selected row for reference display.

## LN Filter

- **`LN_AllOverviewTable::setFilter()`**: Case-insensitive filter on `"ldName/lnName"`. Triggers `beginResetModel()` → `rebuildFlatList()` → `endResetModel()`.
- **TextField at bottom of LN list**: Anchored to parent bottom, ListView bottom anchored to filter top.

## C++ Conventions

- **`m_` prefix** for member variables. No prefix for function parameters (removed `t_` prefix).
- **Command pattern**: all async IED operations go through `CmdThread` with `CmdInterface` commands.
- **`Q_INVOKABLE`** for methods called from QML.
- **No `qDebug()` in production code**: All debug output commented out. Use event log for user-visible messages.
- **Never build without checking**: Do not run `./ci/build_local.sh` unless explicitly asked by the user.

## Watchlist Persistence

- **Watchlist is persisted per device** in `iedclient_config.xml` inside a `<watchlist>` sub-element of each `<device>`:
  ```xml
  <device ip="..." port="..." ...>
      <watchlist>
          <node ref="LD0/LLN0.Mod" fc="ST"/>
      </watchlist>
  </device>
  ```
- **Save on every change**: `WatchlistModel::sigItemsChanged` → `MainPresenter::slotSaveWatchlist()` → `AppSettings::saveWatchlistForDevice()`. Fires on add, remove, and clear.
- **Load on connect**: `MainPresenter::slotCmdEvent(FINISH, ok)` calls `getWatchlistForDevice()` → `IED_Backend::loadWatchlist()`.
- **No infinite save loop**: `loadWatchlist()` uses `WatchlistModel::setItems()` which does NOT emit `sigItemsChanged`.
- **`putConnectionToConfig` preserves watchlist**: When deduplicating a device entry, the existing watchlist is copied to the new entry before the old one is erased.

## Dataset Table

- **FC column displays values in brackets**: The FC column shows `[FC]` (e.g., `[ST]`), not bare `ST`. Reference and FC remain separate columns.

## Global F5 Refresh

- **`updateActivePage()` must handle all page types**: Every `Globals.Page.*` value needs a case in the switch. `IED_TREE` calls `iedBackend.updateWatchlistValues()`.
- **Global `Shortcut` intercepts F5 before local `Keys.onPressed`**: Page-level F5 handlers are unreachable — all F5 logic must go through `updateActivePage()` in `main.qml`.
