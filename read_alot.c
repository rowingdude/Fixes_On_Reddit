#include <stdio.h>
#include <string.h>

int compareResponse(char response[], char answer[]) {
    return strcmp(response, answer) == 0;
}

int main() {
    char answers[7][20] = {"answer1", "answer2", "answer3", "answer4", "answer5", "answer6", "answer7"}; // array of correct answers
    char response[20]; 
    FILE *infile = fopen("filename.txt", "r"); 
    int i = 0; 
    while (fgets(response, 20, infile)) { 
        printf("%s", compareResponse(response, answers[i]) ? "Correct!\n" : "Incorrect.\n"); 
        i += compareResponse(response, answers[i]); 
    }
    fclose(infile); // close the file
    return 0;
}
