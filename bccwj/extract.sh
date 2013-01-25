#!/bin/sh

OUT_NAME=`uuidgen`
DEST="/dev/shm/out"
mkdir -p ${DEST}
./deprel-maker 0 < $@ > ${DEST}/${OUT_NAME}

