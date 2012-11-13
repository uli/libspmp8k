#define _ECOS_ENOERR           0     /* No error */
#define _ECOS_EPERM            1     /* Not permitted */
#define _ECOS_ENOENT           2     /* No such entity */
#define _ECOS_ESRCH            3     /* No such process */
#define _ECOS_EINTR            4     /* Operation interrupted */
#define _ECOS_EIO              5     /* I/O error */
#define _ECOS_EBADF            9     /* Bad file handle */
#define _ECOS_EAGAIN           11    /* Try again later */
#define _ECOS_EWOULDBLOCK      _ECOS_EAGAIN
#define _ECOS_ENOMEM           12    /* Out of memory */
#define _ECOS_EBUSY            16    /* Resource busy */
#define _ECOS_EXDEV            18    /* Cross-device link */    
#define _ECOS_ENODEV           19    /* No such device */
#define _ECOS_ENOTDIR          20    /* Not a directory */
#define _ECOS_EISDIR           21    /* Is a directory */    
#define _ECOS_EINVAL           22    /* Invalid argument */
#define _ECOS_ENFILE           23    /* Too many open files in system */
#define _ECOS_EMFILE           24    /* Too many open files */
#define _ECOS_EFBIG            27    /* File too large */    
#define _ECOS_ENOSPC           28    /* No space left on device */
#define _ECOS_ESPIPE           29    /* Illegal seek */
#define _ECOS_EROFS            30    /* Read-only file system */    
#define _ECOS_EDOM             33    /* Argument to math function outside valid */
                               /* domain */
#define _ECOS_ERANGE           34    /* Math result cannot be represented */
#define _ECOS_EDEADLK          35    /* Resource deadlock would occur */
#define _ECOS_EDEADLOCK        _ECOS_EDEADLK
#define _ECOS_ENOSYS           38    /* Function not implemented */
