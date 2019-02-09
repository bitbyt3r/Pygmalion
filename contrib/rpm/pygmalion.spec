Name: pygmalion
Version: {{{ git_dir_version }}}
Release: 1%{?dist}
Summary: System for controlling camera rigs and generating 3D models from photos
License: MIT
URL: https://photometry.io/
VCS: {{{ git_dir_vcs }}}
Source: {{{ git_dir_pack }}}
BuildRequires: python3-devel
%{?python_provide:%python_provide python3-pygmalion}
%{?python_enable_dependency_generator}

%prep
{{{ git_dir_setup_macro }}}

%build
%py3_build

%install
%py3_install

%files
/usr/lib/systemd/system/pygmalion.service
/usr/bin/pygmalion
%{python3_sitearch}/pygmalion/
%{python3_sitearch}/pygmalion-*.egg-info/

%changelog
{{{ git_dir_changelog }}}