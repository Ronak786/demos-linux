#!/usr/bin/python3

# this script will install all the required packages that you need on
# ubuntu to compile and work with this package.

# TODO:
# - enable the dbgsym package after this script also knows how to add the
# apt repository to the machine...
# - the papi library is not supported by this procedue. have this procedue
# install the papi library for me (either as .deb from my own repository
# or from source).
# - the intel compiler auto-installation is not supported.

import subprocess
import os

release=os.uname().release
short_release=release[:release.rfind('-')]
release_lowlatency='3.11.0-11'
release_generic='3.11.0-12'
#print('release is',release)
#print('short_release is',short_release)
boost_version='1.53.0'
boost_version_short='1.53'

packs=[
	'libncurses5',
	'libncurses5-dev',
	'libncursesw5',
	'libncursesw5-dev',
	'ncurses-doc',
	'libprocps0-dev',
	'libgnomeui-dev',
	'libsigc++-2.0-0c2a',
	'libsigc++-2.0-dev',
	'libsigc++-2.0-doc',
	#'libgnomeuimm-2.6-dev',
	'libpq-dev',
	'liblog4cpp5-dev',
	'binutils-dev',
	'libmysqlclient-dev',
	'libcwd',
	'libqt4-dev',
	'libwxgtk2.8-dev',
	'libmysql++-dev',
	'libsdl1.2-dev',
	'dialog',
	'libace-dev',
	'iptables-dev',
	'libboost'+boost_version_short+'-dev', # for boost threading
	'libboost-thread'+boost_version, # for the actual library
	'libpcap-dev',
	'libasound2',
	'libasound2-dev',
	'libasound2-doc',
	'libdmalloc5',
	'libdmalloc-dev',
	'libcpufreq-dev', # for cpufreq.h
	'aspectc++',
	'libnetfilter-queue1',
	'libnetfilter-queue-dev',
	'libcap-dev', # for capability.h
	'liburcu-dev', # rcu library
	'liburcu1', # rcu library

	# kernel stuff
	'linux-headers-generic',
	'linux-headers-lowlatency',
	'linux-headers-'+release_generic+'-generic',
	'linux-headers-'+release_lowlatency+'-lowlatency',

	# for systemtap - this one does not exist in the regular ubuntu archives
	# ubuntu does not always carry the "-lowlatency" dbgsym and that why
	# it is commented out...
	# TODO: make this script add sources to the dbgsyms if need be
	#'linux-image-'+release_generic+'-generic'+'-dbgsym',
	#'linux-image-'+release_lowlatency+'-lowlatency'+'-dbgsym',

	# tools
	'linux-tools-common', # for perf(1)
	'libreoffice-common', # for soffice conversion
	'python3-uno', # for soffice conversion
	'electric-fence',
	'schedtool',
	'iotop', # for the iotop command
	'jnettop', # for the jnettop command
	'htop',
	'sysprof',
	'mutrace',
	'cpulimit', # for the cpulimit command
	'bridge-utils', # for bridging utilities to demo creation of a bridge
	'iptraf',
	'numactl',
	'pstack',
	'x86info',
	'lsscsi',
	'chrpath',
	'latencytop',

	# package used tools
	'ccache', # for faster building
	'xutils-dev', # for makedepend(1)
	'uncrustify', # for code formatting example
	'indent', # for code formatting example
	'astyle', # for code formatting example

	# these packages are for working with linuxapi...:)
	'vim',
]

args=['sudo','apt-get','install']
args.extend(packs)
subprocess.check_call(args)
