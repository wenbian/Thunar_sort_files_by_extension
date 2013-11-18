Summary:		Thunar File Manager
Name:			Thunar
Version:		1.0.1
Release:		1
License:		GPL
URL:			http://thunar.xfce.org/
Source0:		%{name}-%{version}.tar.gz
Group:			Applications/X11
BuildRoot:		%{_tmppath}/%{name}-root
Requires:		fam
Requires:		libexif
Requires:		shared-mime-info >= 0.15
Requires:		desktop-file-utils >= 0.7
Requires:		exo0.3 >= 0.3.100
Requires:		startup-notification >= 0.4
BuildRequires:		fam-devel
BuildRequires:		libjpeg-devel
BuildRequires:		libexif-devel
BuildRequires:		libpng-devel >= 2:1.2.2-16
BuildRequires:		desktop-file-utils >= 0.7
BuildRequires:		exo0.3-devel >= 0.3.100
BuildRequires:		startup-notification-devel >= 0.4

Provides:		Thunar-devel

%description
Thunar is a new modern file manager for the Xfce Desktop Environment. It has been designed from
the ground up to be fast and easy-to-use. Its user interface is clean and intuitive, and does not
include any confusing or useless options. Thunar is fast and responsive with a good start up time
and directory load time.

%prep
%setup -q

%build
%configure --enable-dbus --enable-final --enable-xsltproc
make

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT mandir=%{_mandir}

%clean
rm -rf $RPM_BUILD_ROOT

%post
update-desktop-database &> /dev/null ||:
touch --no-create %{_datadir}/icons/hicolor || :
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
   %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
fi

%postun
update-desktop-database &> /dev/null ||:
touch --no-create %{_datadir}/icons/hicolor || :
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
   %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
fi

%files
%defattr(-,root,root)
%doc README TODO ChangeLog NEWS INSTALL COPYING AUTHORS HACKING THANKS
%{_bindir}/
%{_datadir}/
%{_includedir}/
%{_libdir}/
%{_libexecdir}/
%{_mandir}/
%{_sysconfdir}/
