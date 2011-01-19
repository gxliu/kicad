#!/bin/sh

BASEDIR=/tmp/eeschema-lib

CATEGORIES="lions tigers kitties"

PARTS="eyes ears feet"

REVS="rev1 rev5 rev10"



for C in ${CATEGORIES}; do

    mkdir -p $BASEDIR/$C

    for P in ${PARTS};  do
        for R in ${REVS}; do
            echo "(part $C/$P (value 22)(footprint SM0805))" > $BASEDIR/$C/$P.part.$R
        done
        # also make the part without a rev:
        echo "(part $C/$P (value 22)(footprint SM0805))" > $BASEDIR/$C/$P.part
    done
done
