#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#include "createProcess.h"
// #define MAX_PROCESS_SIZE 12 //in MB
// #define MIN_PROCESS_SIZE 3 //in MB

void print_physical_memory();
const char* segment_type_to_string(SegmentType type);

int main () {
    
    fork_processes();

}

void print_physical_memory() {
    printf("Physical Memory:\n");
    printf("+---------------------------------------------+\n");
    printf("|     Address     |   PID   |  Segment |\n");
    printf("+---------------------------------------------+\n");

    for (int i = 0; i < 100; i++) {
        
        if (physical_memory[i] != NULL) {
            printf("|      %3d        |   %3d   |       %d       |\n", i, physical_memory[i]->process_id, physical_memory[i]->segment_number);
        } 
        
        else {
            printf("|      %3d        |         |                 |\n", i);
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

