# Memory Hierarchy & Cache Design Simulator

A configurable, cycle-agnostic simulator that models a complete CPU memory hierarchy —
data TLB, page table, L1 data cache, and unified L2 cache — and reports per-access
behavior and aggregate hit/miss statistics for an arbitrary memory reference trace.

Written in modern C++ (C++17) as the capstone project for **COSC 530: Advanced Computer
Architecture** at the University of Tennessee, Knoxville.

---

## Overview

Given a hierarchy configuration and a stream of read/write memory references, the simulator
walks each address through every enabled stage of address translation and caching, printing
a detailed trace of tags, indices, and hit/miss outcomes at each level, followed by a summary
of hit rates and reference counts.

It models the interactions real hardware must handle — virtual-to-physical translation,
TLB/page-table coherence with the caches, write-through propagation between cache levels,
and LRU eviction across the TLB, page table, and set-associative caches.

## Features

- **Fully configurable hierarchy** via a plain-text config file — set associativity, set
  count, line size, and page size independently for each component.
- **Toggleable stages** — virtual addressing, TLB, and L2 can each be enabled or disabled,
  letting you study any subset of the hierarchy (e.g. physically addressed L1-only, or the
  full VA → TLB → page table → L1 → L2 path).
- **Data TLB** — set-associative translation cache with LRU replacement.
- **Page table** — virtual-to-physical translation with a finite physical frame pool, LRU
  page eviction, page-fault handling, and dirty-page write-back.
- **Two-level cache** — L1 data cache backed by a unified L2, each independently configurable
  as write-through / no-write-allocate.
- **Cross-level coherence** — TLB entries are invalidated on frame eviction, and writes
  propagate correctly between L1 and L2 under the configured write policy.
- **Detailed per-access trace + summary statistics** — hit/miss at every level, hit rates,
  read/write ratio, and main-memory / page-table / disk reference counts.

## Architecture

The design separates each hardware component into its own translation unit, coordinated by a
central `Simulator` that threads each reference through the enabled stages.

| Component | Responsibility |
|---|---|
| `Simulator` | Drives the hierarchy; routes each reference through TLB → page table → L1 → L2 and records results |
| `TLB` / `TLBEntry` | Set-associative translation lookaside buffer with LRU eviction |
| `PageTable` / `PageTableEntry` | VA→PA translation, physical frame allocation, page faults, LRU page eviction |
| `Cache` / `CacheLine` | Generic N-way set-associative cache used for both L1 and L2 |
| `Statistics` | Accumulates and reports hit/miss counts, hit rates, and reference totals |
| `ConfigRetrieval` | Parses the hierarchy configuration file |
| `TraceReciever` | Parses the read/write reference trace from standard input |

A single generic `Cache` class is reused for both L1 and L2, parameterized by set count, set
size (associativity), line size, and write policy — the same abstraction real caches share.

## Build

Requires a C++17 compiler (`g++` or `clang++`) and `make`.

```bash
make build      # compile to ./memhier
```

## Usage

The simulator reads its hierarchy configuration from `trace.config` in the working directory
and the memory reference trace from **standard input**:

```bash
./memhier < trace.dat
```

Or build, run against the sample trace, and clean up in one step:

```bash
make            # build + run < test.dat + clean
```

### Trace format

One reference per line, `<R|W>:<hex address>`:

```
R:c84
W:257
R:81c
```

### Configuration format (`trace.config`)

```
Data TLB configuration
Number of sets: 16
Set size: 1

Page Table configuration
Number of virtual pages: 64
Number of physical pages: 4
Page size: 256

Data Cache configuration
Number of sets: 4
Set size: 1
Line size: 16
Write through/no write allocate: y

L2 Cache configuration
Number of sets: 16
Set size: 4
Line size: 16
Write through/no write allocate: y

Virtual addresses: n
TLB: n
L2 cache: n
```

The final three flags toggle whether addresses are treated as virtual, whether the TLB is
consulted, and whether the L2 cache is present. Columns in the output trace appear or
disappear according to which stages are enabled.

## Sample output

```
Virtual  Virt.  Page TLB    TLB TLB  PT   Phys        DC  DC          L2  L2
Address  Page # Off  Tag    Ind Res. Res. Pg # DC Tag Ind Res. L2 Tag Ind Res.
-------- ------ ---- ------ --- ---- ---- ---- ------ --- ---- ------ --- ----
00000257      9   1 miss
00000119      4   1 miss
0000000d      0   0 hit
...

Simulation statistics

dc hits          : 1
dc misses        : 9
dc hit rate      : 0.100000

Total reads      : 6
Total writes     : 4
Read/Write ratio : 0.600000

main memory refs : 9
page table refs  : 0
disk refs        : 0
```

## Concepts demonstrated

- Computer architecture: cache organization, set associativity, address decomposition
  (tag / index / offset), virtual memory, and TLBs
- Replacement and coherence policies: LRU eviction, write-through vs. no-write-allocate,
  dirty-bit write-back, and multi-level cache interaction
- Systems programming in modern C++: modular class design, pointer-linked hierarchy levels,
  and bit-level address manipulation
