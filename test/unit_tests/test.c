#include "unit_test.h"
#include <stdbool.h>
#include "picoRTOS_types.h"

/* global state machine */
static bool halted;
static picoRTOS_core_t core;

#define RESET_STATE_MACHINE()                   \
    halted = false

static inline picoRTOS_core_t arch_core(void)
{
    return core;
}

/*
 * These unit tests will ensure picoRTOS & picoRTOS-SMP get the same
 * expected basic behaviour
 */

#ifndef TEST_PICORTOS_SMP
# include "../../scheduler/picoRTOS.c"
#else
# include "../../scheduler/picoRTOS-SMP.c"
#endif

/* Dummy functions */

static void dummy_fn0(void *priv)
{
    (void)priv;
}

static void dummy_fn1(void *priv)
{
    (void)priv;
}

UNIT_TEST(task_core_init)
{
    struct picoRTOS_task_core task;

    /* reset */
    picoRTOS_init();

    /* no way this will fail, but this is test-driven framework */
    task_core_init(&task);
    u_assert_var_equals(task.sp, NULL);
    u_assert_var_equals(task.state, PICORTOS_TASK_STATE_EMPTY);
    u_assert_var_equals(task.tick, 0);
    u_assert_var_equals(task.stack_bottom, NULL);
    u_assert_var_equals(task.stack_top, NULL);
    u_assert_var_equals(task.stack_count, 0);
    u_assert_var_equals(task.stat.counter, -1);
    u_assert_var_equals(task.stat.watermark_lo, -1);
    u_assert_var_equals(task.stat.watermark_hi, 0);
    u_assert_var_equals(task.prio, TASK_COUNT);
    u_assert_var_equals(task.sub_prio, 0);
}

UNIT_TEST(task_idle_init)
{
    task_idle_init();

    u_assert_var_equals(TASK_BY_PID(TASK_IDLE_PID).state, PICORTOS_TASK_STATE_READY);
    u_assert_var_equals(TASK_BY_PID(TASK_IDLE_PID).stack_count, ARCH_MIN_STACK_COUNT);
    u_assert_var_equals(TASK_BY_PID(TASK_IDLE_PID).prio, TASK_IDLE_PRIO);

#ifndef TEST_PICORTOS_SMP
    u_assert_ptr_equals(TASK_BY_PID(TASK_IDLE_PID).sp, picoRTOS.idle_stack);
    u_assert_ptr_equals(TASK_BY_PID(TASK_IDLE_PID).stack_bottom, picoRTOS.idle_stack);
#else
    u_assert_var_equals(TASK_BY_PID(TASK_IDLE_PID + 1).state, PICORTOS_TASK_STATE_READY);
    u_assert_var_equals(TASK_BY_PID(TASK_IDLE_PID + 1).stack_count, ARCH_MIN_STACK_COUNT);
    u_assert_var_equals(TASK_BY_PID(TASK_IDLE_PID + 1).prio, TASK_IDLE_PRIO);
    u_assert_ptr_equals(TASK_BY_PID(TASK_IDLE_PID).sp, picoRTOS_SMP_stack[0].idle);
    u_assert_ptr_equals(TASK_BY_PID(TASK_IDLE_PID).stack_bottom, picoRTOS_SMP_stack[0].idle);
    u_assert_ptr_equals(TASK_BY_PID(TASK_IDLE_PID + 1).sp, picoRTOS_SMP_stack[1].idle);
    u_assert_ptr_equals(TASK_BY_PID(TASK_IDLE_PID + 1).stack_bottom, picoRTOS_SMP_stack[1].idle);
#endif
}

UNIT_TEST(picoRTOS_init){
    /* no way this will fail, but this is test-driven framework */
    picoRTOS_init();
    u_assert_var_equals(picoRTOS.pid_count, 0);
#ifndef TEST_PICORTOS_SMP
    u_assert_var_equals(picoRTOS.index, TASK_IDLE_PID);
#else
    u_assert_var_equals(picoRTOS.index[0], TASK_IDLE_PID);
    u_assert_var_equals(picoRTOS.index[1], TASK_IDLE_PID + 1);
#endif
    u_assert_var_equals(picoRTOS.tick, (picoRTOS_tick_t)-1);
    u_assert_var_equals(picoRTOS.is_running, false);
}

UNIT_TEST(task_core_is_available)
{
}

UNIT_TEST(task_core_stat_begin)
{
}

UNIT_TEST(core_stat_watermark)
{
}

UNIT_TEST(task_core_stat_switch)
{
}

UNIT_TEST(task_core_stat_preempt)
{
}

UNIT_TEST(picoRTOS_task_init)
{
    int dummy = 0;
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    /* check macro */
    u_assert_var_equals(PICORTOS_STACK_COUNT(stack), CONFIG_DEFAULT_STACK_COUNT);

    /* reset */
    picoRTOS_init();

    /* test #1, must fail */
    RESET_STATE_MACHINE();
    picoRTOS_task_init(&task, dummy_fn0, &dummy, stack, (size_t)1);
    u_assert(halted);

    /* test #2, ok */
    RESET_STATE_MACHINE();
    picoRTOS_task_init(&task, dummy_fn0, &dummy, stack, PICORTOS_STACK_COUNT(stack));
    u_assert(!halted);
    u_assert_ptr_equals(task.fn, dummy_fn0);
    u_assert_ptr_equals(task.priv, &dummy);
    u_assert_ptr_equals(task.stack, stack);
    u_assert_var_equals(task.stack_count, CONFIG_DEFAULT_STACK_COUNT);
}

UNIT_TEST(picoRTOS_add_task)
{
#define RANDOM_PID  (CONFIG_TASK_COUNT / 3)
#define RANDOM_PRIO 4

    size_t n;
    int dummy = 0;
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    /* reset */
    picoRTOS_init();

    /* we already established this one works, so we can use it */
    picoRTOS_init();
    picoRTOS_task_init(&task, dummy_fn0, &dummy, stack, PICORTOS_STACK_COUNT(stack));

    /* test #1, must fail */
    RESET_STATE_MACHINE();
    picoRTOS_add_task(&task, CONFIG_TASK_COUNT);
    u_assert(halted);

    /* test #2, must be ok */
    RESET_STATE_MACHINE();
    for (n = CONFIG_TASK_COUNT; n-- != 0;)
        picoRTOS_add_task(&task, RANDOM_PRIO);

    u_assert(!halted);
    u_assert_ptr_equals(TASK_BY_PID(RANDOM_PID).sp, stack);
    u_assert_var_equals(TASK_BY_PID(RANDOM_PID).state, PICORTOS_TASK_STATE_READY);
    u_assert_ptr_equals(TASK_BY_PID(RANDOM_PID).stack_bottom, stack);
    u_assert_var_equals(TASK_BY_PID(RANDOM_PID).stack_count, CONFIG_DEFAULT_STACK_COUNT);
    u_assert_var_equals(TASK_BY_PID(RANDOM_PID).prio, RANDOM_PRIO);

    /* test #3, must fail */
    RESET_STATE_MACHINE();
    picoRTOS_add_task(&task, RANDOM_PRIO);
    u_assert(halted);
}

UNIT_TEST(task_core_quickcpy)
{
    int dummy0 = 0;
    int dummy1 = 0;
    struct picoRTOS_task task;
    picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    /* reset for next test */
    picoRTOS_init();

    /* we already established these ones work, so we can use them */
    picoRTOS_task_init(&task, dummy_fn0, &dummy0, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, 0);
    picoRTOS_task_init(&task, dummy_fn1, &dummy1, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, 1);

    task_core_quickcpy(&TASK_BY_PID(0), &TASK_BY_PID(1));
    u_assert_ptr_equals(TASK_BY_PID(0).sp, TASK_BY_PID(1).sp);
    u_assert_var_equals(TASK_BY_PID(0).state, TASK_BY_PID(1).state);
    u_assert_ptr_equals(TASK_BY_PID(0).stack_bottom, TASK_BY_PID(1).stack_bottom);
    u_assert_var_equals(TASK_BY_PID(0).stack_count, TASK_BY_PID(1).stack_count);
    u_assert_var_equals(TASK_BY_PID(0).prio, TASK_BY_PID(1).prio);
}

UNIT_TEST(task_core_quickswap)
{
    int dummy0 = 0;
    int dummy1 = 0;
    struct picoRTOS_task task;
    picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT + 1];

    /* reset for next test */
    picoRTOS_init();

    /* we already established these ones work, so we can use them */
    picoRTOS_task_init(&task, dummy_fn0, &dummy0, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, 0);
    picoRTOS_task_init(&task, dummy_fn1, &dummy1, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, 1);

    task_core_quickswap(&TASK_BY_PID(0), &TASK_BY_PID(1));
    /* test 0 */
    u_assert_ptr_equals(TASK_BY_PID(0).sp, stack1);
    u_assert_ptr_equals(TASK_BY_PID(0).stack_bottom, stack1);
    u_assert_var_equals(TASK_BY_PID(0).stack_count, CONFIG_DEFAULT_STACK_COUNT + 1);
    u_assert_var_equals(TASK_BY_PID(0).prio, 1);
    /* test 1 */
    u_assert_ptr_equals(TASK_BY_PID(1).sp, stack0);
    u_assert_ptr_equals(TASK_BY_PID(1).stack_bottom, stack0);
    u_assert_var_equals(TASK_BY_PID(1).stack_count, CONFIG_DEFAULT_STACK_COUNT);
    u_assert_var_equals(TASK_BY_PID(1).prio, 0);
}

UNIT_TEST(picoRTOS_get_next_available_priority)
{
    int dummy = 0;
    picoRTOS_priority_t prio;
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    /* reset */
    picoRTOS_init();

    /* we already established these ones work, so we can use them */
    picoRTOS_task_init(&task, dummy_fn0, &dummy, stack, PICORTOS_STACK_COUNT(stack));
    picoRTOS_add_task(&task, 0);
    picoRTOS_add_task(&task, 2);
    picoRTOS_add_task(&task, 3);
    picoRTOS_add_task(&task, 5);

    prio = picoRTOS_get_next_available_priority();
    u_assert_var_equals(prio, 1);
    picoRTOS_add_task(&task, 1);

    prio = picoRTOS_get_next_available_priority();
    u_assert_var_equals(prio, 4);
    picoRTOS_add_task(&task, 4);

    prio = picoRTOS_get_next_available_priority();
    u_assert_var_equals(prio, 6);
    picoRTOS_add_task(&task, 6);

    prio = picoRTOS_get_next_available_priority();
    u_assert_var_equals(prio, 7);
}

UNIT_TEST(picoRTOS_get_last_available_priority)
{
    int dummy = 0;
    picoRTOS_priority_t prio;
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    /* reset */
    picoRTOS_init();

    /* we already established these ones work, so we can use them */
    picoRTOS_task_init(&task, dummy_fn0, &dummy, stack, PICORTOS_STACK_COUNT(stack));
    picoRTOS_add_task(&task, CONFIG_TASK_COUNT - 1);
    picoRTOS_add_task(&task, CONFIG_TASK_COUNT - 3);
    picoRTOS_add_task(&task, CONFIG_TASK_COUNT - 4);
    picoRTOS_add_task(&task, CONFIG_TASK_COUNT - 6);

    prio = picoRTOS_get_last_available_priority();
    u_assert_var_equals(prio, CONFIG_TASK_COUNT - 2);
    picoRTOS_add_task(&task, CONFIG_TASK_COUNT - 2);

    prio = picoRTOS_get_last_available_priority();
    u_assert_var_equals(prio, CONFIG_TASK_COUNT - 5);
    picoRTOS_add_task(&task, CONFIG_TASK_COUNT - 5);

    prio = picoRTOS_get_last_available_priority();
    u_assert_var_equals(prio, CONFIG_TASK_COUNT - 7);
    picoRTOS_add_task(&task, CONFIG_TASK_COUNT - 7);

    prio = picoRTOS_get_last_available_priority();
    u_assert_var_equals(prio, CONFIG_TASK_COUNT - 8);
}

UNIT_TEST(core_sort_tasks)
{
    int dummy = 0;
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    /* reset */
    picoRTOS_init();

    picoRTOS_task_init(&task, dummy_fn0, &dummy, stack, PICORTOS_STACK_COUNT(stack));
    /* 10 tasks */
    picoRTOS_add_task(&task, 5);
    picoRTOS_add_task(&task, 10);
    picoRTOS_add_task(&task, 9);
    picoRTOS_add_task(&task, 3);
    picoRTOS_add_task(&task, 3);
    picoRTOS_add_task(&task, 7);
    picoRTOS_add_task(&task, 31);
    picoRTOS_add_task(&task, 31);
    picoRTOS_add_task(&task, 31);
    picoRTOS_add_task(&task, 21);

    core_sort_tasks();
    u_assert_var_equals(TASK_BY_PID(0).prio, 3);
    u_assert_var_equals(TASK_BY_PID(1).prio, 3);
    u_assert_var_equals(TASK_BY_PID(2).prio, 5);
    u_assert_var_equals(TASK_BY_PID(3).prio, 7);
    u_assert_var_equals(TASK_BY_PID(4).prio, 9);
    u_assert_var_equals(TASK_BY_PID(5).prio, 10);
    u_assert_var_equals(TASK_BY_PID(6).prio, 21);
    u_assert_var_equals(TASK_BY_PID(7).prio, 31);
    u_assert_var_equals(TASK_BY_PID(8).prio, 31);
    u_assert_var_equals(TASK_BY_PID(9).prio, 31);
}

UNIT_TEST(core_arrange_shared_priorities)
{
    int dummy = 0;
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    /* reset */
    picoRTOS_init();

    picoRTOS_task_init(&task, dummy_fn0, &dummy, stack, PICORTOS_STACK_COUNT(stack));
    /* 10 tasks */
    picoRTOS_add_task(&task, 5);
    picoRTOS_add_task(&task, 10);
    picoRTOS_add_task(&task, 9);
    picoRTOS_add_task(&task, 3);
    picoRTOS_add_task(&task, 3);
    picoRTOS_add_task(&task, 7);
    picoRTOS_add_task(&task, 31);
    picoRTOS_add_task(&task, 31);
    picoRTOS_add_task(&task, 31);
    picoRTOS_add_task(&task, 21);

    core_sort_tasks(); /* needed */
    core_arrange_shared_priorities();

    u_assert_var_equals(TASK_BY_PID(0).sub_prio, 0);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(0).prio).count, 2);
    u_assert_var_equals(TASK_BY_PID(1).sub_prio, 1);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(0).prio).count, 2);
    u_assert_var_equals(TASK_BY_PID(2).sub_prio, 0);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(2).prio).count, 1);
    u_assert_var_equals(TASK_BY_PID(3).sub_prio, 0);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(3).prio).count, 1);
    u_assert_var_equals(TASK_BY_PID(4).sub_prio, 0);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(4).prio).count, 1);
    u_assert_var_equals(TASK_BY_PID(5).sub_prio, 0);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(5).prio).count, 1);
    u_assert_var_equals(TASK_BY_PID(6).sub_prio, 0);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(6).prio).count, 1);
    u_assert_var_equals(TASK_BY_PID(7).sub_prio, 0);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(7).prio).count, 3);
    u_assert_var_equals(TASK_BY_PID(8).sub_prio, 1);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(8).prio).count, 3);
    u_assert_var_equals(TASK_BY_PID(9).sub_prio, 2);
    u_assert_var_equals(SUB_BY_PRIO(TASK_BY_PID(9).prio).count, 3);
}

UNIT_TEST(picoRTOS_start)
{
    /* reset */
    picoRTOS_init();

    picoRTOS_start();
    u_assert(picoRTOS.is_running == true);

    /* reset for next test */
    picoRTOS_init();
}

UNIT_TEST(picoRTOS_suspend)
{
    /* reset */
    picoRTOS_init();

    RESET_STATE_MACHINE();
    picoRTOS_suspend();
    u_assert(halted);
}

UNIT_TEST(picoRTOS_resume)
{
    /* reset */
    picoRTOS_init();

    RESET_STATE_MACHINE();
    picoRTOS_resume();
    u_assert(halted);
}

UNIT_TEST(picoRTOS_schedule)
{
    /* reset */
    picoRTOS_init();

    RESET_STATE_MACHINE();
    picoRTOS_schedule();
    u_assert(halted);
}

UNIT_TEST(picoRTOS_sleep)
{
    /* reset */
    picoRTOS_init();

    RESET_STATE_MACHINE();
    picoRTOS_sleep(0);
    u_assert(halted);
}

UNIT_TEST(picoRTOS_get_tick)
{
    picoRTOS_tick_t tick;

    /* reset */
    picoRTOS_init();

    /* test #1 */
    RESET_STATE_MACHINE();
    picoRTOS_get_tick();
    u_assert(halted);

    /* test #2 */
    RESET_STATE_MACHINE();
    picoRTOS_start();
#ifdef TEST_PICORTOS_SMP
    core = 0;
    picoRTOS.index[core] = 0;
    TASK_CURRENT().state = PICORTOS_TASK_STATE_BUSY;
#endif
    tick = picoRTOS_get_tick();
    u_assert(!halted);
    u_assert_var_equals(tick, (picoRTOS_tick_t)-1);
}

UNIT_TEST(picoRTOS_sleep_until)
{
    picoRTOS_tick_t ref = 0;

    /* reset */
    picoRTOS_init();

    /* test #1, must fail */
    RESET_STATE_MACHINE();
    picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    u_assert(halted);

    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_task_init(&task, dummy_fn0, NULL, stack, PICORTOS_STACK_COUNT(stack));
    picoRTOS_add_task(&task, 0);
    picoRTOS_start();

    /* test #2, must fail */
    RESET_STATE_MACHINE();
    picoRTOS_sleep_until(&ref, 0);
    u_assert(halted);

    /* test #3 */
    RESET_STATE_MACHINE();
    picoRTOS.tick = 0; /* simulate 1st tick */
#ifndef TEST_PICORTOS_SMP
    picoRTOS.index = 0;
#else
    core = 0;
    picoRTOS.index[core] = 0;
    TASK_CURRENT().state = PICORTOS_TASK_STATE_BUSY;
#endif
    picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    u_assert(!halted);

    /* test #4, limit */
    picoRTOS.tick = PICORTOS_DELAY_SEC(1);
    picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    u_assert(!halted);
}

UNIT_TEST(picoRTOS_kill)
{
    /* reset */
    picoRTOS_init();

    RESET_STATE_MACHINE();
    picoRTOS_kill();
    u_assert(halted);
}

UNIT_TEST(picoRTOS_self)
{
    /* reset */
    picoRTOS_init();

    RESET_STATE_MACHINE();
    (void)picoRTOS_self();
    u_assert(halted);
}

UNIT_TEST(syscall_sleep)
{
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    struct syscall_sleep sleep;
    
    /* reset */
    picoRTOS_init();

    picoRTOS_task_init(&task, dummy_fn0, NULL, stack, PICORTOS_STACK_COUNT(stack));
    picoRTOS_add_task(&task, 0);

#ifndef TEST_PICORTOS_SMP
    picoRTOS.index = 0;
#else
    core = 0;
    picoRTOS.index[core] = 0;
#endif

    /* simulate 1st tick */
    picoRTOS.tick = 0;

    /* test #1 */
    sleep.delay = 0;
    syscall_sleep(&TASK_CURRENT(), &sleep);
    u_assert_var_equals(TASK_CURRENT().state, PICORTOS_TASK_STATE_READY);
    u_assert_var_equals(TASK_CURRENT().tick, 0);

    /* test #2 */
    sleep.delay = PICORTOS_DELAY_SEC(1);
    syscall_sleep(&TASK_CURRENT(), &sleep);
    u_assert_var_equals(TASK_CURRENT().state, PICORTOS_TASK_STATE_SLEEP);
    u_assert_var_equals(TASK_CURRENT().tick, PICORTOS_DELAY_SEC(1));
}

UNIT_TEST(syscall_kill)
{
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    /* reset */
    picoRTOS_init();

    picoRTOS_task_init(&task, dummy_fn0, NULL, stack, PICORTOS_STACK_COUNT(stack));
    picoRTOS_add_task(&task, 0);

#ifndef TEST_PICORTOS_SMP
    picoRTOS.index = 0;
#else
    core = 0;
    picoRTOS.index[core] = 0;
#endif

    /* test #1 */
    syscall_kill(&TASK_CURRENT());
    u_assert_var_equals(TASK_CURRENT().state, PICORTOS_TASK_STATE_EMPTY);
}

UNIT_TEST(syscall_sleep_until)
{
      struct picoRTOS_task task;
      picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

      struct syscall_sleep_until sleep_until;
    
      /* reset */
      picoRTOS_init();

      picoRTOS_task_init(&task, dummy_fn0, NULL, stack, PICORTOS_STACK_COUNT(stack));
      picoRTOS_add_task(&task, 0);

#ifndef TEST_PICORTOS_SMP
      picoRTOS.index = 0;
#else
      core = 0;
      picoRTOS.index[core] = 0;
#endif

      /* simulate 1st tick */
      picoRTOS.tick = 0;
      
      /* test #1 */
      sleep_until.ref = 0;
      sleep_until.period = 0;
      syscall_sleep_until(&TASK_CURRENT(), &sleep_until);
      u_assert_var_equals(TASK_CURRENT().state, PICORTOS_TASK_STATE_READY);
      u_assert_var_equals(TASK_CURRENT().tick, 0);
      u_assert_var_equals(TASK_CURRENT().deadline_miss_count, (size_t)1);
      
      /* test #2 */
      sleep_until.ref = 0;
      sleep_until.period = PICORTOS_DELAY_SEC(1);
      syscall_sleep_until(&TASK_CURRENT(), &sleep_until);
      u_assert_var_equals(TASK_CURRENT().state, PICORTOS_TASK_STATE_SLEEP);
      u_assert_var_equals(TASK_CURRENT().tick, PICORTOS_DELAY_SEC(1));
      u_assert_var_equals(TASK_CURRENT().deadline_miss_count, 0);
}

UNIT_TEST(picoRTOS_tick)
{
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];
    struct syscall_sleep sleep = { (picoRTOS_tick_t)3 };
    
    /* reset */
    picoRTOS_init();

    picoRTOS_task_init(&task, dummy_fn0, NULL, stack, PICORTOS_STACK_COUNT(stack));
    picoRTOS_add_task(&task, 0);
    picoRTOS_add_task(&task, 0);
    picoRTOS_add_task(&task, 1);

    picoRTOS_start();

    /* simulate tick */
    (void)picoRTOS_tick(TASK_CURRENT().sp);
#ifndef TEST_PICORTOS_SMP
    u_assert_var_equals(picoRTOS.index, 0);
#else
    u_assert_var_equals(picoRTOS.index[0], 0);
#endif

    (void)picoRTOS_tick(TASK_CURRENT().sp);
#ifndef TEST_PICORTOS_SMP
    u_assert_var_equals(picoRTOS.index, 1);
#else
    u_assert_var_equals(picoRTOS.index[0], 1);
#endif

    /* simulate sleep */
    picoRTOS_syscall(TASK_CURRENT().sp,
                     SYSCALL_SLEEP,
                     (void*)&sleep);

    (void)picoRTOS_tick(TASK_CURRENT().sp);
#ifndef TEST_PICORTOS_SMP
    u_assert_var_equals(picoRTOS.index, 0);
#else
    u_assert_var_equals(picoRTOS.index[0], 0);
#endif

    (void)picoRTOS_tick(TASK_CURRENT().sp);
#ifndef TEST_PICORTOS_SMP
    u_assert_var_equals(picoRTOS.index, 2);
#else
    u_assert_var_equals(picoRTOS.index[0], 2);
#endif
}

UNIT_TEST(syscall_switch_context)
{
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    /* reset */
    picoRTOS_init();

    picoRTOS_task_init(&task, dummy_fn0, NULL, stack, PICORTOS_STACK_COUNT(stack));
    picoRTOS_add_task(&task, 0);
    picoRTOS_add_task(&task, 0);
    picoRTOS_add_task(&task, 1);
    picoRTOS_add_task(&task, 1);

    picoRTOS_start();

    /* simulate tick */
    (void)picoRTOS_tick(TASK_CURRENT().sp);

    /* simulate context switch */
    (void)syscall_switch_context(&TASK_CURRENT());
#ifndef TEST_PICORTOS_SMP
    u_assert_var_equals(picoRTOS.index, 2);
#else
    u_assert_var_equals(picoRTOS.index[0], 2);
#endif

    (void)syscall_switch_context(&TASK_CURRENT());
#ifndef TEST_PICORTOS_SMP
    u_assert_var_equals(picoRTOS.index, TASK_IDLE_PID);
#else
    u_assert_var_equals(picoRTOS.index[0], TASK_IDLE_PID);
#endif

    (void)picoRTOS_tick(TASK_CURRENT().sp);

    (void)syscall_switch_context(&TASK_CURRENT());
#ifndef TEST_PICORTOS_SMP
    u_assert_var_equals(picoRTOS.index, 3);
#else
    u_assert_var_equals(picoRTOS.index[0], 3);
#endif

    (void)syscall_switch_context(&TASK_CURRENT());
#ifndef TEST_PICORTOS_SMP
    u_assert_var_equals(picoRTOS.index, TASK_IDLE_PID);
#else
    u_assert_var_equals(picoRTOS.index[0], TASK_IDLE_PID);
#endif
}

#ifdef TEST_PICORTOS_SMP
UNIT_TEST(picoRTOS_SMP_add_task)
{
    struct picoRTOS_task task;
    picoRTOS_stack_t stack[CONFIG_DEFAULT_STACK_COUNT];

    /* we already established picoRTOS_add_task works, so we just check
     * for core_mask */
    picoRTOS_init();
    picoRTOS_task_init(&task, dummy_fn0, NULL, stack, PICORTOS_STACK_COUNT(stack));

    /* test #1, must fail */
    RESET_STATE_MACHINE();
    picoRTOS_SMP_add_task(&task, 0, 0);
    u_assert(halted);

    /* test #2, must fail */
    RESET_STATE_MACHINE();
    picoRTOS_SMP_add_task(&task, 0, (1 << CONFIG_SMP_CORES));
    u_assert(halted);

    /* test #3 */
    RESET_STATE_MACHINE();
    picoRTOS_SMP_add_task(&task, 0, (1 << 0));
    picoRTOS_SMP_add_task(&task, 0, (1 << (CONFIG_SMP_CORES - 1)));
    u_assert(!halted);
    u_assert_ptr_equals(TASK_BY_PID(0).core_mask, (1 << 0));
    u_assert_ptr_equals(TASK_BY_PID(1).core_mask, (1 << (CONFIG_SMP_CORES - 1)));
}
#endif

int main(void)
{
    RUN_TEST(task_core_init);
    RUN_TEST(task_idle_init);
    RUN_TEST(picoRTOS_init);
    RUN_TEST(picoRTOS_task_init);
    RUN_TEST(picoRTOS_add_task);
    RUN_TEST(task_core_quickcpy);
    RUN_TEST(task_core_quickswap);
    RUN_TEST(picoRTOS_get_next_available_priority);
    RUN_TEST(picoRTOS_get_last_available_priority);
    RUN_TEST(core_sort_tasks);
    RUN_TEST(core_arrange_shared_priorities);
    RUN_TEST(picoRTOS_start);
    RUN_TEST(picoRTOS_suspend);
    RUN_TEST(picoRTOS_resume);
    RUN_TEST(picoRTOS_schedule);
    RUN_TEST(picoRTOS_sleep);
    RUN_TEST(picoRTOS_get_tick);
    RUN_TEST(picoRTOS_sleep_until);
    RUN_TEST(picoRTOS_kill);
    RUN_TEST(picoRTOS_self);
    RUN_TEST(syscall_sleep);
    RUN_TEST(syscall_kill);
    RUN_TEST(syscall_sleep_until);
    
    /* more complex stuff */
    RUN_TEST(picoRTOS_tick);
    RUN_TEST(syscall_switch_context);

#ifdef TEST_PICORTOS_SMP
    RUN_TEST(picoRTOS_SMP_add_task);
#endif

    return 0;
}
