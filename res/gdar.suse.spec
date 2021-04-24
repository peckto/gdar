Name:		gdar
Version:	0.9
Release:	1%{?dist}
Summary:	A graphical user interface to browse and extract dar archives

Group:		Applications/Archiving
License:	GPL-3.0+
URL:		https://github.com/peckto/gdar
Source0:	%{name}-%{version}.tar.gz

BuildRequires:	intltool, make, gcc-c++, gtkmm3-devel, libattr-devel, libgcrypt-devel, lzo-devel, zlib-devel, libdar-devel
#Requires:	gtkmm3, libattr, libgcrypt, lzo, zlib, libdar

%description
A graphical user interface to browse and extract dar archives.


%prep
%setup -q


%build
%configure --docdir=%{_defaultdocdir}/%{name}
make %{?_smp_mflags}


%install
make install DESTDIR=%{buildroot}
%suse_update_desktop_file gdar
%find_lang %{name}

%files
%defattr(-,root,root)
%{_bindir}/%{name}
%{_defaultdocdir}/packages/%{name}/README.md
%{_datadir}/locale/de/LC_MESSAGES/%{name}.mo
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/256x256/apps/%{name}.png
%doc



%changelog
* Thu Mar 20 2014 Tobias Specht <specht.tobias@gmx.de> - 0.9-1
- Initial creation

