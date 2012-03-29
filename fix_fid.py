#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
x:y z:n ...
といった典型的な形式のLDA学習ファイルの
インデックス値を指定したoffset分だけ増やすスクリプト
"""

__author__ = 'Yuta Hayashibe' 
__version__ = ""
__copyright__ = ""
__license__ = "Public Domain"


def change(offset):
        for line in sys.stdin:
                items = line[:-1].split(' ')
                for item in items:
                        if item == '':
                                continue
                        elem =  item.split(':')
                        print '%d:%d' % ( int(elem[0])+offset , int(elem[1]) ) ,
                print ''

if __name__=='__main__':
        import sys
        argvs = sys.argv
        argc = len(argvs)
        
        if(argc!=2):
                print '[Usage] python %s offset < infile > outfile' % argvs[0]
                quit()
        change(int(argvs[1]))

