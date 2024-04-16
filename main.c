#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 


// CONSTANTS
#define MIN_SEGMENT_SIZE 2 // kb
#define MAX_SEGMENT_SIZE 20 // kb
#define NO_OF_SEGMENTS 3
#define PHYSICAL_MEMORY_SIZE 1000 // kb 



// STRUCT AND ENUM DEFINITIONS
typedef enum { // constants for segment types 
    CODE,
    STACK,
    HEAP
} SegmentType;

typedef struct {
    // address range must be within its process' address range. determines the ending address. base + limit = ending address
    int base; // starting physical address
    int limit; // size of segment
    // int growth_direction; // 1 indicates an upward direction, -1 indicates a downward direction, 0 for no growth
    // bool protection_bit; // 1 indicates read-write, 0 indicates read-only
} SegmentTableEntry;

typedef struct {
    int process_id; 
    int segment_number; // 0 for code, 1 for stack, 2 for heap
    int size ; // size of segment
    SegmentType type;
    // void* base_address; // starting virtual address of the segment
} Segment;

typedef struct {
    int id;
    int size; 
    Segment* segments[NO_OF_SEGMENTS]; // code, stack, heap
} Process;

typedef struct {
    int pid;
    int size; // determines the ending address. start address + size = ending address
    int base; // starting address
    int status; // 0 for ready, 1 for running, 2 for blocked
    SegmentTableEntry* segment_table [NO_OF_SEGMENTS];
} ProcessControlBlock; // contains metadata about process



// FUNCTION DECLARATIONS
Segment* create_segment(SegmentType type, int process_id);
Process* create_process(int process_id);
void create_PCB(Process *process);
void fork_processes();
void set_segment_number(SegmentType type, Segment *segment);
int allocate_physical_memory_to_segment(Segment* segment);
void update_segment_table_entry (Segment* segment, int base_address);
void print_segment_table(int pid);
void deallocate_segment_physical_memory(int base, int limit, SegmentTableEntry* temp_seg);
void deallocate_process_physical_memory (int pid);
void compact_physical_memory();
int generate_random_number(int min, int max);
void print_physical_memory();
const char* segment_type_to_string(SegmentType type);
int earliest_process();
void FIFO_swapping(int p1_size);
int translate_logical_to_physical(int segment_number, int offset, int pid);
int simulate_memory_access();
void simulate_segmentation();
void clear_physical_memory();


// VARIABLES AND ARRAYS
int number_of_processes;
ProcessControlBlock* process_table[1000]; // contains process control blocks. 
Segment* physical_memory [PHYSICAL_MEMORY_SIZE];
int no_of_segments_in_physical_memory = 0;
int no_of_deallocated_segments = 0;


int main () {
    simulate_segmentation();
}


void simulate_segmentation(){
    printf("Welcome to Kwaku and Yasmin's Final Project on segmentation!!\n");
    fork_processes();
    print_physical_memory();
    compact_physical_memory();
    print_physical_memory();
    simulate_memory_access();
    clear_physical_memory();
    printf("All processes have finished executing.\n");
    print_physical_memory();
    printf("Thank you!!\n");
}

// FUNCTIONS
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

    create_PCB(process); // create PCB for process

    return process;

}

// create a process control block to store metadata about the process
void create_PCB (Process *process) {
    
    ProcessControlBlock* PCB = malloc(sizeof(ProcessControlBlock)); // allocate memory for PCB

    if (PCB == NULL) {
        printf("Error creating PCB\n");
        exit(1);
    }

    PCB->size = process->size; // set PCB size
    PCB->pid = process->id; // set PCB id

    printf("Created PCB with id %d.\n", PCB->pid);

    process_table[PCB->pid] = PCB; // add PCB to process table

    printf("PCB of process %d has been stored in process table.\n", process_table[PCB->pid]->pid);
    // CREATE SEGMENT TABLEHERE 
    // return *PCB;
}

// Receives user input for number of processes
void fork_processes () {

    printf("How many processes do you want create? (We recommend 35+ processes to view the effects of swapping)\n");
    scanf("%d", &number_of_processes);

    for (int pid = 0; pid<number_of_processes; pid++) {

        Process* process = create_process(pid); //create individual processes

        // printf("created pid is %d\n", process->id);
        // printf("[%d, %d, %d]\n\n", process->segments[0]->size, process->segments[1]->size, process->segments[2]->size);
        
        for (int segment_number = 0; segment_number < NO_OF_SEGMENTS; segment_number++) { // allocate physical memory to each segment of the process
            Segment* segment = process->segments[segment_number];
            int base_address = allocate_physical_memory_to_segment(segment);

            // if allocation was successful, update the process's segment table with the segment's base address and limit.
            // allocate_physical_memory_to_segment(segment); // returns -1 if allocation is unsuccessful.
            // if (base_address == -1){
            //     compact_physical_memory();
            //     base_address = allocate_physical_memory_to_segment(segment);
            // }
            if (base_address == -1){
                FIFO_swapping(process->size);
                base_address = allocate_physical_memory_to_segment(segment);
            }
            if (base_address != -1) {
                no_of_segments_in_physical_memory = no_of_segments_in_physical_memory+1;
                update_segment_table_entry(segment, base_address);
            } 

        }

        print_segment_table(pid);
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
            free_space_count++; // increase free space if the current space is empty and doesn't go beyond the boundaries of physical memory
            i++;
        }

        if (free_space_count >= segment->size) { // if you've found enough space for the segment, fill up those spaces with the segment.
            base_address = i - free_space_count; // base address = current_position minus the free_space_count which is essentially the size of the segment (in this case)
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


// deallocate memory for each process
void deallocate_process_physical_memory(int pid){
    printf("\n\n deallocating process %d segments ----------------\n\n", pid);
    ProcessControlBlock *pcb = process_table[pid];

    for(int i=0; i<3; i++){
        SegmentTableEntry *seg = pcb->segment_table[i];
        if(seg->base != -1){
            deallocate_segment_physical_memory(seg->base, seg->limit, seg);
        }   
    }

    process_table[pid] = NULL;

}

// deallocate memory for each segment 
void deallocate_segment_physical_memory(int base, int limit, SegmentTableEntry* temp_seg){
    // clear the segment space in physical memory 
    for(int i=base; i<base+limit; i++){
        physical_memory[i] =  NULL;
    } 
    no_of_deallocated_segments = no_of_deallocated_segments + 1;
    // set segment base to -1
    temp_seg->base = -1;  
}

// compacts physical memory
void compact_physical_memory () {
    printf("Your memory has external fragmentation. We are compacting\n");

    int length_of_temp = (no_of_segments_in_physical_memory - no_of_deallocated_segments); 

    Segment* temp [length_of_temp]; // temporary location for segments
    int index_of_first_non_empty_space; 
    int current_temp_index; // pointer to the last filled index in temp

    // move first segment to temporary location
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {

        if (physical_memory[i] != NULL) {
            temp[0] = physical_memory[i];
            index_of_first_non_empty_space = i;
            current_temp_index = index_of_first_non_empty_space;
            break;
        }

    }

    //move the rest of the segments to temporary location
    for (int i = index_of_first_non_empty_space; i < PHYSICAL_MEMORY_SIZE; i++) {

        Segment* current_segment = physical_memory[i];
        Segment* current_temp_segment = temp[current_temp_index];

        if (current_segment != NULL && (current_segment->segment_number != current_temp_segment->segment_number)) {
            // we need to check both the segment number and the pid
            current_temp_index += 1;
            temp[current_temp_index] = physical_memory[i];
        }


    }

    // clear physical memory
    clear_physical_memory();

    // print temporary compacted number representation
    // for (int i = 0; i < length_of_temp; i++) { 
    //     if (temp[i] != NULL) {
    //         printf("At %d, pid %d, seg_no %d\n", i, temp[i]->process_id, temp[i]->segment_number);
    //     }
        
    // }

    // rearrange segments in physical memory
    for (int i = 0; i < length_of_temp; i++) { // allocate physical memory to each segment of the process

        if (temp[i] != NULL) {

            int base_address = allocate_physical_memory_to_segment(temp[i]);
            // printf("At %d ,Base Address for pid %d seg_no %d: %d\n", i, segment->process_id, segment->segment_number, base_address);

            if (base_address != -1) {
                update_segment_table_entry(temp[i], base_address);
            }
        }
    }


}

void clear_physical_memory() {
    // clear physical memory
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        if (physical_memory[i]!= NULL) {
            physical_memory[i] = NULL;
        }
    }
}

// remove from physical memory until memory available is >= process size. then i try to allocate process again
void FIFO_swapping(int p1_size){
    int first = earliest_process();
    int first_size = process_table[first]->size;
    int size_found = 0;

    while (size_found < p1_size){
        deallocate_process_physical_memory(first);
        size_found += first_size;
        first = earliest_process();
    }
}

int earliest_process(){
    int first_process;

    for(int i = 0; i < 1000; i++){
        if(process_table[i] != NULL){
            first_process = i;
            break;
        }
    }

    return first_process;
}

int simulate_memory_access(){
    int requesting_process = rand() % 40;
    int seg_num = generate_random_number(0, NO_OF_SEGMENTS);
    int offset = generate_random_number(MIN_SEGMENT_SIZE, MAX_SEGMENT_SIZE);
    int count = 0;

    while(process_table[requesting_process] == NULL && count < 40){
        requesting_process = generate_random_number(0, 40);
        count+=1;
    }

    if(process_table[requesting_process] == NULL){
        printf("No process requesting access.\n");
        return 1;
    }

    translate_logical_to_physical(seg_num, offset, requesting_process);

    return 0;
}


int translate_logical_to_physical(int segment_number, int offset, int pid){
    if(process_table[pid] == NULL){
        return 1;
    }

    ProcessControlBlock *pcb = process_table[pid];
    SegmentTableEntry *seg = pcb->segment_table[segment_number];

    if(offset > seg->limit){
        fprintf(stderr, "An error occurred: Memory address out of bounds\n");
        return 1;
    } else{
        int address_in_physical = offset + seg->base;
        printf("\nProcess %d memory accessed at %d\n", pid, address_in_physical);
    }
    
    return 0;
}


// HELPER FUNCTIONS
int generate_random_number (int min, int max) {
     int random_number = rand() % max + min; 
     return random_number;
}


void print_segment_table(int pid) {
    printf("Segment Table for Process %d:\n", pid);
    printf("+---------------------------------------------+\n");
    printf("|     Segment Number     |   Base   |  Limit |\n");
    printf("+---------------------------------------------+\n");

    ProcessControlBlock* PCB = process_table[pid];
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

    printf("+---------------------------------------------+\n\n");
}

void print_physical_memory() {
    printf("Physical Memory:\n");
    printf("+---------------------------------------------+\n");
    printf("|     Address     |   PID   |      Segment    |\n");
    printf("+---------------------------------------------+\n");

    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        
        if (physical_memory[i] != NULL) {
            printf("|      %3d        |   %3d   |       %d       |\n", i, physical_memory[i]->process_id, physical_memory[i]->segment_number);
        } 
        
        else {
            printf("|      %3d        |         |                |\n", i);
        }

    }

    printf("+---------------------------------------------+\n");
}


const char* segment_type_to_string(SegmentType type) {
    switch (type) {
        case CODE:
            return "Code";
        case STACK:
            return "Stack";
        case HEAP:
            return "Heap";
        default:
            return "Unknown";
    }
}
