//header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    int not_valid_count = 0; //to count the invalid enterires
    int valid_count = 0; //to count the vaild floats in the document
    char buffer[256]; //to store each word in the document

    const char *infile = "data.txt"; //the file which will be read
    const char *outfile = "validdata.txt"; //the file which is being created to show valid data

    FILE *readFile = fopen(infile, "r"); //opening the infile to only read
    FILE *writeFile = fopen(outfile, "w"); //opening and creating the outfile to write

    if (readFile == NULL){
        perror("Error opening input file"); //error message for not being able to open infile
        return 1;
    }

    if (writeFile == NULL){
        perror("Error opening output file"); //error message for not being able to open outfile
        fclose(readFile); //closing the infile
        return 1;
    }

    while(1){
        long current = ftell(readFile); //curent position in the infile

        if(fscanf(readFile, "%255s", buffer) == 1){
            char *checker; //pointer to check remaining characters after conversion 
            float value = strtof(buffer, &checker); //convert word to float

            if (*checker == '\0'){ //if is float
                fprintf(writeFile, "%.2f\n", value); //print the valid float in the outfile 
                valid_count++; //increment the valid flaot counter
            }else{ //if not a float
                fseek(readFile, current + strlen(buffer) + 1, SEEK_SET); //move to the next entry
                not_valid_count++; //increment the invalid counter
            }
        }else{
            if(feof(readFile)){
                break; //exit loop
            }
            if(ferror(readFile)){
                perror("Error reading input file"); //error message for file reading
                break; //exit loop
            }
        }
    }

    fclose(readFile); //closing infile
    fclose(writeFile); //closing outfile

    printf("The total number of invalid float values: %d\n", not_valid_count); //printing the counter for invalids
    printf("The total number of valid float values: %d\n", valid_count); //printing counter for valid floats

    return 0;
}
