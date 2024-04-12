// Header guard to prevent multiple inclusions
#ifndef CREATEPROCESS_H
#define CREATEPROCESS_H
#define MAX_SEGMENT_SIZE 16364 // bits
#define NO_OF_SEGMENTS 3

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
int generate_random_number(int min, int max);
Segment* create_segment(SegmentType type);
Process* create_process(int id);
void create_PCB(Process process);
void fork_processes();
void set_segment_number(SegmentType type, Segment *segment);

#endif 