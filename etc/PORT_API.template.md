# picoRTOS Port API

This document describes the API a port **MUST** use and/or provide in order to be able to run picoRTOS on a
new architecture.

    1. [Single core](#single-core)
    1. [SMP extensions](#smp-extensions)

## Single core

!extractmd.pl arch/include/picoRTOS_port.h

## SMP extensions

!extractmd.pl arch/include/picoRTOS-SMP_port.h
