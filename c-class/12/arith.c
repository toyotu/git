#include <stdio.h>
#include <math.h>
#include <string.h>

#define BUFLEN 1024
#define NUMOP 4

//演算子を表す構造体

typedef struct Operation{
  const char *name; //演算子の名前
  int (*func)(int i, int n); //実際の演算を行う関数へのポインタ  
} Operation;

int add(int m, int n){
  return m + n;  
}

int minus(int m, int n){
  return m - n;
}

int mult(int m, int n){
  return m * n;  
}

int my_pow(int m, int n){
  return pow(m, n);
}

Operation *findOperation(Operation *operations, char *name){
  int i;
  for(i = 0; i < NUMOP; i++){
    if(strcmp(operations[i].name, name) == 0){
      return &operations[i];
    }
  }
  return NULL;
}

int main(){
  char buf[BUFLEN];
  char name[BUFLEN];
  int arg1;
  int arg2;

  struct Operation operations[NUMOP];
  Operation *op;

  operations[0].name = "+";
  operations[0].func = add;

  operations[1].name = "*";
  operations[1].func = mult;

  operations[2].name = "-";
  operations[2].func = minus;

  operations[3].name = "^";
  operations[3].func = my_pow;

  while(scanf("%d %s %d", &arg1, name, &arg2 ) == 3){
    op = findOperation(operations, name);
    if(op == NULL){
      printf("Unknown operation: %s\n", name);
    }else{     
      printf("%d\n", (*(op->func))(arg1, arg2));
    }
  }
  return 0;
}
