#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#define NO_OF_SEGMENTS 3
#define MAX_PROCESS_SIZE 12
#define MIN_PROCESS_SIZE 3
#define MIN_SEGMENT_SIZE 1

// NB: assume all ints pertaining to sizes are in MB.

// Upon creation, a process is assigned a logical address space. This address space is determined by the process' memory requirements. In this program, a process's logical address space will be contiguous. Virtual memory will be an array of segments. The starting logical address can be randomly generated. To keep the program simple, processes will have only 3 segments: stack, code, heap. Each process has 3 blocks in the array (since each process has 3 segments. These segments however will vary in size). The logical address is made up of the segment number and offset.

// STRUCT DEFINTIONS
typedef struct {
    int size;
    int base; // starting physical address 
    int growth_direction; // 1 indicates an upward direction, -1 indicates a downward direction, 0 for no growth
    bool protection_bit; // 1 indicates read-write, 0 indicates read-only
} Segment;


typedef struct {
    int id;
    int size;
    Segment* segments[NO_OF_SEGMENTS];
} Process;


// FUNCTION DECLARATIONS
int generate_random_number(int min, int max);
Segment* create_segment(int max_segment_size, int growth_direction, int protection_bit);
Process* create_process (int id);



int main () {

    srand(time(NULL)); // for random number generation. must run just once.

    Process* process = create_process(1);
    printf("Process size: %d\n", process->size);
    for (int i = 0; i<3; i++) {
        printf("SEGMENT %d\n", i);
        printf("Segment growth direction: %d\n", process->segments[i]->growth_direction);
        printf("Segment protection direction: %d\n", process->segments[i]->protection_bit);
        printf("Segment size: %d\n", process->segments[i]->size);
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
        return NULL;
    }

    process->size = process_size;
    process->id = id;

    int max_segment_size = process_size/NO_OF_SEGMENTS;

    Segment* code = create_segment(max_segment_size, 0, 0);
    Segment* stack = create_segment(max_segment_size, 1, 1);
    Segment* heap = create_segment(max_segment_size, -1, 1);

    process->segments[0] = code;
    process->segments[1] = stack;
    process->segments[2] = heap;

    return process;

}

Segment* create_segment(int max_segment_size, int growth_direction, int protection_bit) {
    int segment_size = generate_random_number(MIN_SEGMENT_SIZE, max_segment_size);

    Segment* segment = malloc(sizeof(Segment));
    if (segment == NULL) {
        printf("Error creating segment.\n");
        return NULL;
    }

    segment->size = segment_size;
    segment->growth_direction = growth_direction;
    segment->protection_bit = protection_bit;

    return segment;
}


// HELPER FUNCTIONS
int generate_random_number (int min, int max) {
     int random_number = rand() % max + min; 
     return random_number;
}