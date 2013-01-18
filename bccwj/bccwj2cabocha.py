#!/usr/bin/env python
# -*- coding: utf-8 -*-


"""
Convert BCCWJ's M-XML format to CaboCha format.
"""

__author__ = 'Yuta Hayashibe' 
__version__ = ""
__copyright__ = ""
__license__ = "GPL v3"

import re

class Sentence(object):
    def __init__(self):
        self.lines = []
        self.bunsetsu_id = 0
        self.data = u""

    def __mask(self, tmp):
        if len(tmp)==0:
            return "*"
        return tmp

    def __getAttr(self, attrs, name):
        if attrs.has_key(name):
            return self.__mask(attrs[name].value)
        return "*"

    def __operate_begin(self, node):
        attrs = node.attributes

        if node.nodeType == node.TEXT_NODE:
            self.data += re.sub(r"\s", '', node.data)

        elif node.tagName=='LUW':
            if attrs.has_key("B"):
                if attrs["B"].value == "S" or attrs["B"].value == "B":
                    line = u"* %d 0D 0/0 0" % self.bunsetsu_id
                    self.lines.append(line)
                    self.bunsetsu_id += 1

        elif node.tagName=='SUW':
            pos = attrs["pos"].value.split(u"-")
            pos += ["*"] * (3 - len(pos))
            pos = ",".join(pos)

            ctype = self.__getAttr(attrs, "cType") #(活用型)
            cform = self.__getAttr(attrs, "cForm") #(活用形)
            lform = self.__getAttr(attrs, "lForm") #(語彙素読み)
            lemma = self.__getAttr(attrs, "lemma") #(語彙素)
        
            self.tmp = u",".join([pos, ctype, cform, lform, lemma])

    def __operate_end(self, node):
        if node.nodeType == node.TEXT_NODE:
            pass
        elif node.tagName=='SUW':
            tmp = u"%s\t%s,%s" % (self.data, self.tmp, self.data)
            self.lines.append(tmp)
            self.data = u""

    def operate(self, elem):
        self.__operate_begin(elem)

        for child in elem.childNodes:
            self.operate(child)

        self.__operate_end(elem)

    def __unicode__(self):
        return  u"\n".join(self.lines) + u"\nEOS\n"

if __name__=='__main__':
    import sys
    import codecs
    import xml.dom.minidom
    sys.stdout = codecs.getwriter('utf8')(sys.stdout)

    argv = sys.argv
    argc = len(argv)
    
    if argc <= 1:
        print "Give me file(s)"
        quit()

    fnames = argv[1:]

    for fname in fnames:
        obj = xml.dom.minidom.parse(fname)
        sentences = obj.getElementsByTagName('sentence')
        for sentence in sentences:
            sent = Sentence()
            sent.operate(sentence)
            sys.stdout.write(unicode(sent))


