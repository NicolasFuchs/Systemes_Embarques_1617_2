#include "dirent.h"


extern dir_t* _opendir (const char* name);
extern struct dirent* _readdir (dir_t* dirp);
extern int _closedir (dir_t* dirp);

dir_t* opendir (const char* name) 
{
	return _opendir (name);
}

struct dirent* readdir (dir_t* dirp)
{
	return _readdir (dirp);
}

int closedir (dir_t* dirp)
{
	return _closedir (dirp);
}




