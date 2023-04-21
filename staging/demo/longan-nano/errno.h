#ifndef TEMPORARY_HACK_ERRNO_H
#define TEMPORARY_HACK_ERRNO_H

/* Temporary hack cause riscv64-unknown-elf doesn't provide errno */

#define EIO              5      /* I/O error */
#define ENXIO            6      /* No such device or address */
#define EAGAIN          11      /* No more processes */
#define ENOMEM          12      /* Out of memory */
#define EFAULT          14      /* Bad address */
#define EBUSY           16      /* Device or resource busy */
#define EINVAL          22      /* Invalid argument */
#define EPIPE           32      /* Broken pipe */
#define ENOSYS          38      /* Invalid system call number */

#endif
