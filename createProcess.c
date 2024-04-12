#include "createProcess.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 

int number_of_processes;
ProcessControlBlock* process_table[100]; 

//creating a single process with 3 segments
Process* create_process (int id) { // takes process id as input

    // create segments
    Segment* code = create_segment(CODE);
    Segment* stack = create_segment(STACK);
    Segment* heap = create_segment(HEAP);

    // get process size
    int process_size = code->size + stack->size + heap->size;

    // int process_size = generate_random_number (MIN_PROCESS_SIZE, MAX_PROCESS_SIZE); // generate random process size
    Process* process = malloc(sizeof(Process)); // allocate memory for process

    if (process == NULL) { // check if memory allocation was successful
        printf("Error creating process.\n");
        exit(1);
    }

    // set process attributes
    process->size = process_size; // set process size
    process->id = id; // set process id
    process->segments[0] = code; // set code segment
    process->segments[1] = stack; // set stack segment
    process->segments[2] = heap; // set heap segment

    create_PCB(*process); // create PCB for process

    return process;
}

// create a process control block to store metadata about the process
void create_PCB (Process process) {
    
    ProcessControlBlock* PCB = malloc(sizeof(ProcessControlBlock)); // allocate memory for PCB

    if (PCB == NULL) {
        printf("Error creating PCB\n");
        exit(1);
    }

    PCB->size = process.size; // set PCB size
    PCB->pid = process.id; // set PCB id

    process_table[PCB->pid] = PCB; // add PCB to process table

    // CREATE SEGMENT TABLEHERE 
    // return *PCB;
}

// Receives user input for number of processes
void fork_processes () {
    printf("How many processes do you want create?\n");
    scanf("%d", &number_of_processes);

    for (int pid = 0; pid<number_of_processes; pid++) {
        Process* process = create_process(pid); //create individual processes
        printf("created pid is %d\n", process->id);
        //why are you assigning to a variable when you can just call?    
    }

}

// create segment for process
Segment* create_segment(SegmentType type) {

    Segment* segment = malloc(sizeof(Segment)); // allocate memory for segment
    if (segment == NULL) {
        printf("Error creating segment.\n");
        exit(1);
    }

    int segment_size = generate_random_number(0, MAX_SEGMENT_SIZE); // generate random segment size

    segment->size = segment_size; // set segment size

    set_segment_number(type, segment); // set segment type

    return segment;
}

// set segment type
void set_segment_number(SegmentType type, Segment *segment) {

    if (type == CODE) {
        segment->segment_number = 0;
    }

    if (type == STACK) {
        segment->segment_number = 1;
    }

    if (type == HEAP) {
        segment->segment_number = 2;
    }

}

// HELPER FUNCTIONS
int generate_random_number (int min, int max) {
     int random_number = rand() % max + min; 
     return random_number;
}