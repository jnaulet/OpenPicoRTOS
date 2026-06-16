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

---
void **picoRTOS_futex_init**(**picoRTOS_futex_t** *<ins>futex</ins>);
> Initializes a <ins>futex</ins> at runtime
### NOTES
> Futexes are fast non-reentrant mutexes on picoRTOS.
>
> A way to statically initialise a futex at startup is this one:
```c
    struct picoRTOS_futex futex = PICORTOS_FUTEX_INITIALIZER;
```
> On memory-protected systems you almost always want to put your futexes
> in the `UNPRIVILEGED_DATA` area, so all your tasks can directly access them:
```c
    struct picoRTOS_futex UNPRIVILEGED_DATA futex = PICORTOS_FUTEX_INITIALIZER;
```

---
int **picoRTOS_futex_trylock**(**struct picoRTOS_futex** *<ins>futex</ins>);
> Tries to acquire a <ins>futex</ins>
### RETURN
> Returns 0 if the futex has been acquired, -`EAGAIN` otherwise

---
void **picoRTOS_futex_lock**(**struct picoRTOS_futex** *<ins>futex</ins>)
> Acquires a <ins>futex</ins> or dies
### NOTES
> The function will make `CONFIG_DEADLOCK_COUNT` attempts to acquire the futex.<br>
> If it fails to acquire it, the calling task will be killed with a `EDEADLK`
> failure code

---
void **picoRTOS_futex_unlock**(**struct picoRTOS_futex** *<ins>futex</ins>)
> Releases a <ins>futex</ins>
### NOTES
> Ths function will kill the calling task with `EINVAL` if the futex
> has already been released.


## Mutexes

Mutexes are you typical MUTual EXclusion mechanism.<br>
They use the more elaborate `arch_compare_and_swap()` port API.

---
void **picoRTOS_mutex_init**(**picoRTOS_mutex_t** *<ins>mutex</ins>);
> Initializes a <ins>mutex</ins> at runtime
### NOTES
> Mutexes are re-entrant on picoRTOS, if you don't need this feature, prefer futexes.<br>
> Another way to statically initialise a mutex at startup is this one:
```c
    struct picoRTOS_mutex mutex = PICORTOS_MUTEX_INITIALIZER;
```
> On memory-protected systems you almost always want to put your mutexes
> in the `UNPRIVILEGED_DATA` area, so all your tasks can directly access them:
```c
    struct picoRTOS_mutex UNPRIVILEGED_DATA mutex = PICORTOS_MUTEX_INITIALIZER;
```

---
int **picoRTOS_mutex_trylock**(**struct picoRTOS_mutex** *<ins>mutex</ins>);
> Tries to acquire a <ins>mutex</ins>
### RETURN
> Returns 0 if the mutex has been acquired, -`EAGAIN` otherwise
### NOTES
> On SMP configurations, if a task is the previous owner of the mutex it
> tries to acquire, it is `picoRTOS_postpone()`d to allow other cores
> some garanteed access to the shared resource.
>
> <ins>Beware</ins>: this is potentially a violation of the tasks priorities order

---
void **picoRTOS_mutex_lock**(**struct picoRTOS_mutex** *<ins>mutex</ins>)
> Acquires a <ins>mutex</ins> or dies
### NOTES
> The function will make `CONFIG_DEADLOCK_COUNT` attempts to acquire the mutex.<br>
> If it fails to acquire it, the calling task will be killed with a `EDEADLK`
> failure code

---
void **picoRTOS_mutex_unlock**(**struct picoRTOS_mutex** *<ins>mutex</ins>)
> Releases a <ins>mutex</ins>
### NOTES
> Ths function will kill the calling task if :
>  - the calling task is not the current owner (EPERM)
>  - the mutex has already been released (EINVAL)


## Conditions

Conditions depend on mutexes.

---
void **picoRTOS_cond_init**(**picoRTOS_cond_t** *<ins>cond</ins>);
> Initializes a condition <ins>cond</ins> at runtime
### NOTES
> Conditions are used to synchronize threads. They work in pair with mutexes.<br>
> See **picoRTOS_cond_wait()** documentation for more info.
>
> Another way to statically init a condition at startup is the following:
```c
    struct picoRTOS_cond cond = PICORTOS_COND_INITIALIZER;
```
> On memory-protected systems you almost always want to put your conditions
> in the `UNPRIVILEGED_DATA` area, so all your tasks can directly access them:
```c
    struct picoRTOS_cond UNPRIVILEGED_DATA cond = PICORTOS_COND_INITIALIZER;
```

---
void **picoRTOS_cond_signal**(**struct picoRTOS_cond** *<ins>cond</ins>);
> Signals a condition to a single waiting tasks
### NOTES
> <ins>Beware</ins>: the task that will wake up is not necessarily the highest
> priority one, as a lower to-be-executed task might be waiting for
> the same condition

---
void **picoRTOS_cond_broadcast**(**struct picoRTOS_cond** *<ins>cond</ins>);
> Signals a condition to a all waiting tasks
### NOTES
> All the waiting tasks will wake up by order of execution (not priority)

---
void **picoRTOS_cond_wait**(**struct picoRTOS_cond** \*<ins>cond</ins>,
**struct picoRTOS_mutex** *<ins>mutex</ins>);
> Waits for a condition to be signaled
### NOTES
> After this call the mutex is released, allowing the signaling task to take it back.<br>
> Right out of this call, you own the mutex again and need to release it.
>
> This is how conditions are typically used:<br>
>> **Thread A:**
```c
    picoRTOS_mutex_lock(&mutex);
    picoRTOS_cond_wait(&cond, &mutex); // <- here we're waiting for ThreadB

    do_something();

    picoRTOS_mutex_unlock(&mutex);
```
>> **Thread B:**
```c
    picoRTOS_mutex_lock(&mutex);

    use_shared_resource();

    picoRTOS_cond_signal(&cond, mutex);
    picoRTOS_mutex_unlock(&mutex); // <- ThreadA can now do_something()
```


## Queues

Queues depend on futexes.

---
**PICORTOS_QUEUE**(<ins>type</ins>, <ins>count</ins>);
> Declares a queue
### NOTES
> Queues are thread safe FIFOs to exchange data between 2 or more
> tasks
>
> <ins>Beware:</ins> <ins>count</ins> MUST be a power of two or the calling
> task will be killed with a `EINVAL` error code. Example:
```c
    static PICORTOS_QUEUE(int, 32) queue;
```
> On memory-protected systems you almost might want to put your queues
> in the `UNPRIVILEGED_DATA` area, but most of the time you want to
> use **picoRTOS_mpu_add_region()** to share it between only 2 tasks

---
**PICORTOS_QUEUE_INIT**(<ins>queue</ins>);
> Dynamically initializes a <ins>queue</ins>.
### NOTES
> The queue has to be declared first.

---
**PICORTOS_QUEUE_READ**(<ins>queue</ins>, <ins>rvalue</ins>);
> Reads the first element of a queue and put it into *<ins>rvalue</ins>
### RETURN
> Returns 0 on successful read, -`EAGAIN` if the queue is empty

---
**PICORTOS_QUEUE_WRITE**(<ins>queue</ins>, <ins>value</ins>);
> Writes a <ins>value</ins> at the end of a queue
### RETURN
> Returns 0 in successful write, -`EAGAIN` if the queue is full
### NOTES
> Queues elements are read using a pointer but written by direct
> copy

