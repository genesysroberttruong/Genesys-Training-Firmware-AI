/*
 * sensor.c - Deterministic simulated temperature source.
 *
 * signal = 2500 (25.00 C baseline)
 *        + one period of a hand-rolled sine look-up table (+/- ~2.5 C)
 *        + small pseudo-random noise from a 16-bit LFSR (+/- 0.05 C)
 */
#include "sensor.h"

/* One period of a sine wave, scaled to +/-250 (== +/-2.50 C). */
static const int16_t s_sine[16] = {
       0,   96,  180,  235,  250,  235,  180,   96,
       0,  -96, -180, -235, -250, -235, -180,  -96,
};

static uint16_t s_idx;
static uint16_t s_lfsr = 0xACE1u;   /* non-zero seed */

void sensor_init(void)
{
    s_idx  = 0;
    s_lfsr = 0xACE1u;
}

int16_t sensor_read(void)
{
    s_idx = (uint16_t)((s_idx + 1u) & 15u);

    /* 16-bit Galois LFSR, taps 0xB400. */
    uint16_t lsb = s_lfsr & 1u;
    s_lfsr >>= 1;
    if (lsb) {
        s_lfsr ^= 0xB400u;
    }
    int16_t noise = (int16_t)((s_lfsr % 11u)) - 5;   /* -5 .. +5 */

    return (int16_t)(2500 + s_sine[s_idx] + noise);
}
