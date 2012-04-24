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

class MyHashEq : std::binary_function<std::pair<size_t , size_t> , std::pair<size_t , size_t> , bool>{
	const char *const buf;
	public:
		MyHashEq(const char * const b) : buf(b) {}
		bool operator() (const std::pair<size_t , size_t> & x , const std::pair<size_t , size_t> & y) const{
			const char * s = buf + x.first;
			const char * t = buf + y.first;
			const char * se = buf + x.second; // *se == '\n'
			for( ; *s == *t && s != se ; ++s , ++t);
			// *s != *t or (*s == '\n' => *t == '\n')
			return *s == *t;
		}
};

class MyHash : std::unary_function<std::pair<size_t , size_t> , size_t>{
	const char * const buf;
	public:
		MyHash(const char * const b) : buf(b){}
		size_t operator()(const std::pair<size_t , size_t> & p) const{
			size_t h = 0;
			for(size_t i = p.first ; i < p.second ; ++i){
				h = h * 31 + buf[i];
			}
			return h;
		};
};

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
typedef std::pair<std::pair<size_t,size_t> , size_t>	MYPAIR;
typedef std::unordered_map< std::pair<size_t , size_t> , size_t  , MyHash , MyHashEq> MYMAP;


void add(MYMAP &m, std::pair<size_t , size_t> &p, const unsigned int freq){
	MYMAP::iterator it = m.find(p); 
	if(it != m.end()){
		it->second += freq;
	}else{
		m.insert(MYPAIR(p , freq));
	}
};

void printout(const int ngram, MYMAP &m, const std::string &out_folder, char* buf, const unsigned int SKIP){
	std::stringstream ss;
	ss << out_folder << "/" << ngram << "gms/";
	yucha::tool::recursive_mkdir(ss.str().c_str());

	//Print out the result
	{
		size_t count = 0;
		size_t filenum = 0;
		std::ofstream ret(getOutName(ss.str(), ngram, filenum).c_str());

		for(MYMAP::const_iterator it = m.begin(); it != m.end(); ++it){
			++count;
			if (SKIP != 0 and count % SKIP == 0){
				++filenum;
				std::ofstream ret(getOutName(ss.str(), ngram, filenum).c_str());
			}

			std::string s(buf + it->first.first , buf + it->first.second);
			ret << s << "\t" << it->second << std::endl;
		}
	}
};


void reduce_do(char* buf, const size_t total, const std::string &out_folder){
	yucha::tool::recursive_mkdir(out_folder.c_str());

	size_t prev = 0;
	size_t split = 0;
	std::pair<size_t , size_t> p;
	MyHash h(buf);
	MyHashEq e(buf);
	MYMAP uni(3 , h , e);
	MYMAP bi(3 , h , e);
	MYMAP tri(3 , h , e);


	{
		for(size_t i = 0 ; i < total; ++i){
			prev = i;
			for(; buf[i] != '\t'; ++i){ //get Word
			}
			buf[i] = ' ';
			size_t end_1 = i;
			std::pair<size_t , size_t> word1(prev, i);
			++i;

			for(; buf[i] != '\t'; ++i){ //get Word
			}
			buf[i] = ' ';
			size_t end_2 = i;
			std::pair<size_t , size_t> word2(end_1+1 , i);
			std::pair<size_t , size_t> bi1(prev, i);
			++i;

			for(; buf[i] != '\t'; ++i){ //get Word
			}
			size_t end_3 = i;
			std::pair<size_t , size_t> word3(end_2+1 , i);
			std::pair<size_t , size_t> bi2(end_1+1, i);
			std::pair<size_t , size_t> tri1(prev, i);
			++i;

			for(; buf[i] != '\n'; ++i){ //get freq
			}

			{
				buf[i] = '\0';
				const unsigned int freq = atoi(&buf[end_3]);

				add(uni, word1, freq);
				add(uni, word2, freq);
				add(uni, word3, freq);

				add(bi, bi1, freq);
				add(bi, bi2, freq);

				add(tri, tri1, freq);
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
