#!/bin/sh
export CROSS_PATH=
export CROSS_PREFIX=
export LOCAL_PREFIX=

MODE=$1

if [ "$MODE" = "cross" ]
then
    echo "======CROSS COMPILING PROCESS======"
    make clean
    make all CROSS_COMPILE=1 


elif [ "$MODE" = "local" ]
then
    echo "======LOCAL COMPILING PROCESS======"
    make LOCAL=1 all
    cd bin
    chmod +x test
else
    echo "please spectify compile mode!"
fi