#include <windows.h>
#include <tchar.h>

int main(int argc, TCHAR* argv[])
{
    if (argc != 2)
    {
        _tprintf(_T("Usage: %s \"Message to display in toast\"\n"), argv[0]);
        return 1;
    }

    // Create and initialize a NOTIFYICONDATA structure
    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = NULL; // This is a message-only window, so set it to NULL
    nid.uID = 1; // Unique ID for the notification icon
    nid.uFlags = NIF_INFO;
    nid.dwInfoFlags = NIIF_INFO;
    nid.uTimeout = 2000; // Display for 2 seconds
    _tcscpy_s(nid.szInfo, argv[1]); // Set the message text

    // Display the toast notification
    Shell_NotifyIcon(NIM_MODIFY, &nid);

    // Wait for 2 seconds
    Sleep(2000);

    // Remove the notification icon
    Shell_NotifyIcon(NIM_DELETE, &nid);

    return 0;
}
