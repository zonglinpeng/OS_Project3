#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
// These values MUST match the unistd_32.h modifications:
#define __NR_cs3013_syscall1 377

long testCall1 ( void) {
  return (long) syscall(__NR_cs3013_syscall1);
}

int main () {
  FILE* not_safe;
  FILE* safe;

  char buff[255];
  for(int i = 0; i < 10; i++){
    printf("\tcs3013_syscall1 returns: %ld\n", testCall1()); //test syscall1
    not_safe = fopen("IAmSafe.txt", "r"); //test open
    printf("opening virus\n");
    safe = fopen("RealSafe.txt", "r"); //test open
    printf("opening safe file\n");

    if(not_safe != NULL){fgets(buff, 255, not_safe);} //test read
    printf("detect virus\n");
    if(safe != NULL){fgets(buff, 255, safe);} //test read
    printf("normal read\n");

    fclose(not_safe); //test close
    printf("closing virus\n");
    fclose(safe); //test close
    printf("closing safe file\n");

    sleep(10);
  }
  return 0;
}
