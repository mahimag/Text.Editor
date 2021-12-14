#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

extern "C" {
#include "gap_buffer.h"
#include "document.h"
}

TEST(Gap, Creating) {
    GapBuffer* gbuf = gap_create();
    ASSERT_TRUE(gbuf != NULL);
}

TEST(Gap, Insert) {
    GapBuffer* gbuf = gap_create();
    gap_insert_char(gbuf, 'T');
    gap_insert_char(gbuf, 'h');
    ASSERT_EQ(gbuf->data[0], 'T');
    ASSERT_EQ(gbuf->data[1], 'h');
    ASSERT_EQ(gbuf->insert_position, 2);
    ASSERT_EQ(gbuf->second_position, 4);
}

TEST(Gap, Setcursor1) {
    GapBuffer* gbuf = gap_create();
    gap_insert_char(gbuf, 'T');
    gap_insert_char(gbuf, 'h');
    gap_insert_char(gbuf, 'e'); 
    gap_insert_char(gbuf, 'y'); 
    gap_set_insert_position(gbuf, 2);

    ASSERT_EQ(gbuf->data[0], 'T');
    ASSERT_EQ(gbuf->data[1], 'h');
    ASSERT_EQ(gbuf->insert_position, 2);
    ASSERT_EQ(gbuf->second_position, 2);
    ASSERT_EQ(gbuf->data[2], 'e');
    ASSERT_EQ(gbuf->data[3], 'y');
}

TEST(Gap, Setcursor2) {
    GapBuffer* gbuf = gap_create();

    printf("checkpoint\n");

    gap_insert_char(gbuf, 'T');
    gap_insert_char(gbuf, 'h');
    gap_insert_char(gbuf, 'e');
    gap_insert_char(gbuf, 'i');
    gap_insert_char(gbuf, 'r');
    printf("checking size = %d\n", gbuf->size);
    printf("checking ip = %d\n", gbuf->insert_position);
    printf("checking sp = %d\n", gbuf->second_position);
    gap_set_insert_position(gbuf, 2);

    printf("checking data pt = %c\n", gbuf->data[1]);

    ASSERT_EQ(gbuf->data[0], 'T');
    ASSERT_EQ(gbuf->data[1], 'h');
    ASSERT_EQ(gbuf->insert_position, 2);
    ASSERT_EQ(gbuf->second_position, 5);
    ASSERT_EQ(gbuf->data[5], 'e');
    ASSERT_EQ(gbuf->data[6], 'i');
    ASSERT_EQ(gbuf->data[7], 'r');
}

TEST(Gap, Remove) {
    GapBuffer* gbuf = gap_create();
    gap_insert_char(gbuf, 'T');
    gap_insert_char(gbuf, 'h');
    gap_insert_char(gbuf, 'e');
    gap_insert_char(gbuf, 'i');
    gap_insert_char(gbuf, 'r');
    gap_remove_char(gbuf);
    gap_set_insert_position(gbuf, 2);
    gap_remove_char(gbuf);

    ASSERT_EQ('T', gbuf->data[0]);
    ASSERT_EQ(1, gbuf->insert_position);
    ASSERT_EQ(6, gbuf->second_position);
    ASSERT_EQ('i', gbuf->data[7]);
}

TEST(Gap, ToString) {
    printf("--------NOW TESTING TOSTRING\n");
    GapBuffer* gbuf = gap_create();
    gap_insert_char(gbuf, 'T');
    gap_insert_char(gbuf, 'h');
    gap_insert_char(gbuf, 'e');
    gap_insert_char(gbuf, 'y');
    gap_insert_char(gbuf, 'F');
    gap_insert_char(gbuf, 'O');
    gap_insert_char(gbuf, 'B');

    printf("--------SETTING NEW IP\n");
    gap_set_insert_position(gbuf, gbuf->insert_position - 1);
    printf("--------SETTING NEW IP\n");
    gap_set_insert_position(gbuf, gbuf->insert_position - 1);
    printf("--------SETTING NEW IP\n");
    gap_set_insert_position(gbuf, gbuf->insert_position - 1);
   
    printf("--------CHECKING STRING EQUIVALENCY\n");
    char* s1 = gap_to_string(gbuf);
    ASSERT_TRUE(strcmp(s1, "TheyFOB") == 0);
    printf("\nSP IS: %d\n", gbuf->second_position);
    free(s1);
    printf("\nSP IS: %d\n", gbuf->second_position);
    printf("\n--------TESTING\n");
    
    printf("--------SETTING NEW IP\n");
    gap_set_insert_position(gbuf, gbuf->insert_position + 1);
    printf("--------SETTING NEW IP\n");
    gap_set_insert_position(gbuf, gbuf->insert_position + 1);
    char* s2 = gap_to_string(gbuf);
    ASSERT_TRUE(strcmp(s2, "TheyFOB") == 0);
    free(s2);
}

TEST(Gap, Length) {
    GapBuffer* gbuf = gap_create();
    gap_insert_char(gbuf, 'T');
    gap_insert_char(gbuf, 'h');
    gap_insert_char(gbuf, 'e');
    gap_insert_char(gbuf, 'y');
    gap_insert_char(gbuf, 'F');
    gap_insert_char(gbuf, 'O');
    ASSERT_EQ(6, gap_length(gbuf));
}

TEST(Gap, Break) {
    GapBuffer* gbuf = gap_create();
    gap_insert_char(gbuf, 'T');
    gap_insert_char(gbuf, 'h');
    gap_insert_char(gbuf, 'e');
    gap_insert_char(gbuf, 'y');
    gap_insert_char(gbuf, 'F');
    gap_insert_char(gbuf, 'O');
    gap_insert_char(gbuf, 'B');
    for(int i = 0; i < gbuf->size; i++){
        printf("data at %d is %c\n", i, gbuf->data[i]);
    }
    printf("--------SETTING POS TO 4\n");
    gap_set_insert_position(gbuf, 4);
    for(int i = 0; i < gbuf->size; i++){
        printf("data at %d is %c\n", i, gbuf->data[i]);
    }
    printf("\n");
    printf("--------STARTING GAP BREAK\n");
    GapBuffer* newbuf = gap_break(gbuf);
    for(int i = 0; i < newbuf->size; i++){
        printf("the data at %d is %c\n", i, newbuf->data[i]);
    }
    //gap_print(gbuf);
    //gap_print(newbuf);

    printf("--------COMPARING STRINGS\n");
    ASSERT_TRUE(strcmp(gap_to_string(gbuf), "They") == 0);
    ASSERT_TRUE(strcmp(gap_to_string(newbuf), "FOB") == 0);
}

TEST(Document, Create) {
    Document* document = document_create();
    ASSERT_TRUE(document != NULL);
    ASSERT_EQ(0, document->num_lines);
    ASSERT_EQ(NULL, document->head);
    ASSERT_EQ(NULL, document->tail);
}

TEST(Document, InsertAfter1) {
    Document* document = document_create();
    Line* line = line_create();
    document_insert_after(document, document->tail, line);

    ASSERT_EQ(1, document->num_lines);
    ASSERT_EQ(line, document->head);
    ASSERT_EQ(line, document->tail);
}

TEST(Document, InsertAfter2) {
    Document* document = document_create();
    Line* line1 = line_create();
    document_insert_after(document, document->tail, line1);

    Line* line2 = line_create();
    document_insert_after(document, line1, line2);

    ASSERT_EQ(line1, document->head);
    ASSERT_EQ(line2, document->tail);
}

TEST(Document, InsertBefore) {
    Document* document = document_create();
    Line* line1 = line_create();
    document_insert_after(document, document->tail, line1);

    Line* line2 = line_create();
    document_insert_after(document, line1, line2);

    ASSERT_EQ(line1, document->head);
    ASSERT_EQ(line2, document->tail);

    Line* line3 = line_create();
    document_insert_before(document, line2, line3);

    ASSERT_EQ(line3, document->head->next);
    ASSERT_EQ(line3, document->tail->previous);
}

TEST(Document, Remove) {
    Document* document = document_create();
    Line* line1 = line_create();
    document_insert_after(document, document->tail, line1);

    Line* line2 = line_create();
    document_insert_after(document, line1, line2);

    Line* line3 = line_create();
    document_insert_before(document, line2, line3);

    document_remove(document, line3);
    ASSERT_EQ(line2, document->head->next);
    ASSERT_EQ(line1, document->tail->previous);

    document_remove(document, line2);
    ASSERT_EQ(line1, document->head);
    ASSERT_EQ(line1, document->tail);
}

// Test reading a file
TEST(DocumentIO, Read) {
    const char* filename = "test/input.txt";
    Document* document = document_read(filename);
    ifstream is(filename);

    for (Line* line = document->head; line != NULL; line = line->next) {
        string input;
        getline(is, input);
        if (input.length() == 0) {
            break;
        }
        cout << input << endl;
        //printf("did i pass?\n");
        char* s = gap_to_string(line->gbuf);
        ASSERT_EQ(s, input);
        free(s);
    }
}

// Test reading then writing a file
TEST(DocumentIO, Write) {
    const char* filename = "test/input.txt";
    Document* document = document_read(filename);
    printf("READ DONE\n");//
    const char* output = "test/output.txt";
    unlink(output);
    document_write(document, output);
    printf("WRITE DONE\n");//
    ifstream is(output);
    int count= 0;//
    for (Line* line = document->head; line != NULL; line = line->next) {
        string data;
        getline(is, data);
        if (data.length() == 0) {
            break;
        }
        cout << data << endl;
        count++;//
        printf("count: %d\n", count); //
        char* s = gap_to_string(line->gbuf);
        ASSERT_EQ(s, data);
        free(s);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}