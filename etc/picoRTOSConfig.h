#ifndef PICORTOSCONFIG_H
#define PICORTOSCONFIG_H

/* Group: Mandatory fields */

/* Define: CONFIG_SYSCLK_HZ
 * The sysctem clock (systick) frequency in hz
 *
 * Typically, this is the CPU frequency here, but not always,
 * PowerPCs, for example have an external periodic interrupt timer
 * that is 40Mhz
 */
#define CONFIG_SYSCLK_HZ 125000000

/* Define: CONFIG_TICK_HZ
 * The scheduling/tick frequency of the system
 *
 * This is the frequency of the preemptive scheduling, on every
 * tick the system will execute the highest non-sleeping priority task
 * and go down by order until it reaches idle
 *
 * 1Khz is a solid foundation, but picoRTOS can go much, much higher, even
 * on small chips (tested at 16Khz on AVR8, >50Khz on Cortex-M0)
 */
#define CONFIG_TICK_HZ 1000

/* Define: CONFIG_TASK_COUNT
 * The number of tasks/threads on your system
 *
 * To avoid costly memory allocations, picoRTOS' core is fully static,
 * which means you have to provide the number of threads it has to support
 *
 * You can have less threads running than specified here, but never more and
 * your threads will be identified by their unique priority (impossible to
 * get 2 tasks with the same priority number)
 */
#define CONFIG_TASK_COUNT 10

/* Define: CONFIG_DEFAULT_STACK_COUNT
 * The default stack size for all your tasks (in picoRTOS_stack_t elements),
 * cannot be < ARCH_MIN_STACK_COUNT (architecture-dependent)
 *
 * 128 is a good starting number
 *
 * If your stack is too small, picoRTOS will throw a debug exception
 * at runtime, letting you know you need to increase it (you can also
 * define a specific size for more memory-hungry tasks)
 */
#define CONFIG_DEFAULT_STACK_COUNT 128

/* Group: SMP extensions */

/* Define:  CONFIG_SMP_CORES
 * Defines the number of cores on your system, can be less than all of them
 * but not more
 *
 * If this is not defined, this defaults to 1
 */
#define CONFIG_SMP_CORES 2

/* Group: Optional fields */

/* Define: CONFIG_DEADLOCK_COUNT
 * Number of attempts picoRTOS will try something before giving up and declaring deadlock
 *
 * As a general rule, you cannot have loops that rely on an external factor to stop spinning,
 * it is not safe. This is the purpose of this constant
 *
 * This is typically used for IPCs or core synchronization in SMP, but a lot of drivers
 * use this constant, too, so you might want it in your config file
 *
 * 1000 is a good starting point
 */
#define CONFIG_DEADLOCK_COUNT 1000

/* Define: CONFIG_ARCH_EMULATE_ATOMIC
 * Emulates atomic operations on systems that don't provide them
 *
 * Certain systems like Cortex-M0 or AVR don't provide native atomic
 * operations, this is a way to emulate them by allowing picoRTOS to
 * enable/disable interrupts
 *
 * This way of doing is not the proper way on a hard RTOS, this is why it
 * has been made optional and requires this field to clearly show how it is
 * handled
 */
#define CONFIG_ARCH_EMULATE_ATOMIC

#endif
