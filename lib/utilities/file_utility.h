



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FILE_UTILITY_H

#define FILE_UTILITY_H

#define FILENAME_PROBLEMA2 "problema2.txt"
#define FILENAME_PROBLEMA1 "problema1.txt"

/**
 * @brief Append the given content to the given file
 * @param filename The name of the file to append to
 * @param content The content to append to the file
 */
void append_to_file(const char* filename, const char* content) {
    // Construct the file path
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "./out/%s", filename);

    // Open the file in append mode, create if it doesn't exist
    FILE* file = fopen(filepath, "a+");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for appending: %s\n", filepath);
        return;
    }

    // Move the file pointer to the end of the file
    fseek(file, 0, SEEK_END);

    // Check if the file is empty
    long size = ftell(file);
    if (size > 0) {
        // Append a new line before the content
        fprintf(file, "\n");
    }

    // Append the content to the file
    fprintf(file, "%s", content);

    // Close the file
    fclose(file);
}





#endif // FILE_UTILITY_H