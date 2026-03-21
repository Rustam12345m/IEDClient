# IEDClient - An Open-Source Client for IEC 61850 Protocols

## What is IEDClient?

IEDClient is an open-source application designed for interaction with IED devices using IEC 61850 protocols. Although there are a few applications serving this purpose, in 2023, I decided to create a new one with a focus on the following features:

- Enhanced observability for the data model.
- Capability to interact with huge models, with numerous logical nodes and multiple logical devices.
- A user-friendly and efficient interface tailored to my needs (:

## License

GPL-v3

## Platforms

The application targets the following platforms:

- Microsoft Windows
- Linux distributions
- Mobile platforms such as Android

## Installation Instructions


## Third-Party Dependencies

This project uses several third-party libraries to enhance functionality:

- Qt
- libiec61850
- Google Breakpad
- Google Test
- PaperColor theme
- Material Icons

## Usage Guidelines

The user's guide is currently in development. Users will be able to access the guide using the F1 key within the application.

## Crash Reporting

IEDClient uses [Google Breakpad](https://chromium.googlesource.com/breakpad/breakpad) for automatic crash dump generation. Crash dumps are saved to `~/.local/share/IEDClient/crashes/`. Symbol archives are published alongside release builds.

See [docs/crash_analysis.md](docs/crash_analysis.md) for instructions on analyzing crash dumps.

## Contributing

## Issues

This is an open-source application provided 'as is'. If you encounter any issues, please feel free to create an issue on the GitHub page, including the following information:

1. Application version.
2. Screenshots, if applicable.
3. A detailed sequence of actions leading to the issue.

