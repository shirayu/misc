/*
 *
 *	Reference: http://d.hatena.ne.jp/tsubosaka/20100524
 */


#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <vector>
#include <cstdlib>

#include "recursive_mkdir.h"


std::string getOutName(const std::string &out_folder, const int ngram, const size_t count){
	std::stringstream ss;
	ss << out_folder << "/" << ngram << "gm-";
       	ss << std::setw( 4 )
  	    << std::setfill('0')
	    << count ;
	
	std::cerr << "output to ... " ;
	std::cerr << ss.str() << std::endl;	
	
	return ss.str();
};


/*
 * Reduce
 */
typedef std::unordered_map< std::string, unsigned long int> MYMAP;


void add(MYMAP &m, const std::string &p, const unsigned long int freq){
	MYMAP::iterator it = m.find(p); 
	if(it != m.end()){
		it->second += freq;
	}else{
		m[p] = freq;
	}
};

void printout(const int ngram, MYMAP &m, const std::string &out_folder, const char* buf, const unsigned int SKIP){
	std::stringstream ss;
	ss << out_folder << "/" << ngram << "gms/";
	yucha::tool::recursive_mkdir(ss.str().c_str());

	//Print out the result
	{
		size_t count = 0;
		size_t filenum = 0;
//                std::ofstream *ret = new std::ofstream(getOutName(ss.str(), ngram, filenum).c_str());
		FILE *ret = fopen(getOutName(ss.str(), ngram, filenum).c_str(), "w" );

		for(MYMAP::const_iterator it = m.begin(); it != m.end(); ++it){
			++count;
			if (SKIP != 0 and count % SKIP == 0){
				++filenum;
//                                ret->close();
//                                delete ret;
//                                ret = new std::ofstream(getOutName(ss.str(), ngram, filenum).c_str());
				fclose(ret);
				ret = fopen(getOutName(ss.str(), ngram, filenum).c_str(), "w" );
			}

			fputs(it->first.c_str(), ret);
			fputc('\t', ret);
			fprintf(ret,"%lu", it->second);
			fputc('\n', ret);
		};
				fclose(ret);
//                ret->close();
//                delete ret;
	}
};


void reduce_do(char* buf, const size_t total, const std::string &out_folder){
	yucha::tool::recursive_mkdir(out_folder.c_str());

	size_t prev = 0;
	size_t split = 0;
	std::pair<size_t , size_t> p;
	MYMAP uni;
	MYMAP bi;
	MYMAP tri;

	{
        /*
         *  2       shown   show.XX in      images. mark    as
         */

		for(size_t i = 0 ; i < total;){
			prev = i;
			for(; buf[i] != '\t'; ++i);
			buf[i] = '\0';
			const unsigned long freq = atoi(&buf[prev]);

            ++i;
			prev = i;
			for(; buf[i] != '\t'; ++i);
			buf[i] = '\0';
			const std::string verb(&buf[prev]);

            ++i;
			prev = i;
			for(; buf[i] != '\t'; ++i);
			buf[i] = '\0';
			const std::string verb_word_sense(&buf[prev]);
            ++i;

            add(uni, verb_word_sense, freq);
//            std::cout << "----" << std::endl;
//            std::cout << verb << std::endl;

			for(bool stop = false; not stop;){
                prev = i;
                for(; buf[i] != '\t'; ++i);
                buf[i] = '\0';
                const std::string prep(&buf[prev]);
                ++i;

                prev = i;
                for(; ;++i){
                    if(buf[i] == '\t'){
                        break;
                    }
                    else if (buf[i] == '\n'){
                        stop = true;
                        break;
                    };
                };
                buf[i] = '\0';
                const std::string arg(&buf[prev]);
                ++i;

                add(uni, prep, freq);
                add(uni, arg, freq);
                add(bi, verb_word_sense + " " + prep, freq);
                add(bi, prep + " " + arg, freq);
                add(tri, verb_word_sense + " " + prep + " " + arg, freq);
//                std::cout << prep << " " << arg << " " << freq << std::endl;
            };
		};
	}

	printout(1, uni, out_folder, buf, 0);
	printout(2, bi, out_folder, buf, 10000000);
	printout(3, tri, out_folder, buf, 10000000);


};

/*
 * main
 */
int main(int argc, char *argv[]){
	//Designate Input file
	if(argc <3){
		std::cerr << "[Usage] " << argv[0] << " out_folder input_file[...]" << std::endl;
		return -1;
	};

	const std::string out_folder(argv[1]);
	std::vector<struct stat> sts;
	std::vector<FILE *> fds;
	size_t total = 0;
	for(int i=2; i < argc; ++i){
		//Open
		struct stat st;
		if(stat(argv[i] , &st)){
			perror("stat");
			return 1;
		};
		total += st.st_size;
		sts.push_back(st);
		FILE* fd = fopen(argv[i], "rb");
		fds.push_back(fd);
	};
	char * buf = new char[total];
	size_t offset = 0;
	for(int i=0; i < argc-2; ++i){
		const size_t readsize = fread(buf+offset, 1, sts[i].st_size, fds[i]);
		fclose(fds[i]);
		offset += sts[i].st_size;
		if( readsize != (size_t) sts[i].st_size){ //size check
			std::cerr << "READ=" << readsize << "\tFSIZE=" << sts[i].st_size << std::endl;
			delete[] buf;
			return 1;
		}
	}

	reduce_do(buf, total, out_folder);

	//clean up
	delete[] buf;
	return 0;
}
