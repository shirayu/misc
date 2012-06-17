#!/usr/bin/sh

sudo apt-get install libnss3-tools
wget http://ca.naist.jp/naist_server_ca.crt
wget http://ca.naist.jp/naist_root_ca.crt
certutil -d sql:$HOME/.pki/nssdb -A -t C,, -n naist_root_ca.crt -i naist_root_ca.crt
certutil -d sql:$HOME/.pki/nssdb -A -t C,, -n naist_server_ca.crt -i naist_server_ca.crt

