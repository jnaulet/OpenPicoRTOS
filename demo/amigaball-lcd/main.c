#include "board.h"
#include "picoRTOS.h"

#define CONFIG_SHORT_STACK_COUNT   (CONFIG_DEFAULT_STACK_COUNT / 2)

static struct render {
    long x;
    long y;
} render;

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

static void amigaBall_render_zero(void)
{
    render.x = 0;
    render.y = 0;
}

static void amigaBall_render_n(uint16_t *pfb, long px, long py, long ph, size_t n)
{
    /* Render into framebuffer */
    for (; render.y < LCD_HEIGHT; render.y++) {
        for (; render.x < LCD_WIDTH; render.x++) {

            long fx = (render.x * 256) - px;
            long fy = (render.y * 256) - py;
            long c = amigaBall(fx, fy, ph);

            if (c >= 0) *pfb++ = LCD_PX16((uint16_t)c);
            else *pfb++ = LCD_PX16(bgcolxy(render.x, render.y));

            if (n-- == 0)
                return;
        }
        /* Quick & dirty fix */
        render.x = 0;
    }
}

#ifndef LCD_FPS
# error LCD_FPS is undefined
#endif

static void lcd_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

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

        int n = LCD_FB_DIVIDER;

        /* init */
        amigaBall_render_zero();
        lcd_zero(&lcd);

        /* Render ball & refresh */
        while (n-- != 0) {
            (void)amigaBall_render_n(lcd.fb, px, py, ph, (size_t)LCD_FB_PIXELS);
            (void)lcd_refresh(&lcd);
        }

        /* Update ball position */
        px += dx;
        py += dy;

        /* Rotation */
        if (dx > 0) ph += 256l;
        else ph -= 256l;

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
        dy += 16l;

        /* LCD_FPS refresh rate */
        picoRTOS_sleep_until(&ref, (picoRTOS_tick_t)(CONFIG_TICK_HZ / LCD_FPS));
    }
}

static void led_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

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
    picoRTOS_assert_fatal(priv != NULL, return );

    struct wd *WD = (struct wd*)priv;

    for (;;) {
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(7l));
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
