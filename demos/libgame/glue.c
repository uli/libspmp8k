/*
 * glue.c
 *
 * holds all the stuff to interface newlib to the SPMP8000
 *
 *
 */
#include "libgame.h"

#include <sys/stat.h>
#include <reent.h>
#include <errno.h>
#undef errno
extern int errno;



/* environ
**  A pointer to a list of environment variables and their values.
**  For a minimal environment, this empty list is adequate:
*/
char *__env[1] = { 0 };
char **environ = __env;

/* _exit
**  Exit a program without cleaning up files.
**  If your system doesn't provide this, it is best to avoid linking with subroutines that
**  require it (exit, system).
*
void _exit(int err)
{
	while (1);
}
*/
/* close
**  Close a file.
**
**  Minimal implementation:
*/
int _close(int file)
{
	return fs_close(file);
//	return -1;
}

/* execve
**  Transfer control to a new process.
**
**  Minimal implementation (for a system without processes):
*/
int _execve(char *name, char **argv, char **env)
{
	errno=ENOMEM;
	return -1;
}

/* fork
**  Create a new process.
**
**  Minimal implementation (for a system without processes):
*/

int _fork()
{
	errno=EAGAIN;
	return -1;
}

/* fstat
**  Status of an open file.
**
**	all files are of type file
**
**  The `sys/stat.h' header file is distributed in the `include' subdirectory for this C library.
*/
int _fstat(int file, struct stat *st)
{
//	if (file < 3) st->st_mode = S_IFCHR;
	st->st_mode = S_IFMT;
	return 0;
}

/* getpid
**  Process-ID;
**
**  This is sometimes used to generate strings unlikely to conflict with other processes.       
**
**  Minimal implementation, for a system without processes:
*/
int _getpid()
{
	return 1;
}

/* isatty
**  Query whether output stream is a terminal.
**
**  For consistency with the other minimal implementations, which only support output to stdout,
**  this minimal implementation is suggested:
*/
int _isatty(int file)
{
//	if (file < 2) return 1;
	return 0;
}

/* kill
**  Send a signal.
**
**  Minimal implementation:
*/
int _kill(int pid, int sig)
{
	errno=EINVAL;
	return(-1);
}

/* link
**  Establish a new name for an existing file.
**
**  Minimal implementation:
*/
int _link(char *old, char *new)
{
	errno=EMLINK;
	return -1;
}

/* lseek
**  Set position in a file.
**
**  Minimal implementation:
*/
int _lseek(int file, int ptr, int dir)
{
	uint64_t ret;
	
	ret = fs_seek(file, ptr, dir);
	return (ret >> 32);
}

/* open
**  Open a file. Minimal implementation:
*/
//#define DBG
//#ifdef DBG
//extern int fbuff_printf(char *format, ...);
//#endif
int _open(const char *name, int flags, int mode)
{
	int fd, _flags;
	
	// _F_CREAT
	_flags = flags & 3;
	if (flags & 0x200) _flags = 8 | (flags & 3);
	if ((flags & 0xf) == 0) _flags = 1;
#ifdef DBG
	dmsg_printf("(%s)ifl=%x->of=%x\n", name, flags, _flags);
#endif
	fs_open(name, _flags, &fd);
	return fd;
}

/* read
**  Read from a file. Minimal implementation:
*/
int _read(int file, char *ptr, int len)
{
	int result = 0;
	int ret;

	ret = fs_read(file, ptr, len, &result);
//	dmsg_printf("read(len=%d), ret=%d, res=%d\n", len, ret, result);
	return result;
//	return 0;
}

/* sbrk
**  Increase program data space.
**  As malloc and related functions depend on this, it is useful to have a working implementation.
**
**  The following suffices for a standalone system;
**  it exploits the symbol end automatically defined by the GNU linker. 	
*/
//char *heap_ending;
int heap_ending;
extern uint32_t __heap_end_asm;

extern caddr_t _sbrk_asm(int incr);

#define RAM_END	0x2000000

caddr_t _sbrk(int incr)
{
	caddr_t ret;
#ifdef DBG
	dmsg_printf("sbrk(incr(0x%x) 0x%x ", incr, __heap_end_asm);
#endif
	ret = _sbrk_asm((int)incr);
#ifdef DBG
	dmsg_printf("-> 0x%x) ", __heap_end_asm);
#endif
	// safety check
	if (__heap_end_asm >= RAM_END) {
		errno = ENOMEM;
		return (caddr_t)(-1);
	}

	return ret;
}

/* stat
**  Status of a file (by name).
**
**  Minimal implementation:
*/
int _stat(const char *file, struct stat *st)
{
//	st->st_mode = S_IFCHR;
	st->st_mode = S_IFMT;
	return 0;
}

/* times
**  Timing information for current process.
**
**  Minimal implementation:
*/
clock_t _times(struct tms *buf)
{
	return -1;
}

/* unlink
**  Remove a file's directory entry.
**
**  Minimal implementation:
*/
int _unlink(char *name)
{
	errno=ENOENT;
	return -1; 
}

/* wait
**  Wait for a child process.
**
**  Minimal implementation:
*/
int _wait(int *status)
{
	errno=ECHILD;
	return -1;
}

/* write
**  Write a character to a file.
**
**  `libc' subroutines will use this system routine for output to all files, 
**  including stdout---so if you need to generate any output, for example to a serial port for
**  debugging, you should make your minimal write capable of doing this. 
**
**  The following minimal implementation is an incomplete example; it relies on a writechar
**  subroutine (not shown; typically, you must write this in assembler from examples provided
**  by your hardware manufacturer) to actually perform the output.
*/
int _write(int file, char *ptr, int len)
{
	int result = 0;

	fs_write(file, ptr, len, &result);
	return result;
/*	int todo;

	for (todo=0; todo<len; todo++) {
		writechar(*ptr++);
	}
	return len;
*/
}

