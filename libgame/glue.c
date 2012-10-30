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
#include <sys/unistd.h>
#include <string.h>
#include <fcntl.h>

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
	return _ecos_close(file);
}

/* execve
**  Transfer control to a new process.
**
**  Minimal implementation (for a system without processes):
*/
int _execve(char *name __attribute__((unused)),
            char **argv __attribute__((unused)),
            char **env __attribute__((unused)))
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

static void _ecos_stat_to_stat(struct stat *st, struct _ecos_stat *est)
{
	memset(st, 0, sizeof(struct stat));
	st->st_mode = est->st_mode;
	st->st_ino = est->st_ino;
	st->st_dev = est->st_dev;
	st->st_nlink = est->st_nlink;
	st->st_uid = est->st_uid;
	st->st_gid = est->st_gid;
	st->st_size = est->st_size;
	st->st_atime = est->st_atime;
	st->st_mtime = est->st_mtime;
	st->st_ctime = est->st_ctime;
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
	struct _ecos_stat est;
	int ret = _ecos_fstat(file, &est);
	if (!ret)
		_ecos_stat_to_stat(st, &est);
	return ret;
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
int _isatty(int file __attribute__((unused)))
{
//	if (file < 2) return 1;
	return 0;
}

/* kill
**  Send a signal.
**
**  Minimal implementation:
*/
int _kill(int pid __attribute__((unused)), int sig __attribute__((unused)))
{
	errno=EINVAL;
	return(-1);
}

/* link
**  Establish a new name for an existing file.
**
**  Minimal implementation:
*/
int _link(char *old __attribute__((unused)), char *new __attribute__((unused)))
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
	return _ecos_lseek(file, ptr, dir);
}

/* open
**  Open a file. Minimal implementation:
*/
//#define DBG
//#ifdef DBG
//extern int fbuff_printf(char *format, ...);
//#endif
int _open(const char *name, int flags, int mode __attribute__((unused)))
{
	int _ecos_flags;
	switch (flags & O_ACCMODE) {
		case O_RDONLY: _ecos_flags = _ECOS_O_RDONLY; break;
		case O_RDWR: _ecos_flags = _ECOS_O_RDWR; break;
		case O_WRONLY: _ecos_flags = _ECOS_O_WRONLY; break;
		default: return -1;
	}
	if (flags & O_APPEND)
		_ecos_flags |= _ECOS_O_APPEND;
	if (flags & O_CREAT)
		_ecos_flags |= _ECOS_O_CREAT;
	if (flags & O_TRUNC)
		_ecos_flags |= _ECOS_O_TRUNC;
	if (flags & O_EXCL)
		_ecos_flags |= _ECOS_O_EXCL;
	if (flags & O_SYNC)
		_ecos_flags |= _ECOS_O_SYNC;
	if (flags & O_NONBLOCK)
		_ecos_flags |= _ECOS_O_NONBLOCK;
	return _ecos_open(name, _ecos_flags, mode);
}

/* read
**  Read from a file. Minimal implementation:
*/
int _read(int file, char *ptr, int len)
{
	return _ecos_read(file, ptr, len);
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
int _stat(const char *file __attribute__((unused)), struct stat *st)
{
	struct _ecos_stat est;
	int ret = _ecos_stat(file, &est);
	if (!ret)
		_ecos_stat_to_stat(st, &est);
	return ret;
}

/* times
**  Timing information for current process.
**
**  Minimal implementation:
*/
clock_t _times(struct tms *buf __attribute__((unused)))
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
	return _ecos_unlink(name);
}

/* wait
**  Wait for a child process.
**
**  Minimal implementation:
*/
int _wait(int *status __attribute__((unused)))
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
	return _ecos_write(file, ptr, len);
}

