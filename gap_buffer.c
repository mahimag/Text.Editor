#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gap_buffer.h"

//global variable
//int gbufcount;

// Create and return an empty gap buffer
// The gap size is DEFAULT_GAP_SIZE
GapBuffer* gap_create() {
    //creating + allocating mem for pointer
    GapBuffer* gbuf;
    gbuf = (GapBuffer*) malloc(sizeof(GapBuffer)); //malloc
    //set values
    gbuf->size = DEFAULT_GAP_SIZE; // 4
    gbuf->insert_position = 0;
    gbuf->second_position = DEFAULT_GAP_SIZE;
    gbuf->data = (char*) malloc(sizeof(char) * gbuf->size);
    return gbuf;
}

// Move the cursor in the gap buffer, copying data as necessary
// to each side of the gap
void gap_set_insert_position(GapBuffer* gbuf, int new_position) {
    // Your code here

    // store amount of data that needs to be copied
    int copy = gbuf->insert_position - new_position; // 7 - 4
    
    int ip = gbuf->insert_position; //store original ip
    int sp = gbuf->second_position;
    //printf("what is copy value: %d\n", copy);

    printf("what is insert pos: %d\n", ip); // 7
    printf("what is second pos: %d\n", sp); // 4

    //copy the data
    if(ip > new_position){ // shift to left
        //for(int i = new_position; i < gbuf->insert_position; i++){
        while(gbuf->insert_position != new_position){
            char c1 = gbuf->data[gbuf->insert_position - 1]; //char before ip 
            //gbuf->data[gbuf->insert_position] = gbuf->data[gbuf->insert_position - 1];
            gbuf->insert_position--;
            gbuf->data[gbuf->insert_position] = '\0';
            gbuf->data[gbuf->second_position - 1] = c1;
            gbuf->second_position--;
            //gbuf->data[gbuf->insert_position] = '\0';
        }
    }
    else if(ip < new_position){ // shift to right
        while(gbuf->insert_position != new_position){
            char c1 = gbuf->data[gbuf->second_position]; //char before ip 
            //gbuf->data[gbuf->insert_position] = gbuf->data[gbuf->insert_position - 1];
            gbuf->insert_position++;
            gbuf->data[gbuf->second_position] = '\0';
            gbuf->data[gbuf->insert_position - 1] = c1;
            gbuf->second_position++;
            //gbuf->data[gbuf->insert_position] = '\0';
        }
    }
    else{ //if ip is new position do nothing
        //
    }
    printf("again what is insert pos: %d\n", gbuf->insert_position);
    printf("again what is second pos: %d\n", gbuf->second_position);
}

// Insert a character at the insert position. This grows
// the gap buffer if necessary.
// For example, if the gbuf is
//    To be[    ]or
// then inserting 'z' makes the gbuf
//    To bez[   ]or
void gap_insert_char(GapBuffer* gbuf, char c) {
    // Your code here
    
    //printf("check ip: %d\n", gbuf->insert_position);

    int gapspace = gbuf->second_position - gbuf->insert_position;

    if(gapspace == 0){ //if gap buf is full
        printf("changing size of buf\n");
        printf("current size of buf = %d\n", gbuf->size);

        char* newdata = (char*) malloc(sizeof(char) * (gbuf->size + DEFAULT_GAP_SIZE));

        int a = gbuf->insert_position;
        int b = gbuf->second_position;
        for(int i = 0; i < gbuf->insert_position; i++){
            newdata[i] = gbuf->data[i];
        }
        for(int i = gbuf->second_position; i < gbuf->size; i++){
            newdata[i + DEFAULT_GAP_SIZE] = gbuf->data[i]; 
        }
        //change size of buf
        //gbuf = (GapBuffer*) realloc(gbuf, sizeof(GapBuffer*)); //malloc
        
        gbuf->size += DEFAULT_GAP_SIZE;
        gbuf->second_position += DEFAULT_GAP_SIZE;
        free(gbuf->data);
        gbuf->data = newdata;
        printf("finished changing size of buf. final buf size = %d\n", gbuf->size);
        printf("finished changing ip. final ip = %d\n", gbuf->insert_position);
        printf("finished changing sp. final sp = %d\n", gbuf->second_position);
        /* 0   1   2   3   4   5   6   7
           T   h   e   i   []
           T   h   [           ]e  i   r
        */
    }
    gbuf->data[gbuf->insert_position] = c;
    gbuf->insert_position++;
}

// Insert count chars of a string at the cursor position. This grows
// the gap buffer if necessary.
void gap_insert_string(GapBuffer* gbuf, int count, const char* s) {
    for(int i = 0; i < count; i++){
        gap_insert_char(gbuf, s[i]);
    }
}

// Remove a character behind the insert position
// For example, if the gbuf is
//    To be[    ]or
// then gbuf would change and become
//    To b[     ]or
void gap_remove_char(GapBuffer* gbuf) {
    // Your code here
    gbuf->data[gbuf->insert_position - 1] = '\0';
    --gbuf->insert_position;
}

// Get the gap buffer as a null terminated string.
// The memory for the string is allocated and must be freed
// by the receiving caller.
// For example, if the gbuf is
//    To be[    ]or
// then the return value would be "To be or"
char* gap_to_string(GapBuffer* gbuf) {
    // Your code here
    /* 0   1   2   3   4   5   6   7
       T   h   e   y   F   O   B   []
       T   h   e   y   [   ]F  O    B
       T   h   e   y   F   [   ]O   B
       T   h   e   y   F   O   [    ]B
    */
    //printf("ip is %d\n", gbuf->insert_position);
    //printf("sp is %d\n", gbuf->second_position);
    //printf("size is %d\n", gbuf->size);
    int gapspace = gbuf->second_position - gbuf->insert_position;
    //printf("gapspace is %d\n", gapspace);
    int i = 0;
    char* buf = (char*) malloc(sizeof(char) * (gbuf->size - gapspace));
    for(i = 0; i < gbuf->insert_position; i++){ // 0 1 2 3
        buf[i] = gbuf->data[i];
    }
    for(i = gbuf->second_position; i < gbuf->size; i++){ // 5 6 7
        buf[i-gapspace] = gbuf->data[i];
    }
    buf[i-gapspace] = '\0';
    return buf;
}

// Length of the valid string in the gap buffer
// For example, if the gbuf is
//    To be[    ]or
// Then the length would be strlen("To be or") = 8
int gap_length(GapBuffer* gbuf) {
    // Your code here
    /*char* buf = gap_to_string(gbuf); //TheyFO
    int count = 0;
    int size = strlen(buf);
    return size;*/
    int gapspace = gbuf->second_position - gbuf->insert_position;
    printf("gapspace is %d\n", gapspace);
    int i = 0;
    char* buf = (char*) malloc(sizeof(char) * (gbuf->size - gapspace));
    for(i = 0; i < gbuf->insert_position; i++){ // 0 1 2 3
        buf[i] = gbuf->data[i];
        //buf[i] += gbuf->data[i];
        //printf("char at %d is %c\n", i, gbuf->data[i]);
    }
    for(i = gbuf->second_position; i < gbuf->size; i++){ // 5 6 7
        buf[i-gapspace] = gbuf->data[i];
        //buf[i-gapspace] += gbuf->data[i]; // 4 5 6
        //printf("char at %d is %c\n", i-gapspace, gbuf->data[i]);
    }
    buf[i-gapspace] = '\0';
    int size = strlen(buf);
    free(buf);
    return size;
}

// Break this gap buffer into two at the insert position, returning
// a new gap buffer for the second half. For example, if gbuf is
//     To be or[   ]not to be
// then breaking it apart would cause gbuf to become
//     To be or[             ]
// and the return value would be the new gbuf
//     not to be[     ]
GapBuffer* gap_break(GapBuffer* gbuf) {
    // Your code here

    //creating new buf
    GapBuffer* newbuf = gap_create();
    newbuf->size = gbuf->size - gbuf->second_position; // 8 - 5
    for(int i = 0; i < newbuf->size; i++){ // 0 1 2
        //newbuf->data = calloc(gbuf->size, sizeof(GapBuffer*));
        newbuf->data[i] = gbuf->data[gbuf->second_position];
        gbuf->data[gbuf->second_position] = '\0'; //?
        gbuf->second_position++;
        //printf("checkin sp: %d\n", gbuf->second_position);
        printf("data at %d is %c\n", i, newbuf->data[i]);
        newbuf->insert_position++;
    }
    return newbuf;
}

void gap_print(GapBuffer* gbuf) {
    printf("%p|", gbuf);
    for (int i = 0; i < gbuf->insert_position; i++) {
        printf("%c", gbuf->data[i]);
    }
    printf("[");
    for (int i = gbuf->insert_position; i < gbuf->second_position; i++) {
        printf(" ");
    }
    printf("]");
    for (int i = gbuf->second_position; i < gbuf->size; i++) {
        printf("%c", gbuf->data[i]);
    }
    printf("|\n");
}
/*
  0 1 2 3 4 5 6 7
  T h e y F O B
  T h e y   F O B

*/