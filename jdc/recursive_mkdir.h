
#ifndef RECURSIVE_MKDIR_H
#define RECURSIVE_MKDIR_H

#include <string.h>


namespace yucha{
	namespace tool{
		void recursive_mkdir( const char *_path)
		{

			int len = strlen(_path) + 1;
			char *d= new char[len + 1];
			char *path= new char[len + 1];
			strcpy(d, _path);
			strcpy(path, _path);
			char *tok = strtok(d, "/");
			struct stat sb;

			if(_path[0]=='/')
				snprintf(path, len, "/%s", tok);
			else
				snprintf(path, len, "%s", tok);

			while(1) {
			    if(stat(path, &sb) < 0) {
				// The path element didn't exist
				mkdir(path, S_IRWXU );
			    }

//                            if(!S_ISDIR(sb.st_mode)) {
				//Path element not a directory
//                                throw;
//                            }

			    tok = strtok(NULL, "/");
			    if(tok==NULL){
				delete[] d;
				delete[] path;
				return;
			    };
			    strncat(path, "/", len);
			    strncat(path, tok, len);
			}
			return;
		};

}
}

#endif
