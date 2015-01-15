#!/bin/bash

SCRIPT=$(readlink -f "$0")
EXEPATH=$(dirname "$SCRIPT")

# In case we're running in a subshell...
if [ -z "$LD_LIBRARY_PATH" ]
then
    export LD_LIBRARY_PATH=/opt/adapteva/esdk/tools/host/lib:/opt/openmpi/lib:
fi
if [ -z "$EPIPHANY_HDF" ]
then
    export EPIPHANY_HDF=/opt/adapteva/esdk/bsps/current/platform.hdf
fi
if [ -z "$EPIPHANY_HOME" ]
then
    export EPIPHANY_HOME=/opt/adapteva/esdk
fi

$EXEPATH/bin/elink-test1-host.elf

#> dotproduct.log

retval=$?

if [ $retval -ne 0 ]
then
    echo "$SCRIPT FAILED"
else
    echo "$SCRIPT PASSED"
fi

exit $retval
