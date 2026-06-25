#include "unit_test.h"

#include <stdint.h>
#include <stdbool.h>

/* picoRTOS scheduler only */
#include "../../scheduler/picoRTOS.c"

UNIT_TEST(L1_CACHE_ALIGN){ //
    const char *ptr = (const char*)1;

    /* align is statically checked as a pow2 */
    for (size_t p = 0; p < (size_t)32; p++) {
        void *aligned = L1_CACHE_ALIGN(ptr, (1 << p));
        u_assert_var_equals((uintptr_t)aligned & ((1u << p) - 1), 0);
        u_assert_var_equals(((uintptr_t)aligned >> p) & 0x1, 1);
    }
}

UNIT_TEST(task_core_quickswap){ //
    struct picoRTOS_task_core t1 = { .state = TASK_STATE_DISABLED, .prio = 0 };
    struct picoRTOS_task_core t2 = { .state = TASK_STATE_READY, .prio = 1 };

    /* take decent odd number */
    for (int n = 99; n-- != 0;)
        task_core_quickswap(&t1, &t2);

    u_assert_var_equals(t1.state, TASK_STATE_READY);
    u_assert_var_equals(t1.prio, 1);
    u_assert_var_equals(t2.state, TASK_STATE_DISABLED);
    u_assert_var_equals(t2.prio, 0);
}

static void fn(void *priv)
{
    /*@i@*/ (void)priv;
}

UNIT_TEST(task_append){ //
    int res;

    /* reset */
    picoRTOS_init();

    /* fail */
    res = task_append((picoRTOS_pid_t)TASK_COUNT, NULL, (picoRTOS_priority_t)0);
    u_assert_var_equals(res, -1);
    res = task_append((picoRTOS_pid_t)0, NULL, (picoRTOS_priority_t)TASK_COUNT);
    u_assert_var_equals(res, -1);

    /* fill them all */
    for (int i = TASK_COUNT; i-- != 0;) {

        struct picoRTOS_task task;
        static picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

        picoRTOS_task_init(&task, fn, NULL, stack, PICORTOS_STACK_COUNT(stack));
        task_append((picoRTOS_pid_t)i, &task, (picoRTOS_priority_t)i);
    }

    /* all slots already occupied, try */
    size_t count = 0;

    for (int i = TASK_COUNT; i-- != 0;)
        if (task_append((picoRTOS_pid_t)i, NULL, (picoRTOS_priority_t)i) < 0)
            count++;

    u_assert_var_equals(count, TASK_COUNT);
}

UNIT_TEST(picoRTOS_add_task){ //
    struct picoRTOS_task task = { 0 };

    /* reset */
    picoRTOS_init();

    /* fail */
    picoRTOS_add_task(&task, (picoRTOS_priority_t)CONFIG_TASK_COUNT);
    u_assert_var_equals(picoRTOS.pid_count, 0);

    for (int i = CONFIG_TASK_COUNT; i-- != 0;)
        picoRTOS_add_task(&task, (picoRTOS_priority_t)i);

    u_assert_var_equals(picoRTOS.pid_count, CONFIG_TASK_COUNT);

    size_t count = 0;

    for (int i = TASK_COUNT; i-- != 0;) {
        picoRTOS_add_task(&task, (picoRTOS_priority_t)i);
        if (picoRTOS.pid_count == CONFIG_TASK_COUNT) count++;
    }

    u_assert_var_equals(picoRTOS.pid_count, CONFIG_TASK_COUNT);
}

UNIT_TEST(core_sort_tasks){ //
#define TASK_PRIO_MAX (CONFIG_TASK_COUNT - 1)
    /* reset */
    picoRTOS_init();

    /* worst case scenario: order is reversed */
    for (int i = CONFIG_TASK_COUNT; i-- != 0;) {

        struct picoRTOS_task task;
        static picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

        picoRTOS_task_init(&task, fn, NULL, stack, PICORTOS_STACK_COUNT(stack));
        picoRTOS_add_task(&task, i);
    }

    /* check reverse order */
    size_t count = 0;
    for (int i = CONFIG_TASK_COUNT; i-- != 0;)
        if (TASK_BY_PID(i).prio == (TASK_PRIO_MAX - i)) count++;

    u_assert_var_equals(count, CONFIG_TASK_COUNT);

    /* main test point */
    core_sort_tasks();

    /* check right order */
    count = 0;
    for (int i = CONFIG_TASK_COUNT; i-- != 0;)
        if (TASK_BY_PID(i).prio == i) count++;

    u_assert_var_equals(count, CONFIG_TASK_COUNT);
}

UNIT_TEST(core_arrange_shared_priorities){ //
    /* reset */
    picoRTOS_init();

    /* worst case scenario: all same priority */
    for (int i = CONFIG_TASK_COUNT; i-- != 0;) {

        struct picoRTOS_task task;
        static picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

        picoRTOS_task_init(&task, fn, NULL, stack, PICORTOS_STACK_COUNT(stack));
        picoRTOS_add_task(&task, 0);
    }

    /* the real amgic happens */
    core_arrange_shared_priorities();

    size_t count = 0;

    for (int i = CONFIG_TASK_COUNT; i-- != 0;)
        if (TASK_BY_PID(i).prio == 0 && TASK_BY_PID(i).sub_prio == i)
            count++;

    u_assert_var_equals(count, CONFIG_TASK_COUNT);
}

int main(void)
{
    RUN_TEST(L1_CACHE_ALIGN);
    RUN_TEST(task_core_quickswap);
    RUN_TEST(task_append);
    RUN_TEST(picoRTOS_add_task);
    RUN_TEST(core_sort_tasks);
    RUN_TEST(core_arrange_shared_priorities);

    return 0;
}
