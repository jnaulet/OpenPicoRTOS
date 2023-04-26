#ifndef DRIVER_WRAP_ERRNO_H
#define DRIVER_WRAP_ERRNO_H

#if __STDC_HOSTED__
# include_next <errno.h>
#else
# define EIO              5         /* I/O error */
# define ENXIO            6         /* No such device or address */
# define EAGAIN          11         /* No more processes */
# define ENOMEM          12         /* Out of memory */
# define EFAULT          14         /* Bad address */
# define EBUSY           16         /* Device or resource busy */
# define EINVAL          22         /* Invalid argument */
# define EPIPE           32         /* Broken pipe */
# define ENOSYS          38         /* Invalid system call number */
#endif

#endif
