# Virtual Memory Manager & TLB Simulator

A C project that simulates virtual memory address translation using page tables, a Translation Lookaside Buffer (TLB), page faults, physical memory, and FIFO page replacement.

## What it does

- Reads logical addresses from `addresses.txt`
- Extracts the page number and offset
- Checks the TLB first
- Falls back to the page table on a TLB miss
- Handles page faults by loading pages from `BACKING_STORE.bin`
- Uses FIFO replacement when physical memory is full
- Prints TLB hit rate, page fault rate, and page replacements

## Files

```text
main.c          main program and address translation loop
tlb.c           TLB lookup, insert, and update logic
tlb.h           TLB function declarations
memory.c        page table, physical memory, and page fault handling
memory.h        memory function declarations
config.h        constants such as page size, TLB size, and memory size
Makefile        simple build commands
README.md       project explanation
```

## Build

```bash
make
```

or manually:

```bash
gcc -Wall -Wextra -std=c11 main.c tlb.c memory.c -o vmm
```

## Run

Put `addresses.txt` and `BACKING_STORE.bin` in the same folder, then run:

```bash
./vmm
```

You can also pass file paths manually:

```bash
./vmm addresses.txt BACKING_STORE.bin
```
