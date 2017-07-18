#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRLEN   32
#define BUF_LEN 128
#define CITYDB_R_FILE "cities.txt" // 読み込み用ファイル
#define CITYDB_W_FILE "citiesDB.txt" // 書き出し用ファイル

struct cities{
  char pref[STRLEN];
  char city[STRLEN];
  int popl;
  float area;
  float dens;
  char founded[STRLEN];
  struct cities *next;
};

struct cities *root=NULL; // rootへのポインタは大域変数として定義

void *mymalloc(size_t sz){
  void *p = (void *)malloc(sz);
  if(p == NULL){
    fprintf(stderr,"ERR: Can't malloc memory %d bytes.",(int)sz);
    exit(1);
  }
  memset(p,0,sz);
  return p;
}

void showCity(struct cities *c, FILE *fp){
  fprintf(fp,"%s\t%s\t%d\t%.2f\t%.2f\t%s\n",
          c->pref, c->city, c->popl, c->area, c->dens, c->founded);
}


struct cities *genNewCityCell(char *pref, char *city, int popl, float area, float dens, char *founded ){
  struct cities *c;
  c = mymalloc(sizeof(struct cities));
  strcpy(c->pref, pref);
  strcpy(c->city, city);
  c->popl = popl;
  c->area = area;
  c->dens = dens;
  strcpy(c->founded, founded);  
  return c;
}

void showList(FILE *fp){
  struct cities *cur= root;
  while(cur != NULL){
    showCity(cur,fp);
    cur = cur->next;
  }
}

void saveList(){
  FILE *fp;
  if ((fp = fopen(CITYDB_W_FILE, "w")) == NULL){
    fprintf(stderr, "Can't open the file:%s", CITYDB_W_FILE);
    exit(-1);
  }      
  showList(fp);
  fclose(fp);
  printf("File saved to %s\n",CITYDB_W_FILE); 
}

struct cities *line2City(char *buf){
  struct cities *c=NULL;
  int popl;
  char pref[STRLEN], city[STRLEN], founded[STRLEN];
  float area, dens;

  sscanf(buf, "%s %s %d %f %f %s", pref, city, &popl, &area, &dens, founded);
  /* sscanf()の代わりに strtok() を使う別解部分
    ただしその際は宣言を，char *pref, *city, *founded とすること．
  pref = strtok(buf,"\t");
  city = strtok(NULL,"\t");
  popl = atoi(strtok(NULL,"\t"));
  area = atof(strtok(NULL,"\t"));
  dens = atof(strtok(NULL,"\t"));
  founded = strtok(NULL,"\t\n");
   */

  c = genNewCityCell(pref,city,popl,area,dens,founded);
  return c;
}

void addCity(struct cities *c){
  struct cities **cur= &root;
  while(*cur != NULL){
    cur = &((*cur)->next);
  }
  *cur = c;
  return ;
}

void inputStr(char *buf,int len){
  fgets(buf,len,stdin);
  buf[strlen(buf)-1]='\0';
}

void addNewCity(){
  char buf[BUF_LEN] = "";
  struct cities *c;

  while(strlen(buf)<=0){
    printf("Input one line:\n");
    inputStr(buf, BUF_LEN);
  }
  if(strncmp(buf, "Prefecture", 10) == 0) return;
  
  c = line2City(buf);
  addCity(c);
}

void deleteCityFromList(char *delcity){
  struct cities **cur = &root;
  if (*cur == NULL){ return ;}
  while (strcmp((*cur)->city, delcity) != 0){
    cur = &((*cur)->next);
  }
  *cur=((*cur)->next);
  free(*cur);
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
  while( fgets(buf,BUF_LEN,fp) != NULL){
    if(strncmp(buf, "Prefecture", 10) == 0) continue;
    c = line2City(buf);
    if(c != NULL) addCity(c);
  }
}


void readList(){
  FILE *fp;
  if((fp = fopen(CITYDB_R_FILE, "r")) == NULL){
    fprintf(stderr, "Can't open file\n");
    exit(-1);
  }

  readListFILE(fp);
  fclose(fp);
  printf("File loaded\n");
}

void insertCityByPopl(struct cities *sc){
  struct cities **cur= &root;
  struct cities **tmp;
  while ((*cur != NULL) && (*cur)->popl > sc->popl){
    cur = &((*cur)->next);
  }

  if (*cur == NULL){
    *cur = sc;
  }else{
    tmp = &((*cur)->next);
    *cur = sc;
    (*cur)->next = *tmp;
  } 
}


void sortList() {
  struct cities *cur, *fr;
  cur = root;
  root = NULL; // この時点で root は空リストになる
  while (cur != NULL) { // ひとつづつ root が指すリストへコピーして挿入
    insertCityByPopl(genNewCityCell(cur->pref, cur->city, cur->popl, cur->area, cur->dens, cur->founded));
    fr = cur; // 現在のポインタをセルの削除(free)のために一時保存
    cur = cur->next; // cur を一つ先へ進める
    free(fr); // セルを削除
  }
  
  showList(stdout);
}

int main(int argc, char *argv[]){
  char buf[BUF_LEN];

  while(1){
    printf("Menu(a:add, d:delete, l:list, s:sort, w:write file, r:read file q:quit):\n");
    
    fgets(buf,BUF_LEN,stdin);
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
      sortList();
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
  }
}


