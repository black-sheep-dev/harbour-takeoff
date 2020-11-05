# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.32
# 

Name:       harbour-takeoff

# >> macros
# << macros

Summary:    Takeoff
Version:    0.6.10
Release:    1
Group:      Qt/Qt
License:    GPLv3
URL:        https://github.com/black-sheep-dev/harbour-takeoff
Source0:    %{name}-%{version}.tar.bz2
Source100:  harbour-takeoff.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils

%description
Takeoff is an autostart manager for native Sailfish apps


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qmake5 

make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
%post
if [[ ! -d "/home/nemo/.config/harbour-takeoff" ]]
then
mkdir /home/nemo/.config/harbour-takeoff
touch /home/nemo/.config/harbour-takeoff/takeoff.def
fi

echo "#!/bin/bash" > /home/nemo/.config/harbour-takeoff/takeoff.sh
echo "/usr/bin/invoker -n -s --type=silica-qt5 /usr/bin/harbour-takeoff --takeoff" >> /home/nemo/.config/harbour-takeoff/takeoff.sh
chmod a+x /home/nemo/.config/harbour-takeoff/takeoff.sh
chown -R nemo:nemo /home/nemo/.config/harbour-takeoff

if [ $1 -eq 1 ]; then
# Package install,
systemctl-user enable org.nubecula.takeoff.service >/dev/null 2>&1 || :
#systemctl-user start org.nubecula.takeoff.service > /dev/null 2>&1 || :
else
# Package upgrade
if systemctl-user --quiet is-enabled org.nubecula.takeoff.service ; then
systemctl reenable org.nubecula.takeoff.service >/dev/null 2>&1 || :
fi
fi

# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%preun
# >> preun
if [ "$1" = 0 ]; then
systemctl-user stop org.nubecula.takeoff.service  > /dev/null 2>&1 || :
systemctl-user disable org.nubecula.takeoff.service  > /dev/null 2>&1 || :
fi

#systemctl-user stop org.nubecula.takeoff.service || true
#systemctl-user disable org.nubecula.takeoff.service || true
#systemctl-user daemon-reload
if /sbin/pidof harbour-takeoff > /dev/null; then
killall harbour-takeoff || true
fi
# << preun

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
%{_libdir}/systemd/user/org.nubecula.takeoff.service
%{_datadir}/%{name}/translations
# >> files
# << files
