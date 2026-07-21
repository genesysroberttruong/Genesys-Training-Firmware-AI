/*
 * syscalls.c - Minimal newlib system-call stubs.
 *
 * We build with a real C library (for vsnprintf), which expects a few
 * low-level hooks. We provide a tiny heap for _sbrk() (used indirectly by
 * the formatting code) and stub out the rest, since this firmware has no
 * filesystem or OS underneath it.
 */
#include <sys/stat.h>
#include <stddef.h>
#include <stdint.h>

extern uint32_t _end;   /* start of heap, from the linker script */

void *_sbrk(ptrdiff_t incr)
{
    static uint8_t *heap = NULL;
    uint8_t *prev;

    if (heap == NULL) {
        heap = (uint8_t *)&_end;
    }
    prev = heap;
    heap += incr;
    return prev;
}

int   _write(int f, const char *b, int n) { (void)f; (void)b; return n; }
int   _read(int f, char *b, int n)        { (void)f; (void)b; (void)n; return 0; }
int   _close(int f)                       { (void)f; return -1; }
int   _lseek(int f, int o, int w)         { (void)f; (void)o; (void)w; return 0; }
int   _fstat(int f, struct stat *s)       { (void)f; s->st_mode = S_IFCHR; return 0; }
int   _isatty(int f)                      { (void)f; return 1; }
int   _getpid(void)                       { return 1; }
int   _kill(int p, int s)                 { (void)p; (void)s; return -1; }
void  _exit(int code)                     { (void)code; for (;;) { } }
