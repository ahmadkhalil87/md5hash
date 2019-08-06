// You can use these includes and defines as a hint what functions you might want to use ;)
#define _GNU_SOURCE
#include "md5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define HASH_LENGTH 16

int main (){
	unsigned char target[HASH_LENGTH] = {0xe6, 0x54, 0x93, 0xcc, 0xde, 0xe9, 0xc4, 0x51, 0x4f, 0xe2, 0x0e, 0x04, 0x04, 0xf3, 0xbc, 0xb8}; // "threads help" - to be used for les_miserables.txt
	//unsigned char target[HASH_LENGTH] = {0x1c, 0x01, 0x43, 0xc6, 0xac, 0x75, 0x05, 0xc8, 0x86, 0x6d, 0x10, 0x27, 0x0a, 0x48, 0x0d, 0xec}; // "Les Miserables" - to be used for les_miserables_preface.txt
	time_t start = time(NULL);
	FILE* f;

	//f = fopen("les_miserables_preface.txt", "r");
    f = fopen("les_miserables.txt", "r");
	if(f == NULL){
		return 1;
	}
	const char* delim = "\n\r\t .,;-:0123456789?!\"*+()|&[]#$/%%'";

    // find the file size
    fseek(f, 0L, SEEK_END);
    long filesize = ftell(f);
    rewind(f);
    char * array = malloc(sizeof(char)*filesize);
    // read the file in lines und will be puted in the array
    char buf[128];
    while(!feof(f)){
        fgets(buf, 128, f);
        strcat(array,buf);
    }
    long n=0;
    char * array2 = malloc(sizeof(char)*filesize);
    sprintf(array2,array);
    // counter for all words in the file
    char * count;
    count = strtok(array, delim);
    do{
        n++;
        count = strtok(NULL, delim);
    } while (count!=NULL);
    // put all words in token array and delete the duplicate
    if (n > 64000) n /= 4;
    char * tok = strtok(array2, delim);
    char token[n][16];
    long i = 0;
    do{
        for (int j = 0; j<i;j++){
            if (strcmp(token[j],tok) == 0){
                tok = strtok(NULL, delim);
                if (tok!=NULL) continue;
                else break;
            }
        }
        sprintf(token[i],tok);
        i++;
        tok = strtok(NULL, delim);
    } while (tok!=NULL);
    n=i-1;
    char buffer[1024];
    unsigned char hash[HASH_LENGTH] = {0};
    MD5_CTX md5_ctx;
    char res[1024];
    int b = 0;
//#pragma omp parallel
//#pragma cancel for
    // put something into buffer to hash it
    for (int i=0; i<n; i++){
        //if (i%100 == 0) printf("\n%d%%",i);
        for (int j=0; j<n; j++){
            sprintf(buffer,"%s %s", token[i],token[j]);
            MD5_Init(&md5_ctx);
            MD5_Update(&md5_ctx, buffer, strlen(buffer));
            MD5_Final(hash, &md5_ctx);
            if (memcmp(hash, target,16) == 0){
                sprintf(res,buffer);
                printf("\nDone\n");
                b=1;
                break;
            }
        }
        if (b) break;
    }
    printf("\nthe Word: %s\n",res);
	time_t end = time(NULL);
	printf("\nExecution took ~%fs\n", difftime(end, start));
	return 0;
}
