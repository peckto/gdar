Name:		gdar
Version:	0.9
Release:	1%{?dist}
Summary:	A graphical user interface to brows and extract dar archives

Group:		Applications/Archiving
License:	GPLv3
URL:		https://github.com/peckto/gdar
Source0:	%{name}-%{version}.tar.gz

BuildRequires:	intltool, make, gcc-c++, gtkmm30-devel, boost-regex, libattr-devel, libgcrypt-devel, lzo-devel, bzip2-devel, zlib-devel, libdar-devel >= 5.3.0
Requires:	gtkmm30, libattr, libgcrypt, lzo, bzip2-libs, zlib, libdar

%description
A graphical user interface to brows and extract dar archives.


%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
make install DESTDIR=%{buildroot}
%find_lang %{name}


%files
%{_bindir}/%{name}
%{_defaultdocdir}/%{name}/
%{_datadir}/locale/de/LC_MESSAGES/%{name}.mo
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/256x256/apps/%{name}.png
%doc



%changelog
* Thu Mar 20 2014 Tobias Specht <specht.tobias@gmx.de> - 0.9-1
- Initial creation

