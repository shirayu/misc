#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
* Input file style
 word ... word word freq
 word ... word word freq
 ...

 - The number of words are fixed.
"""

import sys
import os

def mkdirs(folder_name):
    assert isinstance(folder_name, str)
    try:
        os.makedirs(folder_name)
    except OSError as exc: # Python >2.5
        import errno
        if exc.errno == errno.EEXIST:
            pass
        else: raise

import collections
def gets(fname, oname):
    dic = collections.defaultdict(int)
    dic_bi = collections.defaultdict(int)
    mkdirs(oname + "/3gms/")
    tri = open(oname + '/3gms/3gm-0000', 'w')

    for line in open(fname):
        items = line.split()
        tri.write("%s\t%s\n" % (" ".join(items[:-1]), items[-1]))

        count = int(items[-1])
        for i in items[:-1]:
            dic[i] += count
        l = len(items)
        for j in xrange(0, l-2):
            string = " ".join(items[j:j+2])
            dic_bi[string] += count

    mkdirs(oname + "/2gms/")
    bi = open(oname + '/2gms/2gm-0000', 'w')
    for (k,v) in dic_bi.items():
        bi.write("%s\t%s" % (k, v))
        bi.write("\n")

    mkdirs(oname + "/1gms/")
#    uni = open(oname + '/1gms/vocab', 'w')
    uni = open(oname + '/1gms/1gm-0000', 'w')
    for (k,v) in dic.items():
        uni.write("%s\t%s" % (k, v))
        uni.write("\n")





if __name__=='__main__':
    import sys
    argv = sys.argv
    argc = len(argv)
    
    USAGE="""python %s INPUT_FILE OUTPUT_FOLDERNAME""" % __file__

    if argc != 3:
        print USAGE
        quit()

    fname = argv[1]
    oname = argv[2]

    mkdirs(oname)
    gets(fname, oname)



