/*
 * Very minimal picoRTOS application (only running idle)
 */
#include "picoRTOS.h"

int main(void)
{
    picoRTOS_init();
    picoRTOS_start();

    /* we're not supposed to end here */
    picoRTOS_assert_void(false);
    return -1;
}
