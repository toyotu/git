#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define STRSIZE 512
#define BUFSIZE 1024
#define DATSIZE 1024

struct rain{
  char city[STRSIZE];
  float Jan,Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec;
};

int data_size = 0;
struct rain rda[DATSIZE];

int main(int argc, char *argv[]){

  int i = 0;
  int flag = -1;
  int index = 0;
  int max = 0;
  char month[STRSIZE];
  float a,b,c,d,e,f,g,h,j,k,l,m,n;
  if(argc != 3){
    fprintf(stderr, "Usage: %s [option] [month]", argv[0]);
    exit(-1);
  }

  if(strcmp(argv[1], "-M") == 0){flag = 1;}
  else if(strcmp(argv[1], "-m") == 0){flag = -1;}
  else{fprintf(stderr, "Illegal option %s", argv[1]); exit(1);};
  
  while (fscanf(stdin, "%s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", rda[i].city, &(rda[i].Jan), &(rda[i].Feb), &(rda[i].Mar), &(rda[i].Apr), &(rda[i].May), &(rda[i].Jun),  &(rda[i].Jul), &(rda[i].Aug), &(rda[i].Sep), &(rda[i].Oct), &(rda[i].Nov), &(rda[i].Dec)) != EOF){
    i++;
  }
  data_size = i;

  for(i = 0; i < data_size; i++){
    if(flag * rda[i]. > max){ index = i; max = rda[i]}
  }
  
  return 0;
}
