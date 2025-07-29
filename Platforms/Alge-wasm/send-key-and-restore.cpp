#include <cstdlib>   // for system()
#include <cstdio>    // for printf()
#include <cstring>   // for strlen()


void sendkeystroke_main_applescript_way(const char* apptitle, const char* keystroke) {
    // Construct the AppleScript command
    char scriptCommand[512] = {0};
    snprintf(scriptCommand, sizeof(scriptCommand), "osascript send-key-and-restore.applescript \"%s\" \"%s\"", apptitle, keystroke);

    // Run the command
    int result = system(scriptCommand);

    // Print result
    if (result == 0) {
    //    printf("AppleScript executed successfully.\n");
    } else {
   //     printf("AppleScript failed with code %d\n", result);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <AppTitle> <keystroke>\n", argv[0]);
        return 1;
    }

    // Get the keystroke from the first argument
    sendkeystroke_main_applescript_way(argv[1], argv[2]);

    return 0;
}
