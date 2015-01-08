#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/internal.ldf

SCRIPT=$(readlink -f "$0")
EXEPATH=$(dirname "$SCRIPT")
cd $EXEPATH

# Create the binaries directory
mkdir -p bin/

# Build HOST side application
#gcc src/f-test.c -o bin/f-test.elf -I ${EINCS} -L ${ELIBS} -le-hal
make -C src
