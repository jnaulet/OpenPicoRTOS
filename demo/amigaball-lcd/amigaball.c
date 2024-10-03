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
