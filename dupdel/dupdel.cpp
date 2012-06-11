
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <vector>
#include <cstdlib>

#include "recursive_mkdir.h"
#include "listup.h"

const unsigned long int ONE_FILE_LINE_NUM = 2100000;

/*
 * First letter is...
 * mode0 -> -H
 * mode1 -> I-R
 * mode2 -> S-
 *
 * mind it may be a digit or a symbol
 */
const int sep_letter0 = 'H';
const int sep_letter1 = 'R';

std::unordered_set<std::string> myset;

int main(int argc, char *argv[]){
    //Designate Input file
    if(argc != 4){
        std::cerr << "[Usage] " << argv[0] << " input_folder output_folder mode[0,1,2]" << std::endl;
        return -1;
    };

    const int mode = atoi(argv[3]);
    if (mode < 0 or mode > 2){
        std::cerr << "mode must be in 0 to 2," << std::endl;
        return -1;
    };

    ////////////////////////////////////////////////////
    std::vector<std::string> fnames = getFNames(argv[1]);
    const unsigned int file_num = fnames.size();
    unsigned int j=0;
    for(std::vector<std::string>::iterator it = fnames.begin(); it!=fnames.end(); ++it){
        struct stat st;
        if(stat(it->c_str() , &st)){
            perror("stat");
            return 1;
        };
        const size_t total = st.st_size;
        FILE* fd = fopen(it->c_str(), "rb");

        char * buf = new char[total];
        size_t offset = 0;
        fprintf(stderr, "\rLoading...  now: %06d   TOTAL: %06d   %.2f%%  (%s)      ", j, file_num, j/(float) file_num*100, it->c_str());
        const size_t readsize = fread(buf+offset, 1, st.st_size, fd);
        fclose(fd);
        if( readsize != (size_t) st.st_size){ //size check
            std::cerr << "READ=" << readsize << "\tFSIZE=" << st.st_size << std::endl;
            delete[] buf;
            return 1;
        };

        size_t prev = 0;
        for(size_t pos = 0 ; pos < total; ++pos){
            if(buf[pos] == '\n'){
                if ((mode == 0 and buf[prev] <= sep_letter0)
                        or (mode==1 and buf[prev] > sep_letter0 and buf[prev] <= sep_letter1)
                        or (mode==2 and buf[prev] > sep_letter1)
                        ){
                    std::string s(buf + prev , buf + pos);
                    myset.insert(s);
                }
                prev = pos + 1;
            };
        };
        delete[] buf; //clean up
        ++j;
    };
    fprintf(stderr, "\nFinished Extracting!!\n");

    const std::string out_folder(argv[2]);
    yucha::tool::recursive_mkdir(out_folder.c_str());
    unsigned int now = 0;
    unsigned long int count = 0;
    std::string out_fname = out_folder + "/0";
    std::cerr << "output to ... " << out_fname << std::endl;
    std::ofstream* ofs = new std::ofstream(out_fname);

    for(std::unordered_set<std::string>::iterator it = myset.begin(); it != myset.end(); ++it){
        *ofs << *it << std::endl;

        ++count;
        if(count == ONE_FILE_LINE_NUM){
            count = 0;
            ++now;
            std::stringstream ss;
            ss << out_folder << "/" << now;
            out_fname = ss.str();
            std::cerr << "output to ... " ;
            std::cerr << out_fname << std::endl;
            ofs->close();
            ofs = new std::ofstream(out_fname);
        };
    };
    ofs->close();


    return 0;
}
