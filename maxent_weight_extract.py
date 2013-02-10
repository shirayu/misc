#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
This script outputs the weight of each feature from the model of [MaxEnt](https://github.com/lzhang10/maxent).
Simply, give the model name as -i option, and the prefix as -o option,
like ``python weight.py -i model -o weights``.

If you converted feature names into numbers, give the dictionary name as -f option.
"""

__author__ = 'Yuta Hayashibe' 
__version__ = ""
__copyright__ = ""
__license__ = "GPL v3"



import cPickle

def read_seialized(filename, mode='rb'):
    obj_file = open(filename, mode)
    data = cPickle.load(obj_file)
    obj_file.close()
    return data

def parse(filename, outpath, feature_name_dic=None):

    lines = open(filename).read().split("\n")
    feature_number = int(lines[1])
    label_number = int(lines[feature_number+2])


    FNAME_START = 2
    TARGETS_START = feature_number+3+label_number
    labels = lines[feature_number+3 : TARGETS_START]
    assert len(labels) == label_number

    LABELID2FID2WEIGHT = {}
    for i, label in enumerate(labels):
        LABELID2FID2WEIGHT[i] = {}

    WEIGHT_START = TARGETS_START + feature_number + 1
    TARGETS_END = WEIGHT_START -1

    for fid, line in enumerate(lines[TARGETS_START:TARGETS_END]):
        items = [int(x) for x in line.split(" ")[:-1]]
#        target_num = items[0]
        fname = lines[FNAME_START + fid]

        weight = float(lines[WEIGHT_START + fid])
        for _target_labelid in items[1:]:
            LABELID2FID2WEIGHT[_target_labelid][fname] = weight
            

    _fnamedic = None
    if feature_name_dic is not None:
        _tmp = read_seialized(feature_name_dic)
        _fnamedic = {}
        for k, v in _tmp.items():
            _fnamedic[v] = k

    FEATURE_NAME_SATRT = 2
    import codecs
    for lid, weights in  LABELID2FID2WEIGHT.items():
        label = labels[lid]
        outf = codecs.open(outpath + "." + label, "w", "utf8")

        for feature_name, weight in weights.items():
            if _fnamedic is None:
                outf.write("%s\t%s\n" % (feature_name, weight))
            else:
                _fn = _fnamedic[int(feature_name)]
                outf.write("%s\t%s\n" % (_fn, weight))


if __name__=='__main__':
    USAGE = """"""

    import optparse, sys
    oparser = optparse.OptionParser(usage=USAGE)

    oparser.add_option('-i', dest = 'input_filename', help='Converted corpus',)
    oparser.add_option('-o', dest = 'output_filename', help='The path to save model files')
    oparser.add_option('-f', dest = 'feature_name_dic', help='The filename of feature name dictionary', default=None)


    (opts, args) = oparser.parse_args()


    parse(opts.input_filename, opts.output_filename, opts.feature_name_dic)



