#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#define NO_OF_SEGMENTS 3
#define MAX_PROCESS_SIZE 12
#define MIN_PROCESS_SIZE 3
#define MIN_SEGMENT_SIZE 1

// NB: assume all ints pertaining to sizes are in MB.

typedef enum {
    CODE,
    STACK,
    HEAP
} SegmentType;

// STRUCT DEFINTIONS
typedef struct {
    // address range must be within its process' address range.
    int limit; // determines the ending address. base + limit = ending address
    int base; // starting physical address.
    int growth_direction; // 1 indicates an upward direction, -1 indicates a downward direction, 0 for no growth
    bool protection_bit; // 1 indicates read-write, 0 indicates read-only
    SegmentType type;
} Segment;


typedef struct {
    int id;
    int size;
    Segment* segments[NO_OF_SEGMENTS];
} Process;


typedef struct {
    int id;
    int size; // determines the ending address. start address + size = ending address
    int base; // starting address
    int status; // 0 for ready, 1 for running, 2 for blocked
} ProcessControlBlock; // contains metadata about process

ProcessControlBlock* process_table[20]; // contains process control blocks. 



// FUNCTION DECLARATIONS
int generate_random_number(int min, int max);
Segment* create_segment(int max_segment_size, SegmentType type);
Process* create_process (int id);
ProcessControlBlock create_PCB (Process process);
void set_segment_type(SegmentType type, Segment *segment);



int main () {

    srand(time(NULL)); // for random number generation. must run just once.

    Process* process = create_process(1);

    printf("Process size: %d\n", process->size);

    for (int i = 0; i<3; i++) {
        printf("SEGMENT %d\n", i);
        printf("Segment type: %d\n", process->segments[i]->type);
        printf("Segment size: %d\n", process->segments[i]->limit);
    }

    free(process);

    return 0;

}

// MAIN FUNCTIONS
Process* create_process (int id) {

    int process_size = generate_random_number (MIN_PROCESS_SIZE, MAX_PROCESS_SIZE);
    Process* process = malloc(sizeof(Process));

    if (process == NULL) {
        printf("Error creating process.\n");
        exit(1);
    }

    process->size = process_size;
    process->id = id;

    int max_segment_size = process_size/NO_OF_SEGMENTS;

    process->segments[0] = create_segment(max_segment_size, CODE);
    process->segments[1] = create_segment(max_segment_size, STACK);
    process->segments[2] = create_segment(max_segment_size, HEAP);

    create_PCB(*process);

    return process;

}

ProcessControlBlock create_PCB (Process process) {
    
    ProcessControlBlock* PCB = malloc(sizeof(ProcessControlBlock));

    if (PCB == NULL) {
        printf("Error creating PCB\n");
        exit(1);
    }

    PCB->size = process.size;
    PCB->id = process.id;

    process_table[PCB->id] = PCB;

    return *PCB;

}

Segment* create_segment(int max_segment_size, SegmentType type) {

    int segment_limit = generate_random_number(MIN_SEGMENT_SIZE, max_segment_size);

    Segment* segment = malloc(sizeof(Segment));
    if (segment == NULL) {
        printf("Error creating segment.\n");
        exit(1);
    }

    segment->limit = segment_limit;

    set_segment_type(type, segment);

    return segment;

}

void set_segment_type(SegmentType type, Segment *segment) {

    if (type == CODE) {
        segment->type = CODE;
        segment->growth_direction = 0;
        segment->protection_bit = 0;
    }

    if (type == STACK) {
        segment->type = STACK;
        segment->growth_direction = 1;
        segment->protection_bit = 1;
    }

    if (type == HEAP) {
        segment->type = HEAP;
        segment->growth_direction = -1;
        segment->protection_bit = 1;
    }

}


// HELPER FUNCTIONS
int generate_random_number (int min, int max) {
     int random_number = rand() % max + min; 
     return random_number;
}