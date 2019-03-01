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
#define TRUE 1
#define FALSE 0
#define BUFFER 20

  void print_prompt(int line_num){
    printf("(#%d)G’day, Commander! What command would you like?\n", line_num);
    printf("  0. whoami : Prints out the result of whoami\n");
    printf("  1. last: Prints out the result of login info\n");
    printf("  2. ls: Prints out the result of path\n");
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

  int read_command(char* arg, char** cmd){
    switch (atoi(arg)) {
      case 0: //whoami
        printf("\n-- Who Am I? --\n");
        cmd[0] = "whoami";
        cmd[1] = NULL;
        break;

      case 1: //last
        printf("\n-- Last Logins --\n");
        cmd[0] = "last";
        cmd[1] = NULL;
        break;

      case 2: //ls
        printf("\n-- Directory Listing --\n");
        char arguement_c2 [BUFFER];
        char path_c2 [BUFFER];
        char cmd_line_c2 [BUFFER*2];
        char** temp_cmd = malloc (BUFFER * sizeof(char*));
        int index_bwd_c2 = 3, index_fwd_c2 = 0;
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
        break;

      default: //FALSE
        printf("No such command\n");
        return FALSE;
    }
    return TRUE;
  }


  int main(int argc, char *argv[]){
    printf("===== Mid-Day Commander, v0 ====\n");
    int line_num = 0;

    while(TRUE){
      print_prompt(line_num++);
      char *pos;
      char* arg = (char*) malloc(3* sizeof(char));
      char** cmd  = (char**) malloc(BUFFER * sizeof(char*));
      for(int i = 0; i < BUFFER; i++) {
        cmd[i] = (char*) malloc(BUFFER * sizeof(char));
        memset(cmd[i], '\0', BUFFER);
      }
      memset(arg, '\0', 3);
      while(arg[0] == '\0')fgets(arg, 3, stdin);
      if((pos = strchr(arg, '\n')) != NULL) *pos = '\0';
      if(arg[0] >= '0' && arg[0] <= '3') {
        read_command(arg, cmd);
        handle_request(cmd);
      }
      else printf("Invalid input, try again:\n");
      // }
      free(arg);
      free(cmd);
    }
    return FALSE;
  }
