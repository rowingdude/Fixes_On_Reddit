#include <stdio.h>
#include <stdlib.h>

void extract_info(const char* input);

const char* input1 = "8096 SHA256:RoWJBGWIRLhNM01nNAVtMKN+b6AoKc7IzIBLjESd3Lc bob@workstation (RSA)";
const char* input2 = "3072 SHA256:3KM6bS6rLN2ErpKZ/q6w9ofPoclxC1NIQas3ngZoR1A no comment (RSA)";
const char* input3 = "1024 SHA256:5alMYgak0SlJOljkZCrSYhQbu5RpmsFtv3aSx+2irNU fred@laptop (DSA)";
const char* input4 = "256 SHA256:lx/aoiwuLcHSmEwk5+gfokM+6BJ1HLPRbAh9ItgDWNs jack@laptop (ED25519)";

int main(void) {
    extract_info(input1);
    extract_info(input2);
    extract_info(input3);
    extract_info(input4);
    return 0;
}

void extract_info(const char* input) {
    int value;
    char algorithm[256];
    char name[256];

    if (sscanf(input, "%d%*s%s%*[^(](%[^)])", &value, algorithm, name) == 3) {
        printf("Match!\n");
        printf("Value: %d\n", value);
        printf("Algorithm: %s\n", algorithm);
        printf("Name: %s\n", name);
    } else {
        fprintf(stderr, "No match found for input: %s\n", input);
    }
}
