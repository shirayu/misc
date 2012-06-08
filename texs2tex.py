#!/usr/bin/env python
# -*- coding: utf-8 -*-

__author__ = 'Yuta Hayashibe'
__version__ = ""
__copyright__ = "Copyright (c) 2011"
__license__ = "GPL v3"

"""
This script converts multiple latex files to a single tex file.
"""
import sys


def merge(filename):
    import os.path
    dirpath = "/".join( filename.split('/')[:-1] )  + "/"
    import re
    cmt1 = re.compile('^ *%')
    cmt2 = re.compile(r'%[^%]*$')
    inpsplit = re.compile('[ {}]')
    for l in open(filename).readlines():
        l = l.rstrip()
        s =  inpsplit.split(l)
        if s[0]=="\input":
            source = s[1]
            if not source.endswith(".tex"):
                source += ".tex"
            merge( dirpath + source )
        elif not cmt1.match( l ):
            l = re.sub( cmt2, '', l )
            sys.stdout.write( l )
            sys.stdout.write( "\n" )


if __name__=='__main__':
    import sys
    argv = sys.argv
    argc = len(argv)


    if argc == 2:
        merge( argv[1] )
    quit()


