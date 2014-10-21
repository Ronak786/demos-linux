#!/usr/bin/python3

'''
this script will install all the required packages that you need on
ubuntu to compile and work with this package.
'''

# TODO:
# - enable the dbgsym package after this script also knows how to add the
# apt repository to the machine...
# - the intel compiler auto-installation is not supported.

import subprocess # for check_call
import os # for uname

##############
# parameters #
##############
boost_version='1.54.0'
boost_version_short='1.54'
debug=False
do_kernel=True

########
# code #
########
release=os.uname().release
short_release=release[:release.rfind('-')]
source_release=short_release[:short_release.rfind('-')]
release_lowlatency=short_release
release_generic=short_release
if debug:
	print('release is',release)
	print('short_release is',short_release)
	print('source_release is',source_release)
	print('release_lowlatency is',release_lowlatency)
	print('release_generic is',release_generic)

packs=[
        # most important - compiler parts
        'gcc-doc', # gcc documentation
        'cpp-doc', # gcc documentation

	# manual pages
	'manpages',
	'manpages-dev',
	'manpages-posix',
	'manpages-posix-dev',
	'gnulib',
	'bikeshed',

	# debugging
	'cgdb',
	'xxgdb',
	'qtcreator',

	# code measurements
	'sloccount',
	'cloc',

        # basic tools
	'binutils-dev', # for demangle.h
	'binutils-doc', # for demangle.h
	'aspectc++',
	'dialog',
	'iptables-dev',
	'unoconv',

        # libraries
        'libiberty-dev',
	'libncurses5',
	'libncurses5-dev',
	'libncursesw5',
	'libncursesw5-dev',
	'ncurses-doc',
	'libprocps3',
	'libprocps3-dev',
	'libgnomeui-0',
	'libgnomeui-dev',
	'libgnomeui-doc',
	'libsigc++-2.0-0c2a',
	'libsigc++-2.0-dev',
	'libsigc++-2.0-doc',
	'libgtkmm-2.4-dev',
	'libgtkmm-2.4-doc',
	'libgtkmm-3.0-dev',
	'libgtkmm-3.0-doc',
	'libpq-dev',
	'liblog4cpp5-dev',
	'libmysqlclient-dev',
	'libcwd',
	'libqt4-dev',
	'libwxgtk2.8-dev',
	'libmysql++-dev',
	'libsdl1.2-dev',
	'libace-dev',
	'libboost'+boost_version_short+'-dev', # for boost threading
	'libboost-thread'+boost_version, # for the actual library
	'libpcap-dev',
	'libasound2',
	'libasound2-dev',
	'libasound2-doc',
	'libdmalloc5',
	'libdmalloc-dev',
	'libcpufreq-dev', # for cpufreq.h
	'libnetfilter-queue1',
	'libnetfilter-queue-dev',
	'libcap-dev', # for capability.h
	'liburcu1', # rcu library
	'liburcu-dev', # rcu library
	'libunwind8', # unwind library
	'libunwind8-dev', # unwind library
	'libelf1', # for reading elf files
	'libdw1',
	'libdw-dev',
	'libasm1',
	'libasm-dev',
	'libstdc++6-4.4-doc',
	'aspell-doc',
	'libaspell-dev',
	'libacl1-dev',
	'libattr1-dev',

	# compilers
	'g++-4.4',
	'gcc-4.6-plugin-dev',
	'gcc-4.7-plugin-dev',
	'gcc-4.8-plugin-dev',
	'gcc-4.6',
	'g++-4.6',
	'gcc-4.6-doc',
	'gcc-4.7',
	'g++-4.7',
	'gcc-4.7-doc',
	'gcc-4.8',
	'g++-4.8',
	'gcc-4.8-doc',
	#'g++-4.9',
	'gcc-snapshot',

	# tools for demos
	'linux-tools-common', # for perf(1)
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
	'devmem2',
	'elfutils', # for manipulating elf files
	'dstat',
	'netperf',
	'sysstat',
	'nmon',
	'lttng-tools',
	#'lttng-modules-dkms',

	# fun stuff
	'figlet',
	'sl',

	# package used tools used by the linuxapi package
	'libreoffice-common', # for soffice conversion
	'ccache', # for ccache(1) (faster building)
	'xutils-dev', # for makedepend(1)
	'uncrustify', # for uncrustify(1) (code formatting)
	'indent', # for indent(1) (code formatting)
	'astyle', # for indent(1) (code formatting)

	# these packages are for the developers pleasure...:)
	'vim',
	'tofrodos',
	'pipemeter',
	'blktool',
	'scons',
	'scons-doc',
	'doxygen',
	'make',
	'make-doc',

	# memory testing tools
	'memtester',
	'memtest86+',

	# papi
	'libpapi-dev', # PAPI development files (headers and API documentation)
	'libpapi5.3', # PAPI runtime (shared libraries)
	'papi-dbg', # PAPI debug symbols
	'papi-examples', # PAPI example files and test programs
	'papi-tools', # PAPI utilities

	# assmebly
	'nasm',

	# kernel debugging
	'systemtap',
	'systemtap-common',
	'systemtap-runtime',
	'systemtap-doc',

	# kexec and kernel crash debugging
	'crash',
	'kdump-tools',
	'kexec-tools',
	'linux-crashdump',
	'makedumpfile',
]

if do_kernel:
	packs.extend([
		# kernel stuff
		'linux-headers-generic',
		'linux-headers-lowlatency',
		'linux-headers-'+release_generic+'-generic',
		'linux-headers-'+release_lowlatency+'-lowlatency',
		'linux-tools-'+release_lowlatency+'-lowlatency',
		'linux-source',
		'linux-source-'+source_release,

		# for systemtap - this one does not exist in the regular ubuntu archives
		# ubuntu does not always carry the "-lowlatency" dbgsym and that why
		# it is commented out...
		# TODO: make this script add sources to the dbgsyms if need be
		'linux-image-'+release_generic+'-generic'+'-dbgsym',
		'linux-image-'+release_lowlatency+'-lowlatency'+'-dbgsym',
	])

args=['sudo','apt-get','install','--assume-yes']
args.extend(packs)
subprocess.check_call(args)
