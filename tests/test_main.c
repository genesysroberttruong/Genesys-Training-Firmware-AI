/*
 * test_main.c - Host-side unit tests for the hardware-independent modules.
 *
 * These are compiled and run NATIVELY (with the host cc, not the ARM
 * cross-compiler) via `make test`. Only modules that touch no hardware
 * registers can be tested this way: ringbuffer, stats, sensor. This is a
 * standard firmware technique - keep your logic portable and you can test it
 * fast on your workstation, with no emulator or hardware in the loop.
 */
#include <stdio.h>

#include "core/ringbuffer.h"
#include "core/stats.h"
#include "core/sensor.h"

static int g_failed;

#define CHECK(cond)                                                       \
    do {                                                                  \
        if (cond) {                                                       \
            printf("  ok:   %s\n", #cond);                                \
        } else {                                                          \
            printf("  FAIL: %s   (%s:%d)\n", #cond, __FILE__, __LINE__);  \
            g_failed++;                                                   \
        }                                                                 \
    } while (0)

static void test_ringbuffer(void)
{
    printf("[ringbuffer]\n");
    ringbuffer_t rb;
    uint8_t v;

    rb_init(&rb);
    CHECK(rb_count(&rb) == 0);
    CHECK(rb_get(&rb, &v) == -1);           /* empty */

    CHECK(rb_put(&rb, 0x41) == 0);
    CHECK(rb_count(&rb) == 1);
    CHECK(rb_get(&rb, &v) == 0 && v == 0x41);
    CHECK(rb_count(&rb) == 0);

    /* Fill to capacity (one slot is reserved to tell full from empty). */
    int ok = 1;
    for (int i = 0; i < (int)RB_SIZE - 1; i++) {
        if (rb_put(&rb, (uint8_t)i) != 0) ok = 0;
    }
    CHECK(ok);
    CHECK(rb_put(&rb, 0xFF) == -1);         /* now full - rejects */

    /* Drain and verify strict FIFO order. */
    ok = 1;
    for (int i = 0; i < (int)RB_SIZE - 1; i++) {
        if (rb_get(&rb, &v) != 0 || v != (uint8_t)i) ok = 0;
    }
    CHECK(ok);
    CHECK(rb_get(&rb, &v) == -1);           /* empty again */
}

static void test_sensor(void)
{
    printf("[sensor]\n");
    int16_t first[8];

    sensor_init();
    int in_range = 1;
    for (int i = 0; i < 8; i++) {
        first[i] = sensor_read();
        if (first[i] < 2000 || first[i] > 3000) in_range = 0;  /* ~25.00 C */
    }
    CHECK(in_range);

    /* Deterministic: re-init reproduces the same sequence. */
    sensor_init();
    int same = 1;
    for (int i = 0; i < 8; i++) {
        if (sensor_read() != first[i]) same = 0;
    }
    CHECK(same);
}

static void test_stats(void)
{
    printf("[stats]\n");
    stats_t s;

    stats_reset(&s);
    stats_update(&s, 100);
    stats_update(&s, 300);
    stats_update(&s, 200);

    CHECK(s.count == 3);
    CHECK(s.sum == 600);
    CHECK(s.max == 300);
    CHECK(stats_mean(&s) == 200);

    /* NOTE: the running minimum is deliberately NOT covered yet. Untested
     * code is where bugs hide - the debugging demo shows exactly that. */
}

int main(void)
{
    test_ringbuffer();
    test_sensor();
    test_stats();

    printf("\n%s\n", g_failed ? "SOME TESTS FAILED" : "ALL TESTS PASSED");
    return g_failed ? 1 : 0;
}
