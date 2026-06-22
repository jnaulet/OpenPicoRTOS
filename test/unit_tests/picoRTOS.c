#include "unit_test.h"
#include <stdbool.h>

#include "picoRTOS.h"
#include "picoRTOS_types.h"

/* picoRTOS scheduler only */
#include "../../scheduler/picoRTOS.c"

UNIT_TEST(L1_CACHE_ALIGN) //
{
  const char *ptr = (const char*)-1;

  /* align is statically checked as a pow2 */
  for(int p = 0; p < 32; p++){
    void *aligned = L1_CACHE_ALIGN(ptr, (1 << p));
    u_assert((uintptr_t)aligned & ((1 << p) - 1) == 0);
    u_assert(((uintptr_t)aligned >> p) & 0x1) == 1);
  }
}

UNIT_TEST(task_core_quickswap) //
{
  struct picoRTOS_task_core t1 = { .state = TASK_STATE_DISABLED, .prio = 0 };
  struct picoRTOS_task_core t2 = { .state = TASK_STATE_READY, .prio = 1 };

  /* take decent prime number */
  for(int n = 19; n-- != 0;)
    task_core_quickswap(&t1, &t2);

  u_assert(t1.state == TASK_STATE_READY);
  u_assert(t1.prio == 1);
  u_assert(t2.state == TASK_STATE_DISABLED);
  u_assert(t1.prio == 0);
}

int main(void)
{
  RUN_TEST(L1_CACHE_ALIGN);
  RUN_TEST(task_core_quickswap);

  return 0;
}
