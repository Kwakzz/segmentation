#include "createProcess.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 


//creating a single process with 3 segments
Process* create_process (int process_id) { // takes process id as input

    // create segments
    Segment* code = create_segment(CODE, process_id);
    Segment* stack = create_segment(STACK, process_id);
    Segment* heap = create_segment(HEAP, process_id);

    // int process_size = generate_random_number (MIN_PROCESS_SIZE, MAX_PROCESS_SIZE); // generate random process size
    Process* process = malloc(sizeof(Process)); // allocate memory for process

    if (process == NULL) { // check if memory allocation was successful
        printf("Error creating process.\n");
        exit(1);
    }

    // set process attributes
    process->size = code->size + stack->size + heap->size; // set process size
    process->id = process_id; // set process id
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
        printf("[%d, %d, %d]\n\n", process->segments[0]->size, process->segments[1]->size, process->segments[2]->size);
        
        for (int segment_number = 0; segment_number < NO_OF_SEGMENTS; segment_number++) {
            Segment* segment = process->segments[segment_number];
            int base_address = allocate_physical_memory_to_segment(segment);

            if (base_address != -1) {
                update_segment_table_entry(segment, base_address);
            }
        }

        print_segment_table(process);
        printf("\n");



    }

}

// create segment for process
Segment* create_segment(SegmentType type, int process_id) {

    Segment* segment = malloc(sizeof(Segment)); // allocate memory for segment
    if (segment == NULL) {
        printf("Error creating segment.\n");
        exit(1);
    }


    segment->size = generate_random_number(MIN_SEGMENT_SIZE, MAX_SEGMENT_SIZE); // set segment size
    segment->type = type;
    segment->process_id = process_id;

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

int allocate_physical_memory_to_segment(Segment *segment) {
    
    int base_address = -1;

    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        int free_space_count = 0;

        while (i < PHYSICAL_MEMORY_SIZE && physical_memory[i] == NULL) {
            free_space_count++;
            i++;
        }

        if (free_space_count >= segment->size) {
            base_address = i - free_space_count;
            for (int j = base_address; j < base_address + segment->size; j++) {
                physical_memory[j] = segment;
            }
            break;
        }
    }

    return base_address;
}



// updates segment table when segment is assigned physical memory 
void update_segment_table_entry(Segment* segment, int base_address) {

    if (segment == NULL || segment->process_id < 0 || segment->process_id >= number_of_processes) {
        printf("Invalid segment\n");
        return;
    }

    ProcessControlBlock* PCB = process_table[segment->process_id];
    if (PCB == NULL) {
        printf("Process Control Block not found\n");
        return;
    }

    SegmentTableEntry** segment_table = PCB->segment_table;
    if (segment_table == NULL) {
        printf("Segment table not initialized\n");
        return;
    }

    if (segment->segment_number < 0 || segment->segment_number >= NO_OF_SEGMENTS) {
        printf("Invalid segment number\n");
        return;
    }

    // Allocate memory for the new segment table entry
    SegmentTableEntry* segment_table_entry = malloc(sizeof(SegmentTableEntry));
    if (segment_table_entry == NULL) {
        printf("Error creating segment table entry.\n");
        return;
    }

    // Populate the new segment table entry
    segment_table_entry->base = base_address;
    segment_table_entry->limit = segment->size;

    // Update the process's segment table with the new entry
    segment_table[segment->segment_number] = segment_table_entry;
}


// HELPER FUNCTIONS
int generate_random_number (int min, int max) {
     int random_number = rand() % max + min; 
     return random_number;
}

void print_segment_table(Process* process) {
    printf("Segment Table for Process %d:\n", process->id);
    printf("+---------------------------------------------+\n");
    printf("|     Segment Number     |   Base   |  Limit |\n");
    printf("+---------------------------------------------+\n");

    ProcessControlBlock* PCB = process_table[process->id];
    if (PCB != NULL) {
        SegmentTableEntry** segment_table = PCB->segment_table;

        for (int i = 0; i < NO_OF_SEGMENTS; i++) {
            printf("|      %3d        |", i);
            if (segment_table[i] != NULL) {
                printf("   %3d   |       %3d       |\n", segment_table[i]->base, segment_table[i]->limit);
            } else {
                printf("         |                 |\n");
            }
        }
    } else {
        printf("Process Control Block not found\n");
    }

    printf("+---------------------------------------------+\n");
}
