# picoRTOS unprivileged API

This document describes the picoRTOS unprivileged API, which are the functions that can be
safely called whithin a task.

On memory-protected systems picoRTOS enforces this safety & will kill any tasks that
attempts to make a privileged call.

All these functions are available to picoRTOS, picoRTOS-SMP & picoRTOS-lite.

!extractmd.pl scheduler/picoRTOS_u.c
