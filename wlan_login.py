#!/usr/bin/env python
# -*- coding: utf-8 -*-

__author__ = 'Yuta Hayashibe'
__copyright__ = 'Copyright (c) 2011, Yuta Hayashibe'
__license__ = 'GPL v3'


SSID='mm2010'
AUTHURL='https://aruba.naist.jp/cgi-bin/login'
NAME='input your id here'
PASS='input your password here'
DUMMYURL='http://www.example.com'



def auth(name, password ):
  import urllib,urllib2
  r = urllib2.urlopen(DUMMYURL)
  data = urllib.urlencode( {'user':name, 'password':password, 'cmd':'authenticate'} )
  try:
    r = urllib2.urlopen(AUTHURL, data)
    return True
  except:
    return False
  #print r.read()

if __name__ == '__main__':
  #http://docs.solab.jp/sl4a/contents/p/api-wifi
  import android
  droid = android.Android()

  droid.makeToast("Enabling Wifi...")
  droid.toggleWifiState(True) #enable
  droid.wifiReconnect()
  winfo=droid.wifiGetConnectionInfo().result 

  #print winfo
  if u'ssid' not in winfo or winfo[u'ssid']!=SSID:
    droid.makeToast('Error: '+ SSID + ' not connecting.')
  else:
    droid.makeToast('Start login...')
    if auth(NAME, PASS):
      droid.makeToast('Login finished!')
    else:
      droid.makeToast('Erorr: Failed authentication.')

