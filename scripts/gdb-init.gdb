# GDB startup script for the workshop debug session.
# Loaded by scripts/gdb.sh once QEMU is halted and waiting on :1234.

set architecture arm
set pagination off
target remote :1234

# Break where the running statistics are updated - the debug demo inspects
# the accumulator here to see why 'min' never changes.
break stats_update

echo \n
echo [gdb] Connected to QEMU. A breakpoint is set at stats_update().\n
echo [gdb] Useful commands:\n
echo         continue            resume until the next breakpoint\n
echo         print *s            dump the whole stats accumulator\n
echo         print s->min        inspect the running minimum\n
echo         print value         inspect the incoming sample\n
echo         bt                  backtrace\n
echo \n
