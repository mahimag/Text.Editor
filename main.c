#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gap_buffer.h"
#include "document.h"
#include "window.h"
#include "terminal.h"
#include "log.h"
#include "status.h"

int main(int argc, char* argv[]) {
    Document* document = NULL;
    char* filename = NULL;

    if (argc > 1) {
        // Read filename if provided
        filename = argv[1];
        document = document_read(filename);
    } else {
        // Otherwise create a new document with one empty line
        document = document_create();
        Line* line = line_create();
        document_insert_after(document, document->tail, line);
    }

    // Initialize the terminal screen
    terminal_init();
    // Get the width and height of the terminal so we know how much to display
    int width;
    int height;
    terminal_size(&width, &height);

    // Create a window which is a view of the document.
    // Reduce the height by 1 so that we can show the status bar at the bottom
    Window* window = window_create(document, width, height - 1);

    //variables
    int shift = 0;  //shifting window down
    //Line* linesave; //save previous line
    
    for (;;) {
        // Clear the screen
        terminal_clear();

        // Draw the contents of the window
        window_draw(window);
        // Draw the status bar message here
        /* Task 4. Status Bar (5 points) */
        status_message(0, height - 1, "--Wee--");
        /* file name appear */
        status_message(width / 2, height - 1, filename);
        /* num of lines */

        // Get the current line that the cursor is on
        Line* currentline = document_get(document, window->current);
        // Move the cursor to the correct location on the screen
        terminal_move(currentline->gbuf->insert_position,
            window->current - window->first_row);
    
        // Show everything to the user all at once
        terminal_refresh();

        // Wait for user input
        int ch = terminal_read();
        // Handle user input here
        // ...
        if (ch == KEY_BACKSPACE || strcmp(terminal_keyname(ch), "^?") == 0) { // user hit Backspace
            /* Task 14. Removing Text (10 points) */
            if(currentline->gbuf->insert_position > 0){
                gap_remove_char(currentline->gbuf);
            }
            /* Task 15. Remove at Beginning Combines Lines (10 points) */
            // When the user hits the backspace key at the beginning of a line (ie. when the insert position of the current line is 0), it combines the current line with the previous line. 
            // If the cursor is at the beginning of the first line, then hitting backspace has no effect, since there is no previous line to combine with.
            else{ // if ip == 0
                if(window->current != 0){
                    Line* currentline = document_get(document, window->current); //dont need
                    //clearings
                    //window->current--;
                    Line* prevline = document_get(document, window->current--);
                    //his stump...western

                    //save ip of prevline
                    int ip = prevline->previous->gbuf->insert_position;

                    //BREAKPOINT HERE. might change size later (2nd parameter)

                    //gets length of currentline
                    int wholeline = currentline->gbuf->size;
                    int gapspace = currentline->gbuf->second_position - currentline->gbuf->insert_position;
                    int endindex = wholeline - gapspace;

                    //gets length of prevline
                    int wholeline2 = prevline->gbuf->size;
                    int gapspace2 = prevline->gbuf->second_position - prevline->gbuf->insert_position;
                    int endindex2 = wholeline2 - gapspace2;

                    char * str = gap_to_string(prevline->gbuf);
                    // log_debug("string is: %s\n", str); //clearings

                    gap_insert_string(prevline->previous->gbuf, endindex, str);

                    // char * newstr = gap_to_string(prevline->gbuf);
                    // log_debug("string is: %s\n", newstr);

                    document_remove(document, currentline);

                    gap_set_insert_position(prevline->previous->gbuf, ip);
                }
            }
        }
        else if (strcmp(terminal_keyname(ch), "^J") == 0) {
            // user hit Enter
            //check if at end of line
            int wholeline = currentline->gbuf->size;
            int gapspace = currentline->gbuf->second_position - currentline->gbuf->insert_position;
            int endindex = wholeline - gapspace;
            /* Task 12. Enter At Beginning Adds Lines Before Current (5 points) */
            //better
            int ip = currentline->gbuf->insert_position;
            log_debug("ip is : %d\n", ip);
            if(currentline->gbuf->insert_position == 0){
                Line* newline = line_create();
                document_insert_before(document, currentline, newline);
                if(window->height >= document->num_lines - 1){
                    window->height += 1;
                    //window->current += 1;
                }
                gap_set_insert_position(newline->gbuf, newline->gbuf->insert_position);
                window->current += 1;
            }
            /* Task 11. Enter At End Inserts Lines After Current (5 points) */
            else if(currentline->gbuf->insert_position == endindex){
                
                Line* newline = line_create();
                document_insert_after(document, currentline, newline);
                //currentline - On Tower-hill
                //newline - blank
                int wh = window->height;
                log_debug("wh: %d\n", wh); // wh ranges from ~4-11
                if(window->height <= document->num_lines - 1){
                    window->height += 1;
                    window->current += 1;
                    //Line* newcur = document_get(document, window->current++);
                    gap_set_insert_position(newline->gbuf, 0);
                }
                else{
                    gap_set_insert_position(newline->gbuf, newline->gbuf->insert_position);
                }
            }
            /* Task 13. Enter Breaks Lines (10 points)
            When the user hits enter at the middle of a line, it breaks the line into two. Implement this by using gap_break() and inserting a new line into the document, and changing the current line to the new line. The insertion point should also be set to the beginning of the new line. */
            else{
                //Line* currentline = document_get(document, window->current);
                
                //save ip of currentline
                int ip = currentline->gbuf->insert_position; //same as endindex
                //66
                //IMPLEMENT LATER - gap_set_insert_position(gbuf, ip);
                Line* newline = line_create();

                //storing old currentline
                //Line* storeline = currentline;
            
                // document_insert_after(document, currentline, newline);
                GapBuffer* newgbuf = gap_break(currentline->gbuf);

                //curline-> crippled beggar...painted
                //newline-> board
                newline = line_create_gap(newgbuf);

                char * s1 = currentline->gbuf->data;
                log_debug("string1: %s\n", s1); //
                char * s2 = newline->gbuf->data;
                log_debug("string2: %s\n", s2); //

                int ip1 = currentline->gbuf->insert_position;
                log_debug("ip1: %d\n", ip1); //
                int ip2 = newline->gbuf->insert_position;
                log_debug("ip2: %d\n", ip2); //

                // //save ip of prevline
                // int ip = prevline->previous->gbuf->insert_position;

                //BREAKPOINT HERE.

                //update sp for cur line
                GapBuffer* temp1 = gap_create();
                gap_insert_string(temp1, (int) strlen(currentline->gbuf->data), currentline->gbuf->data);
                currentline->gbuf->second_position = temp1->second_position;
                free(temp1);
                //update sp for newline
                GapBuffer* temp2 = gap_create();
                gap_insert_string(temp2, (int) strlen(newline->gbuf->data), newline->gbuf->data);
                newline->gbuf->second_position = temp2->second_position;
                //newline->gbuf->insert_position = temp2->second_position;
                free(temp2);

                

                //gets length of currentline
                int wholeline = currentline->gbuf->size;
                int gapspace = currentline->gbuf->second_position - currentline->gbuf->insert_position;
                int endindex = wholeline - gapspace;

                //gets length of newline
                int storeip = newline->gbuf->insert_position;
                int storesp = newline->gbuf->second_position;
                int wholeline2 = newline->gbuf->size;
                int gapspace2 = newline->gbuf->second_position - newline->gbuf->insert_position;
                int endindex2 = wholeline2 - gapspace2;

                //newline - board//

                // //update sp for cur line
                // GapBuffer* temp = gap_create();
                // gap_insert_string(temp, strlen(newline), newline);
                // currentline->gbuf->second_position = temp->second_position;
                // free(temp);

                //MAYBE MOVE THIS EARLIER
                document_insert_after(document, currentline, newline);
                //document_insert_before(document, newline, currentline);
                
                char * news = newline->gbuf->data;
                log_debug("new s: %s \n", news);

                int insertp = newline->gbuf->insert_position;

                //gap_set_insert_position(newline->gbuf, newline->gbuf->insert_position);
                gap_set_insert_position(newline->gbuf, insertp);

                // newline->gbuf->insert_position = storeip;
                // newline->gbuf->second_position = storesp;
                
                //MAYBE
                //gap_set_insert_position(newline->gbuf, 0);
                window->height++;
                window->current++;
                
            }
        }
        /* Task 5. Quitting (5 points) */
        // user hit Ctrl-Q
        else if (strcmp(terminal_keyname(ch), "^Q") == 0) {
            /* Task 6. Command Line (5 points)
               Task 16. Saving (5 points) */
            if(argc > 1){
                document_write(document, "test/input.txt");
                document_write(document, "test/output.txt");
            }
            else{
                document_write(document, "unnamed.txt");
                document_write(document, "test/output.txt");
            }
            break;
        }
        /* Task 8. Key Up and Key Down Navigation (5 points)
           Task 9. Key Left and Key Right Navigation (5 points) */
        // first_row, window->current
        else if(ch == KEY_LEFT){
            //get current line's insert position
            //checks if at left edge
            if(currentline->gbuf->insert_position > 0){
            //if(window->first_row != currentline->gbuf->insert_position){
                gap_set_insert_position(currentline->gbuf, currentline->gbuf->insert_position - 1);
            }
        }
        else if(ch == KEY_RIGHT){
            //if(currentline->gbuf->insert_position != [])){
                if(currentline->gbuf->second_position < currentline->gbuf->size){
                    gap_set_insert_position(currentline->gbuf, currentline->gbuf->insert_position + 1);
                }
            //}
            //check if cursor is at end of cur line and dont let it go past
        }
        else if(ch == KEY_DOWN){
            //gets next line
            //checks if at bottom ledge
            //log_debug("doc height : %d\n", document->num_lines);
            if(window->current != (document->num_lines - 1)){
                // int ip = currentline->gbuf->insert_position;
                // log_debug("ip : %d\n", ip);
                //linesave = document_get(document, window->current);
                //window->current += 1;
                Line* nextline = document_get(document, window->current++);
            
                // ip = nextline->gbuf->insert_position;
                // // int sp = prevline->gbuf->second_position;
                // log_debug("ip : %d\n", ip);
                // // log_debug("sp : %d\n", sp);
                
                //if nextline isnt end of window
                //log_debug("window height : %d\n", window->height);
                //log_debug("curr height : %d\n", window->current);
                gap_set_insert_position(currentline->gbuf, nextline->gbuf->insert_position);
                //if((window->current) <= (window->height + shift){
                if(window->current > (window->height - 1 + shift)){
                    shift++;
                    window->first_row += 1; //update current line ur looking at
                }
            }
        }
        else if(ch == KEY_UP){
            //gets next line
            //checks for upper ledge
            if(window->current != 0){
                
                //int ip = currentline->gbuf->insert_position;
                //log_debug("ip : %d\n", ip);
                //window->current -= 1; // 9
                //log_debug("curline : %d\n", window->current);
                Line* prevline = document_get(document, window->current--);
                //line 0

                // int ip = prevline->previous->gbuf->insert_position;
                // int ip2 = prevline->gbuf->insert_position;
                // int sp = prevline->gbuf->second_position;
                //log_debug("ip : %d\n", ip);
                // log_debug("sp : %d\n", sp);

                gap_set_insert_position(prevline->gbuf, prevline->gbuf->insert_position);
                // 3 > = 4
                if(window->current < window->first_row){
                    shift--;
                    window->first_row -= 1;
                }
            }
        }
        /* Task 10. Inserting Text (10 points) */
        /* When the user types text, the values get inserted into the current line at the insert position. Implement this by adding a character to the gap buffer for the current line. */
        else if(ch != -1){
            gap_insert_char(currentline->gbuf, ch);
            int ip = currentline->gbuf->insert_position;
            gap_set_insert_position(currentline->gbuf, ip);
        }
        else{
            //
        }
    }

    terminal_end();

    // On quit, write the document to output file here
}