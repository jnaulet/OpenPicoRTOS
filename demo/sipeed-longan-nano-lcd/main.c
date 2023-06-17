#include "picoRTOS.h"
#include "sipeed-longan-nano.h"

static uint16_t bgcolxy(int x, int y)
{
    int r;

    x -= (LCD_WIDTH / 2);
    y -= (LCD_HEIGHT / 2);
    y *= 2;

    r = x * x + y * y;
    return (uint16_t)r >> 11;
}

/*@external@*/ extern int amigaBall(int x_, int y_, int ph_);

static void amigaBall_render(uint16_t *pfb, int px, int py, int ph)
{
    /* Render into framebuffer */
    int x, y;

    for (y = 0; y < LCD_HEIGHT; y++)
        for (x = 0; x < LCD_WIDTH; x++) {
            int fx = (x * 256) - px;
            int fy = (y * 256) - py;
            int c = amigaBall(fx, fy, ph);

            if (c >= 0) *pfb++ = (uint16_t)c;
            else *pfb++ = bgcolxy(x, y);
        }
}

static void lcd_main(void *priv)
{
    static struct lcd lcd;
    picoRTOS_tick_t ref = picoRTOS_get_tick();
    struct lcd_phys *phys = (struct lcd_phys*)priv;

    /* Example with framebuffer upload after each frame completion (Amiga ball) */
    int px = 0;
    int py = 0;
    int dx = 1 << 8;
    int dy = 1 << 8;
    int ph = 0;

    (void)lcd_init(&lcd, phys);

    for (;;) {

        /* Render ball & refresh */
        (void)amigaBall_render((uint16_t*)lcd.fb, px, py, ph);
        (void)lcd_refresh(&lcd);

        /* Update ball position */
        px += dx;
        py += dy;

        /* Rotation */
        if (dx > 0) ph += 256;
        else ph -= 256;

        /* Floor */
        if (py > (16 * 256)) {
            py = (16 * 512) - py;
            dy = -dy;
        }

        /* Right wall */
        if (px > (96 * 256)) {
            px = (96 * 512) - px;
            dx = -dx;
        }

        /* Left wall */
        if (px < (-64 * 256)) {
            px = (-64 * 512) - px;
            dx = -dx;
        }

        /* Apply gravity */
        dy += 16;

        /* 16.6fps refresh */
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_MSEC(60));
    }
}

static void led_main(void *priv)
{
    int status = 0;
    struct led *LED = (struct led*)priv;

    for (;;) {
        /* turn off LEDs */
        gpio_write(&LED->R, true);
        gpio_write(&LED->G, true);
        gpio_write(&LED->B, true);

        switch (status++ % 3) {
        case 0: gpio_write(&LED->R, false); break;
        case 1: gpio_write(&LED->G, false); break;
        case 2: gpio_write(&LED->B, false); break;
        default: picoRTOS_break();
        }

        picoRTOS_sleep(PICORTOS_DELAY_SEC(1));
    }
}

int main(void)
{
    static struct sipeed_longan_nano nano;

    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    (void)sipeed_longan_nano_init(&nano);
    picoRTOS_init();

    /* LCD */
    picoRTOS_task_init(&task, lcd_main, &nano.lcd_phys, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());
    /* LEDs */
    picoRTOS_task_init(&task, led_main, &nano.LED, stack1, PICORTOS_STACK_COUNT(stack1));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    /* start scheduling */
    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
