#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void extract_info(const char* input, const char* pattern);

const char* input1 = "8096 SHA256:RoWJBGWIRLhNM01nNAVtMKN+b6AoKc7IzIBLjESd3Lc bob@workstation (RSA)";
const char* input2 = "3072 SHA256:3KM6bS6rLN2ErpKZ/q6w9ofPoclxC1NIQas3ngZoR1A no comment (RSA)";
const char* input3 = "1024 SHA256:5alMYgak0SlJOljkZCrSYhQbu5RpmsFtv3aSx+2irNU fred@laptop (DSA)";
const char* input4 = "256 SHA256:lx/aoiwuLcHSmEwk5+gfokM+6BJ1HLPRbAh9ItgDWNs jack@laptop (ED25519)";

int main(void) {
    const char* pattern = "^(\\d+)\\s([A-Za-z0-9:]+)\\s.*\\(([^\\s()]+)\\)$";
    extract_info(input1, pattern);
    extract_info(input2, pattern);
    extract_info(input3, pattern);
    extract_info(input4, pattern);
    return 0;
}

void extract_info(const char* input, const char* pattern) {
    regex_t regex;
    regmatch_t matches[4];
    size_t substring_length;
    char substring[256]; // Assuming a maximum substring length of 255 characters

    // Compile regex
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Regex compilation failed\n");
        exit(1);
    }

    // Run regex
    int result = regexec(&regex, input, 4, matches, 0);
    if (result == REG_NOMATCH) {
        fprintf(stderr, "No match found for input: %s\n", input);
    } else if (result != 0) {
        char error_message[256];
        regerror(result, &regex, error_message, sizeof(error_message));
        fprintf(stderr, "Regex match failed with error: %s\n", error_message);
    } else {
        printf("Match found for input: %s\n", input);
        for (int i = 1; i < 4; ++i) {
            if (matches[i].rm_so != -1 && matches[i].rm_eo != -1) {
                substring_length = matches[i].rm_eo - matches[i].rm_so;
                if (substring_length >= sizeof(substring)) {
                    fprintf(stderr, "Substring length exceeds buffer size\n");
                    exit(1);
                }
                strncpy(substring, input + matches[i].rm_so, substring_length);
                substring[substring_length] = '\0';
                printf("Group %d: %s\n", i, substring);
            }
        }
    }

    regfree(&regex);
}
