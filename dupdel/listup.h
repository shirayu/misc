
#include <sys/types.h>
#include <dirent.h>
#include <string>


//get filenames which exclude the suffix in designated path 
std::vector<std::string> getFNames(const char *path){
    DIR* dp=opendir(path);
    std::vector<std::string> lst;
    if (dp!=NULL)
    {
        for(struct dirent* dent = readdir(dp); dent != NULL; dent = readdir(dp)){
            std::string dname = std::string(dent->d_name);
            if (dname != "." and dname!=".."){
                lst.push_back( std::string(path) + "/" + dname );
            };
        };
        closedir(dp);
    };
    return lst;
};

