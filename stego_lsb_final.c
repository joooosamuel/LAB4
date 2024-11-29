//header
#include <stdio.h>
#include <stdlib.h>

//global 
#define WIDTH 512 //image width
#define HEIGHT 512 //image height

// Function prototypes
int readPGMText(const char *filename, unsigned char *pixels, int width, int height);
int writePGMText(const char *filename, unsigned char *pixels, int width, int height);
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height);
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height);
void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height);

int main() {
    char cover_image[] = "baboon.pgm"; //cover image file name
    char secret_image[] = "farm.pgm"; //secret image file name
    char stego_image[] = "stego_image_bin.pgm"; //output image file name
    char extracted_secret[] = "extracted_secret.pgm"; //gotten secret image file name

    unsigned char *coverPixels; //pointers for image
    unsigned char *secretPixels; //pointers for image
    unsigned char *outputPixels; //pointers for image

    int coverWidth = WIDTH; //cover image width
    int coverHeight = HEIGHT; //cover image height
    int secretWidth = WIDTH; //secret image width
    int secretHeight = HEIGHT; //secret image height

    //Dynamically allocate memory for the cover image
    coverPixels = (unsigned char *)malloc(WIDTH * HEIGHT);
    if (coverPixels == NULL) {
        printf("Error: Memory allocation for coverPixels failed.\n");
        return 1;
    }

    //reading the cover image
    if (readPGMText(cover_image, coverPixels, coverWidth, coverHeight) != 0) {
        printf("Error: Unable to read cover image.\n");
        free(coverPixels);
        return 1;
    }

    
    secretPixels = (unsigned char *)malloc(WIDTH * HEIGHT); //Dynamically allocate memory for the secret image
    if (secretPixels == NULL) {
        printf("Error: Memory allocation for secretPixels failed.\n");
        free(coverPixels);
        return 1;
    }

    //reading the secret image
    if (readPGMText(secret_image, secretPixels, secretWidth, secretHeight) != 0) {
        printf("Error: Unable to read secret image.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }

    //checking if dimensions match of both the coverimage and secretimage
    if (coverWidth != secretWidth || coverHeight != secretHeight) {
        printf("Error: Image dimensions do not match.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }

    embedLSB(coverPixels, secretPixels, WIDTH, HEIGHT); //calling the embedLSB() function so the secret image is put into the cover image

    
    if (writePGMBinary(stego_image, coverPixels, WIDTH, HEIGHT) != 0) { //saving the stego image in binary format
        printf("Error: Unable to save stego image.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }

    outputPixels = (unsigned char *)malloc(WIDTH * HEIGHT);//dynamically allocate memory for the extracted secret image
    if (outputPixels == NULL){
        printf("Error: Memory allocation for outputPixels failed.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }

    
    extractLSB(coverPixels, outputPixels, WIDTH, HEIGHT); //calling the extractLSB() function to extract the secret image from the stego image

    
    if (writePGMText(extracted_secret, outputPixels, WIDTH, HEIGHT) != 0){// Save the secret image gotten in text format
        printf("Error: Unable to save extracted secret image.\n");
    }

    
    free(coverPixels); //free the allocated memory
    free(secretPixels); //free the allocated memory
    free(outputPixels); //free the allocated memory

    return 0;
}

// Function to read an ASCII PGM file
int readPGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "r"); //opening file in read only 
    if (file == NULL) {
        return 1; //error opening file
    }

    char buffer[16];
    if (fgets(buffer, sizeof(buffer), file) == NULL || buffer[0] != 'P' || buffer[1] != '2') {
        fclose(file);
        return 1; // Invalid PGM format
    }

    // Skip comments
    do{
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            fclose(file); //close file
            return 1; //error reading file
        }
    } while(buffer[0] == '#');

    int w;
    int h;
    int maxVal;
    sscanf(buffer, "%d %d", &w, &h); //stroing image dimensions and max pixel value
    if(fscanf(file, "%d", &maxVal)!=1||w!=width||h!=height||maxVal!=255){
        fclose(file);//close file
        return 1; //Mismatched dimensions or unsupported format
    }

    for(int i=0;i<width*height;i++){
        if(fscanf(file, "%hhu", &pixels[i])!=1){ //read the pixel data
            fclose(file); //closing file
            return 1; //error reading pixel data
        }
    }

    fclose(file); //close file
    return 0; //when carried out succesfully
}

// Function to write an ASCII PGM file
int writePGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file=fopen(filename, "w"); //opening file in write mode
    if(file==NULL){
        return 1; //error opening file
    }

    fprintf(file, "P2\n# Created by stego_lsb.c\n%d %d\n255\n", width, height); //wrting the header

    for(int i=0;i<width*height;i++) {
        fprintf(file,"%d\n", pixels[i]); //writing the pixel data
    }

    fclose(file); //closing file
    return 0; //when carried out succesfully
}

// Function to write a binary PGM file
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height){
    FILE *file=fopen(filename, "wb"); //opening the file in binary write mode
    if(file==NULL){
        return 1; //error opening file
    }

    fprintf(file, "P5\n# Created by stego_lsb.c\n%d %d\n255\n", width, height); //writing the header
    fwrite(pixels, sizeof(unsigned char), width * height, file); //writing the binary pixel data

    fclose(file); //closing file
    return 0; //when carried out succesfully
}

//putting the secret image into the cover image 
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height){
    for(int i=0;i<width*height;i++){ 
        coverPixels[i]=(coverPixels[i]/16)*16+(secretPixels[i]/16); //putting the upper 4 bits of secret in to the cover image
    }
}

//taking the secret image from the stego image 
void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height){
    for(int i=0;i<width*height;i++){
        outputPixels[i]=(coverPixels[i]%16)*16; //after extraction reconstrcution of secret pixel
    }
}