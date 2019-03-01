/**
*Author: Zonglin Peng
*Date: 2018-2019
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <ctype.h>
#define TRUE 1
#define FALSE 0
#define BUFFER 32
#define STORAGE 16
#define READIN 4

char* gUserCommand [STORAGE];
int gCommandCounter = 3;

  void print_prompt(int line_num){
    printf("(#%d)G’day, Commander! What command would you like?\n", line_num);
    printf("  0. whoami : Prints out the result of whoami\n");
    printf("  1. last: Prints out the result of login info\n");
    printf("  2. ls: Prints out the result of path\n");
    for(int i = 3; i < gCommandCounter; i++){
      printf("  %d. %s: User Added Command\n", i, gUserCommand[i]);
    }
    printf("  a. add command : Adds a new command to the menu.\n");
    printf("  c. change directory : Changes process working directory\n");
    printf("  e. exit : Leave Mid-Day Commander\n");
    printf("  p. pwd : Prints working directory\n");
    printf("Option?:\n>>");
  }

  void print_stats(long int ecl_time, long hard_faults, long soft_faults){
    float time_acc = (float) ecl_time/1000;
    printf("\n--- Statistics ---\n");
    printf("Elapsed time: %.3f milliseconds\n",time_acc);
    printf("Page Faults: %ld\n", hard_faults);
    printf("Page Faults (reclaimed): %ld\n", soft_faults);
    printf("----------------------------------------------\n\n");
  }

  void multi_command_handler (char str[], char** res){
    char *p = strtok (str, " ");
    while (p!=NULL) {
      *(res++) = p;
      p = strtok (NULL, " ");
    }
  }

  // struct Node{
  //   char* arg;
  //   struct *Node next;
  // };

  int handle_request(char** cmd){
    struct rusage usage;
    struct timeval start_time;
    struct timeval end_time;
    int pid = 0;
    long pre_hard_faults, pre_soft_faults, pos_hard_faults, pos_soft_faults;
    long int pre_ms, pos_ms, pre_s, pos_s;

    //---before execution---
    getrusage(RUSAGE_CHILDREN, &usage);
    pre_hard_faults = usage.ru_majflt;
    pre_soft_faults = usage.ru_minflt;
    gettimeofday(&start_time, NULL);
    pre_ms = start_time.tv_usec;
    pre_s = start_time.tv_sec;

    pid = fork();
    //Failed to fork
    if (pid < 0 ){
      printf("Fork failed\n");
      exit(FALSE);
    }
    //Parent Process
    else if(pid > 0 ){wait(NULL);}
    //Child process
    else if (pid == 0){
      execvp(cmd[0], cmd);
      printf("Execution failed\n");
      exit(FALSE);
    }

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

    return TRUE;
  }

  int read_command(char arg, char** cmd){
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
        while(arguement_c2[0] == '\0') fgets(arguement_c2, BUFFER, stdin);
        arguement_c2[strlen(arguement_c2) - 1] = '\0';
        //---get path---
        printf("\n>>Path: ");
        while(path_c2[0] == '\0') fgets(path_c2, BUFFER, stdin);
        path_c2[strlen(path_c2) - 1] = '\0';
        //---load in cmd---
        sprintf(cmd_line_c2, "ls %s %s", arguement_c2, path_c2);
        multi_command_handler(cmd_line_c2, temp_cmd);
        int i = 0;
        while(temp_cmd[i] != NULL) { strcpy(cmd[i], temp_cmd[i]); i++;}
        cmd[i] = NULL;
        printf("\n");
        //free command storage
        free(temp_cmd);
        break;

      case 'a':
        printf("\n-- Add Command --\n>> ");
        char arguement_ca [BUFFER];
        memset(arguement_ca, '\0', BUFFER);
        while(arguement_ca[0] == '\0') fgets(arguement_ca, BUFFER, stdin);
        arguement_ca[strlen(arguement_ca) - 1] = '\0';
        gUserCommand[gCommandCounter] = malloc(BUFFER*sizeof(char));
        strcpy(gUserCommand[gCommandCounter], arguement_ca);
        printf("\nSystem: OK, added with ID: %d\n", gCommandCounter++);
        break;

      case 'c':
        printf("\n-- Change Directory --\n");
        printf("\n>>New Directory: ");
        char arguement_cc [BUFFER];
        memset(arguement_cc, '\0', BUFFER);
        while(arguement_cc[0] == '\0') fgets(arguement_cc, BUFFER, stdin);
        arguement_cc[strlen(arguement_cc) - 1] = '\0';
        chdir(arguement_cc);
        break;

      case 'e':
        printf("Logging you out, Boss\n\n");
        exit(0);
        break;

      case 'p':
        printf("\n-- Current Directory --\n\n");
        cmd[0] = "pwd";
        cmd[1] = NULL;
        break;

      default: //FALSE
        if(isdigit(arg) && gUserCommand[(int)arg - '0'] != NULL){
          char* read_user_cmd = malloc(BUFFER * sizeof(char));
          strcpy(read_user_cmd, gUserCommand[(int)arg - '0']);
          char** temp_cmd = malloc (BUFFER * sizeof(char*));
          printf("\n-- Command: %s --\n", read_user_cmd);
          multi_command_handler(read_user_cmd, temp_cmd);
          int i = 0;
          while(temp_cmd[i] != NULL) { strcpy(cmd[i], temp_cmd[i]); i++;}
          cmd[i] = NULL;
          free(read_user_cmd);
          free(temp_cmd);
          break;
        }
        else {
          printf("No such command\n");
          return FALSE;
        }
    }
    return TRUE;
  }


  int main(int argc, char *argv[]){
    printf("===== Mid-Day Commander, v1====\n");
    int line_num = 0;
    // memset(gUserCommand, NULL, STORAGE);

    while(TRUE){
      print_prompt(line_num++);
      char *pos;
      char* arg = (char*) malloc(READIN* sizeof(char));
      char** cmd  = (char**) malloc(BUFFER * sizeof(char*));
      for(int i = 0; i < BUFFER; i++) {
        cmd[i] = (char*) malloc(BUFFER * sizeof(char));
        memset(cmd[i], '\0', BUFFER);
      }
      memset(arg, '\0', READIN);
      while(arg[0] == '\0')fgets(arg, READIN, stdin);
      if((pos = strchr(arg, '\n')) != NULL) *pos = '\0';
      if(arg[1] == '\0'){
        read_command(arg[0], cmd);
        handle_request(cmd);
      }
      else printf("No such command\n");
      // memset(arg, '\0', READIN+1);
      free(arg);
      free(cmd);
    }
    return FALSE;
  }
