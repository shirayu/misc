/*
 *
 *	Reference: http://d.hatena.ne.jp/tsubosaka/20100524
 */


#include <unordered_set>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <vector>
#include <cstdlib>


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

/*
 * Reduce
 */
typedef std::pair<std::pair<size_t,size_t> , size_t>	MYPAIR;
typedef std::unordered_set< std::pair<size_t , size_t> , MyHash , MyHashEq> MYMAP;
MYMAP reduce_do(char* buf, const size_t total){
	size_t prev = 0;
	size_t split = 0;
	std::pair<size_t , size_t> p;
	MyHash h(buf);
	MyHashEq e(buf);
       MYMAP	m(3 , h , e);

		for(size_t i = 0 ; i < total ; ++i){
			if(buf[i] == '\n'){ //get freq and store
				p = std::pair<size_t , size_t>(prev , i);
				MYMAP::iterator it = m.find(p); 
				if(it != m.end()){
				}else{
					m.insert(p);
				}
				prev = i + 1;
			};
		};

	return m;
};

/*
 * main
 */
int main(int argc, char *argv[]){
	//Designate Input file
	if(argc <2){
		std::cerr << "[Usage] " << argv[0] << " file[...]" << std::endl;
		return -1;
	};

	std::vector<struct stat> sts;
	std::vector<FILE *> fds;
	size_t total = 0;
	for(int i=1; i < argc; ++i){
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
	for(int i=0; i < argc-1; ++i){
		const size_t readsize = fread(buf+offset, 1, sts[i].st_size, fds[i]);
		fclose(fds[i]);
		offset += sts[i].st_size;
		if( readsize != (size_t) sts[i].st_size){ //size check
			std::cerr << "READ=" << readsize << "\tFSIZE=" << sts[i].st_size << std::endl;
			delete[] buf;
			return 1;
		}
	}

	MYMAP m = reduce_do(buf, total);

	//Print out the result
	for(MYMAP::const_iterator it = m.begin(); it != m.end(); ++it){
		for(size_t j=it->first; j<it->second; ++j){
			fputc(buf[j], stdout);
		};
		fputc('\n', stdout);
	}

	//clean up
	delete[] buf;
	return 0;
}
