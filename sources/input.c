#include "input.h"

#ifndef INPUT_STREAM
#define INPUT_STREAM stdin
#endif

int ask_for_number(int min, int max, char *question) {
    int number;
    char buf[1024]; // use 1KiB just to be sure
    int success;

    do {
        printf("%s", question);
        if (!fgets(buf, 1024, INPUT_STREAM)) {
            // reading input failed, give up:
            return 1;
        }

        // have some input, convert it to integer:
        char *endptr;

        errno = 0; // reset error number
        number = strtol(buf, &endptr, 10);
        if (errno == ERANGE) {
            printf("Sorry, this number is too small or too large.\n");
            success = 0;
        } else if (endptr == buf) {
            // no character was read
            success = 0;
        } else if (*endptr && *endptr != '\n') {
            // *endptr is neither end of string nor newline,
            // so we didn't convert the *whole* input
            success = 0;
        } else {
            success = 1;
        }
    } while (!success || number < min || number > max); // repeat until we got number valid number

    return number;
}

void ask_for_string(int max, char *question, char *dest) {
    char str[max + 2]; // on for '\0' another to detect if the string is too long
    int error = 1;
    while (error == 1) {
        error = 0;
        printf("%s", question);
        fgets(str, sizeof(str), stdin);
        size_t length = strlen(str);
        if (length > (size_t) max) {
            printf("String too long please try again\n");
            scanf("%*[^\n]");  // Discard all characters until newline
            scanf("%*c");      // Discard the newline itself
            error = 1;
        }
    }
    str[strcspn(str, "\n")] = '\0';
    strcpy(dest, str);
}