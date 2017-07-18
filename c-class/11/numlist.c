//////////////////
//////リストソートのプログラム
//////////////////
#include <stdio.h>
#include <stdlib.h>

#define INSERT 1
#define DELETE 2

struct numlist{
  int data;
  struct numlist *nextptr;
};

void traverseList(struct numlist *ptr){
  while(ptr != NULL){
    printf("%d ", ptr->data);
    ptr = ptr->nextptr;
  }
  putchar('\n');
}

void insertList(struct numlist **ptr, int newdata){
  if (*ptr == NULL || (*ptr)->data > newdata){
    struct numlist *newcell;
    newcell = (struct numlist *)malloc(sizeof(struct numlist));
    if(newcell == NULL ){ printf("No memory!\n"); exit(0);}
    newcell->data=newdata;
    newcell->nextptr=*ptr;
    *ptr=newcell;
  }else{
    insertList(&(*ptr)->nextptr, newdata);
  }
}

void deleteList(struct numlist **ptr, int deldata){
  if (*ptr == NULL){ return ;}    
  if ((*ptr)->data == deldata){
    *ptr = (*ptr)->nextptr;
  }
  else{
    deleteList(&(*ptr)->nextptr, deldata);
    free(*ptr);
  }
}

int main(void){
  struct numlist *root = NULL;
  int newdata, deldata;
  int mode = INSERT;

  while(1){
    switch(mode){
    case INSERT:
      while(1){
	printf("Data > ");
	scanf("%d", &newdata);
	if (newdata < 0) break;
	insertList(&root, newdata);
	traverseList(root);
      }
      break;
    case DELETE:
      printf("Input delete number\n");
      scanf("%d", &deldata);
      deleteList(&root, deldata);
      traverseList(root);
      break;
    case 0:
      return 0;
    }
    printf("Input Command(1:Insert, 2:Delete):");
    scanf("%d",&mode);
  }

  return 0;
}
