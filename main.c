#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#define NO_OF_SEGMENTS 3
// #define MAX_PROCESS_SIZE 12 //in MB
// #define MIN_PROCESS_SIZE 3 //in MB
#define MAX_SEGMENT_SIZE 16364 // bits, also 2KB


typedef enum { // constants for segment types 
    CODE,
    STACK,
    HEAP
} SegmentType;

// // STRUCT DEFINTIONS
// // these are actually entries in the segment table of the process.
// typedef struct {
//     // address range must be within its process' address range. determines the ending address. base + limit = ending address
//     int limit; // size of segment
//     int base; // starting physical address.
//     int growth_direction; // 1 indicates an upward direction, -1 indicates a downward direction, 0 for no growth
//     bool protection_bit; // 1 indicates read-write, 0 indicates read-only
//     SegmentType type;
// } Segment;

typedef struct {
    int segment_number; // 0 for code, 1 for stack, 2 for heap
    size_t size ; // size of segment
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
    // int base; // starting address
    int status; // 0 for ready, 1 for running, 2 for blocked
} ProcessControlBlock; // contains metadata about process

ProcessControlBlock* process_table[20]; // contains process control blocks. 



// FUNCTION DECLARATIONS
int generate_random_number(int min, int max);
Segment* create_segment(SegmentType type);
Process* create_process (int id);
void create_PCB (Process process);
void set_segment_number(SegmentType type, Segment *segment);

int main () {

    // srand(time(NULL)); // for random number generation. must run just once.

    // Process* process = create_process(1);

    // printf("Process size: %d\n", process->size);

    // for (int i = 0; i<3; i++) {
    //     printf("SEGMENT %d\n", i);
    //     printf("Segment type: %d\n", process->segments[i]->type);
    //     printf("Segment size: %d\n", process->segments[i]->limit);
    // }

    // free(process);

    // return 0;

}


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
    // return *PCB;
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
        // segment->growth_direction = 0;
        // segment->protection_bit = 0;
    }

    if (type == STACK) {
        segment->segment_number = 1;
        // segment->growth_direction = 1;
        // segment->protection_bit = 1;
    }

    if (type == HEAP) {
        segment->segment_number = 2;
        // segment->growth_direction = -1;
        // segment->protection_bit = 1;
    }

}


// HELPER FUNCTIONS
int generate_random_number (int min, int max) {
     int random_number = rand() % max + min; 
     return random_number;
}