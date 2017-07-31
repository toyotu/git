#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define STRSIZE 1024
#define BUFSIZE 1024

int main(int argc, char *argv[]){
  char buf[BUFSIZE];
  FILE *fp;
  if(argc != 3){
    fprintf(stderr, "Usage: ./%s [filename] [string]\n", argv[0]);
  }
  
  if((fp = fopen(argv[1], "r")) == NULL){
    fprintf(stderr, "Error! Can't open the file %s \n", argv[1]);
  }

  while(fgets(buf, BUFSIZE, fp) != NULL ){
    if(strstr(buf, argv[2]) != NULL){
    printf("%s", buf);
    }
  }
  fclose(fp);
  return 0;
}
