#!/bin/sh

## Unzip xml files in advance
#find /data2/BCCWJ/BCCWJ11VOL2/LUW/ | grep zip | xargs -l -P 5 unzip -d /data2/BCCWJ/uncomp-xml/

find /data2/BCCWJ/uncomp-xml/ |grep xml$ | xargs -P 10 ./convert.sh

