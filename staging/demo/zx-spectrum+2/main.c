#include <picoRTOS.h>
#include <picoRTOS_device.h>

static unsigned char *SCREEN1 = (unsigned char*)ADDR_SCREEN1;
static unsigned char *COLOR = (unsigned char*)ADDR_COLOR;

static void tick_main(void *priv)
{
    picoRTOS_assert_fatal(priv == NULL, return );

    int i = 0;
    picoRTOS_tick_t ref = picoRTOS_get_tick();

    for (;;) {
        *SCREEN1 = (unsigned char)0xff;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60));
        *SCREEN1 = (unsigned char)0;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(60));
        *SCREEN1 = (unsigned char)0xff;
        picoRTOS_sleep(PICORTOS_DELAY_MSEC(120));
        *SCREEN1 = (unsigned char)0;

        /* delay */
        picoRTOS_sleep_until(&ref, PICORTOS_DELAY_SEC(1));
    }
}

// ------------------------------------------------------------------------
// Code for rendering the "Amiga ball".
// ------------------------------------------------------------------------

static long w_approx(long x)
{
    // Generated in Python:
    // for i in range(32):
    //     print(int(8192 / ((1 - (i/32.0)**2)**0.5)))
    // print(65535)
    static const long s_lut[33] = {
        8192l,  8196l,  8208l,  8228l,  8256l,  8293l,  8339l,  8395l,  8460l,  8536l,
        8623l,  8723l,  8836l,  8965l,  9110l,  9273l,  9459l,  9669l,  9908l,  10180l,
        10494l, 10856l, 11280l, 11782l, 12385l, 13123l, 14052l, 15262l, 16921l, 19378l,
        23541l, 33027l, 65535l
    };

    /* fix values */
    if (x < 0) x = -x;
    if (x > 8191l) x = 8191l;

    long m = x / 256l;
    long f = x & 0xffl;
    long v0 = s_lut[m];
    long v1 = s_lut[m + 1];

    return ((v0 * (0xffl - f)) + (v1 * f)) / 256l;
}

static long approx_asin(long x)
{
    x = (x * 11l) / 8l;
    long x2 = (x * x) / 8192l;
    long x3 = (x2 * x) / 8192l;
    long x5 = (x2 * x3) / 8192l;

    x += (1365l * x3 + 614l * x5) / 8192l;
    return x;
}

static long c_remap(long x)
{
    if (x < 256l)
        return 256l - x;

    if (x < 4096l)
        return 0;

    if (x < 4096l + 256l)
        return x - 4096l;

    return 256l;
}

long amigaBall(long x_, long y_, long ph_)
{
    x_ /= 2l;
    y_ /= 2l;
    x_ -= 8192l;
    y_ -= 8192l;

    long x = ( 8028l * x_ + 1627l * y_) / 8192l;
    long y = (-1627l * x_ + 8028l * y_) / 8192l;

    long r = x * x + y * y;

    if (r > (1l << 26))
        return -1l;

    x = (x * w_approx(y)) / 8192l;
    y = approx_asin(y);
    x = approx_asin(x) + ph_;

    x &= 0x1fffl;
    y &= 0x1fffl;

    long cx = c_remap(x);
    long cy = c_remap(y);
    long cc = (((cx - 128l) * (cy - 128l)) / 128l) + 128l;

    long R = 0x1fl;
    long G = (0x3fl * cc) / 256l;
    long B = (0x1fl * cc) / 256l;

    cc = 256l - c_remap((1l << 12) - (r / 16384l));
    R = (R * cc) / 256l;
    G = (G * cc) / 256l;
    B = (B * cc) / 256l;

    return (long)(((unsigned long)R << 11) + ((unsigned long)G << 5)) + B;
}

// ------------------------------------------------------------------------

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

static void display_main(void *priv)
{
    picoRTOS_assert_fatal(priv != NULL, return );

    picoRTOS_tick_t ref = picoRTOS_get_tick();

    /* Example with framebuffer upload after each frame completion (Amiga ball) */
    long px = 0;
    long py = 0;
    long dx = 1l << 8;
    long dy = 1l << 8;
    long ph = 0;

    for (;;) {

        int n = LCD_FB_DIVIDER;

        /* init */
        amigaBall_render_zero();

        /* Render ball & refresh */
        while (n-- != 0)
            (void)amigaBall_render_n(lcd.fb, px, py, ph, (size_t)LCD_FB_PIXELS)

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
        //picoRTOS_sleep_until(&ref, (picoRTOS_tick_t)(CONFIG_TICK_HZ / LCD_FPS));
	picoRTOS_schedule();
    }
}

int main(void)
{
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_init();

    /* dummy */
    picoRTOS_task_init(&task, tick_main, NULL, stack0, PICORTOS_STACK_COUNT(stack0));
    picoRTOS_add_task(&task, picoRTOS_get_next_available_priority());

    picoRTOS_start();

    /* not supposed to end there */
    picoRTOS_assert_void(false);
    return 1;
}
