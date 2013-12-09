#!/usr/local/bin/python
# -*- coding: utf-8 -*-

import os
def mkdirs(folder_name):                   
#    assert isinstance(folder_name, unicode)
    try:
        os.makedirs(folder_name)
    except OSError as exc: # Python >2.5
        import errno
        if exc.errno == errno.EEXIST:
            pass
        else:
            raise

import os.path
def get_new_name(fname):
    bn = os.path.basename(fname)
    dn = os.path.dirname(fname)

    items = bn.split("_")
    if len(items) != 3:
        raise "ERROR"
    
    date = items[2]
    year = date[0:4]
    mon = date[4:6]
    day = date[6:]

    return os.path.join(year, mon), bn


# 第1引数は移動先ディレクトリ
# 残りに対象ファイルを与える

if __name__=='__main__':
    import sys
    import shutil

    out_root = sys.argv[1]

    for fname in sys.argv[2:]:
        new_dest, nn = get_new_name(fname)
        new_dir = os.path.join(out_root, new_dest)

        mkdirs(new_dir)
        shutil.move(fname, os.path.join(new_dir, nn))
        
#        print os.path.join(new_dir, nn)



