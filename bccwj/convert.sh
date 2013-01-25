#!/bin/sh

OUT_NAME=`uuidgen`
DEST="~/bccwj-cabochaed/"
CABOCHA_PATH="~/local/bin/cabocha"

python -O ./bccwj2cabocha.py $@ | eval ${CABOCHA_PATH} -I2 -f1 > ${DEST}/${OUT_NAME}

