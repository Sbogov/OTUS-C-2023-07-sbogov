#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIGNATURE_LENGTH 22

int is_rarjpeg(const char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, -SIGNATURE_LENGTH, SEEK_END);
    char end_signature[SIGNATURE_LENGTH];
    fread(end_signature, 1, SIGNATURE_LENGTH, file);

    fclose(file);

    return strncmp(end_signature, "PK\x05\x06", 4) == 0;  // Сигнатура ZIP-архива
}

void list_files_in_zip(const char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, file_size - SIGNATURE_LENGTH, SEEK_SET);

    char end_signature[SIGNATURE_LENGTH];
    fread(end_signature, 1, SIGNATURE_LENGTH, file);

    if (strncmp(end_signature, "PK\x05\x06", 4) == 0) {
        // File is a Rarjpeg, list files in the archive
        fseek(file, file_size - SIGNATURE_LENGTH, SEEK_SET);

        // Skip to the start of the central directory record
        fseek(file, -16, SEEK_CUR);

        // Read the central directory record
        char central_dir_record[16];
        fread(central_dir_record, 1, 16, file);

        // Extract the offset of the start of the central directory
        unsigned int central_dir_offset = *(unsigned int*)(central_dir_record + 12);

        // Move to the start of the central directory
        fseek(file, central_dir_offset, SEEK_SET);

        // Read the central directory entries
        while (1) {
            char entry_header[46];
            fread(entry_header, 1, 46, file);

            if (strncmp(entry_header, "\x50\x4B\x01\x02", 4) != 0) {
                // Not a central directory entry header
                break;
            }

            // Extract file name length and file name
            unsigned short file_name_length = *(unsigned short*)(entry_header + 28);
            char file_name[file_name_length + 1];
            fread(file_name, 1, file_name_length, file);
            file_name[file_name_length] = '\0';

            printf("%s\n", file_name);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *input_file = argv[1];

    if (is_rarjpeg(input_file)) {
        printf("%s is a Rarjpeg file.\n", input_file);
        printf("Files in the archive:\n");
        list_files_in_zip(input_file);
    } else {
        printf("%s is not a Rarjpeg file.\n", input_file);
    }

    return EXIT_SUCCESS;
}
