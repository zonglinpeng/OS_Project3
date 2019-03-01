/**
*Title: OS Project 1 Phase 3
*Author: Zonglin Peng
*Date: 1/25/2018
***All rights preserved***
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <ctype.h>
/*SYMBOLIC*/
#define TRUE 1
#define FALSE 0
#define BUFFER 128
#define STORAGE 128
#define READIN 4
/*GLOBAL VARIABLES*/
char* gUserCommand [STORAGE];
int gCommandCounter = 3;
int gProcessCounter = 0;
int gIsBackground = FALSE;
int gIsHandle = TRUE;

/*print shell prompt*/
void print_prompt(int line_num){
  printf("===============================================================\n");
  printf("(#%d) G’day, Commander! What command would you like?\n", line_num);
  printf("  0. whoam\t\t: Prints out the result of whoami\n");
  printf("  1. last\t\t: Prints out the result of login info\n");
  printf("  2. ls\t\t\t: Prints out the result of path\n");
  for(int i = 3; i < gCommandCounter; i++){
    printf("  %d. %s\t\t: User Added Command\n", i, gUserCommand[i]);
  }
  printf("  a. add command\t: Adds a new command to the menu\n");
  printf("  c. change directory\t: Changes process working directory\n");
  printf("  e. exit\t\t: Leave Mid-Day Commander\n");
  printf("  p. pwd\t\t: Prints working directory\n");
  printf("  r. running procress\t: Prints the list of running process\n");
  printf("===============================================================\n");
  printf("Option?:\n>>");
}

/*calculate and print process stats*/
void print_stats(long int ecl_time, long hard_faults, long soft_faults){
  float time_acc = (float) ecl_time/1000;
  printf("\n--- Statistics ---\n");
  printf("Elapsed time: %.3f milliseconds\n",time_acc);
  printf("Page Faults: %ld\n", hard_faults);
  printf("Page Faults (reclaimed): %ld\n", soft_faults);
  printf("----------------------------------------------\n\n");
}

/*seperate command by space*/
void multi_command_handler (char str[], char** res){
  char *p = strtok (str, " ");
  while (p!=NULL) {
    *(res++) = p;
    p = strtok (NULL, " ");
  }
}

/*linked list of processes*/
struct process_list{
  int pid;
  int process_ID;
  char* arg;
  struct rusage usage;
  struct timeval start_time;
  struct process_list* next;
};

/*linked list: add*/
struct process_list* add_list(struct process_list* list,
  int pid, int given_ID, char** cmd,
  struct rusage usage, struct timeval start_time){
    struct process_list* new_list = malloc(sizeof(struct process_list));
    new_list->pid = pid;
    new_list->process_ID = given_ID;
    new_list->arg = malloc(sizeof(char)*STORAGE);
    memset(new_list->arg, '\0', STORAGE);
    for(int i = 0; cmd[i]!= NULL; ++i){
      if(new_list->arg != NULL)
      sprintf(new_list->arg, "%s %s",new_list->arg,  cmd[i]);
      else   sprintf(new_list->arg, "%s ",  cmd[i]);
    }
    new_list->usage = usage;
    new_list->start_time = start_time;
    new_list->next = malloc(sizeof(struct process_list));
    new_list->next = list;
    return new_list;
}

/*linked list: remove*/
struct process_list* remove_list(struct process_list* list, int pid){
  if(list == NULL) return list;
  struct process_list* prior = list;
  struct process_list* cur = list->next;
  if(prior->pid == pid) return cur;
  while(cur != NULL){
    if(cur->pid == pid){
      prior->next = cur->next;
      free(cur);
      break;
    }
    prior = cur;
    cur =  cur->next;
  }
  return list;
}

/*linked list: find a node*/
struct process_list* list_iterate(struct process_list* list, int pid){
  if(list == NULL) return list;
  struct process_list* iterator = list;
  while(iterator != NULL){
    if(iterator->pid == pid) {
      return iterator;
    }
    iterator = iterator->next;
  }
  return NULL;
}

/*linked list: print all node info*/
void print_process(struct process_list* list){
  struct process_list* iterator = list;
  int isPrinted = FALSE;
  printf("\n-- Background Processes --\n");
  while(iterator != NULL &&  iterator-> arg != NULL ){
    printf("  [%d]\t%s\n",  iterator->pid,  iterator-> arg);
    iterator = iterator->next;
    isPrinted = TRUE;
  }
  if(!isPrinted){printf("  [No backgound]\n");}
  printf("\n");
}

/*check and handle all finished backgrounds*/
struct process_list* check_background (struct process_list* list, pid_t pid){
  struct rusage usage;
  struct timeval start_time;
  struct timeval end_time;
  long pre_hard_faults, pre_soft_faults, pos_hard_faults, pos_soft_faults;
  long int pre_ms, pos_ms, pre_s, pos_s;

  //pick up remaining tasks
  pid_t wait_error = wait3(NULL, WNOHANG, &usage); //terminated childID
  while(wait_error != pid && wait_error > 0 ){ //wait_error != gPid &&
     struct process_list* temp_list = list_iterate(list, wait_error);
     printf("\n-- Job Complete [%d] --\n", temp_list->process_ID);
     printf("  Process ID : %d\n", wait_error);
     printf("  Command: %s\n",  temp_list->arg );
     gProcessCounter--;

     //---reach data---
     usage = temp_list->usage;
     pre_hard_faults = usage.ru_majflt;
     pre_soft_faults = usage.ru_minflt;
     start_time = temp_list->start_time;
     pre_ms = start_time.tv_usec;
     pre_s = start_time.tv_sec;
     //---after execution---
     getrusage(RUSAGE_CHILDREN, &usage);
     pos_hard_faults = usage.ru_majflt;
     pos_soft_faults = usage.ru_minflt;
     gettimeofday(&end_time, NULL);
     pos_ms = end_time.tv_usec;
     pos_s = end_time.tv_sec;
     //---calcuate & print stats---
     print_stats((pos_s*1000000+pos_ms)-(pre_s*1000000+pre_ms),
       pos_hard_faults-pre_hard_faults,
       pos_soft_faults-pre_soft_faults);

     list = remove_list(list, wait_error);

     wait_error = wait3(NULL, WNOHANG, &usage); //terminated childID
 }
 return list;
}

/*handle foreground and background processes*/
struct process_list*  mixed_handler (char** cmd, struct process_list* list){
  int isBackground = gIsBackground;
  struct rusage usage;
  struct timeval start_time;
  struct timeval end_time;
  long pre_hard_faults, pre_soft_faults, pos_hard_faults, pos_soft_faults;
  long int pre_ms, pos_ms, pre_s, pos_s;

  //--- Before Execution ---
  getrusage(RUSAGE_CHILDREN, &usage);
  pre_hard_faults = usage.ru_majflt;
  pre_soft_faults = usage.ru_minflt;
  gettimeofday(&start_time, NULL);
  pre_ms = start_time.tv_usec;
  pre_s = start_time.tv_sec;

  //---in execution---
  pid_t gPid = fork();
  //======== Failed to fork ==========
  if (gPid < 0 ){
    printf("Fork failed\n");
    exit(FALSE);
  }
  //======= Child process =========
  else if (gPid == 0){
    if(cmd[0][0] != '\0') {
      execvp(cmd[0], cmd);
      printf("Execution failed\n");
      exit(FALSE);
    }else exit(TRUE);
  }
  //======= Parent Process =========
  if(gPid > 0){

    //--- background ---
    if (isBackground){ //pid = childID
      list = add_list(list, gPid, gProcessCounter++, cmd, usage, start_time);
      //print the backgound info
      printf("[%d]\t%d\n", gProcessCounter, gPid);

      //wait and get backgound
      pid_t pid_child = wait3(NULL, WNOHANG, &usage); //no wait
      while(pid_child > 0 && pid_child != gPid){
        //backgrounding
        if(pid_child <= 0){
          printf("Child is runing, ID: %d\n", gPid);
        }
        //background processes done
        else if(pid_child > 0){
          struct process_list* temp_list = list_iterate(list, pid_child);
          printf("\n-- Job Complete [%d] --\n", temp_list->process_ID);
          printf("  Process ID : %d\n", pid_child);
          printf("  Command: %s\n",  temp_list->arg );
          gProcessCounter--;

          //---reach data---
          usage = temp_list->usage;
          pre_hard_faults = usage.ru_majflt;
          pre_soft_faults = usage.ru_minflt;
          start_time = temp_list->start_time;
          pre_ms = start_time.tv_usec;
          pre_s = start_time.tv_sec;
          //---after execution---
          getrusage(RUSAGE_CHILDREN, &usage);
          pos_hard_faults = usage.ru_majflt;
          pos_soft_faults = usage.ru_minflt;
          gettimeofday(&end_time, NULL);
          pos_ms = end_time.tv_usec;
          pos_s = end_time.tv_sec;
          //---calcuate & print stats---
          print_stats((pos_s*1000000+pos_ms)-(pre_s*1000000+pre_ms),
            pos_hard_faults-pre_hard_faults,
            pos_soft_faults-pre_soft_faults);

          list = remove_list(list, pid_child);
        }
        pid_child = wait3(NULL, WNOHANG, &usage); //no wait
      }
    }

    //--- foregraound --
    else if(!isBackground){
        //do foreground
        while (wait4(gPid, NULL, FALSE, &usage) != -1) {
          //---after execution---
          getrusage(RUSAGE_CHILDREN, &usage);
          pos_hard_faults = usage.ru_majflt;
          pos_soft_faults = usage.ru_minflt;
          gettimeofday(&end_time, NULL);
          pos_ms = end_time.tv_usec;
          pos_s = end_time.tv_sec;

          //---calcuate & print stats---
          print_stats((pos_s*1000000+pos_ms)-(pre_s*1000000+pre_ms),
            pos_hard_faults-pre_hard_faults,
            pos_soft_faults-pre_soft_faults);
        }

        //check finished backgound
        list = check_background(list, gPid);
    }
  }
  return list;
}

/*read user command*/
struct process_list*  read_command(char arg, char** cmd,
  struct process_list* list){
  gIsBackground = FALSE;
  //background chekc
  list = check_background(list, 0); //let it MINGLE!!

  switch (arg) {
    case '0': //whoami
      printf("\n-- Who Am I? --\n");
      cmd[0] = "whoami";
      cmd[1] = NULL;
      break;

    case '1': //last
      printf("\n-- Last Logins --\n");
      cmd[0] = "last";
      cmd[1] = NULL;
      break;

    case '2': //ls
      printf("\n-- Directory Listing --\n");
      char arguement_c2 [BUFFER];
      char path_c2 [BUFFER];
      char cmd_line_c2 [BUFFER];
      char** temp_cmd = malloc (BUFFER * sizeof(char*));
      memset(arguement_c2, '\0', BUFFER);
      memset(path_c2, '\0', BUFFER);
      //---get args---
      printf("\n>>Argurements: ");
      //backgound
      list = check_background(list, 0); //let it MINGLE!!
      while(arguement_c2[0] == '\0') fgets(arguement_c2, BUFFER, stdin);
      arguement_c2[strlen(arguement_c2) - 1] = '\0';
      //---get path---
      printf("\n>>Path: ");
      list = check_background(list, 0); //let it MINGLE!!
      //read input
      while(path_c2[0] == '\0') fgets(path_c2, BUFFER, stdin);
      path_c2[strlen(path_c2) - 1] = '\0';
      sprintf(cmd_line_c2, "ls %s %s", arguement_c2, path_c2);
      //break input string by spaces
      multi_command_handler(cmd_line_c2, temp_cmd);
      //load in string to command
      int i = 0;
      while(temp_cmd[i] != NULL) { strcpy(cmd[i], temp_cmd[i]); i++;}
      cmd[i] = NULL;
      printf("\n");
      //free command storage
      free(temp_cmd);
      break;

    case 'a':
      printf("\n-- Add Command --\n>> ");
      //check_background
      list = check_background(list, 0); //let it MINGLE!!
      char arguement_ca [BUFFER];
      memset(arguement_ca, '\0', BUFFER);
      //read input
      while(arguement_ca[0] == '\0') fgets(arguement_ca, BUFFER, stdin);
      arguement_ca[strlen(arguement_ca) - 1] = '\0';
      gUserCommand[gCommandCounter] = malloc(BUFFER*sizeof(char));
      strcpy(gUserCommand[gCommandCounter], arguement_ca);
      printf("\nSystem: OK, added with ID: %d\n", gCommandCounter++);
      break;

    case 'c':
      printf("\n-- Change Directory --\n");
      printf("\n>>New Directory: ");
      //chekc backgound
      list = check_background(list, 0); //let it MINGLE!!
      char arguement_cc [BUFFER];
      memset(arguement_cc, '\0', BUFFER);
      //read input
      while(arguement_cc[0] == '\0') fgets(arguement_cc, BUFFER, stdin);
      arguement_cc[strlen(arguement_cc) - 1] = '\0';
      chdir(arguement_cc);
      break;

    case 'e':
      if(gProcessCounter>0) {
        printf("Still Backgrounding ...\n\n");
        print_process(list);
        break;
      }
      else {
        printf("Logging you out, Boss\n\n");
        for(int i = 0; i < gCommandCounter; i++) {free(gUserCommand[i]);}
        exit(TRUE);
        return list;
      }

    case 'p':
      printf("\n-- Current Directory --\n\n");
      cmd[0] = "pwd";
      cmd[1] = NULL;
      break;

    case 'r':
      print_process(list);
      break;

    default: //FALSE
      if(isdigit(arg) && gUserCommand[(int)arg - '0'] != NULL){
        char* read_user_cmd = malloc(BUFFER * sizeof(char));
        strcpy(read_user_cmd, gUserCommand[(int)arg - '0']);
        char** temp_cmd = malloc (BUFFER * sizeof(char*));
        printf("\n-- Command: %s --\n", read_user_cmd);
        list = check_background(list, 0); //let it MINGLE!!
        //background command
        char* p = strchr(read_user_cmd, '&');
        if(p != NULL){
          gIsBackground = TRUE;
          //remove amphersand
           for(int i = 0; i < strlen(read_user_cmd); ++i){
             if(read_user_cmd[i] == '&'){read_user_cmd[i] = '\0';}
           }
        //foreground command
        }else{ gIsBackground = FALSE;}
        //break input string by spaces
        multi_command_handler(read_user_cmd, temp_cmd);
        //load in string to command
        int i = 0;
        while(temp_cmd[i] != NULL) { strcpy(cmd[i], temp_cmd[i]); i++;}
        cmd[i] = NULL;
        //free some space for good
        free(read_user_cmd);
        free(temp_cmd);
        break;
      }
      //invalid command
      else {
        printf("No such command\n");
        gIsHandle = FALSE;
        break;
      }
  }
  return check_background(list, 0);
}

/*main*/
int main(int argc, char *argv[]){
  printf("===== Mid-Day Commander, v2====\n");
  int line_num = 0;
  gProcessCounter = 0;
  char* eof_detect ;
  struct process_list* list = malloc(sizeof(struct process_list));

  //WHILE(1) LOOP
  while(TRUE){
    eof_detect = "NOPE";
    gIsHandle = TRUE;
    //print the prompt
    print_prompt(line_num++);
    //background process check
    list = check_background(list, 0); //let it MINGLE!!
    int c;
    char *pos;
    char* arg = (char*) malloc(BUFFER* sizeof(char));
    char** cmd  = (char**) malloc(BUFFER * sizeof(char*));
    for(int i = 0; i < BUFFER; i++) {
      cmd[i] = (char*) malloc(BUFFER * sizeof(char));
      memset(cmd[i], '\0', BUFFER);
    }
    memset(arg, '\0', READIN);
    //EOF handle
    if(*fgets(arg, READIN + 3, stdin) == '\n' ||feof(stdin)){
      printf("No Input Detected\n");
      arg[0] = 'e'; arg[1] = '\0';
    }
    //input re-arrangements
    if((pos = strchr(arg, '\n')) != NULL) *pos = '\0';
    if (strlen(arg) > READIN + 1){
        printf("\nERROR: input exceeds the limit (%d).\n", READIN+3);
        if(arg[READIN+1]!='\n') while((c=getchar()) != '\n' && c!= EOF);
    }
    //-- check input -- handle input --
    else if(arg[0] != '\0' && arg[1] == '\0'){
      list = read_command(arg[0], cmd, list);
      if(gIsHandle) list = mixed_handler(cmd, list);
    }
    //illegal input
    else printf("No such command\n");
    //free mem
    free(arg);
    free(cmd);
  }
  return FALSE;
}
