
#BCCWJ Dependency Extraction Toolkit
- Unzip xml files in the "M-XML" folder in "BCCWJ11VOL1" in advance
    - (eg) `find /media/BCCWJ11VOL1/M-XML/ | grep zip | xargs -l -P 5 unzip -d ~/uncomp-xml/`
- Convert M-XML format of BCCWJ to CaboCha format
    - Please fix variables in convert.sh
    - (eg) `find ~/uncomp-xml/ |grep xml$ | xargs -P 10 ./convert.sh`
- Extract dependencies
    - Please fix variables in extract.sh
    - (eg) `g++ -O3 ./deprel-maker.cpp -std=c++0x -o ./deprel-maker `
    - `find ~/bccwj-cabochaed/ -type f |  xargs -l -P 10 ./extract.sh`
- Count lines the files
    - (eg) `g++ -O3 ./deprel-reducer.cpp -std=c++0x -o ./deprel-reducer`
    - `./deprel-reducer 1 /dev/shm/out/* > ncv.txt`

#License
- GPL v3
