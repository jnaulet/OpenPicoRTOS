#!/bin/sh
docker run -v /home/${USER}/dev/OpenPicoRTOS-dev:/home/${USER}/dev/OpenPicoRTOS-dev -w $(pwd) -it s32ds-power-v1-2:latest make V=1
