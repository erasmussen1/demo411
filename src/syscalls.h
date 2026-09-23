#pragma once

#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int _write(int file, char* ptr, int len);

int _read(int file, char* ptr, int len);

int _close(int file);

int _fstat(int file, struct stat* st);

int _isatty(int file);

off_t _lseek(int file, off_t offset, int whence);

int _getpid(void);

int _kill(int pid, int sig);

void _exit(int status);

void* _sbrk(ptrdiff_t increment);
