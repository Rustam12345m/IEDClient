# Technical Decisions

Key design choices and their rationale.

## Language & Framework

**C++20 + Qt6/QML** was chosen because:
- Qt provides the cross-platform GUI framework, QML for declarative UI
- C++20 for structured bindings, ranges, concepts
- libiec61850 is a C library — C++ wrapping is natural
- Qt's model/view architecture maps well to IEC 61850 hierarchical data

## Architecture: Layered with Abstract Interfaces

The app uses 6 layers with strict dependency direction:

```
adapter ← core ← cmd ← models ← app ← executable
```

**Why abstract API interfaces (`cmd/interface/`)?**
- Decouples the command layer from the concrete libiec61850 adapter
- Enables mock-based unit testing without a real IED connection
- If the underlying library changes, only the adapter needs updating

**Why separate core from models?**
- Core is pure domain logic (IEC 61850 object hierarchy), no Qt model dependencies
- Models are Qt-specific (`QAbstractTableModel`), depend on core but not the other way
- Core can be reused in a non-GUI context (CLI tools, headless)

## Command Queue Pattern

All IED operations run through a single `CmdThread` worker with a `CommandQueue`:

**Why not direct API calls from the GUI thread?**
- libiec61850 calls are blocking (network I/O). GUI would freeze.
- Single command queue ensures sequential execution — avoids concurrent access to the libiec61850 connection object (which is not thread-safe).
- `CmdEvent` signals report progress back to the GUI thread via Qt's queued connections.

**Why not a thread pool?**
- libiec61850's `IedConnection` is single-threaded — concurrent calls would corrupt state.
- Sequential execution is correct for MMS protocol semantics.

## Composite Tree for Data Model

The IEC 61850 data model is a natural tree:
```
IED → LD → LN → DO → DA → SubAttribute
```

`ModelItem` is the base class with parent/child navigation via `QSharedPointer`. This enables:
- Unified tree display in `IED_ModelTree`
- Path-based lookups via `getReference()` / `recFindModelItem()`
- Value updates that bubble up via `notifyFromChild()` to table models

**Why QObject dual inheritance for LogicalNode/LogicalDevice/DataSet?**
- These nodes need Qt signals (`sigDataObjectUpdated`, `sigItemUpdated`) to notify models
- Leaf nodes (DataAttribute, SubAttribute) don't need signals — parent notification is sufficient
- QObject overhead only where signals are required

## FC-Aware Model Building

**Problem:** A DataObject can have multiple DataAttributes with the same name but different Functional Constraints (e.g., `phsA[MX]` and `phsA[DC]`). Standard `getDataDirectory()` returns all sub-attributes regardless of FC, causing mixed children.

**Solution:** `recursiveReadAttributes()` uses `IedConnection_getDataDirectoryByFC()` to only discover sub-attributes matching the parent's FC. Each DA node only contains children from its own functional constraint.

**Path resolution:** `recFindModelItem()` uses backtracking — if the first `phsA` child doesn't match the expected sub-path, it tries the next `phsA` sibling with a different FC.

## Value Notification: Pointer-Based, Not Name-Based

`ModelItem::updateValue()` → `notifyFromChild()` traverses the parent pointer chain. This avoids ambiguity from duplicate DA names (same name, different FC). The notification reaches `LogicalNode`/`LogicalDevice` which emit `sigDataObjectUpdated`, connected to table models.

**Dataset exception:** `DataSetItem::m_valItem` points into the main tree (under LogicalNode). `notifyFromChild` bubbles through the LN hierarchy, never reaching `DataSet`. Dataset models must be notified explicitly after value updates.

## Report Handler Lifecycle

libiec61850 report callbacks run on an internal network thread. Design constraints:
1. **Install handler before enabling RCB** — no reports missed
2. **Copy MMS data to QString inside callback** — pointers invalid after callback returns
3. **`sigReportReceived` with `Qt::QueuedConnection`** — marshal to GUI thread
4. **Uninstall handlers before `IedConnection_abort()`** — prevent dangling callback pointers
5. **`ReportStorage` is mutex-guarded** — safe for callback thread writes and GUI thread reads

## QML Integration via Context Properties

`MainPresenter::setQmlContextMembers()` exposes `presenter`, `appBackend`, `iedBackend`, `fsBackend` as QML context properties. Models are accessed via `Q_INVOKABLE` getters (e.g., `iedBackend.getLD_GridModel()`).

**Why context properties over QML modules?**
- Simpler setup for a single-window application
- All backends are singletons anyway — no need for component instantiation
- `Q_INVOKABLE` methods are callable directly from QML without additional registration

**QML singletons** (`Globals.qml`, `VisualStyle.qml`) are registered via `qmlRegisterSingletonType` in `main.cpp`. They hold shared helpers and theme constants.

## Global Shortcut for F5 Refresh

A `Shortcut { sequence: "F5" }` at the `ApplicationWindow` level intercepts F5 globally. The `updateActivePage()` function dispatches to the correct backend refresh method based on the active page.

**Why global Shortcut instead of per-page `Keys.onPressed`?**
- `Shortcut` bypasses the QML focus chain — works regardless of which child has focus (TextField, CheckBox, Button, etc.)
- Per-page `Keys.onPressed` handlers are unreachable when focus is on an input element
- Single dispatch point avoids duplicated F5 handling across pages

## XML Config for Persistence

Connection history and per-device watchlist are stored in `iedclient_config.xml` using `QXmlStreamReader/Writer`.

**Why XML over QSettings/INI/JSON?**
- Hierarchical structure: watchlist nodes nested inside device entries
- QXmlStream is built into Qt (no extra dependency)
- Human-readable for manual inspection
- Existing codebase convention — the config was XML from the start

**Watchlist per device:** Each `<device>` entry contains a `<watchlist>` with `<node ref="" fc=""/>` children. Watchlist is saved on every add/remove/clear and restored on reconnect to the same IED (matched by IP/port/TLS).

## Embedded Font (Noto Sans Mono)

The app embeds Noto Sans Mono via Qt resources and sets it as the app-wide default font.

**Why embed?**
- Monospace font essential for aligned table data and MMS references
- System monospace fonts vary across Linux distributions
- Consistent rendering on all platforms without runtime font dependency

## Crash Reporting (Breakpad)

Google Breakpad generates minidump files on crash.

**Why Breakpad over Qt's built-in crash handling?**
- Produces compact binary dumps with full stack traces
- Works with stripped release binaries (symbols extracted separately)
- Supports both Linux and Windows with the same API
- Exception handler initialized before QGuiApplication — catches early crashes

**Symbol management:** `dump_syms` extracts symbols from release builds. Shipped separately as ZIP artifacts. Crash dumps can be analyzed offline without the original binary.

## Docker-Based Build

All builds run inside a Docker container (`ci/build_local.sh`).

**Why containerized builds?**
- Reproducible environment regardless of host OS/distro
- Qt6 dependency pinned to specific Ubuntu packages
- linuxdeploy for AppImage packaging requires specific tooling
- clang-tidy and sanitizers configured in the container
- Same environment for local development and CI (GitHub Actions)

## AppImage Distribution

The primary distribution format is AppImage (self-contained Linux binary).

**Why AppImage?**
- Single file, no installation required
- Bundles Qt6 libraries and plugins — works on any modern Linux
- Users can run directly after download (chmod +x)
- linuxdeploy handles Qt plugin bundling automatically

## Multi-Row Selection Pattern (`selVer` Counter)

QML `TableView.selectionModel.isSelected()` is not reactive — property bindings don't update when selection changes.

**Solution:** A `selVer` counter on the TableView increments on every `selectionChanged`. Delegates reference `selVer` in their `selected` binding, forcing re-evaluation:
```qml
selected: { tableID.selVer; return tableID.selectionModel.isSelected(...) }
```

## `forceLayout()` After `dataChanged`

Qt6 `TableView` may not repaint visible delegates after `dataChanged` alone — cached delegates show stale values.

**Solution:** A `Connections` block on the model calls `tableID.forceLayout()` on `onDataChanged`, forcing delegate re-query. Applied to all tables that receive live value updates.
