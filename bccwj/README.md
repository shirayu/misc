
#BCCWJ Dependency Extraction Toolkit
- Unzip xml files in the "M-XML" folder in "BCCWJ11VOL1" in advance
    - (eg) `find /media/BCCWJ11VOL1/M-XM/ | grep zip | xargs -l -P 5 unzip -d ~/uncomp-xml/`
- Convert BCCWJ's M-XML format to CaboCha format
    - Please fix variables in convert.sh
    - (eg) `find ~/uncomp-xml/ |grep xml$ | xargs -P 10 ./convert.sh`
- Extract dependencies
    - Please fix variables in extract.sh
    - (eg) `g++ -O3 ./deprel-maker.cpp -std=c++0x -o ./deprel-maker `
    - `find ~/bccwj-cabochaed/ -type f |  xargs -l -P 10 extract.sh`
- Uniq the files
    - `wget https://raw.github.com/shirayu/misc/master/uniq.cpp -O myuniq.cpp`
    - `g++ -O3 ./myuniq.cpp -std=c++0x -o ./myuniq`
    - `myuniq 1 /dev/shm/out/* > ncv.txt`

#License
- GPL v3