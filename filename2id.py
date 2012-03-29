#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
ファイル名とidの変換を行うプログラム
"""

__author__ = 'Yuta Hayashibe' 
__version__ = ""
__copyright__ = ""
__license__ = "Public Domain"

def getTotal(directory):
    import os
    files = os.listdir(directory)
    print len(files)

def getName(directory, no):
    import os
    files = os.listdir(directory)
    print directory+"/"+files[no]


if __name__=='__main__':
    import sys
    argvs = sys.argv
    argc = len(argvs)
    if(argc==2):
        getTotal(argvs[1])
    elif(argc==3):
        getName(argvs[1], int(argvs[2])-1)
    else:
        print """[Usage]    python %s directory [id]
        * id starts 1
        * Not give id, return total file nums""" % argvs[0]
        quit()

