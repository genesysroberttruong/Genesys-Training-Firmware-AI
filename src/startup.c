/*
 * startup.c - Cortex-M3 reset vector and C runtime bring-up.
 *
 * On reset the CPU loads the initial stack pointer from vector[0] and jumps
 * to vector[1] (Reset_Handler). Reset_Handler copies initialised data into
 * SRAM, zeroes the .bss, then calls main().
 */
#include <stdint.h>

/* Symbols provided by the linker script (linker/lm3s6965.ld). */
extern uint32_t _sidata;   /* flash image of .data          */
extern uint32_t _sdata;    /* start of .data in SRAM        */
extern uint32_t _edata;    /* end   of .data in SRAM        */
extern uint32_t _sbss;     /* start of .bss  in SRAM        */
extern uint32_t _ebss;     /* end   of .bss  in SRAM        */
extern uint32_t _estack;   /* top of stack (end of SRAM)    */

int  main(void);
void Reset_Handler(void);
void Default_Handler(void);

/* SysTick_Handler is defined in core/scheduler.c. */
void SysTick_Handler(void);

typedef void (*vector_t)(void);

/*
 * The ARMv7-M vector table. Entry 0 is the initial stack pointer; the rest
 * are exception handlers. We only wire up Reset and SysTick; everything else
 * traps into Default_Handler so a fault is easy to spot in the debugger.
 */
__attribute__((section(".isr_vector"), used))
const vector_t g_vectors[] = {
    (vector_t)((uintptr_t)&_estack), /*  0: Initial stack pointer   */
    Reset_Handler,                   /*  1: Reset                   */
    Default_Handler,                 /*  2: NMI                     */
    Default_Handler,                 /*  3: HardFault               */
    Default_Handler,                 /*  4: MemManage               */
    Default_Handler,                 /*  5: BusFault                */
    Default_Handler,                 /*  6: UsageFault              */
    0, 0, 0, 0,                      /*  7-10: reserved             */
    Default_Handler,                 /* 11: SVCall                  */
    Default_Handler,                 /* 12: DebugMonitor            */
    0,                               /* 13: reserved                */
    Default_Handler,                 /* 14: PendSV                  */
    SysTick_Handler,                 /* 15: SysTick                 */
};

void Reset_Handler(void)
{
    /* Copy the .data section from its flash image into SRAM. */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero the .bss section. */
    for (dst = &_sbss; dst < &_ebss; ) {
        *dst++ = 0u;
    }

    main();

    /* main() should never return; if it does, hang. */
    for (;;) { }
}

void Default_Handler(void)
{
    for (;;) { }
}
