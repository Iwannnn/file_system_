#ifndef GLOBAL_H
#define GLOBAL_H
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/**
 ** 	int open(const char *pathname, int oflag, int perms);
 **		int creat (const char *pathname,mode_t mode);
 **		int mkdir (const char *__path, __mode_t __mode)
 **/
#define FAILURE 0
#define SUCCESS 1
#define NAME_MAX 20
#define PATH_MAX 100
#define USERINFO_DIR "./userinfo"
#define USERINFO_MODE "a+"
#define FILEINFO_DIR "./fileinfo"
#define FILEINFO_READ_MODE "a+"
#define FILEINFO_SAVE_MODE "w"
#define CREAT_MODE 0775

extern const char *FOLDER_;
extern const char *FILE_;
extern const char *EMPTY_;
extern const char *SLASH;

extern char current_user[];
extern char current_dir[PATH_MAX];

#endif