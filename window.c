#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include "terminal.h"
#include "log.h"
#include <string.h>

/* Task 7. Window (10 points) */
Window* window_create(Document* document, int width, int height) {
    // Your code here
    Window* window = (Window*) malloc(sizeof(Window));
    window->document = document;
    window->width = width;
    window->height = height;
    window->first_row = 0;
    window->first_col = 0;
    window->current = 0;
    return window;
}

void window_draw(Window* window) {
    // Your code here
    if(window->height > window->document->num_lines){
        window->height = window->document->num_lines;
    }
    //loop through window height
    for(int i = 0; i < window->height; i++){
        //line is null or not
        Line* cur = document_get(window->document, window->first_row + i);
        if(cur != NULL){
            //gap_insert_char(cur->gbuf, cur->gbuf->data[i]);
            terminal_prints(0, i, gap_to_string(cur->gbuf));
        }
        else{
            terminal_end();
        }
    }
    //void terminal_prints(int x, int y, const char* s) {
    //    mvaddstr(y, x, s);
    //    }
}