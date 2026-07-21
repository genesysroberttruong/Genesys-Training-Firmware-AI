# examples/

Reference code you copy from, not code that ships. Nothing here is part of the
firmware build (the `Makefile` only compiles `src/`).

## driver_template.[ch]

The canonical shape of a memory-mapped peripheral driver for this project:
a register map filled from the datasheet, bounded hardware waits, explicit
status codes, and static internal helpers.

Use it as a concrete "write like this" reference for the AI:

```
Write a driver for <your peripheral> following examples/driver_template.c,
using the register map in docs/datasheets/. Put it in src/hal/.
```

Giving the model an example beats describing conventions in prose — it copies
the structure, error handling, and naming automatically.
