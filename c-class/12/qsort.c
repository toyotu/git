#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN1 10
#define LEN2 5

int ncomp(const void *m, const void *n){
  return *(int * ) m - *(int *) n;
}

//ポインタで比較する
int scomp1(const void *s1, const void *s2){
  return strcmp(*(char **) s1, *(char **) s2);
}

//文字列で比較する
int scomp2(const void* s1, const void* s2) {
  return strcmp((char *) s1, (char *) s2 );
}

int main(void){
  int numbers[LEN1] = {3, -1, 5, 8, 2, 2, -10, 8, 4, 9};
  char *strings1[LEN2] = {"tama", "pochi", "taro", "saburo", "akira"};
  char strings2[LEN2][1000] = {"tama", "pochi", "taro", "saburo", "akira"};
  int i;


  qsort(numbers, LEN1, sizeof(int), ncomp);
  for(i = 0; i < LEN1; i++){
    printf("%d ", numbers[i]);
  }

  qsort(strings1, LEN2, sizeof(*strings1), scomp1);
  for(i = 0; i < LEN2; i++){
    printf(" %s", strings1[i]);
  }
  printf("\n");
  for(i = 0; i < LEN2; i++){
    printf(" %s", strings2[i]);
  }

  qsort(strings2, LEN2, sizeof(strings2[0]), scomp2);
  for(i = 0; i < LEN2; i++){
    printf(" %s", strings2[i]);
  }

  
  return 0;
}

/* int compare_char(const void* left, const void* right) { */
/*     char *left_char = (char *)left; */
/*     char *right_char = (char *)right; */

/*     return strcmp( left_char, right_char ); */
/* } */

/* int main() { */

/*     int i=0; */
/*     char table[10][20] = {  */
/*         "123", */
/*         "A0", */
/*         "Z0", */
/*         "9w3" */
/*     }; */

/*     for(i=0; i<10; i++) { */
/*         printf("%d %s\n", i, table[i]); */
/*     } */

/*     printf(" --- sort\n"); */

/*     qsort(table, 4, sizeof(table[0]), compare_char); */

/*     for(i=0; i<10; i++) { */
/*         printf("%d %s\n", i, table[i]); */
/*     } */
/* } */
