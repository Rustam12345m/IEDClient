# IEDClient

An open-source desktop client for IEC 61850 Intelligent Electronic Devices.

IEDClient connects to IEDs via MMS and provides a visual interface for browsing the data model, monitoring real-time values, controlling equipment, and managing report/GOOSE/SV control blocks.

## Features

- **Data model browser** — Logical Devices, Logical Nodes, Data Objects with full attribute tree
- **Real-time monitoring** — Live values for ST/MX attributes with quality and timestamps
- **Watchlist** — Pin attributes from any LN for side-by-side monitoring, persisted per device
- **Control operations** — Direct Control and SBO (Select-Before-Operate) with Normal and Enhanced Security, CommandTermination feedback, test/interlock/synchrocheck flags
- **MMS write** — Edit SP/SE/CF/DC attribute values via double-click in the Settings tab
- **Setting Groups (SGCB)** — Browse and switch active setting groups, edit sessions with confirm/cancel
- **Report Control Blocks** — Enable/disable BRCB and URCB, receive and display reports
- **GOOSE / SV control blocks** — View configuration, enable/disable publishing
- **Datasets** — Browse dataset contents with live value updates
- **File system** — Browse, download, and delete IED files with progress indication
- **IED Tree** — Full hierarchical view with text filter
- **Event log** — All operations logged with timestamps
- **Crash reporting** — Breakpad integration with symbol archives for post-mortem analysis

## Platforms

- Linux (AppImage, .deb, .rpm)
- Windows (portable zip)

## Building

The project uses CMake, Qt 6, and builds inside Docker — no host dependencies needed besides Docker/Podman.

```bash
# Initialize submodules
git submodule update --init --recursive

# Build + test + package (default)
./ci/build_local.sh

# Other build modes
./ci/build_local.sh --check             # Debug + sanitizers + linter + tests
./ci/build_local.sh --rebuild --check   # Incremental rebuild + tests
./ci/build_local.sh --debug             # Debug build
./ci/build_local.sh --rebuild-image     # Force rebuild the Docker image
```

Output:
- `dist/` — AppImage (Linux portable binary)
- `build/src/tests/unit_tests` — Test binary (run via `--check`)

## Third-Party Libraries

| Library | Purpose |
|---------|---------|
| [Qt 6](https://www.qt.io/) | UI framework (QML/Quick) |
| [libiec61850](https://libiec61850.com/) | IEC 61850 MMS client implementation |
| [Google Breakpad](https://chromium.googlesource.com/breakpad/breakpad) | Crash dump generation |
| [Google Test](https://github.com/google/googletest) | Unit testing framework |
| [Noto Sans Mono](https://fonts.google.com/noto/specimen/Noto+Sans+Mono) | Embedded monospace font |

## Logs and Crash Reporting

IEDClient writes both kinds of forensic data to the same per-user data directory:

| Platform | Location |
|----------|----------|
| Linux    | `~/.local/share/IEDClient/` |
| Windows  | `%LOCALAPPDATA%\IEDClient\` |

Inside that directory:

- `logs/iedclient.log` — running log of connection events, command failures, and warnings. Rotates to `iedclient.log.1` once it crosses 1 MB; only the most recent two files are kept.
- `crashes/*.dmp` — Breakpad minidumps written if the app crashes. Symbol archives are published alongside each release for offline analysis.

See [docs/crash_analysis.md](docs/crash_analysis.md) for crash dump analysis instructions.

## Documentation

- [Crash Analysis](docs/crash_analysis.md)

## Issues

If you encounter a bug, please [open an issue](../../issues) with:

1. Application version (shown in the About dialog: F1 or Ctrl+H)
2. Steps to reproduce
3. The latest `logs/iedclient.log` (and `iedclient.log.1` if present)
4. Any `crashes/*.dmp` if the app crashed
5. Screenshots if applicable

## License

[GPL-v3](COPYING)
