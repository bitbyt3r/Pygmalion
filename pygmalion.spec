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

%{?python_enable_dependency_generator}

%description
Pygmalion allows you to produce 3D models of real objects using one or more cameras.

%prep
{{{ git_dir_setup_macro }}}

%build
%py3_build

%install
%py3_install

%files
/usr/lib/systemd/system/pygmalion.service
/usr/bin/pygmalion
/usr/bin/pygmalion-server
%{python3_sitearch}/pygmalion/
%{python3_sitearch}/pygmalion-*.egg-info/

%changelog
{{{ git_dir_changelog }}}