#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define STRSIZE 1024

int main(int argc, char *argv[]){
  int opt;
  char c;
  int i;
  char str[STRSIZE];
  char str1[STRSIZE];
  char str2[STRSIZE];
  /* char *str1; */
  /* char *str2; <- この宣言でstrcatをするとエラーになるので注意*/ 
  char *str3;
  int flag = 0;
  while ((opt = getopt(argc, argv, "a:c:r:")) != -1) {
    switch (opt) {
    case 'a':
      strcpy(str1, optarg);
      strcpy(str2, argv[optind]);
      strcat(str1, str2);
      flag = 1;
      break;
    case 'c':
      i = 0;
      while(1){
	if(optarg[i] == '\0'){break;};
	printf("%c", toupper(optarg[i]));
	i++;
      }
      printf("\n");
      flag = 1;
      break;
    case 'r':
      i = 0;
      while(1){
	if(optarg[i] == '\0'){break;};
	i++;
      }
      for(;i >= 0; i--){printf("%c", optarg[i]);}
      printf("\n");
      flag = 1;
      break;
    default:
      printf("ERROR!");
      return 1;
    }
  }
  if(flag == 0){printf("ERROR!");}
  return 0;
}
