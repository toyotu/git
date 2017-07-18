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
};

struct cities *root = NULL;
struct cities all_cities[1000];
int c_count = 0; //citiesの数

void *mymalloc(size_t sz){
  void *p = (void *) malloc(sz);
  if(p == NULL){
    fprintf(stderr, "ERROR: Can't malloc memory %d bytes.", (int) sz);
    exit(1);
  }
  memset(p, 0, sz);
  return p;
}

void showCity(struct cities c, FILE *fp){
  fprintf(fp, "%s\t%s\t%d\t%.2f\t%.2f\t%s\n",
	  c.pref, c.city, c.popl, c.area, c.dens, c.founded);
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
  int i;
  for(i = 0; i < c_count; i++){
    showCity(all_cities[i], fp);
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
  all_cities[c_count] = *c;
  c_count += 1;
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
  int i, j;
  int flag = 0; 
  for(i = 0; i < c_count; i++){
    if(strcmp(all_cities[i].city, delcity) == 0){
      flag = 1;
      break;
    }
  }
  if(flag == 0){
    printf("The city name %s is not found in the list", delcity);
    return;
  }
  for(j = i; j < c_count - 1; j++){
    all_cities[j] = all_cities[j+1];
  }
  c_count--;
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

int poplAscend(const void* c1, const void *c2){
  return (*(struct cities *) c1).popl - (*(struct cities *) c2).popl;
}

int poplDescend(const void* c1, const void *c2){
  return (*(struct cities *) c2).popl - (*(struct cities *) c1).popl;
}

int areaAscend(const void* c1, const void *c2){
  return (*(struct cities *) c1).area - (*(struct cities *) c2).area;
}

int areaDescend(const void* c1, const void *c2){
  return (*(struct cities *) c2).area - (*(struct cities *) c1).area;
}

int denseAscend(const void* c1, const void *c2){
  return (*(struct cities *) c1).dens - (*(struct cities *) c2).dens;
}

int denseDescend(const void* c1, const void *c2){
  return (*(struct cities *) c2).dens - (*(struct cities *) c1).dens;
}

int foundedAscend(const void* c1, const void *c2){
  return strcmp((*(struct cities *) c1).founded, (*(struct cities *) c2).founded);
}

int foundedDescend(const void* c1, const void *c2){
  return strcmp((*(struct cities *) c2).founded, (*(struct cities *) c1).founded);
}

void sortList(int (*comp)(const void* c1, const void *c2)){

  qsort(all_cities, c_count, sizeof(struct cities), comp);  
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
