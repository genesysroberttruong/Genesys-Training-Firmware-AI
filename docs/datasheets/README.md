# Datasheets & hardware references

Drop the reference material for your target here so the AI can **ground its
work in the real hardware** instead of guessing register addresses or bit
positions — the single biggest source of hallucinated firmware code.

## What to put here

- MCU / SoC **reference manual** and **datasheet** (PDF is fine — Claude Code
  can read PDFs).
- Peripheral datasheets for external parts (sensors, radios, PMICs...).
- **Board schematic** or a pin-mux / net-list summary.
- Errata sheets.
- Anything you'd otherwise keep in a browser tab while coding.

## Tips that make a real difference

- **Prefer extracted text/markdown for the parts you use often.** A PDF works,
  but a short markdown table of the exact registers you touch (like
  [lm3s6965-peripherals.md](lm3s6965-peripherals.md)) is faster and more
  reliable for the model to cite — and easy for you to verify.
- **Point the AI at the specific file**: *"Using `docs/datasheets/tmp102.pdf`,
  write the register read for the temperature register."* Named sources beat
  "look it up".
- **Keep the register names identical** to the datasheet so the AI's code and
  the manual line up when you review.
- Large PDFs: tell the AI the section or page range to focus on.

## In this repo

This QEMU-based demo only exercises the UART and SysTick, so the extracted
reference in [lm3s6965-peripherals.md](lm3s6965-peripherals.md) covers just
those. On a real project you'd have the full TI LM3S6965 datasheet here too.
