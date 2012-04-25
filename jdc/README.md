#How to convert JDC to Google n-gram style

##Requirements
- a lot of memory

##Commands
- g++ convert2google_ngram_style.cpp -std=c++0x -O2 -o convert2google_ngram_style
- ./convert2google_ngram_style ./nwc2010.verb
- ln -s ./out/1gms/1gm-0000 ./out/1gms/vocab
- ssgnc-build.sh out index

