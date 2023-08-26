#include "board.h"
#include "picoRTOS.h"

#include <generated/autoconf.h>

#define CONFIG_SHORT_STACK_COUNT   128

static uint16_t bgcolxy(long x, long y)
{
    long r;

    x -= (LCD_WIDTH / 2);
    y -= (LCD_HEIGHT / 2);
    y *= 2;

    r = x * x + y * y;
    return (uint16_t)r >> 11;
}

/*@external@*/ extern long amigaBall(long x_, long y_, long ph_);

static void amigaBall_render(uint16_t *pfb, long px, long py, long ph)
{
    /* Render into framebuffer */
    long x, y;

    for (y = 0; y < LCD_HEIGHT; y++)
        for (x = 0; x < LCD_WIDTH; x++) {
            long fx = (x * 256) - px;
            long fy = (y * 256) - py;
            long c = amigaBall(fx, fy, ph);

            if (c >= 0) *pfb++ = LCD_PX16((uint16_t)c);
            else *pfb++ = LCD_PX16(bgcolxy(x, y));
        }
}

#ifndef LCD_FPS
# error LCD_FPS is undefined
#endif

static void lcd_main(void *priv)
{
    picoRTOS_tick_t ref;
    static struct lcd lcd;
    struct lcd_phys *phys = (struct lcd_phys*)priv;

    /* Example with framebuffer upload after each frame completion (Amiga ball) */
    long px = 0;
    long py = 0;
    long dx = 1l << 8;
    long dy = 1l << 8;
    long ph = 0;

    (void)lcd_init(&lcd, phys);

    /* init ref AFTER LCD */
    ref = picoRTOS_get_tick();

    for (;;) {

        /* Render ball & refresh */
        (void)amigaBall_render(lcd.fb, px, py, ph);
        (void)lcd_refresh(&lcd);

        /* Update ball position */
        px += dx;
        py += dy;

        /* Rotation */
        if (dx > 0) ph += 256;
        else ph -= 256;

        /* Floor */
        if (py > (16l * 256l)) {
            py = (16l * 512l) - py;
            dy = -dy;
        }

        /* Right wall */
        if (px > (96l * 256l)) {
            px = (96l * 512l) - px;
            dx = -dx;
        }

        /* Left wall */
        if (px < (-64l * 256l)) {
            px = (-64l * 512l) - px;
            dx = -dx;
        }

        /* Apply gravity */
        dy += 16;

        /* LCD_FPS refresh rate */
        picoRTOS_sleep_until(&ref, (picoRTOS_tick_t)(CONFIG_TICK_HZ / LCD_FPS));
    }
}

static void led_main(void *priv)
{
    struct gpio *LED = (struct gpio*)priv;

    for (;;) {
        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30l));
        gpio_write(LED, true);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(30l));
        gpio_write(LED, false);
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60l));
        gpio_write(LED, true);

        picoRTOS_sleep(PICORTOS_DELAY_SEC(1));
    }
}

static void wd_main(void *priv)
{
    picoRTOS_assert_void(priv != NULL);

    struct wd *WD = (struct wd*)priv;

    for (;;) {
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(7));
        wd_refresh(WD);
    }
}

int main(void)
{
    static struct board board;

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_SHORT_STACK_COUNT];
    static picoRTOS_stack_t stack2[CONFIG_SHORT_STACK_COUNT];

    (void)board_init(&board);
    picoRTOS_init();

    /* Watchdog */
    picoRTOS_task_init(&task, wd_main, board.WD, stack2, PICORTOS_STACK_COUNT(stack2));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* LCD */
    picoRTOS_task_init(&task, lcd_main, &board.lcd_phys, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* LEDs */
    picoRTOS_task_init(&task, led_main, board.LED, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* start scheduling */
    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
