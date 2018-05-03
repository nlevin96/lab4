#include "lab4_util.h"

#define SYS_WRITE 1
#define SYS_OPEN 2
#define SYS_CLOSE 3
#define SYS_LSEEK 8
#define STDOUT 1

extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
  int file_num = system_call(SYS_OPEN,argv[1],2,0777);
  /* check if file_num valid */
  /* if is for checking the result */
  if(file_num < 0)  return 0x55;
  if(system_call(SYS_LSEEK, file_num, 0x1015,1)<0) return 0x55;
  if(system_call(SYS_WRITE, file_num, argv[2], simple_strlen(argv[2]))<0) return 0x55;
  if(system_call(SYS_WRITE, file_num, ". ", simple_strlen(". "))<0) return 0x55;
  if(system_call(SYS_CLOSE, file_num) < 0) return 0x55;
  
  
  return 0;
  
}
