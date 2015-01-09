#!/usr/bin/bash


#You'll need to modify these to suite
export PIDGIN_DEV_ROOT=$HOME/pidgin_dev
export PIDGIN_SRC_ROOT=${PIDGIN_DEV_ROOT}/pidgin-2.10.9

export PURPLE_INSTALL_PLUGINS_DIR=`cygpath %APPDATA%`/.purple/plugins


if [ -z "${RELATIVE_PATH_TO_PIDGIN_SRC+x}" ]; then

    #export PURPLE_TOP=${PIDGIN_SRC_ROOT}/libpurple

    export RELATIVE_PATH_TO_PIDGIN_SRC=$(perl -MFile::Spec -e 'print File::Spec->abs2rel("'$PIDGIN_SRC_ROOT'","'`pwd`'")')

    #export PATH="$PIDGIN_DEV_ROOT/win32-dev/mingw-gcc-4.7.2/bin:$PIDGIN_DEV_ROOT/win32-dev/strawberry-perl-5.10.1.5/perl/bin:$PIDGIN_DEV_ROOT/win32-dev/nsis-2.46:$PATH"

    #Not sure how muhch it matters but part of the build generating the perl bindings failed when using strawberry perl.  Worked fine when using cygwin native one
    export PATH="$PIDGIN_DEV_ROOT/win32-dev/mingw-gcc-4.7.2/bin:$PIDGIN_DEV_ROOT/win32-dev/nsis-2.46:$PATH"


fi


