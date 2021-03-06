#!/usr/bin/env python
# -*- coding: utf-8 -*-

__author__ = 'Yuta Hayashibe'
__version__ = ""
__copyright__ = "Copyright (c) 2014"
__license__ = "GPL v3"

"""
This script converts multiple latex files to a single tex file.
This needs refactoring.
"""
import sys

import os.path
import re
import codecs
inpsplit = re.compile('[ {}]')

dobib = False
myencode = "utf8" #"euc-jp"

def merge(rootdir, filename):

    global myencode
    global dobib
    myfilename = os.path.join(rootdir, filename)
    for l in codecs.open(myfilename, "r", myencode).readlines():
        s =  inpsplit.split(l.lstrip().rstrip().replace(u" ",u""))
        if s[0]=="\input":
            source = s[1]
            if not os.path.exists(source) and not source.endswith(".tex"):
                source += ".tex"
            merge(rootdir, source )
        else:
            #Delete comments
            pos = 0
            while pos < len(l):
                if l[pos] == u"%":
                    if pos == 0:
                        break
                    elif l[pos-1] != u"\\":
                        break
                pos += 1
            if pos == 0:
                l = u"%"
            elif pos < len(l):
                l = l[:pos] + u"%"
            else:
                l = l[:pos].replace(u"\n", u"")

            #replace bib
            if l.startswith(u"\\bibliograp"):
                if dobib == False:
                    bfilename = filename.replace(u".tex", u"") + u".bbl"
                    bdata = codecs.open(bfilename, "r", myencode).read()
                    sys.stdout.write(bdata.encode("utf8"))
                    sys.stdout.write( "\n" )
                    dobib = True
            else:
                sys.stdout.write( l.encode("utf8") )
                sys.stdout.write( "\n" )


if __name__=='__main__':
    import sys
    argv = sys.argv
    argc = len(argv)


    if argc == 2:
        filename = argv[1]
        if filename.startswith("/") or filename.startswith("./"):
            dirpath = "/".join( filename.split('/')[:-1] )  + "/"
        else:
            dirpath = ""
        merge(dirpath, filename)
    quit()

