# LM3S6965 — extracted register reference (subset)

Only the peripherals this project uses, transcribed for quick reference and
for grounding AI-generated register code. For anything else, consult the full
**TI Stellaris LM3S6965 Microcontroller Data Sheet** (place the PDF in this
folder). Names/offsets below match `include/lm3s6965.h`.

## UART0 — Stellaris UART (PL011-family)

**Base address:** `0x4000C000`

| Register | Offset | Notes                                    |
|----------|--------|------------------------------------------|
| UARTDR   | 0x000  | Data register (read = RX, write = TX)    |
| UARTFR   | 0x018  | Flag register (see bits below)           |
| UARTIBRD | 0x024  | Integer baud-rate divisor                |
| UARTFBRD | 0x028  | Fractional baud-rate divisor             |
| UARTLCRH | 0x02C  | Line control (word length, FIFO enable)  |
| UARTCTL  | 0x030  | Control (UART enable, TX/RX enable)      |
| UARTIM   | 0x038  | Interrupt mask                           |
| UARTICR  | 0x044  | Interrupt clear                          |

**UARTFR (flag register) bits:**

| Bit | Name | Meaning                          |
|-----|------|----------------------------------|
| 3   | BUSY | UART is transmitting             |
| 4   | RXFE | Receive FIFO empty               |
| 5   | TXFF | Transmit FIFO full               |
| 6   | RXFF | Receive FIFO full                |
| 7   | TXFE | Transmit FIFO empty              |

> On real silicon you must enable the UART's peripheral clock (via the RCGC
> registers) and program the baud-rate divisors before use. Under QEMU with
> `-nographic` the model needs none of that — UART0 is wired straight to the
> terminal — which is why `uart_init()` is empty in this demo.

## SysTick — Cortex-M3 system timer (ARMv7-M core peripheral)

**Base address:** `0xE000E010`

| Register  | Offset | Notes                                       |
|-----------|--------|---------------------------------------------|
| SYST_CSR  | 0x000  | Control & status                            |
| SYST_RVR  | 0x004  | Reload value (24-bit); tick period − 1      |
| SYST_CVR  | 0x008  | Current value (write any value to clear)    |

**SYST_CSR bits:**

| Bit | Name      | Meaning                                     |
|-----|-----------|---------------------------------------------|
| 0   | ENABLE    | Enable the counter                          |
| 1   | TICKINT   | Assert the SysTick exception on count to 0  |
| 2   | CLKSOURCE | 1 = processor clock, 0 = external reference |

The SysTick exception is vector 15 in the table (`src/startup.c`); its handler
is `SysTick_Handler()` in `src/core/scheduler.c`.
