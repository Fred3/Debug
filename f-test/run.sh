#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib:${LD_LIBRARY_PATH}
EHDF=${EPIPHANY_HDF}

SCRIPT=$(readlink -f "$0")
EXEPATH=$(dirname "$SCRIPT")

#sudo -E LD_LIBRARY_PATH=${ELIBS} EPIPHANY_HDF=${EHDF} $EXEPATH/bin/f-test.elf
sudo $EXEPATH/bin/f-test $@

#if [ $? -ne 0 ] 
#then
#    echo "$SCRIPT FAILED"
#else
#    echo "$SCRIPT PASSED"
#fi

