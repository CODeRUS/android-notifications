Name:       android-notifications

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    Android notifications
Version:    0.6.0
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        https://github.com/CODeRUS/android-notifications
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(dconf)

%description
Simple daemon adding sound (IM category), led and icon on lockscreen for android notifications.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5  \
    VERSION=%{version}

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
mkdir -p %{buildroot}/usr/lib/systemd/user/post-user-session.target.wants
ln -s ../android-notifications.service %{buildroot}/usr/lib/systemd/user/post-user-session.target.wants/android-notifications.service
# << install post

desktop-file-install --delete-original --dir %{buildroot}%{_datadir}/applications %{buildroot}%{_datadir}/applications/*.desktop

%pre
# >> pre
systemctl-user stop android-notifications.service

if /sbin/pidof android-notifications > /dev/null; then
killall android-notifications || true
fi
# << pre

%preun
# >> preun
systemctl-user stop android-notifications.service

if /sbin/pidof android-notifications > /dev/null; then
killall android-notifications || true
fi
# << preun

%post
# >> post
systemctl-user restart ngfd.service
systemctl-user restart android-notifications.service
# << post

%files
%defattr(-,root,root,-)
%attr(4755, root, root) %{_bindir}/*
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png
%{_datadir}/dbus-1/services/org.coderus.androidnotifications.service
%{_libdir}/systemd/user/android-notifications.service
%{_libdir}/systemd/user/post-user-session.target.wants/android-notifications.service
%{_sysconfdir}/profiled/*.ini
%{_datadir}/ngfd/events.d/*.ini
# >> files
# << files
