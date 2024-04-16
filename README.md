# Segmentation Simulation

Welcome to Kwaku and Yasmin's Final Project on segmentation!

## Overview

This project simulates memory segmentation in a computer system. Memory segmentation is the division of computer memory into segments or sections, each with its own properties and purposes. 

In this simulation, we create processes consisting of three segments: code, stack, and heap. We manage physical memory allocation for these segments and implement functionalities like compacting memory, swapping using FIFO (First In, First Out) algorithm, and simulating memory access.

Here's a [link](https://youtu.be/27bfPtfzAaw) to a demo of our project.

## Getting Started

To run the simulation, simply compile the C program provided (`segmentation_simulation.c`) and execute the generated executable.

```bash
gcc main.c -o main
./main
```

## Functionalities

### Process Creation (`create_process`)

This function creates a single process with three segments: code, stack, and heap.

### Physical Memory Allocation (`allocate_physical_memory_to_segment`)

This function allocates physical memory to a segment based on its size and availability in the physical memory array.

### Process Control Block (PCB) Creation (`create_PCB`)

This function creates a Process Control Block (PCB) to store metadata about a process, such as its size, ID, and segment table.

### Compacting Memory (`compact_physical_memory`)

This function manages external fragmentation in physical memory by compacting memory, moving segments to fill up empty spaces.

### FIFO Swapping (`FIFO_swapping`)

This function implements FIFO swapping to deal with memory shortage. It swaps out processes from physical memory based on the first-in-first-out principle.

### Memory Access Simulation (`simulate_memory_access`)

This function simulates memory access by randomly selecting a process and accessing its segments.

### Segment Table Display (`print_segment_table`)

This function displays the segment table for each process, showing the base address and limit for each segment.

### Physical Memory Display (`print_physical_memory`)

This function displays the layout of physical memory, showing which segments are allocated to which addresses.

## Files

- `main.c`: The main C source code file containing the simulation implementation.
- `README.md`: This readme file providing an overview of the project.

## Usage

Follow the prompts to specify the number of processes to create. We recommend creating at least 35 processes to observe the effects of swapping. The simulation will then proceed to allocate memory, perform operations, and display results.

## Program Output

The program output includes information about the segment table for each process, the physical memory layout before and after operations, and any memory access simulations performed.

## Authors

- Kwaku Osafo
- Yasmin Kamal-Deen
