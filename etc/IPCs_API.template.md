# Inter-Process Communication (IPCs) API

picoRTOS offers a variety of IPCs, strongly inspired by what can be found
on POSIX & Linux.

These basic tasks sync mechanisms are listed here:

   1. [Futexes](#futexes)
   1. [Mutexes](#mutexes)
   1. [Conditions](#conditions)
   1. [Queues](#queues)

## Futexes

Futexes are "fast mutexes" used by the kernel to quickly acquire a lock.<br>
The kind of mutexes are non-reentrant and use the basic `arch_test_and_set()`
port API.

!extractmd.pl ipc/picoRTOS_futex.c

## Mutexes

Mutexes are you typical MUTual EXclusion mechanism.<br>
They use the more elaborate `arch_compare_and_swap()` port API.

!extractmd.pl ipc/picoRTOS_mutex.c

## Conditions

Conditions depend on mutexes.

!extractmd.pl ipc/picoRTOS_cond.c

## Queues

Queues depend on futexes.

!extractmd.pl ipc/picoRTOS_queue.h
