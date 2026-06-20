# picoRTOS privileged API

This document describes the picoRTOS privileged API.<br>
This API can be called anytime up until `picoRTOS_start()`.

On memory-protected systems, picoRTOS enforces the privileged/unprivileged model
and will kill any tasks trying to access these resources directly.

 1. [picoRTOS & lite](#single-core)
 1. [picoRTOS-SMP extensions](#multi-core-smp-extensions)

## Macros & shortcuts


### TASK MANAGEMENT

---
**PICORTOS_STACK_COUNT**(<ins>x</ins>);
> Computes stack count from array <ins>x</ins>


## Single core

This API is provided by picoRTOS & picoRTOS-lite

---
void **picoRTOS_init**(void);
> Initializes picoRTOS
### NOTES
> This is the first function you need to call to be able to run picoRTOS.
> It will:
> - partition the memory (on mpu-supported systems)
> - init the idle task

---
void **picoRTOS_task_init**(**struct picoRTOS_task** \*<ins>task</ins>,
**picoRTOS_task_fn** <ins>fn</ins>, **void** \*<ins>priv</ins>, **picoRTOS_stack_t** \*<ins>stack</ins>,
**size_t** <ins>stack_count</ins>);
> Initialises a task structure with all relevant information.
### RETURN
> picoRTOS will assert stack_count >= ARCH_MIN_STACK_COUNT & return immediately, leaving
> *<ins>task</ins> untouched if it fails
### NOTES
> Here's your typical use case:
```c
    struct picoRTOS_task task;
    struct picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_task_init(&task, task_entry_point, &task_context, stack, (size_t)CONFIG_DEFAULT_STACK_COUNT);
```
> Remark: you can use the helper `PICORTOS_STACK_COUNT(stack)` to make your life easier

---
void **picoRTOS_add_task**(**struct picoRTOS_task** \*<ins>task</ins>,
**picoRTOS_priority_t** <ins>prio</ins>);
> Adds a <ins>task</ins> to picoRTOS & sets its priority <ins>prio</ins>.
### NOTES
> Priorities goes from 0 (highest) to CONFIG_TASK_COUNT (lowest).<br>
> picoRTOS will assert prio < CONFIG_TASK_COUNT & return without adding
> the task to the system if it fails

---
picoRTOS_priority_t **picoRTOS_get_next_available_priority**(void);
> Gets the first available priority by ascending order (0 -> n)
### NOTES
> This call will provide the next unused available priority & can be helpful to
> avoid managing priorities by hand & create potential involuntary round-robin
> situations. Example:
```c
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
```
> Round-robin can still be achieved this way:
```c
   picoRTOS_priority_t prio = picoRTOS_get_next_available_priority();
   picoRTOS_add_task(&task1, prio);
   picoRTOS_add_task(&task2, prio);
```
### RETURN
> picoRTOS will return priority -1 if no more priorities are available

---
picoRTOS_priority_t **picoRTOS_get_last_available_priority**(void);
> Gets the first available priority by descending order (n -> 0)
### NOTES
> This call will provide the next unused available priority & can be helpful to
> avoid managing priorities by hand & create potential involuntary round-robin
> situations. Example:
```c
    picoRTOS_add_task(&task, picoRTOS_get_last_available_priority());
```
> Round-robin can still be achieved this way:
```c
   picoRTOS_priority_t prio = picoRTOS_get_last_available_priority();
   picoRTOS_add_task(&task1, prio);
   picoRTOS_add_task(&task2, prio);
```
> This call is usually used for low-priority refreshing tasks like watchdog
> management
### RETURN
> picoRTOS will return priority -1 if no more priorities are available

---
void **picoRTOS_start**(void);
> Starts picoRTOS' scheduler
### NOTES
> On memory-portected systems, call to unprivileged functions if forbidden
> after this one.

---
void **picoRTOS_register_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>,
**picoRTOS_isr_fn** <ins>fn</ins>, **void** \*<ins>priv</ins>);
> Registers an <ins>irq</ins> handler.<br>
> On <ins>irq</ins>, the handler <ins>fn</ins> will be called with the param
> <ins>priv</ins>


## Multi core SMP extensions

This extended API is provided by picoRTOS-SMP

---
void **picoRTOS_SMP_add_task**(**struct picoRTOS_task** \*<u>task</u>,
**picoRTOS_priority_t** <u>prio</u>, **picoRTOS_core_mask_t** <u>mask</u>);
> Adds a <u>task</u> to picoRTOS, sets its priority <u>prio</u> & its SMP core
> <u>mask</u>
### NOTES
> Priorities goes from 0 (highest) to CONFIG_TASK_COUNT (lowest).<br>
> picoRTOS will assert prio < CONFIG_TASK_COUNT & return without adding
> the task to the system if it fails.
>
> <u>mask</u> takes the form of a bitfield where core0 is (1 << 0),
> core1 is (1 << 1), etc.<br>
> If you want your task to run on any core, just use the regular *
> `picoRTOS_add_task()` call.

---
void **picoRTOS_SMP_register_interrupt**(**picoRTOS_irq_t** <ins>irq</ins>,
**picoRTOS_isr_fn** <ins>fn</ins>, **void** \*<ins>priv</ins>,
**picoRTOS_mask_t** <ins>core_mask</ins>);
> Registers an <ins>irq</ins> handler on a dedicated set of cores.<br>
> On <ins>irq</ins>, the handler <ins>fn</ins> will be called with the param
> <ins>priv</ins> on any core matching <ins>core_mask</ins>

