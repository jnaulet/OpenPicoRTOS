# picoRTOS privileged API

This document describes the picoRTOS privileged API.<br>
This API can be called anytime up until `picoRTOS_start()`.

On memory-protected systems, picoRTOS enforces the privileged/unprivileged model
and will kill any tasks trying to access these resources directly.

    1. [picoRTOS & lite](#single-core)
    1. [picoRTOS-SMP extensions](#multi-core-smp-extensions)

## Single core

This API is provided by picoRTOS & picoRTOS-lite

!extractmd.pl scheduler/picoRTOS.c

## Muilti core SMP extensions

This extended API is provided by picoRTOS-SMP

!extractmd.pl scheduler/picoRTOS-SMP.c
