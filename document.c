#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "document.h"
#include "log.h"

// Create a line node with an empty gap buffer
Line* line_create() {
    GapBuffer* gbuf = gap_create();
    return line_create_gap(gbuf);
}

// Create a line node with the given gap buffer
Line* line_create_gap(GapBuffer* gbuf) {
    Line* line = (Line*) malloc(sizeof(Line));
    line->gbuf = gbuf;
    line->next = line->previous = NULL;
    return line;
}

// Make a new document
Document* document_create() {
    // Your code here
    Document* doc = (Document*) malloc(sizeof(Document));
    doc->head = NULL;
    doc->tail = NULL;
    doc->num_lines = 0;
    return doc;
}

// Get the Line at the given row from the document.
// Iterate the list from the head till you find the right Line.
Line* document_get(Document* document, int row) {
    // Your code here
    int count = 0;
    Line* cur = document->head;
    while(cur->next != NULL){
        if(count == row){
            return cur;
        }
        cur = cur->next;
        count++;
    }
    return cur;
}

// Insert a new Line into the document, after the given node.
// InsertAfter should only be null if the document is empty
// with no lines.
void document_insert_after(Document* document, Line* insertAfter, Line* newline) {
    // Your code here
    if(document->num_lines == 0){ //empty doc
        document->head = newline;
        document->tail = newline;
        document->num_lines++;
    }
    else{
        if(insertAfter->next != NULL){ //if smth after line 1
            insertAfter->next->previous = newline;
            newline->next = insertAfter->next;
        } //BREAKPOINT. this might be wrong
        insertAfter->next = newline;
        newline->previous = insertAfter;
        //if insertAfter == tail
        if(insertAfter == document->tail){
            document->tail = newline;
        }
        document->num_lines++;
    }

}

// Insert a new Line into the document, before the given node.
// InsertBefore should only be null if the document is empty
// with no lines.
void document_insert_before(Document* document, Line* insertBefore, Line* newline) {
    // Your code here
    if(document->num_lines == 0){
        document->head = newline;
        document->tail = newline;
        document->num_lines++;
    }
    else{
        if(insertBefore->previous != NULL){ //if smth before line 1
            insertBefore->previous->next = newline;
            newline->previous = insertBefore->previous;
        }
        insertBefore->previous = newline;
        newline->next = insertBefore;
        //if insertAfter == tail
        if(insertBefore == document->head){
            document->head = newline;
        }
        document->num_lines++;
    }
}

// Remove a line node from the document
void document_remove(Document* document, Line* line) {
    // Your code here
    //iterate through lines until line is found
    Line* cur = document->head;
    if(cur == line){
        document->head = cur->next;
        document->head->previous = NULL;
        free(document->head->previous);
    }
    else{
        while(cur != line){
            cur = cur->next;
        } // cur = line 2
        Line* prev = cur->previous; // line 1
        prev->next = cur->next; // 1 -> NULL
        if(prev->next != NULL){
            cur->next->previous = prev; // 3 <- 1 !
        }
        cur = NULL;
        free(cur); // line 2
        if(line == document->tail){
            document->tail = prev;
        }
    }
    document->num_lines--;

}

// Read from a file from the given filename.
// Create a document object and the lines for each line in the file,
// and return the document object.

//if u create Line, u dont need to create GBUF
//insert every char into Line
Document* document_read(const char* filename) {
    // Your code here

    //Create a document, values to be inputed later!
    Document* document = document_create();
    /*
        document->head = 
        document->tail = 
        num_lines = 
    */
    //Create file pointer and Open file (r to read)!
    FILE* fp = fopen(filename, "r+");

    //variables
    char buf[300];
    int index = 0;
    int length = 0;
    Line* prev;
    bool flag = true;

    while(fp != EOF){ //Loop thru lines


        //Create line with gbuf above
        Line* line = line_create();

        //reads fp and stores (1) entire line into buf
        if(fgets(buf, 256, fp) == NULL){
            return document;
        }
        length = strlen(buf); //Gets length of each line

        //count words
        int words = 1;
        for(int i = 0; i < length; i++){
            if(buf[i] == ' '){
                words++;
            }
        }

        //checking if last line
        if(words != 1){
            //Filling gap with lines
            for(int i = 0; i < length - 1; i++){
                gap_insert_char(line->gbuf, buf[i]); //mem alloc?
            }
        }
        else{
            for(int i = 0; i < length; i++){
                gap_insert_char(line->gbuf, buf[i]); //mem alloc?
            }
        }
        /*//counting for test
        for(int i = 0; i < length; i++){
            gap_insert_char(line->gbuf, buf[i]); //mem alloc?
        }

        printf("num of chars in string: %d\n", line->gbuf);*/

        //print statements
        //printf("how many words: %d\n", words);
        printf("how many letters in each line (length): %d\n", length);
        printf("\n%s\n", buf);

        //increase document's num of lines by 1
        int lines = document->num_lines++;
        printf("how many lines? %d\n", lines);

        //printf("testing123\n");
        /*document_insert_after(document, line->previous, line);*/

        if(flag){ //flag to ensure only go in once
            flag = false;
            document->head = line;
            document->tail = line;
            line->next = NULL;
            line->previous = NULL;
            prev = line;
            //free(line);
        }
        else{
            line->previous = prev;
            document_insert_after(document, line->previous, line);
            prev = line;
            document->num_lines--;
            //free(line);
        }
        document_print(document);
    }
    //close file!
    fclose(fp);
    return document;
}

// Write all lines of the document out to the given filename.
// Return true on success, or false if there was an error
bool document_write(Document* document, const char* filename) {
    // Your code here

    //go through lines of document and write it out to file
    //create file
    FILE* fp = fopen(filename, "w"); // type is subject to change

    if(fp == NULL){
        printf("Error1\n");
        return false;
    }

    int lines = document->num_lines;
    printf("num of lines? %d\n", lines);

    Line* line;
    for(int i = 0; i < lines; i++){
        line = document_get(document, i);
        if(line != document->tail){
            fprintf(fp, "%s\n", gap_to_string(line->gbuf));
        }
        else{
            fprintf(fp, "%s", gap_to_string(line->gbuf));
        }
    }
    fclose(fp);
    return true;
}

void document_print(Document* document) {
    printf("Document %p lines=%d [\n", document, document->num_lines);
    for (Line* line = document->head; line != NULL; line = line->next) {
        printf("\t");
        gap_print(line->gbuf);
    }
    printf("]\n");
}