#ifndef __LPC_CLIENT_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


int FmOpenFile(const char *pathname, int flags);
int FmCloseFile(int filedes);
int FmReadFile(int filedese, void *buf, size_t nbytes);
int FmWriteFile(int filedes, void *buffer, size_t n);
DIR *FmOpenDir(const char *dirname);
struct dirent *FmReadDir(DIR *dirp);
int FmCloseDir(DIR *dirp);
int FmStatFile(const char *path, struct stat *buf);

void Init(void);

#endif /* __LPC_CLIENT_H__ */
