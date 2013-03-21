#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
「CD-毎日新聞」からデータを抽出して，JSON形式で保存するスクリプト．
新旧(91〜93)どちらのフォーマットにも対応している．

「ID，記事ID，掲載面種別コード，見出し，本文」を抽出する．
本文がない記事は抽出しない．

予めプレーンテキストをUTF8に変換しておいてください
"""

__author__ = 'Yuta Hayashibe' 
__version__ = ""
__copyright__ = ""
__license__ = "GPL v3"

import sys
import codecs
 
import unicodedata
import json

SAVE_TAG_NAME = { u"ID" : u"ID",  #ID
    u"ADD" : u"AID", #記事ID (91〜93)
    u"C0" : u"AID", #記事ID (94〜)
    u"ME" : u"CODE", #掲載面種別コード
    u"AD" : u"CODE", #掲載面種別コード
    u"TIN" : u"TITLE",
    u"T1" : u"TITLE",
    u"HON" : u"BODY",
    u"T2" : u"BODY",
    }
INTEGER_DATA = [u"ID", u"AID", u"CODE"]


def getData(line):
    """Extract normalized tag and data."""
    pos = line[1:].index(u"＼")
    
    tag = line[1:pos+1]
    tag = unicodedata.normalize('NFKC', tag)
    data = line[pos+2:].lstrip().rstrip()
    data = unicodedata.normalize('NFKC', data)

    return tag, data

def trim(article):
    out_article = {}
    if (article.get(u"CF", 1)) == 0 or (u"ZZ" in article):
            return out_article

    for k,v in article.items():
        if k not in SAVE_TAG_NAME:
            continue
        myk = SAVE_TAG_NAME[k]
        if k != u"HON":
            out_article[myk] = v[0]
        else:
            out_article[myk] = v

    for k in out_article:
        if k in INTEGER_DATA:
            try:
                int_v = int(out_article[k])
                out_article[k] = int_v
            except:
                pass

    return out_article


def extract(fname):
    article = {}

    print "["
    isFirst = True
#    for line in codecs.open(fname, 'r', 'shift_jis'): #XXX
    for line in codecs.open(fname, 'r', "utf8"):
        try:
            tag, data = getData(line)
        except:
            continue
        if tag == u"ID":
            out_article = trim(article)
            if (out_article != {}) and (u"BODY" in out_article):
                if isFirst:
                    isFirst = False
                else:
                    print ","
                print json.dumps(out_article, ensure_ascii=False, indent=4).encode("utf8")


            article = {} #reset

        #store information
        if tag in article:
            article[tag].append(data)
        else:
            article[tag] = [data]

    print "]"



if __name__=='__main__':
    argv = sys.argv
    argc = len(sys.argv) 

    if argc <= 1:
        print "Give me a filename"
        quit()
    elif argc <= 2:
        fname = argv[1]
    
    extract(fname)


