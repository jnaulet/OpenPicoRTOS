# OpenPicoRTOS Book of Requirements

## 1. Compliance with "The Power of 10" from the NASA/JPL

JPL guidelines can be found here: https://en.wikipedia.org/wiki/The_Power_of_10:_Rules_for_Developing_Safety-Critical_Code

### 1.1: Restrict all code to very simple control flow constructs

OpenPicoRTOS' core satisfies this rule by avoiding nested loops, gotos and providing functions that do only one thing at a time and jump immediately after the first error occurs.

Some drivers, however, especially when they compute bitrates or baudrates might use nested loops and provide more complexity.

No recursion is used in the project but function pointers are unavoidable for task creation or interrupt management.

We consider these function pointers a necessary evil & they are safe as long as they are not stored or called directly at runtime.

### 1.2: Give all loops a fixed upper bound

This rule is satisfied by the use of CONFIG_DEADLOCK_COUNT, which is the maximum attempts OpenPicoRTOS will do before giving up and declaring deadlock.

The deadlocks will trigger a breakpoint and can easily be identified by running a target in debug mode with a probe attached to it.

### 1.3: Do not use dynamic memory allocation after initialization

This is satisfied by not using any dynamic memory allocation. OpenPicoRTOS is fully static.

### 1.4: No function should be longer than what can be printed on a single sheet of paper

The longest function in OpenPicoRTOS' core is picoRTOS_tick() in picoRTOS-SMP.c. It is the most complex one on the system and is 52 lines long (counting asserts).

### 1.5: The code's assertion density should average to minimally two assertions per function

This rule is mostly satisfed by asserting each and every parameter a function takes, plus the current state of the system on syscalls.

These asserts will trigger a breakpoint & help catching any out of boundaries parameter before it can lead to a system crash.

However, syscall_switch_context() doesn't check the stack boundaries again as it is already checked
by its calling function and would only add overhead (or be ignored by the optimization).
This might be an infringement.

### 1.6: Declare alldata objects at the smallest possible level of scope

This rule is satisfied by using cppcheck to check for possible scope reductions.
Unfortunately, it breaks another rule here, as this static analyzer doesn't pass with a clean record.

Making splint AND cppcheck pass with no errors or warnings might be the next challenge in line, as they sometimes disagree and sometimes disagree with these rules themselves.

### 1.7: Each calling function must check the return value of nonvoid functions

Most OpenPicoRTOS' core functions are void to make developers lifes easier. The system will stall if anything serious goes wrong instead of returning a value that risks to be voided by the user in the first place.

However, some drivers and a lot of the demo code voids a lot of return values for convenience. Don't follow this example.

### 1.8: The use of the preprocessor must be limited to the inclusion of header files and simple macro definitions

This is mostly the case as the macros used in OpenPicoRTOS are mostly shortcuts.

However, the assert part is more debatable, as we have macros calling macros.
These calls are the only way to force a breakpoint on a bad assert and are considered a necessary AND temporary evil, as release builds should use the -DNDEBUG flag to remove them anyway.

### 1.9: The use of pointers must be restricted. Specifically, no more than one level of dereferencing should be used

This is satisfied and can be easily checked with a command similar to this:

    # find . -name "*.c" -exec grep "\*[a-zA-Z]\+ =" {} \+

### 1.10: All code must be compiled, from the first day of development, with all compiler warnings enabled at the most pedantic setting available

OpenPicoRTOS should satisfy this rule but is not a standalone application so it's hard to check, as usually SDKs and applications themselves tend to ignore this rule.

The way OpenPicoRTOS attempts to satisfy this rule in a standalone way is by using "splint" with the -checks flags.
This can be tested by going to the test/static_analysis/picoRTOS or test/static_analysis/picoRTOS-SMP directory and type:

    # make splint

or

    # make cppcheck

## 2. Limited use of inline assembly
### 2.1 Inline assembly should be side-effect free

OpenPicoRTOS doesn't offer any warranty a developer unaware of this requirement
would not use the extended inline assembly directives.

This can be checked relatviely easily, though, by using the following command:

    # find . -name "*.c" -exec grep ASM {} \+

### 2.2 Inline assembly can be safely removed from the static analysis

OpenPicoRTOS achieves this by defining a "ASM" macro for each and every port.
This macro is ignored when the static analysis is run.

## 3. Smallest footprint possible

OpenPicoRTOS satisfies this requirement by providing 2 schedulers: picoRTOS and picoRTOS-SMP, to avoid overhead on targets that won't use the SMP features.

A lot of effort has been put to keep the code small but readable.
Types are part of the port to allow the best suitable width for any platform.
Architecture-specific routines have been written directly in assembly language.

## 4. Fully static

OpenPicoRTOS doesn't provide any dynamic memory management. Problem solved.

## 5. Fully predictable

This is achieved by reducing the task management to its simplest expression:
  - No round-robin support
  - Tasks have fixed priorities
  - Tasks have unique and individual priorities

Priorities should be understood as "order of execution", more than anything else.

On every tick the scheduler will parse the tasks list backwards to find the one that has the highest priority and the READY state and run it.

Any "syscall" will allow the scheduler to move to the next task until it reaches idle or a new tick occurs.

## 6. High portability

The port API has been designed in a minimalistic way, with SMP seen as an optional extension of the system once again.
Most of the ports calls can be written directly in assembly language or C.

OpenPicoRTOS assumes the compiler is GCC, though. To allow more compilers, a change of structure similar to FreeRTOS might be necessary.

## 7. Lowest overhead possible

This is highly dependent on the quality of the port, but the main gain in terms of speed is the way OpenPicoRTOS chooses its tasks.
The order of execution never changes, so on every tick the algorithm will make a reverse loop to find the first ready-to-run task.
Then any syscall will simply increase the task index to look for the next available task until it reaches idle (or is preempted).

No complex algorithm, no linked lists, just an array of tasks that run in a fixed order.

## 8. Support for SMP

OpenPicoRTOS-SMP offers this feature and tries to keep it simple. Cores are always running in sync.
On every tick, all cores are forced to execute picoRTOS_tick() and will all find their first ready-to-run task.
Then any syscall will allow a core to find its next one, excluding tasks currently running on other cores and tasks already executed.

This adds a bit of overhead, as a core needs to lock the kernel using arch_spin_lock() and arch_spin_unlock() and might temporarily stall the other ones.
Furthermore, a core might execute more loops to find its next available task.
