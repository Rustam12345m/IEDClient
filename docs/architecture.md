# Architecture

IEDClient is a Qt6/QML desktop application for interacting with IED (Intelligent Electronic Device) devices over IEC 61850 MMS. It wraps the libiec61850 C library with a C++20/Qt layer and provides a QML-based UI.

## Layer Diagram

```
 ┌─────────────────────────────────────────────────────────┐
 │                   QML UI (ui/)                          │
 │  main.qml ─ pages ─ common components ─ singletons     │
 └────────────────────────┬────────────────────────────────┘
                          │  context properties
 ┌────────────────────────▼────────────────────────────────┐
 │              Application Layer (src/app/)                │
 │  MainPresenter ─ AppBackend ─ IED_Backend ─ FS_Backend  │
 └────────────────────────┬────────────────────────────────┘
                          │  putCmdToQueue()
 ┌────────────────────────▼────────────────────────────────┐
 │         Models Layer           │    Command Layer        │
 │       (src/models/)            │     (src/cmd/)          │
 │  QAbstractTableModel           │  CmdThread + CmdQueue   │
 │  QAbstractItemModel            │  CmdInterface commands   │
 └────────────┬───────────────────┴────────┬───────────────┘
              │                            │  execute(api)
 ┌────────────▼────────────────────────────▼───────────────┐
 │               Core Layer (src/core/)                     │
 │  IED ─ DataModel ─ LogicalDevice ─ LogicalNode           │
 │  DataObject ─ DataAttribute ─ SubAttribute               │
 │  DataSet ─ ReportBlock ─ GooseCB ─ SV_CB                 │
 └─────────────────────────────────────────────────────────┘
              │
 ┌────────────▼────────────────────────────────────────────┐
 │        Adapter Layer (src/libiec61850_stub/)              │
 │  ApiAdapter ─ ModelAPI ─ StateAPI ─ ControlAPI ─ FS_API  │
 └────────────────────────┬────────────────────────────────┘
                          │  C function calls
 ┌────────────────────────▼────────────────────────────────┐
 │           libiec61850 (3rdparty/, C library)             │
 └─────────────────────────────────────────────────────────┘
```

**Dependency direction** (compile-time):
```
lib61850_adapter ← core_lib ← cmd_lib ← models_lib ← app_lib ← executable
```

## Layer Responsibilities

### 1. Adapter Layer (`src/libiec61850_stub/` -> `lib61850_adapter`)

C++ wrapper over the libiec61850 C library. Implements abstract API interfaces defined in `src/cmd/interface/`:

| Class | Interface | Role |
|---|---|---|
| `ApiAdapter` | `IEC61850_API` | Connection lifecycle, aggregate API access |
| `IED_ModelAPI_Impl` | `IED_ModelAPI` | Fetch MMS data model from IED |
| `IED_StateAPI_Impl` | `IED_StateAPI` | Read attribute values and status |
| `IED_ControlAPI_Impl` | `IED_ControlAPI` | RCB, GOOSE, SV, direct control operations |
| `IED_FS_API_Impl` | `IED_FS_API` | File system browse, download, delete |

The interface headers in `cmd/interface/` form the **abstraction boundary** — the command layer and above never depend on libiec61850 directly.

### 2. Core Layer (`src/core/` -> `core_lib`)

Domain model mirroring the IEC 61850 object hierarchy as a composite tree:

```
IED
 └─ DataModel
     ├─ LogicalDevice (QObject + ModelItem)
     │   └─ LogicalNode (QObject + ModelItem)
     │       ├─ DataObject
     │       │   └─ DataAttribute (with FC)
     │       │       └─ SubAttribute
     │       ├─ DataSet (QObject + ModelItem)
     │       │   └─ DataSetItem
     │       ├─ ReportBlock
     │       ├─ GooseControlBlock (QObject)
     │       └─ SV_ControlBlock (QObject)
     ├─ ReportStorage (per RCB, circular buffer)
     └─ LN_SignalMatrix (tabular view of LN signals)
```

Key classes:
- **`ModelItem`** — Base class. Parent/child tree via `QSharedPointer`. Holds `ModelItemValue`.
- **`DataModelBuilder`** — Fluent builder API used by adapter to construct the model.
- **`ModelStateUpdater`** — Batches value updates for safe GUI-thread execution.
- **`ReportStorage`** — Mutex-guarded circular buffer (100 reports) per RCB.

Value update notification path:
```
ModelItem::updateValue() → notifyFromChild() → parent chain
  → LogicalNode::sigDataObjectUpdated / LogicalDevice::sigDataObjectUpdated
    → connected table models call slotDataUpdated()
```

### 3. Command Layer (`src/cmd/` -> `cmd_lib`)

Async operations on the IED via a dedicated worker thread:

- **`CmdThread`** — `QThread` running a command loop, consuming from `CommandQueue`.
- **`CommandQueue<T>`** — Thread-safe queue (mutex + condition variable, 100ms poll).
- **`CmdInterface`** — Base class. `execute(api)` runs on the worker thread. Emits `sigCmdEvent(CmdEvent)`.
- **`CmdEvent`** — Progress/completion messages: `START_EVENT`, `PROCESS_EVENT`, `FINISH_EVENT`.

Commands:

| Command | Purpose |
|---|---|
| `ConnectCmd` / `DisConnectCmd` | Connection lifecycle |
| `UpdateLDs_StatusCmd` | Read status signals for all LDs |
| `UpdateLNs_StatusCmd` | Read status for a specific LD's LNs |
| `UpdateDatasetCmd` | Read dataset values |
| `UpdateRCBsCmd` | Refresh RCB state |
| `SetRCBValues_Cmd` | Configure and enable/disable RCBs |
| `UpdateGooseCBsCmd` / `UpdateSVCBsCmd` | Fetch GOOSE/SV control blocks |
| `SetGooseEnable_Cmd` / `SetSVEnable_Cmd` | Toggle GOOSE/SV publishing |
| `ControlCmd` | Direct control / SBO operations |
| `GetFilelistCmd` / `DownloadFileCmd` / `RemoveFileCmd` | File system |
| `UpdateWatchlistCmd` | Batch-read watchlist item values |
| `MMSDumpCmd` | Export model to file |

### 4. Models Layer (`src/models/` -> `models_lib`)

Qt models (`QAbstractTableModel` / `QAbstractItemModel`) exposing core data to QML:

**Tree models:**
- `IED_ModelTree` — Full IED data model as tree (Name, Value, FC columns)
- `TreeFilterProxy` — Recursive `QSortFilterProxyModel` with dot-path matching
- `LN_CommonTree` — Hierarchical DO/DA tree for selected LN

**Table models:**
- `LD_OverviewGrid` — Grid of logical devices with status LEDs
- `LN_AllOverviewTable` — Flat LN list across all LDs with filter
- `LN_SignalTable` — State/Controls/Settings signal matrix for selected LN
- `DS_OverviewTable` / `DS_SignalsTable` — Dataset list and signal values
- `RCB_OverviewTable` — BRCB/URCB overview with enable/config fields
- `ReportsTable` — Received reports per RCB
- `GooseOverviewTable` / `SV_OverviewTable` — Control block overviews
- `WatchlistModel` — User-defined monitored items
- `IED_FileTable` — IED file system listing
- `HistConTable` / `AppEventsTable` — Connection history and event log

**Sorting/filtering:**
- `SortProxyModel` — Custom `QSortFilterProxyModel` for sortable tables

### 5. Application Layer (`src/app/` -> `app_lib`)

Orchestration and QML bridge:

- **`MainPresenter`** — Top-level QObject exposed to QML as `presenter`. Owns all backends. Manages connection lifecycle, emits `sigCmdProgress`, `sigConnectionError`, `sigIEDConChanged`.
- **`IED_Backend`** — Creates and wires 15+ models. Exposes `Q_INVOKABLE` methods for data refresh, control operations, watchlist management. Connected to QML as `iedBackend`.
- **`IED_FS_Backend`** — File operations and download progress. Connected as `fsBackend`.
- **`AppBackend`** — App-level: event log, connection history, version info, settings. Connected as `appBackend`.
- **`BackendInterface`** — Base class providing `putCmdToQueue()` and common `slotCmdEvent()` handling.
- **`IEDConContainer`** — Holds all per-connection state: `IED`, `IEDCredentials`, `IEC61850_API`, `CmdThread`.
- **`AppSettings`** — XML config persistence (`iedclient_config.xml`): connection history, per-device watchlist.

### 6. QML UI (`ui/`)

Feature-organized QML with shared components:

```
ui/
  main.qml                  Root window: toolbar, tabs, page stack, status bar
  singletons/
    Globals.qml              Page/Panel enums, table helpers, clipboard
    VisualStyle.qml           Colors, fonts, dimensions
  common/
    ModalDialog.qml           Reusable modal (double border, overlay)
    TextDelegate.qml          Table cell with click/double-click/ctrl-click
    TableHeader.qml           Standard table header
    CustomTabBar.qml          Horizontal/vertical tab bar
    Led.qml, ToolBarButton.qml, SubWindow.qml, AppStatusBar.qml
  home/Page.qml              Connection dialog + history list
  logical_device/Page.qml    LD grid with status LEDs
  logical_node/Page.qml      LN list + DO/Controls/Settings tab views
  dataset/Page.qml           Dataset list + signal values table
  reports/Page.qml           BRCB/URCB/GOOSE/SV tabs + received reports
  iedtree/Page.qml           Full model tree + watchlist panel
  filesystem/Page.qml        File listing with download/delete
```

**Navigation:** 7 tabs in a `StackLayout` (`HOME=0, LD=1, LN=2, DS=3, RCB=4, IED_TREE=5, FS=6`). Tabs hidden before first connection. Property panels in a separate `StackLayout` on the right side of a `SplitView`.

## Threading Model

```
┌──────────────────────────────────────┐
│           GUI Thread (main)          │
│  QML ↔ MainPresenter ↔ Backends     │
│  Models ↔ ModelItem value updates    │
│  Signal/slot dispatch                │
└──────────────┬───────────────────────┘
               │ putCmdToQueue()
               │ sigCmdEvent (queued connection)
┌──────────────▼───────────────────────┐
│         CmdThread (worker)           │
│  CommandQueue → CmdInterface::execute│
│  Calls API adapter methods           │
└──────────────┬───────────────────────┘
               │
┌──────────────▼───────────────────────┐
│    libiec61850 internal threads      │
│  Report callbacks, connection close  │
│  Data copied to QString before return│
└──────────────────────────────────────┘
```

- **GUI thread**: All QML, model updates, signal/slot dispatch.
- **CmdThread**: Worker executing IED commands sequentially. Reports back via `sigCmdEvent`.
- **libiec61850 threads**: Network I/O and callbacks. Data must be copied to Qt types inside callbacks (pointers invalid after return).

## Build System

CMake with Qt6. Static libraries linked into one executable:

```
3rdparty/
  libiec61850/          Git submodule, C library
  googletest/           Git submodule, test framework
  breakpad/             Git submodule, crash reporting

src/
  core/       → core_lib
  cmd/        → cmd_lib
  models/     → models_lib
  app/        → app_lib
  tools/      → tools_lib
  libiec61850_stub/ → lib61850_adapter
  tests/      → unit_tests (gtest)

ui/           → qt6_add_resources (QML, fonts, icons)
```

Build via Docker: `./ci/build_local.sh [--release|--debug|--check] [--archive]`

Output: `dist/*.AppImage` (self-contained Linux binary).

## Configuration

`iedclient_config.xml` — XML file storing connection history and per-device watchlist:

```xml
<app>
  <main/>
  <history_connections>
    <device ip="..." port="..." tls="..." ied="..." date="...">
      <watchlist>
        <node ref="LD0/LLN0.Mod" fc="ST"/>
      </watchlist>
    </device>
  </history_connections>
  <history_scl_files/>
</app>
```

Location: `~/.config/OSI/IEDClient/` (Linux) or app directory.
