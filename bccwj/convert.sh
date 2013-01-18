#!/bin/sh

OUT_NAME=`uuidgen`
DEST="/data2/BCCWJ/data/cabochaed/"
python ./bccwj2cabocha.py $@ | ~/local/bin/cabocha -I2 -f1 > ${DEST}/${OUT_NAME}

