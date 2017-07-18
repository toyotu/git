#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define STRLEN 32
#define BUF_LEN 128
#define CITYDB_R_FILE "../11/cities.txt"
#define CITYDB_W_FILE "citiesDB.txt"

struct cities {
  char pref[STRLEN];
  char city[STRLEN];
  int popl;
  float area;
  float dens;
  char founded[STRLEN];
  struct cities *next;
};

struct cities *root = NULL;

void *mymalloc(size_t sz){
  void *p = (void *) malloc(sz);
  if(p == NULL){
    fprintf(stderr, "ERROR: Can't malloc memory %d bytes.", (int) sz);
    exit(1);
  }
  memset(p, 0, sz);  
  return p; 
}

void showCity(struct cities *c, FILE *fp){
  fprintf(fp, "%s\t%s\t%d\t%.2f\t%.2f\t%s\n",
	  c->pref, c->city, c->popl, c->area, c->dens, c->founded);
}

struct cities *genNewCityCell(char *pref, char *city, int popl, float area, float dens, char *founded){
  struct cities *c;
  c = mymalloc(sizeof(struct cities));
  strcpy(c->pref, pref);
  strcpy(c->city, city);
  strcpy(c->founded, founded);
  c->popl = popl;
  c->area = area;
  c->dens = dens;
  
  return c;
}

void showList(FILE *fp){
  struct cities *cur = root;
  while(cur != NULL){
    showCity(cur, fp);
    cur = cur->next;
  }
}

struct cities *line2City(char *buf){
  struct cities *c = NULL; 
  int popl;
  char pref[STRLEN], city[STRLEN], founded[STRLEN];
  float area, dens;
  sscanf(buf, "%s %s %d %f %f %s", pref, city, &popl, &area, &dens, founded);
  c = genNewCityCell(pref, city, popl, area, dens, founded);
  return c;
}

void addCity(struct cities *c){
  struct cities **cur;
  cur = & root;
  while(*cur != NULL){
    cur = &((*cur)->next);
  }
  //cur = &c; これはダメ
  *cur = c;
}

void inputStr(char *buf, int len){
  if (fgets(buf, len, stdin ) != NULL){
    buf[strlen(buf)-1] = '\0';    
  }else{
    exit(1);
  }
}

void addNewCity(){
  char buf[BUF_LEN] = "";
  struct cities *c;  

  while(strlen(buf) <= 0 ){
    printf("Input ont line \n");
    inputStr(buf, BUF_LEN);
  }
  if(strncmp(buf, "Prefeacture", 10) == 0)return;
  c = line2City(buf);
  addCity(c);
}

void deleteCityFromList(char *delcity){
  struct cities **cur;
  struct cities *del;
  cur = & root;
  while(*cur != NULL){
    if(strcmp((*cur)->city, delcity) == 0){
      printf("%s\n", delcity);
      del = *cur;
      *cur = (*cur)->next;
      free(del);
    }
    cur = &((*cur)->next);
  }
}

void deleteCity(){
  char buf[STRLEN];

  showList(stdout);

  printf("Select Delete City Name:");
  inputStr(buf, STRLEN);
  deleteCityFromList(buf);
}

void readListFILE(FILE *fp){
  char buf[BUF_LEN];
  struct cities *c;
  while(fgets(buf, BUF_LEN, fp) != NULL){
    if(strncmp(buf, "Prefecture", 10) == 0) continue;
    c = line2City(buf);
    if(c != NULL) addCity(c);
  }
}

void readList(){
  FILE *fp;
  if((fp = fopen(CITYDB_R_FILE, "r")) == NULL){
    fprintf(stderr, "Can't open the file %s", CITYDB_R_FILE);
  }
  readListFILE(fp);
  fclose(fp);
}

void saveList(){
  FILE *fp;
  if((fp = fopen(CITYDB_W_FILE, "w")) == NULL){
    fprintf(stderr, "Can't open the file %s", CITYDB_W_FILE);
    exit(1);
  }
  readListFILE(fp);
  fclose(fp);  
}

int poplAscend(struct cities *p, struct cities *q){  
  return p->popl - q->popl;
}

int poplDescend(struct cities *p, struct cities *q){  
  return q->popl - p->popl;
}

int areaAscend(struct cities *p, struct cities *q){  
  return p->area - q->area;
}

int areaDescend(struct cities *p, struct cities *q){  
  return q->area - p->area;
}

int denseAscend(struct cities *p, struct cities *q){  
  return p->dens - q->dens;
}

int denseDescend(struct cities *p, struct cities *q){  
  return q->dens - p->dens;
}

int foundedAscend(struct cities *p, struct cities *q){  
  return strcmp(p->founded, q->founded);
}

int foundedDescend(struct cities *p, struct cities *q){  
  return strcmp(q->founded, p->founded);
}

//一般化したもの
void insertCityBycomp(struct cities *sc, int (*comp)(struct cities *, struct cities *)){
  struct cities **cur = &root;
  struct cities *tmp;
  while(1){
    if(*cur == NULL){ *cur = sc; break;} //末尾にきた場合
    else if(comp(*cur, sc) > 0 ){
      tmp = *cur; //挿入する後方にある構造体へのポインタ
      *cur = sc;
      (*cur)->next = tmp;
      return ;
    }
    else{
      cur = &((*cur)->next); //次に進める
    }
  }
}

void insertCityByPopl(struct cities *sc){
  struct cities **cur = &root;
  struct cities *tmp;
  while(1){
    if(*cur == NULL){ *cur = sc; break;} //末尾にきた場合
    else if((*cur)->popl > sc->popl){
      tmp = *cur; //挿入する後方にある構造体へのポインタ
      *cur = sc;
      (*cur)->next = tmp;
      return ;
    }
    else{
      cur = &((*cur)->next); //次に進める
    }
  }
}

void sortList(int (*comp)(struct cities *, struct cities *)){
  struct cities *cur, *fr;
  cur = root;
  root = NULL;

  while(cur != NULL){
    insertCityBycomp(genNewCityCell(cur->pref, cur->city, cur->popl, cur->area, cur->dens, cur->founded), comp);
    fr = cur;
    cur = cur->next;
    free(fr);
  }
  showList(stdout);
}

int main(int argc, char *argv[]){
  char buf[BUF_LEN];
  char opt;
  char bin;

  while(1){
    printf("Menu(a:add, d:delete, l:list, s:sort, w:write file, r:read file ,q:quit)\n");
    if(fgets(buf, BUF_LEN, stdin) != NULL){
      switch(buf[0]){
      case 'a':
	addNewCity();
	break;
      case 'd':
	deleteCity();
	break;
      case 'l':      
	showList(stdout);
	break;
      case 's':
	printf("Key? (p: 人口, a:面積, d:人口密度, f:設立年月日)\n ");
	opt = getchar();
	rewind(stdin);
	printf("Order? (a: 昇順, b:降順) \n");
	bin = getchar();
	switch(opt){
	case 'p':
	  if(bin == 'a'){sortList(poplAscend);}
	  else{sortList(poplDescend);}
	    break;
	case 'a':
	  if(bin == 'a'){sortList(areaAscend);}
	  else{sortList(areaDescend);}
	  break;
	case 'd':
	  if(bin == 'a'){sortList(denseAscend);}
	  else{sortList(denseDescend);}
	  break;
	case 'f':
	  if(bin == 'a'){sortList(foundedAscend);}
	  else{sortList(foundedDescend);}
	  break;
	default:
	  printf("Invalid option \n");
	}
	break;
      case 'w':
	saveList();
	break;
      case 'r':
	readList();
	break;
      case 'q':
	exit(0);
	break;          
      }
    }else{
      break;
    }
  }
  return 0;
}
