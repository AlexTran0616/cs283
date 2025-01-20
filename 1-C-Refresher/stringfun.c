#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here



// [SETUP BUFFER START] //TODO: 
// #4: Implement the setup buff as per the directions
int setup_buff(char *buff, char *user_str, int len) {

    int i = 0;
    int length = 0;
    char *temp = user_str;
    int wordSwitch = 0;

    // This counts the number of characters that are in the string.
    while (*temp != '\0') {
        length++;
        temp++;
    }

    // This checks if the length of the string is too long.
    // [IF] the length is too long, this is invalid and we
    // don't need to do anything else. [RETURN] an ERROR
    if (length > len) {
        return -1;
    }

    // Checks to the end of the user string '\0' is the end of the string
    while (*user_str != '\0' && i < len) {
        if (*user_str != '\t' && *user_str != ' ') { // [IF] this is NOT a space or tab.
            *(buff + i) = *user_str;
            i++;
            wordSwitch = 1;
            // [THEN] we share the character to the buffer
        } else {
            if (wordSwitch) { // [IF] this is a space (we know it's a space because the last condition failed)
                *(buff + i) = ' ';
                i++;
                wordSwitch = 0;
            } // [THEN] we convert all these buffer spaces to just 1 space (so "       " = " ").
        }
        user_str++; // go to next character in the string.
    }

    int meaningful_length = i; // Track the length of the meaningful part of the buffer
                               // NOTE: I had buffer issues, so I added this so the other functions would know this.
    
    // Adds "......" to the rest of the list until buffer len = BUFFER_SZ;
    while (i < len) {
        *(buff + i) = '.';
        i++;
    }

    return meaningful_length; // Return the length of the modified string
}
// [SETUP BUFFER END]




 
void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}



int count_words(char *buff, int len, int str_len) {
    int numberOfWords;
    int inWord;
    int i;
    i = 0;
    inWord = 0;
    numberOfWords = 0;

    if (str_len > len) {
        printf("Error: Input string is too long");
        return -1;
    }

    for (i = 0; i < str_len; i++) {
        if (*(buff + i) == '\t' || *(buff + i) == ' ') { // [IF] current element isn't a space or tab.
            if (inWord) { // [If] if we were in a word.
                numberOfWords++; // [THEN] now we're out of a word. Increment word and reset to say we're no longer in a word.
                inWord = 0;
            }
        } else {
            inWord = 1; // [ELSE] we in a word right now.
        }
    }

    if (inWord) {
        numberOfWords++;
    }

    return numberOfWords;
}



int reverse_string(char *buff, int str_len) {
    int i = 0, j = str_len - 1;
    char temp;

    if (str_len == 0) {
        return -1; // Error if the string is empty
    }

    // We iterate from the front and back of the buffer and swap them.
    while (i < j) {
        temp = *(buff + i);
        *(buff + i) = *(buff + j);
        *(buff + j) = temp;
        i++;
        j--;
    }
    
    // Prints the reversed string (self explanatory)
    printf("Reversed String: ");
    for (i = 0; i < str_len; i++) {
        printf("%c", *(buff + i));
    }
    printf("\n");

    return 0; 
}




int word_print(char *buff, int len, int str_len) {
    int i;
    int inWord;
    int wordNum;
    int numChar;
    numChar = 0;
    wordNum = 1;
    inWord = 0;
    i = 0;

    if (str_len > len) {
        printf("Error Input string is too long");
        return -3;
    }

    printf("Word Print\n---------\n1. ");
    
    // Checks if the word is a space or tab.
    for(i = 0; i < str_len && i < len; i++) {
        if(*(buff + i) == '\t' || *(buff + i) == ' ') {
            if(inWord) { // If it's a space/tab, if we were in a word. print the word since we're now passed it.
                printf(" (%d)", numChar);
                numChar = 0;
                wordNum++;
                printf("\n%d. ", wordNum);
                inWord = 0;
            }
        } else {
            printf("%c", *(buff + i)); // else, we are in a word and we should iterate through it.
            inWord = 1;
            numChar++;
        }
    }
    if(inWord){
        printf(" (%d)", numChar);
        printf("\n"); // we print the word_print list.
    }

    return 0;
}





//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    
    //___________________________________ 
    // [1]EXPLANATION: The reason this is safe is because of how we require the input to be.
    // stringfun requires the user to input -"some input"; however, if the "-" isn't there, an issue 
    // should occur as the user didn't follow the given format. So, this assures the user inputs 
    // their command as intended for the program to run properly.
    //___________________________________
    
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    
    //___________________________________ 
    // [2] EXPLANATION: The purpose of the if statement below is to check that the user has 
    // used the correct number of arguments when inputting their prompt to the command line. 
    // The if statement checks if there are too little arguments (The program needs at least 3 to work). 
    // If there are too few arguments, the program exits with code (1), which indicates a command line problem. 
    //___________________________________
   
   
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string




    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    //          CODE GOES HERE FOR #3

    //[CODE FOR [3]______________________
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL)
    {
            printf("Memory allocation failed\n");
            exit(99);
    }
    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }
    //___________________________________
   
   
   
    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //__________________________________________________________________  
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
	//__________________________________________________________________ 	    
        case 'r':
            rc = reverse_string(buff, user_str_len);
            if (rc < 0){
                printf("Error reversing string, rc = %d", rc);
                exit(2);
            }
            break;

        case 'w':
            rc = word_print(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error printing string, rc = %d", rc);
                exit(2);
            }
            break;

        default:
            usage(argv[0]);
            exit(1);
    
    }
    
    
    
    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//
//          [7] EXPLANATION: I think adding these helper functions and pointer wouold make future changes in the program very flexible.
//          If we ever want to adjust anything to the BUFFER_SZ in the future, we can just change it from that variable. This would mean
//          everywhere in the code we want adjustments to the size of the internal buffer, it would change. This saves time and makes the code
//          more manageable. This also reduces the risk of error. We know this variable will stay equal to 50, so there isn't error when applying
//          it to other parts of the code and "miss-typing". Finally, functions like these allow for reuseability in the program. If we want to
//          add new tests to the code that use that same functionality, we don't need to type if over and over again.
