#!/bin/bash
CURDIR=$(pwd)

if [[ $1 == "aa" ]]
then
    cd $CURDIR/../NirvanaCommon/Debug && rm -f *.a
    if [[ $2 == "c" ]]
    then
        make clean
        make -j6 all
    else
        make -j6 all
    fi
fi

if [[ $1 == "aa" || $1 == "a" ]]
then
    cd $CURDIR/../NirvanaInfrastructure/Debug && rm -f *.a
    if [[ $2 == "c" ]]
    then
        make clean
        make -j6 all
    else
        make -j6 all
    fi
fi

cd $CURDIR/Debug && rm -f NirvanaCommonTest

if [[ $1 == "c" || $2 == "c" ]]
then
    make clean
fi

make -j6 all
