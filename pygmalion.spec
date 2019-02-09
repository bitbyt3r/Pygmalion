Name: {{{ git_name pygmalion }}}
Version: {{{ git_dir_version }}}
Release: 1%{?dist}
Summary: System for controlling camera rigs and generating 3D models from photos
License: MIT
URL: https://photometry.io/
VCS: {{{ git_dir_vcs }}}
Source: {{{ git_dir_pack }}}
BuildRequires: python3-devel
BuildRequires: libusb-devel
BuildRequires: gcc
BuildRequires: npm

%{?python_enable_dependency_generator}

%description
Pygmalion allows you to produce 3D models of real objects using one or more cameras.

%prep
{{{ git_dir_setup_macro }}}

%build
%py3_build
cd ./frontend
npm install
npm run build

%install
%py3_install
mkdir -p %{buildroot}/usr/lib/systemd/system
cp contrib/pygmalion-server.service %{buildroot}/usr/lib/systemd/system/
mkdir -p %{buildroot}/usr/share/pygmalion/www
cp frontend/dist/pygmalion.js %{buildroot}/usr/share/pygmalion/www/
cp frontend/dist/pygmalion.js.map %{buildroot}/usr/share/pygmalion/www/
cp frontend/index.html %{buildroot}/usr/share/pygmalion/www/
mkdir -p %{buildroot}/usr/share/pygmalion/migrations
cp migrations/* %{buildroot}/usr/share/pygmalion/migrations/
mkdir -p %{buildroot}/usr/share/pygmalion/scripts
cp scripts/* %{buildroot}/usr/share/pygmalion/scripts/
mkdir -p %{buildroot}/etc/pygmalion
cp contrib/conf.json %{buildroot}/etc/pygmalion/

%files
%config /etc/pygmalion/conf.json
/usr/lib/systemd/system/pygmalion-server.service
/usr/bin/pygmalion
/usr/bin/pygmalion-server
/usr/share/pygmalion
%{python3_sitearch}/pygmalion/
%{python3_sitearch}/pygmalion-*.egg-info/

%changelog
{{{ git_dir_changelog }}}