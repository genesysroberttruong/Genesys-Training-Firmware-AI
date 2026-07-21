/*
 * Linker script for the LM3S6965 (QEMU lm3s6965evb).
 *   FLASH : 256 KB at 0x00000000  (code + read-only data + initial .data image)
 *   SRAM  :  64 KB at 0x20000000  (.data, .bss, stack, heap)
 */
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
  SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Top of stack = end of SRAM (stack grows downward on Cortex-M). */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
  /* Interrupt vector table must sit at the very start of flash (0x0). */
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  .text :
  {
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  /* .data lives in SRAM at run time but is stored ("loaded") in flash.
     The startup code copies it from _sidata -> [_sdata.._edata). */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > SRAM AT> FLASH
  _sidata = LOADADDR(.data);

  /* .bss is zero-initialised by the startup code. */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > SRAM

  /* _end marks the start of the heap used by _sbrk() (see syscalls.c). */
  . = ALIGN(8);
  _end = .;
}
