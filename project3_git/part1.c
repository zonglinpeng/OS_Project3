/**
*Author: Zonglin Peng
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#define TRUE 1
#define FALSE 0
#define MAXNINJIA 50
#define MAXPIRATE 50
int gTeamNumber = 4;
int gIsNinjia = TRUE;
int total_n, total_p;
/*broadcast screen for sem_wait*/
int* broadcast;

// sem_t declarations;
sem_t semNin, semPir;
sem_t semIsNinjia;
sem_t semMixedQueue;
sem_t semBroadcast;

/*linked list of queues including pirates and ninjias*/
struct NPQueue{
  int is_ninjia;
  int thread_index;
  struct NPQueue *next;
};

/*Slot stores info of a person*/
typedef struct Info{
  int outfit_time;
  long int wait_time;
  int cost;
  pthread_t thread;
  struct timeval start_wait;
  struct timeval end_wait;
}Info;

/* queues for N and P */
Info* queueNin[MAXNINJIA];
Info* queuePir[MAXPIRATE];
struct NPQueue* mixedQueue; //for sem_post

/* Mixed Queue */
void enqueuePN(int ind, int isN){
  sem_wait(&semMixedQueue);
  struct NPQueue* tempHead = mixedQueue;
  //store queue head
  if(mixedQueue==NULL){
    mixedQueue = malloc(sizeof(struct NPQueue));
    mixedQueue->is_ninjia = isN;
    mixedQueue->thread_index = ind;
    mixedQueue->next = NULL;
  }
  else{
    struct NPQueue* new_node = malloc(sizeof(struct NPQueue)); //allocate spaces
    new_node->is_ninjia = isN;
    new_node->thread_index = ind;
    new_node->next = NULL;

    while(tempHead->next!=NULL){
      tempHead = tempHead->next;
    }//find tail
    tempHead->next = new_node;

  }
  sem_post(&semMixedQueue);
  // printf("%d: enqueued %d!!!\n", isN, ind);
}
void dequeuePN(int ind){
  struct NPQueue* tempHead;
  struct NPQueue* prevNode;
  struct NPQueue* nextNode;

  sem_wait(&semMixedQueue);

  tempHead = mixedQueue;//store queue head
  prevNode = NULL;

  while(tempHead!=NULL && tempHead->thread_index!=ind){
    prevNode = tempHead;
    tempHead = tempHead->next;
    // printf("-");
  }//find target
  // printf("!\n");
  if(tempHead == NULL) {
    sem_post(&semMixedQueue);
    return;
  }
  nextNode = tempHead->next;
  free(tempHead);
  if(prevNode != NULL){prevNode->next = nextNode;}
  else{mixedQueue = nextNode;}
  sem_post(&semMixedQueue);
}
int queue_length(){
  sem_wait(&semMixedQueue);
  int counter = 0;
  struct NPQueue* tempHead;
  tempHead = mixedQueue;//store queue head
  while(tempHead!=NULL){
    counter++;
    tempHead = tempHead->next;
  }//find tail
  sem_post(&semMixedQueue);

  return counter;
}
void queue_display(){
  sem_wait(&semMixedQueue);
  if(mixedQueue == NULL) printf("EMPTY\n");
  struct NPQueue* tempHead;
  tempHead = mixedQueue;//store queue head
  while(tempHead!=NULL){
    printf("[ID:%d,%d]->", tempHead->thread_index, tempHead->is_ninjia);
    tempHead = tempHead->next;
  }//find tail
  sem_post(&semMixedQueue);

  printf("\n");
}
void destoryPN(){
  sem_wait(&semMixedQueue);
  struct NPQueue* tempHead;
  while(mixedQueue!=NULL){
    tempHead = mixedQueue;//store queue head
    free(mixedQueue);
    mixedQueue = tempHead->next;
  }//find tail
  sem_post(&semMixedQueue);
}

/* Broadcast */
int add_broadcast(int ind){
  int flag = FALSE;
  // sem_wait(&semBroadcast);
  for(int i = 0; i < gTeamNumber; i++){
    if(broadcast[i]==-1) {
      broadcast[i] = ind;
      flag = TRUE;
      break;
    }
  }
  // sem_post(&semBroadcast);
  return flag;
}
int remove_broadcast(int ind){
  int flag = FALSE;
  // sem_wait(&semBroadcast);
  for(int i = 0; i < gTeamNumber; i++){
    if(broadcast[i]==ind) {
      broadcast[i] = -1;
      flag = TRUE;
      break;
    }//set to default
  }
  // sem_post(&semBroadcast);
  return flag;
}
void prinf_broadcast(){
  printf("\nbroadcast:[");
  for(int i = 0; i < gTeamNumber; i++){
    printf("%d,", broadcast[i]);
  }
  printf("]\n");

}
int is_nominated(int ind){
  // sem_wait(&semBroadcast);
  int flag = FALSE;
  for(int i = 0; i < gTeamNumber; i++){
    if(broadcast[i] == ind) flag = TRUE;
  }
  // sem_post(&semBroadcast);
  return flag;
}
int is_evacuated(){
  // sem_wait(&semBroadcast);
  int flag = TRUE;
  for(int i = 0; i < gTeamNumber; i++){
    if(broadcast[i] != -1) {flag = FALSE;}
  }
  // sem_post(&semBroadcast);
  return flag;
}
void broadcast_manager(int ind){
  int deq_ind = 0;
  int deq_list [gTeamNumber];
  for(int i = 0; i < gTeamNumber; i++){deq_list[i] = -1;}

  sem_wait(&semIsNinjia);
  int is_now_N = gIsNinjia;
  sem_post(&semIsNinjia);

  sem_wait(&semMixedQueue);
  remove_broadcast(ind);
  //get the head of queue (head is not nominated)
  int ticket = 0;
  struct NPQueue* tempHead = mixedQueue;

  if(tempHead == NULL) {
    sem_post(&semMixedQueue);
    return;
  }  //nothing to do

  while(tempHead!=NULL && is_nominated(tempHead->thread_index
    && tempHead->is_ninjia == is_now_N)){
    tempHead = tempHead->next;
  }//find tail

  if(tempHead == NULL) {  //switch to the other kind
    if(ticket==0 && is_evacuated()){
      printf("===DONE %d: switch===\n", is_now_N);
      // dequeue & add head and try to find 3 pf same type, post 4 sem
      int counter = 0;
      gIsNinjia = !is_now_N;
      tempHead = mixedQueue;
      while(tempHead!=NULL && counter<gTeamNumber){
        if(tempHead->is_ninjia == is_now_N){
          deq_list[deq_ind++] = tempHead->thread_index;
          add_broadcast(tempHead->thread_index);
          if(is_now_N) sem_post(&semNin);
          else sem_post(&semPir);
          counter++;
        }
        tempHead = tempHead->next;
      }//find tail
    }

    sem_post(&semMixedQueue);

    for(int i = 0; i < gTeamNumber; i++){
      if(deq_list[i] != -1) {dequeuePN(deq_list[i]);}
    }
    return; //=====EXIT======
  }

  int is_next_N = tempHead->is_ninjia; //who is next

  //is head same type as people in building?
  //if yes: dequeue head, add to broadcast, post 1 sem
  if(is_next_N == is_now_N){
    printf("===keep serving===\n");
    gIsNinjia = is_next_N;
    deq_list[deq_ind++] = tempHead->thread_index;
    add_broadcast(tempHead->thread_index);
    if(is_next_N) sem_post(&semNin);
    else sem_post(&semPir);
  }

  //else no:
  else{
    printf("===Try switch...");
    if(is_now_N)sem_getvalue(&semNin, &ticket);
    else sem_getvalue(&semPir, &ticket);
    //if pre sem is 0 && broadcast is empty:
    if(ticket==0 && is_evacuated()){
      printf("switchED!!===\n");
      // dequeue & add head and try to find 3 pf same type, post 4 sem
      int counter = 0;
      gIsNinjia = is_next_N;
      while(tempHead!=NULL && counter<gTeamNumber){
        if(tempHead->is_ninjia == is_next_N){
          deq_list[deq_ind++] = tempHead->thread_index;
          add_broadcast(tempHead->thread_index);
          if(is_next_N) sem_post(&semNin);
          else sem_post(&semPir);
          counter++;
        }
        tempHead = tempHead->next;
      }//find tail
    }
    //else quit
  }

  prinf_broadcast();

  sem_post(&semMixedQueue);

  for(int i = 0; i < gTeamNumber; i++){
    if(deq_list[i] != -1) {dequeuePN(deq_list[i]);}
  }
}

/* ==== ninijias routine ==== */
void *nin_routine(void* index){
  int is_coming_back = FALSE;
  int ind = (int) index;
  int is_cost_free = FALSE;
  long int pre_us, pos_us, pre_s, pos_s;
  struct timeval start_time;
  struct timeval end_time;

  sleep((unsigned int) rand()%5); //time of arrival
  // printf("Ninjia %d: im here\n", ind);
  enqueuePN(ind, TRUE); //in line
  //begin wait in queue
  gettimeofday(&start_time, NULL);
  pre_us = start_time.tv_usec;
  pre_s = start_time.tv_sec;

  //WAITTING...
  //only when queue head is you, you can contend semaphore
  //else spinwait
  while(!is_nominated(ind));
  sem_wait(&semNin);

  printf("Ninjia %d: my turn\n", ind);

  //end wait in queue
  gettimeofday(&end_time, NULL);
  pos_us = end_time.tv_usec;
  pos_s = end_time.tv_sec;
  queueNin[ind]->wait_time
  = (long int)(pos_s+pos_us/1000000)-(pre_s+pre_us/1000000);

  if(queueNin[ind]->wait_time >= 30) {
    is_cost_free = TRUE;
    printf("Ninjia %d: waited %li,FREE DEAL!!!\n",ind,queueNin[ind]->wait_time);
  }

  int outfitting = rand()%3; //TODO: Normal dist
  queueNin[ind]->outfit_time = outfitting;
  if(!is_cost_free) queueNin[ind]->cost = outfitting;
  else queueNin[ind]->cost = 0;

  sleep(outfitting); //change custome

  //25% chance to come back
  if(rand()%4 == 0){ //TODO: 25%
    printf("Ninjia %d: imma comming back\n", ind);
    is_coming_back = TRUE;
  }

  broadcast_manager(ind);

  if(is_coming_back){nin_routine(index);}

  printf("Ninjia %d: byebye\n", ind);
  pthread_exit(NULL);
}

/* ==== pirates routine ==== */
void * pir_routine(void* index){
  int is_coming_back = FALSE;
  int ind = (int) index -total_n;
  int indAct = (int) index ;
  int is_cost_free = FALSE;
  long int pre_us, pos_us, pre_s, pos_s;
  struct timeval start_time;
  struct timeval end_time;

  sleep((unsigned int) rand()%5); //time of arrival

  enqueuePN(indAct, FALSE); //in line
  //begin wait in queue
  gettimeofday(&start_time, NULL);
  pre_us = start_time.tv_usec;
  pre_s = start_time.tv_sec;

  //WAITTING...
  while(!is_nominated(indAct));
  sem_wait(&semPir);

  printf("Pirate %d: my turn\n", indAct);

  //end wait in queue
  gettimeofday(&end_time, NULL);
  pos_us = end_time.tv_usec;
  pos_s = end_time.tv_sec;
  queuePir[ind]->wait_time
   = (long int)(pos_s+pos_us/1000000)-(pre_s+pre_us/1000000);

  if(queuePir[ind]->wait_time >= 30) {
    is_cost_free = TRUE;
    printf("Pirate %d: waited %li,FREE DEAL!!!\n",indAct,queuePir[ind]->wait_time);
  }
  //not in queue anymore
  // dequeuePN(ind);

  int outfitting = rand()%5; //TODO: Normal dist
  queuePir[ind]->outfit_time = outfitting;
  if(!is_cost_free) queuePir[ind]->cost = outfitting;
  else queuePir[ind]->cost = 0;

  sleep(outfitting); //change custome

  //25% chance to come back
  if(rand()%4 == 0){ //TODO: 25%
    printf("Pirate %d: imma comming back\n", indAct);
    is_coming_back = TRUE;
  }

  broadcast_manager(indAct);

  if(is_coming_back) {pir_routine(index);}

  printf("Pirate %d: byebye\n", indAct);
  pthread_exit(NULL);
}

/*create random arrivals */
int main(int argc, char *argv[]){
  //TODO: input process

  total_n = MAXNINJIA-rand()%40;
  total_p = MAXPIRATE-rand()%40;
  broadcast = malloc(sizeof(int) * gTeamNumber);
  for(int i = 0; i < gTeamNumber; i++){broadcast[i] = -1;}
  //allocate space for queues
  for(int i = 0; i < MAXNINJIA; i++){
    queueNin[i] = malloc(sizeof(Info));
  }

  for(int i = 0; i < MAXPIRATE; i++){
    queuePir[i] = malloc(sizeof(Info));
  }

  //initialize semaphore
  sem_init(&semMixedQueue, 0, 1);
  sem_init(&semBroadcast, 0, 1);
  sem_init(&semIsNinjia, 0, 1);
  sem_init(&semNin, 0, 0); //ninjia first
  sem_init(&semPir, 0, 0); //pirate last

  //create N
  for(int i = 0; i < total_n; i++){
    if((int)pthread_create(&queueNin[i]->thread, NULL, nin_routine,  (void *)i )){
      printf("ERROR in creating ninjia: %d\n", i);
      exit(-1);
    }
  }

  //create P
  for(int i = total_n; i < total_p+total_n; i++){
    if((int)pthread_create(&queuePir[i-total_n]->thread, NULL, pir_routine, (void *)i )){
      printf("ERROR in creating pirate: %d\n", i);
      exit(-1);
    }
  }

  //accumulate some people in queue
  sleep(2);
  sem_wait(&semMixedQueue);
  sem_wait(&semIsNinjia);
  gIsNinjia = !mixedQueue->is_ninjia;
  sem_post(&semIsNinjia);
  sem_post(&semMixedQueue);

  broadcast_manager(-1);

  //join N
  for(int i = 0; i < total_n; i++){
    if((int)pthread_join(queueNin[i]->thread, NULL)){
      printf("ERROR in joining ninjia: %d\n", i);
      exit(-1);
    }
  }

  //join P
  for(int i = total_n; i < total_p+total_n; i++){
    if((int)pthread_join(queuePir[i-total_n]->thread, NULL)){
      printf("ERROR in joining pirate: %d\n", i);
      exit(-1);
    }
  }

  //TODO: print stats


  //free queues
  free(broadcast);
  for(int i = 0; i < MAXNINJIA; i++){free(queueNin[i]); }
  for(int i = 0; i < MAXPIRATE; i++){free(queuePir[i]); }

  exit(FALSE);
  return FALSE;
}
