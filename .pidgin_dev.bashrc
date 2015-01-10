#!/usr/bin/bash


#You'll need to modify these to suite
export PIDGIN_DEV_ROOT=${HOME}/pidgin_dev
export PIDGIN_SRC_ROOT=${PIDGIN_DEV_ROOT}/pidgin-2.10.9


if [ -z "${RELATIVE_PATH_TO_PIDGIN_SRC+x}" ]; then


    #Figure out where the plugin dir is located

    SOURCE="${BASH_SOURCE[0]}"

    #Resolve $SOURCE until the file is no longer a symlink
    while [ -h "${SOURCE}" ]; do

        DIR="$( cd -P "$( dirname "${SOURCE}" )" && pwd )"
        SOURCE="$(readlink "${SOURCE}")"

        #If $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
        [[ $SOURCE != /* ]] && SOURCE="${DIR}/${SOURCE}"

    done

    export PIDGIN_NOTICE_PLUGIN_ROOT="$( cd -P "$( dirname "$SOURCE" )" && pwd )"


    #Now we can figure out the relative path to the pidgin source directory is
    export RELATIVE_PATH_TO_PIDGIN_SRC=$(perl -MFile::Spec -e 'print File::Spec->abs2rel("'${PIDGIN_SRC_ROOT}'","'${PIDGIN_NOTICE_PLUGIN_ROOT}'")')


    #export PATH="$PIDGIN_DEV_ROOT/win32-dev/mingw-gcc-4.7.2/bin:$PIDGIN_DEV_ROOT/win32-dev/strawberry-perl-5.10.1.5/perl/bin:$PIDGIN_DEV_ROOT/win32-dev/nsis-2.46:$PATH"

    #Not sure how muhch it matters but part of the build generating the perl bindings failed when using strawberry perl.  Worked fine when using cygwin native one
    export PATH="${PIDGIN_DEV_ROOT}/win32-dev/mingw-gcc-4.7.2/bin:${PIDGIN_DEV_ROOT}/win32-dev/nsis-2.46:${PATH}"

fi


