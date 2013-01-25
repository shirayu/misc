/*
 * Copyright (c) Yuta Hayashibe
 * LICENSE : GNU GENERAL PUBLIC LICENSE Version 3
 *
 * Extract dependency relation from CaboCha-parsed input.
 */


#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

const static int NORMAL_FORM_POSITION = 6; //the normal_form position in unidic style

struct morpheme{
	std::string surface;
	std::vector<std::string> info;

    public:
        const std::string& getNormalForm() const{
            return this->info[NORMAL_FORM_POSITION];
        };
};

struct segment{
	int id;
	int link; //係り先文節番号 (0から)
	int head; //主辞形態素番号 (文節内、0から)
	int function; //機能形態素番号 (文節内、0から)
	std::vector<morpheme> morphemes;
	std::vector<std::string> ns; //Ns
};


morpheme getMorp(const std::string &line ) {
	morpheme morp;
	std::vector<std::string> morpheme_str;

	boost::split( morpheme_str, line, boost::algorithm::is_any_of( "\t" ) );
	morp.surface = morpheme_str[0];  
	boost::split( morp.info, morpheme_str[1], boost::algorithm::is_any_of( "," ) );
	return morp;
}


segment getSegment(const std::string &line, std::vector<std::string> &infs ) {
	segment seg;
	boost::split( infs, line, boost::algorithm::is_any_of( " /" ) );

	seg.id = boost::lexical_cast<int>( infs.at(1) );
	seg.link = boost::lexical_cast<int>( infs.at(2).substr( 0, infs.at(2).length()-1 ) );
	seg.head = boost::lexical_cast<int>( infs.at(3) );
	seg.function = boost::lexical_cast<int>( infs.at(4) );

	return seg;
}


void extract_dependency_relation( const std::vector<segment> &segs ) {

	for( std::vector<segment>::const_iterator it=segs.begin(); it!=segs.end(); ++it){
                if( it->link == -1 or it->head == -1
                        or it->morphemes[ it->head ].info[0] != "名詞"
//                        or it->morphemes[ it->function ].info[1] != "格助詞"
                 ) continue;

                const segment * const p_dest = &segs.at( it->link );
                std::string entry = it->morphemes[ it->head ].surface;
                std::string dep = p_dest->morphemes[ p_dest->head ].info[6];

                if( p_dest->morphemes[ p_dest->head ].info[6] == "なる" and p_dest->head > 0 ) {
                        dep = p_dest->morphemes[ p_dest->head-1 ].info[6];
                        if( p_dest->morphemes[ p_dest->head-1 ].info[1] == "格助詞" )
                                continue;
                }
                else if( p_dest->morphemes[ p_dest->head ].info[6] == "する" ) {
                        if( p_dest->head+1 < (int) p_dest->morphemes.size() and
                                p_dest->morphemes[ p_dest->head+1 ].info[6] == "れる" )
                                dep = "される";

                        if( p_dest->head > 0 ) {
                                dep = p_dest->morphemes[ p_dest->head-1 ].info[6] + dep;

                                if( p_dest->morphemes[ p_dest->head-1 ].info[1] == "格助詞"  and  p_dest->head-1 > 0 ) {
                                        dep = p_dest->morphemes[ p_dest->head-2 ].surface + dep;
                                        if( p_dest->morphemes[ p_dest->head-2 ].info[1] == "非自立" and  p_dest->head-2 > 0 )
                                                dep = p_dest->morphemes[ p_dest->head-3 ].surface + dep;
                                }
                        }
                };

                if( it->morphemes[ it->head ].info[1] == "固有名詞" ){
                        entry = ( boost::format( "<固有名詞,%s>" ) % it->morphemes[ it->head ].info[2] ).str();
                }
                else if( it->morphemes[ it->head ].info[1] == "接尾"){
                       if( it->morphemes[ it->head ].info[2] != "助数詞"
                                       and  it->head > 0
                                       and  it->morphemes[ it->head-1 ].info[1] != "数" )
                                entry = it->morphemes[ it->head-1 ].info[6] + entry; //get Root
                };

                //Final output
                if(
                        p_dest->morphemes[ p_dest->head ].info[0] == "動詞"
//                        or
//                     p_dest->morphemes[ p_dest->head ].info[0] == "形容詞"
                     )
                {
	                	std::cout << entry
                            << "\t" 
                            << it->morphemes[ it->function ].surface
                            << "\t" 
                            << dep 
                            << std::endl;
                };
        }


}


void extract_caseframe( std::vector<segment> &segs ) {

	for( std::vector<segment>::iterator it=segs.begin(); it!=segs.end(); ++it){

		//get N
		if( it->link != -1 and it->head != -1 and it->morphemes[ it->head ].info[0] == "名詞"
			and it->morphemes[ it->function ].info[1] == "格助詞"){	//getN
			std::string entry = it->morphemes[ it->head ].getNormalForm();
			if( it->morphemes[ it->head ].info[1] == "固有名詞" ){
				entry = ( boost::format( "<固有名詞,%s>" ) % it->morphemes[ it->head ].info[2] ).str();
			}
			else if( it->morphemes[ it->head ].info[1] == "接尾"){
			       if( it->morphemes[ it->head ].info[2] != "助数詞"
					       and  it->head > 0
					       and  it->morphemes[ it->head-1 ].info[1] != "数" )
					entry = it->morphemes[ it->head-1 ].info[6] + entry; //get Root
			};
			segs[it->link].ns.push_back(entry + it->morphemes[ it->function ].surface);
		}//get V
		else if( it->morphemes[ it->head ].info[0] == "動詞" or it->morphemes[ it->head ].info[0] == "形容詞"){
			std::string dep = it->morphemes[ it->head ].getNormalForm();

			if( it->morphemes[ it->head ].info[6] == "なる" and it->head > 0 ) {
				dep = it->morphemes[ it->head-1 ].info[6];
				if( it->morphemes[ it->head-1 ].info[1] == "格助詞" )
					continue;
			}
			else if( it->morphemes[ it->head ].info[6] == "する" ) {
				if( it->head+1 < (int) it->morphemes.size() and
					it->morphemes[ it->head+1 ].info[6] == "れる" )
					dep = "される";

				if( it->head > 0 ) {
					dep = it->morphemes[ it->head-1 ].info[6] + dep;

					if( it->morphemes[ it->head-1 ].info[1] == "格助詞"  and  it->head-1 > 0 ) {
						dep = it->morphemes[ it->head-2 ].surface + dep;
						if( it->morphemes[ it->head-2 ].info[1] == "非自立" and  it->head-2 > 0 )
							dep = it->morphemes[ it->head-3 ].surface + dep;
					}
				}
			};

			std::cout << dep;
			for(std::vector<std::string>::const_iterator ite=it->ns.begin(); ite!=it->ns.end(); ++ite){
				std::cout << "\t" << *ite;
			}
			std::cout << std::endl;
		};
		
		//Final output

	}

}

int getMode(const char* c){
	        return atoi(c);
};

int main( int argc, char **argv ) {
	if(argc!=2){
		std::cerr << "[Usage] " << argv[0] << " mode" << std::endl;
		std::cerr << "  [Mode]" << std::endl
		<< "    0       NC V" << std::endl
		<< "    1       NC1 NC2 ... V" << std::endl;
		return -1;
	};
	const int mode = getMode(argv[1]);

	std::string line;
	std::vector<segment> segs;

    bool  waiting_next_begin = true; //For bug; sometimes after EOS, it does not come *
	while( getline( std::cin, line ) ) {
		if( line == "EOS" ) { //end of a sentence
			if(mode==0)
				extract_dependency_relation( segs );
			else
				extract_caseframe( segs );
			segs.clear();
            waiting_next_begin = true;
		}
		else if( line[0] == '*' ) { //segment information
			std::vector<std::string> infs;
			boost::split( infs, line, boost::algorithm::is_any_of( " /" ), boost::algorithm::token_compress_on );
			if(infs.size()==6){
				segs.push_back( getSegment( line, infs ) );
                waiting_next_begin = false;
			}
			else{
				segs[ segs.size()-1 ].morphemes.push_back( getMorp(line) );
			};
		}
		else{ //information about compornents of a segment
            if (not waiting_next_begin)
    			segs[ segs.size()-1 ].morphemes.push_back( getMorp(line) );
		}
	}

	return 0;
}
