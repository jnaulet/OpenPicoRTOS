# picoRTOS schedulers

## Description

```
               fixed priority       round-robin                       FIFO                    
    TICK                                                                                      
    ───────────────X────────────────X──────────X───────────────────────X──────────X──────────►
                                                                                              
    PRIO#0──►      O  TASK#0        O  TASK#0                   ┌──────O  TASK#0              
                   │                │                           │      │                      
    PRIO#1──►      O  TASK#1        O─ TASK#1 ─┐                │      O─ TASK#1 ─┐           
                   |                │          │                │      │          │           
    PRIO#2──►      O  TASK#2        O  TASK#2  O TASK#3         │      O  TASK#2  O TASK#3    
                   |                │          │           postpone()  │          │           
    PRIO#3──►     ...               O─ TASK#4 ─┘                │      O─ TASK#4 ─┘           
                   |                │                           │      │                      
                   O  IDLE         ...                          │     ...                     
                                    │                           │      │                      
                                    O IDLE                      └─────►O  TASK#0              
                                                                       │                      
                                                                       O IDLE                 
```

*fixed priority*: this is the default picoRTOS scheduling, where every task has an unique
fixed priority.

On every new cycle (TICK), picoRTOS will preempt the current task (usually idle) & choose
the first available highest priority task.

If this task goes to sleep, picoRTOS will choose the next available highest priority task,
and so on until it reaches idle.

*round-robin*: when two or more tasks share the same priority, picoRTOS will alternate between
these based on the tick modulo. This ensures none of these tasks can be locked out.

*FIFO*: when using the `picoRTOS_postpone()` call, the task won't wait for the net tick cycle but
will be inserted back before idle, by order of priority. This can be used to reduce latencies.

## Task availability

A few criteria make a task available for scheduling, it has to be:
  - Ready (aka not sleeping/busy)
  - The tick modulo has to match the task sub-priority (see shared priorities)
  - In case of SMP, the task core mask has to match the current running core
