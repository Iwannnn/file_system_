#ifndef API_H
#define API_H
#include "file.h"
#include "global.h"

void _cd_(char foldername[]);

void _ls_();

void _mkdir_(char foldername[]);

void _rmdir_(char foldername[]);

void _touch_(char filename[]);

void _rm_(char filename[]);

void _chmod_(char filename[], int owner_mode, int other_mode);

FILE *_open_(char filename[]);

void _cat_(char filename[]);

void _write_(char filename[]);

void _excute_(char filename[]);

void _close_(FILE *f);

#endif
