#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#define TRUE 1
#define FALSE 0

#define RIGHT 0
#define STRAIGHT 1
#define LEFT 2

#define N 0
#define E 1
#define S 2
#define W 3

#define NW 0
#define NE 1
#define SE 2
#define SW 3

#define CAR 20
int gLimit = 0;

/*lane mutexs*/
pthread_mutex_t mutNQ;
pthread_mutex_t mutEQ;
pthread_mutex_t mutSQ;
pthread_mutex_t mutWQ;

/*intersection mutex*/
pthread_mutex_t mutInter[4];

/*limit mutex*/
pthread_mutex_t mutLimit;

/*linked list of each lane*/
struct carQueue{
  int index;
  struct carQueue *next;
};

/*Slot stores info of a car*/
typedef struct Info{
  // int index;
  // int from;
  // int to;
  // int at;
  // int* path;
  pthread_t thread;
}Info;

/* queues */
Info* infoSet[CAR];
struct carQueue* NQueue;
struct carQueue* EQueue;
struct carQueue* SQueue;
struct carQueue* WQueue;

/* my modulo */
int mod(int a, int b){
  int re = a%b;
  if(re<0)re+=b;
  return re;
}

/* translators */
char* dir_trans(int dir){
  switch (dir) {
    case 0: return "RIGHT";
    case 1: return "STRAIGHT";
    case 2: return "LEFT";
    default: return "NOT FOUND";
  }
}
char* from_to_trans(int ft){
  switch (ft) {
    case 0: return "NORTH";
    case 1: return "EAST";
    case 2: return "SOUTH";
    case 3: return "WEST";
    default: return "NOT FOUND";
  }
}
char* at_trans(int at){
  switch (at) {
    case 0: return "NW";
    case 1: return "NE";
    case 2: return "SE";
    case 3: return "SW";
    default: return "NOT FOUND";
  }
}

/* Mixed Queue */
void enqueue(int ind, int from){
  int flag = TRUE;
  struct carQueue* tempHead;

  switch (from) {
    case 0:
      pthread_mutex_lock(&mutNQ);
      tempHead = NQueue;
      if(NQueue==NULL){
        NQueue = malloc(sizeof(struct carQueue));
        NQueue->index = ind;
        NQueue->next = NULL;
        flag = FALSE;
      }
      break;
    case 1:
      pthread_mutex_lock(&mutEQ);
      tempHead = EQueue;
      if(EQueue==NULL){
        EQueue = malloc(sizeof(struct carQueue));
        EQueue->index = ind;
        EQueue->next = NULL;
        flag = FALSE;
      }
      break;
    case 2:
      pthread_mutex_lock(&mutSQ);
      tempHead = SQueue;
      if(SQueue==NULL){
        SQueue = malloc(sizeof(struct carQueue));
        SQueue->index = ind;
        SQueue->next = NULL;
        flag = FALSE;
      }
      break;
    case 3:
      pthread_mutex_lock(&mutWQ);
      tempHead = WQueue;
      if(WQueue==NULL){
        WQueue = malloc(sizeof(struct carQueue));
        WQueue->index = ind;
        WQueue->next = NULL;
        flag = FALSE;
      }
      break;
    default:
      printf("ERROR: queue not found\n");
      return;
  }

  if(flag){
    struct carQueue* new_node = malloc(sizeof(struct carQueue)); //allocate spaces
    new_node->index = ind;
    new_node->next = NULL;

    while(tempHead->next!=NULL){
      tempHead = tempHead->next;
    }//find tail

    tempHead->next = new_node;
  }

  switch (from) {
    case 0:
      pthread_mutex_unlock(&mutNQ);
      break;
    case 1:
      pthread_mutex_unlock(&mutEQ);
      break;
    case 2:
      pthread_mutex_unlock(&mutSQ);
      break;
    case 3:
      pthread_mutex_unlock(&mutWQ);
      break;
    default:
      printf("ERROR: queue not found\n");
      return;
  }
}
void dequeue(int ind, int from){
  int flag = FALSE;
  struct carQueue* tempHead;
  struct carQueue* prevNode;
  struct carQueue* nextNode;

  switch (from) {
    case 0:
      pthread_mutex_lock(&mutNQ);
      tempHead = NQueue;
      break;
    case 1:
      pthread_mutex_lock(&mutEQ);
      tempHead = EQueue;
      break;
    case 2:
      pthread_mutex_lock(&mutSQ);
      tempHead = SQueue;
      break;
    case 3:
      pthread_mutex_lock(&mutWQ);
      tempHead = WQueue;
      break;
    default:
      printf("ERROR: queue not found\n");
      return;
  }

  prevNode = NULL;

  while(tempHead!=NULL && tempHead->index!=ind){
    prevNode = tempHead;
    tempHead = tempHead->next;
    // printf("-");
  }//find target
  // printf("!\n");
  if(tempHead != NULL) {
    nextNode = tempHead->next;
    free(tempHead);
    if(prevNode != NULL){prevNode->next = nextNode;}
    else{flag = TRUE;}
  }else{printf("FAILED: dequeue target not found\n");}

  switch (from) {
    case 0:
      if(flag){NQueue = nextNode;}
      pthread_mutex_unlock(&mutNQ);
      break;
    case 1:
      if(flag){EQueue = nextNode;}
      pthread_mutex_unlock(&mutEQ);
      break;
    case 2:
      if(flag){SQueue = nextNode;}
      pthread_mutex_unlock(&mutSQ);
      break;
    case 3:
      if(flag){WQueue = nextNode;}
      pthread_mutex_unlock(&mutWQ);
      break;
    default:
      printf("ERROR: queue not found\n");
      return;
  }
}
void queue_display(struct carQueue* tempHead){
  if(tempHead == NULL) printf("EMPTY\n");

  while(tempHead!=NULL){
    printf("[ID:%d]->", tempHead->index);
    tempHead = tempHead->next;
  }//find tail

  printf("\n");
}
void destoryPN(struct carQueue* Head){
  struct carQueue* tempHead;
  while(Head!=NULL){
    tempHead = Head;//store queue head
    free(Head);
    Head = tempHead->next;
  }//find tail
}
int is_queue_head(int ind, int from){
  int flag = FALSE;
  switch (from) {
    case 0:
      pthread_mutex_lock(&mutNQ);
      if(NQueue!=NULL&&NQueue->index == ind) {flag = TRUE;}
      pthread_mutex_unlock(&mutNQ);
      break;
    case 1:
      pthread_mutex_lock(&mutEQ);
      if(EQueue!=NULL&&EQueue->index == ind) {flag = TRUE;}
      pthread_mutex_unlock(&mutEQ);
      break;
    case 2:
      pthread_mutex_lock(&mutSQ);
      if(SQueue!=NULL&&SQueue->index == ind) {flag = TRUE;}
      pthread_mutex_unlock(&mutSQ);
      break;
    case 3:
      pthread_mutex_lock(&mutWQ);
      if(WQueue!=NULL&&WQueue->index == ind) {flag = TRUE;}
      pthread_mutex_unlock(&mutWQ);
      break;
    default:
      printf("ERROR: queue not found\n");
  }
  return flag;
}

// int* path_generator(int from, int dir){
//   int* path;
//   return path;
// }

/* ==== car routine ==== */
void *car_routine(void* index){
  //create info
  int ind = (int) index;
  int from = mod(rand(), 4); //N,E,S,W
  int dir = mod(rand(), 3); //R,S,L
  int at = from;
  int to = mod((from-dir-1), 4);

  //random arrival time
  sleep((unsigned int) mod(rand(), 3)); //time of arrival

  //WAITING...
  enqueue(ind, from); //in line
  printf("Car %d do %s from %s to %s: waiting in queue\n",
    ind, dir_trans(dir), from_to_trans(from), from_to_trans(to));

  /*must be queue head;
    can enter 4 cars if at least one does RIGHT turn;
    or enter at most 3 cars if all do STRAIGHT/LEFT turns*/
  while(TRUE){
    pthread_mutex_lock(&mutLimit);
    if(is_queue_head(ind, from) && ((dir==0 && gLimit<4)||(dir>0 && gLimit<3))) {
      printf("Car %d do %s from %s to %s: entering intersection\n",
        ind, dir_trans(dir), from_to_trans(from), from_to_trans(to));
      gLimit++;
      pthread_mutex_unlock(&mutLimit);
      break;
    }
    pthread_mutex_unlock(&mutLimit);
  }

  //ENTERED!!!
  pthread_mutex_lock(&mutInter[at]);

  printf("Entered:Car %d: at %s\n", ind, at_trans(at)); //sucessfully moved!!

  dequeue(ind, from);

  sleep(rand()%4);

  //for S and L
  while(mod(at-1, 4) != to){
    pthread_mutex_lock(&mutInter[mod(at-1, 4)]); // try to next postion
    printf("\tCar %d: at %s\n", ind, at_trans(mod(at-1, 4))); //sucessfully moved!!
    pthread_mutex_unlock(&mutInter[mod(at, 4)]); //release previous position
    printf("\tCar %d: freed %s\n", ind, at_trans(mod(at, 4)));
    at--;
    sleep(rand()%4);
  }

  //LEAVING!!!
  pthread_mutex_unlock(&mutInter[mod(at,4)]);//release previous position
  printf("\tCar %d: freed %s\n", ind, at_trans(mod(at, 4)));

  pthread_mutex_lock(&mutLimit);
  gLimit--;
  pthread_mutex_unlock(&mutLimit);

  printf("Car %d do %s from %s to %s: left intersection\n",
    ind, dir_trans(dir), from_to_trans(from), from_to_trans(to));

  //80% chance to come back
  if(rand()%5 != 0){
    printf("Car %d: just kidding, imma commin back\n", ind);
    car_routine(index);
  }

  pthread_exit(NULL);
}

/*create random arrivals */
int main(int argc, char *argv[]){

  //allocate space for infoSet
  for(int i = 0; i < CAR; i++){infoSet[i] = malloc(sizeof(Info));}

  //initialize mutex
  for(int i = 0; i < 4; i++){pthread_mutex_init(&mutInter[i] , NULL);}
  pthread_mutex_init(&mutNQ , NULL);
  pthread_mutex_init(&mutEQ , NULL);
  pthread_mutex_init(&mutSQ , NULL);
  pthread_mutex_init(&mutWQ , NULL);
  pthread_mutex_init(&mutLimit , NULL);

  srand(time(NULL));

  //create car
  for(int i = 0; i < CAR; i++){
    if((int)pthread_create(&infoSet[i]->thread, NULL, car_routine,  (void *)i )){
      printf("ERROR in creating car: %d\n", i);
      exit(-1);
    }
  }

  //join car
  for(int i = 0; i < CAR; i++){
    if((int)pthread_join(infoSet[i]->thread, NULL)){
      printf("ERROR in joining car: %d\n", i);
      exit(-1);
    }
  }

  //free queues
  for(int i = 0; i < CAR; i++){free(infoSet[i]); }

  printf("MA DRIVERS ALL SURVUVED!!!\n");
  exit(FALSE);
  return FALSE;
}
