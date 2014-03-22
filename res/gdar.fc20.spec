Name:		gdar
Version:	0.9
Release:	1%{?dist}
Summary:	A graphical user interface to brows and extract dar archives

Group:		Applications/Archiving
License:	GPLv3
URL:		https://github.com/peckto/gdar
Source0:	%{name}-%{version}.tar.gz

BuildRequires:	intltool, make, gcc-c++, gtkmm30-devel, boost-regex, libattr-devel, libgcrypt-devel, lzo-devel, bzip2-devel, zlib-devel
Requires:	gtkmm30, libattr, libgcrypt, lzo, bzip2-libs, zlib

%description
A graphical user interface to brows and extract dar archives.


%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
make install DESTDIR=%{buildroot}


%files
%{_bindir}/%{name}
#%{_defaultdocdir}/%{name}/README.md
%{_datadir}/locale/de/LC_MESSAGES/%{name}.mo
%{_datadir}/applications/%{name}.desktop
%doc



%changelog

