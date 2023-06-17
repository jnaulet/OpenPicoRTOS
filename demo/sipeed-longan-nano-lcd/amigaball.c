// ------------------------------------------------------------------------
// Code for rendering the "Amiga ball".
// ------------------------------------------------------------------------

static int w_approx(int x)
{
    // Generated in Python:
    // for i in range(32):
    //     print(int(8192 / ((1 - (i/32.0)**2)**0.5)))
    // print(65535)
    static const int s_lut[33] = {
        8192,  8196,  8208,  8228,  8256,  8293,  8339,  8395,  8460,  8536,
        8623,  8723,  8836,  8965,  9110,  9273,  9459,  9669,  9908,  10180,
        10494, 10856, 11280, 11782, 12385, 13123, 14052, 15262, 16921, 19378,
        23541, 33027, 65535
    };

    /* fix values */
    if (x < 0) x = -x;
    if (x > 8191) x = 8191;

    int m = x / 256;
    int f = x & 0xff;
    int v0 = s_lut[m];
    int v1 = s_lut[m + 1];

    return ((v0 * (0xff - f)) + (v1 * f)) / 256;
}

static int approx_asin(int x)
{
    x = (x * 11) / 8;
    int x2 = (x * x) / 8192;
    int x3 = (x2 * x) / 8192;
    int x5 = (x2 * x3) / 8192;

    x += (1365 * x3 + 614 * x5) / 8192;
    return x;
}

static int c_remap(int x)
{
    if (x < 256)
        return 256 - x;

    if (x < 4096)
        return 0;

    if (x < 4096 + 256)
        return x - 4096;

    return 256;
}

int amigaBall(int x_, int y_, int ph_)
{
    x_ /= 2;
    y_ /= 2;
    x_ -= 8192;
    y_ -= 8192;

    int x = ( 8028 * x_ + 1627 * y_) / 8192;
    int y = (-1627 * x_ + 8028 * y_) / 8192;

    int r = x * x + y * y;

    if (r > (1 << 26))
        return -1;

    x = (x * w_approx(y)) / 8192;
    y = approx_asin(y);
    x = approx_asin(x) + ph_;

    x &= 0x1fff;
    y &= 0x1fff;

    int cx = c_remap(x);
    int cy = c_remap(y);
    int cc = (((cx - 128) * (cy - 128)) / 128) + 128;

    int R = 0x1f;
    int G = (0x3f * cc) / 256;
    int B = (0x1f * cc) / 256;

    cc = 256 - c_remap((1 << 12) - (r / 16384));
    R = (R * cc) / 256;
    G = (G * cc) / 256;
    B = (B * cc) / 256;

    return (int)(((unsigned)R << 11) + ((unsigned)G << 5)) + B;
}

// ------------------------------------------------------------------------
