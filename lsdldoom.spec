%define doomprefix /usr
%define version 1.5
%define doomname lsdldoom
%define doomrelease 0

Name: %{doomname}
Summary: Doom - classic 3D shoot-em-up game
Version: %{version}
Release: %{doomrelease}
Source: http://firehead.org/~jessh/lsdldoom/src/lsdldoom-1.5.tar.gz
URL: http://firehead.org/~jessh/lsdldoom/
Group: Amusements/Games
BuildRoot: /var/tmp/%{doomname}-buildroot
Copyright: GPL
Prefix: %{doomprefix}

%description
Doom is the classic 3D shoot-em-up game. It must have been one of the best
selling games ever; it totally outclassed any  3D world games that preceded
it, with amazing speed, flexibility, and outstanding gameplay. The specs to
the game were released, and thousands of extra levels were written by fans of
the game; even today new levels are written for Doom faster then any one person
could play them. 

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q

%build
export DOOMDIR=%{doomprefix}
./configure \
	--prefix=$DOOMDIR --disable-cpu-opt \
	--with-install-root=$RPM_BUILD_ROOT

make CXXFLAGS="$RPM_OPT_FLAGS -DNO_DEBUG -DNDEBUG"

%install
make install-strip prefix=$RPM_BUILD_ROOT%{doomprefix}

cd $RPM_BUILD_ROOT
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > \
	$RPM_BUILD_DIR/file.list.%{doomname}

find . -type f | sed -e 's,^\.,\%attr(-\,root\,root) ,' \
	-e '/\/config\//s|^|%config|' >> \
	$RPM_BUILD_DIR/file.list.%{doomname}

find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >> \
	$RPM_BUILD_DIR/file.list.%{doomname}

#echo "%docdir /usr/doc/doom" >> $RPM_BUILD_DIR/file.list.%{doomname}

%clean
rm -rf $RPM_BUILD_ROOT $RPM_BUILD_DIR/file.list.%{doomname}

%files -f ../file.list.%{doomname}

%changelog
* Thu Mar 14 2000 Colin Phipps <cph@lxdoom.linuxgames.com>
- Merge rpm spec file into official distribution
- Integrate with autoconf stuff
- This is the last changelog entry going here

* Thu Oct 21 1999 Mattias Kunkel <mattias@kunkel.freeservers.com>
- updated to Version 1.4.1

* Thu Oct 14 1999 Mattias Kunkel <mattias@kunkel.freeservers.com>
- made intial RPM
