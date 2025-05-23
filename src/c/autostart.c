#include "../h/autostart.h"
#include "../h/verbose.h"
#include <windows.h>
#include <wchar.h>
#include <shlobj.h>

int IsInStartup() {
    // initialize path variables
    wchar_t appdata[MAX_PATH];
    wchar_t startup_folder[MAX_PATH];
    wchar_t exe_path[MAX_PATH];

    verbosemsg("Searching AppData folder", "inf");

    // Get the AppData folder path
    if (SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, appdata) != S_OK) {
        verbosemsg("Failed to retrieve AppData folder using SHGetFolderPathW", "err");
        return -1;
    }
    verbosemsg("AppData folder retrieved successfully", "suc");
    
    // Construct the Startup folder path
    if (swprintf(startup_folder, MAX_PATH, L"%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup", appdata) >= MAX_PATH) {
        verbosemsg("Startup folder path too long", "err");
        return -1;
    }

    verbosemsg("Searching Startup folder", "inf");
    
    // Check if the Startup folder exists
    if (GetFileAttributesW(startup_folder) == INVALID_FILE_ATTRIBUTES) {
        verbosemsg("Startup folder does not exist", "err");
        return -1;
    }
    // Construct the path to the executable in the Startup folder
    if (swprintf(exe_path, MAX_PATH, L"%s\\trdasn.exe", startup_folder) < 0) {
        verbosemsg("Failed to construct executable path", "err");
        return -1;
    }
    
    // Check if the executable exists in the Startup folder
    if (GetFileAttributesW(exe_path) != INVALID_FILE_ATTRIBUTES) {
        verbosemsg("In Autostart", "suc");
        return 1;
    } else {
        verbosemsg("Not in Autostart", "war");
        return 0;
    }
}

void toggle_autostart() {
    int status = IsInStartup();
    if (status == 1) {
        verbosemsg("Disabling Autostart", "inf");
        remove_from_startup();
    } else if (status == 0) {
        verbosemsg("Enabling Autostart", "inf");
        copy_to_startup();
    } else {
        verbosemsg("Error checking Autostart status", "err");
    }
}

void copy_to_startup() {
    // initialize path variables
    wchar_t appdata[MAX_PATH] = {0};
    wchar_t startup_folder[MAX_PATH];
    wchar_t exe_path[MAX_PATH];
    wchar_t destination[MAX_PATH];

    verbosemsg("Copying to Startup folder", "inf");
    
    // Get the AppData folder path
    if (SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, appdata) != S_OK) {
        verbosemsg("Failed to retrieve AppData folder using SHGetFolderPathW", "err");
        return;
    }
    verbosemsg("AppData folder retrieved successfully", "suc");
    wprintf(L"AppData folder: '%s'\n", appdata);

    // Construct the Startup folder path
    if (swprintf(startup_folder, MAX_PATH, L"%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup", appdata) >= MAX_PATH) {
        verbosemsg("Startup folder path too long", "err");
        return;
    }

    // Get the path of the current executable
    if (!GetModuleFileNameW(NULL, exe_path, MAX_PATH)) {
        verbosemsg("Failed to retrieve executable path", "err");
        return;
    }

    // Construct the destination path
    swprintf(destination, MAX_PATH, L"%s\\trdasn.exe", startup_folder);

    // Copy the executable to the Startup folder
    if (CopyFileW(exe_path, destination, FALSE)) {
        verbosemsg("Copied to Startup folder successfully", "suc");
    } else {
        verbosemsg("Failed to copy to Startup folder", "err");
    }
}

void remove_from_startup() {
    // initialize path variables
    wchar_t appdata[MAX_PATH] = {0};
    wchar_t startup_folder[MAX_PATH];
    wchar_t exe_path[MAX_PATH];

    verbosemsg("Removing from Startup folder", "inf");
    // Get the AppData folder path
    if (SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, appdata) != S_OK) {
        verbosemsg("Failed to retrieve AppData folder using SHGetFolderPathW", "err");
        return;
    }
    verbosemsg("AppData folder retrieved successfully", "suc");
    wprintf(L"AppData folder: '%s'\n", appdata);

    // Construct the Startup folder path
    if (swprintf(startup_folder, MAX_PATH, L"%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup", appdata) >= MAX_PATH) {
        verbosemsg("Startup folder path too long", "err");
        return;
    }

    // Construct the path to the executable in the Startup folder
    swprintf(exe_path, MAX_PATH, L"%s\\prcsigma.exe", startup_folder);

    // Delete the file if it exists
    if (DeleteFileW(exe_path)) {
        verbosemsg("Removed from Startup folder successfully", "suc");
    } else {
        verbosemsg("Failed to remove from Startup folder", "err");
    }
}