Name:           iedclient
Version:        %{pkg_version}
Release:        %{pkg_release}%{?dist}
Summary:        IEC 61850 IED client application
License:        GPL-3.0-only

Requires:       qt6-qtbase >= 6.4
Requires:       qt6-qtdeclarative >= 6.4
Requires:       qt6-qtquickcontrols2 >= 6.4
Requires:       libxkbcommon

%description
An open-source application for interacting with IED devices
using IEC 61850 protocols. Provides enhanced observability for
the data model and supports large configurations with numerous
logical nodes and multiple logical devices.

%install
install -Dm755 %{install_dir}/bin/IEDClient       %{buildroot}/usr/bin/IEDClient
install -Dm644 %{script_dir}/iedclient.desktop    %{buildroot}/usr/share/applications/iedclient.desktop

%files
/usr/bin/IEDClient
/usr/share/applications/iedclient.desktop

%changelog
* Mon Mar 09 2026 IEDClient Project <> - 0.0.1-1
- Initial package
