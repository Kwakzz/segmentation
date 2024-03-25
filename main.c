#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#define NO_OF_SEGMENTS_PER_PROCESS 3
#define MAX_PROCESS_SIZE 12

// NB: assume all ints pertaining to sizes are in MB.

// STRUCT DEFINTIONS
typedef struct {
    int size;
    int base; // starting physical address 
    bool growth_direction; // 1 indicates an upward direction, -1 indicates a downward direction, 0 for no growth
    bool protection_bit; // 1 indicates read-write, 0 indicates read-only
} Segment;


typedef struct {
    int id;
    int size;
    Segment* segments[NO_OF_SEGMENTS_PER_PROCESS];
} Process;


int main () {

    srand(time(NULL)); // for random number generation. must run just once.
    return 0;

}

// FUNCTION DECLARATIONS
int generate_random_number(int min, int max);
Segment* create_segment(int max_segment_size, int growth_direction, int protection_bit);


// MAIN FUNCTIONS
Process* create_process (int id) {

    int process_size = generate_random_number (0, MAX_PROCESS_SIZE);
    Process* process = malloc(sizeof(Process));

    if (process == NULL) {
        printf("Error creating process.\n");
        return NULL;
    }

    process->size = process_size;
    process->id = id;

    int max_segment_size = process_size/NO_OF_SEGMENTS_PER_PROCESS;

    Segment* code = create_segment(max_segment_size, 0, 0);
    Segment* stack = create_segment(max_segment_size, 1, 1);
    Segment* heap = create_segment(max_segment_size, -1, 1);

    process->segments[0] = code;
    process->segments[1] = stack;
    process->segments[2] = heap;

    return process;

}

Segment* create_segment(int max_segment_size, int growth_direction, int protection_bit) {
    int segment_size = generate_random_number(0, max_segment_size);

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