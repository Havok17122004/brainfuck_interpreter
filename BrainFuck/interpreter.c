#include <stdio.h>
#include <stdlib.h>

/**
 * Write a code interpret brainfuck into C.
 * Since we know that we need an array of 30000 boxes each of 1 byte, we use a static array of char of 30,000 boxes.
 * The values at chars must be cyclic, as seen on the brainfuck interpreter. 
 * but storing 30000 elements is very difficult! More efficient?
 * LinkedList, hashmap, trie, csrarray - chatgpt
 * leave it each have more disadvantages as compared to static arrays
 * 
 * 
 * STEP 1-
 *      Validate the input. ONLY BALANCE BRACKETS, OTHERWISE ALL ARE OKK.
 *      switch-case for ,.-+><
 * the array indices must be cyclic.
 * In case the number of inputs needed is more than the inputs given, reinitialise it to zero
 * Prompt when unbalanced brackets
 *  WHAT SHOULD I DO IF IT RUNS INTO AN INFINITE LOOP!? ON THE ONLINE INTERPRETER, THE WEBSITE HANGS.
 * 
*/
int ind = 0; // current index being worked on
char box[30000];

int validInput(){
    FILE* ptr = fopen("input.txt", "r");
    int bracCnt = 0;
    char c;
    if(ptr == NULL){
        printf("MAKE SURE THE INPUT FILE EXISTS");
        fclose(ptr);
        return 0;
    }
    do {
        c = fgetc(ptr);
        if (c == '[') {
            bracCnt++;
        } else if (c == ']') {
            bracCnt--;
        }
        if(bracCnt < 0){
            printf("THE BRACKETS ARE UNBALANCED");
            return 0;
        }
    } while(c != EOF);
    if(bracCnt){
        printf("THE BRACKETS ARE UNBALANCED");
        return 0;
    }
    fclose(ptr);
    return 1;
}

void oneByOne(char c){
    switch (c){
        case '.' :
            putchar(*(box + ind));
            break;
        case ',':
            char tmp = getchar();
            if(tmp != EOF){
                *(box + ind) = tmp;
            } else {
                *(box + ind) = 0;
            }
            break;
        case '>':
            ind++;
            if(ind >= 30000) ind -= 30000;
            break;
        case '<':
            ind--;
            if(ind < 0) ind += 30000;
            break;
        case '+':
            *(box + ind) = *(box + ind) + 1;
            break;
        case '-':
            *(box + ind) = *(box + ind) - 1;
            break;
    }
}
/*
if bracket, maintain the needed input in a char array of variable length.
fill the array until encounter ]
if length is exceeded, increase length by 20
maintain index of ] so that garbage values are not accessed
run the do - while loop which reads the inputs --> c = arr[i], arr[i+1], etc while c != ]
if [ is encountered once more, arr[r] == '[', run bracketEncountered once again 
from r+1 
*/

int putInArray(char **p, FILE* ptr){
    int bracCnt = 1;
    char c;
    int i = 0;
    int k = 1;
    while(bracCnt){
        c = fgetc(ptr);
        if(c=='+' || c=='-' ||c=='>' ||c=='<' ||c==',' ||c=='.'){

        } else if(c == '['){
            bracCnt++;
        } else if(c == ']'){
            bracCnt--;
        } else continue;
        if(bracCnt){
            *(*p+i) = c;
            i++;
            if(i >= 20*k){
                k++;
                char *tmp = realloc(*p, k * 20 * sizeof(char));
                if (tmp == NULL) {
                    // Handle realloc failure
                    printf("Memory allocation failed\n");
                    // Optionally, free the original memory block pointed to by p
                    free(*p);
                    exit(EXIT_FAILURE);
                } else {
                    *p = tmp; // Update p with the new memory address returned by realloc
                }
            }
        }
    }
    return i;
}

// p contains now [-.]
void solveBracs(char *p, int start, int len){
    char c;
    int bracCnt;
    int k;
    while(box[ind]){
        // printf("%d\t", box[ind]);
        for(int i = start; i < start + len; i++){ // 0 to 4(exclusive)
            c = *(p+i);
            if(c=='+' || c=='-' ||c=='>' ||c=='<' ||c==',' ||c=='.'){ // [-.]
                // printf("%c\t", c);
                oneByOne(c);
                // i++;
            } else if(c == '['){
                bracCnt = 1;
                k = 1;
                while(bracCnt){
                    if(*(p + i + k) == '['){ // i = 0, p + i + 1 = -, p + i + 2 = ., k = 3 for ']'
                        bracCnt++;
                        k++;
                    } else if(*(p + i + k) == ']'){
                        bracCnt--;
                        if(bracCnt){
                            k++;
                        } else k--;
                    } else{
                        k++;// 
                    }
                } // k = 2, i = 0 [-.]
                solveBracs(p, i+1, k);
                i = i + k + 1;
            }
        }
    }
}

int main(){
    for(int i = 0; i < 30000; i++) box[i] = 0;
    if(!validInput()) return 0;
    FILE* ptr = fopen("input.txt", "r");
    char c;
    int len;
    char *p = (char *) malloc (20*sizeof(char));
    do{
        c = fgetc(ptr);
        if(c == '['){
            len = putInArray(&p, ptr);
            solveBracs(p, 0, len);
        } else if(c=='+' || c=='-' ||c=='>' ||c=='<' ||c==',' ||c=='.'){
            oneByOne(c);
        }
    }while(c != EOF);
    fclose(ptr);
    return 0;
}
