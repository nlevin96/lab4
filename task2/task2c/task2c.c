#include "lab4_util.h"

#define SYS_WRITE 1
#define SYS_OPEN 2
#define SYS_CLOSE 3
#define DT_DIR 4
#define SYS_LSEEK 8
#define SYS_GETDENTS 78
#define STDOUT 1

extern int system_call();

int check_if_file_exists = 0;
int exit_flag = 0;
int n_flag = 0;
int e_flag = 0;
char* name_to_find;
char* command;

struct linux_dirent { /* getdents man */
    unsigned long  d_ino;     
    unsigned long  d_off;    
    unsigned short d_reclen; /* length of the linux dirent */
    char           d_name[]; 
    /* char           d_type; */
} linux_dirent;

void get_path(char* path, char* name, char* new_path){
    int i;
    int j;
    for(i=0; i < simple_strlen(path); i++){
        new_path[i] = path[i];
    }
    new_path[i] = '/';
    i++;
    for(j=0; j < simple_strlen(name); j++){
        new_path[i] = name[j];
        i++;
    }
    new_path[i] = '\0';
}

void append_command(char* command, char* path, char* path_and_command){
    int i;
    int j;
    for(i=0; i < simple_strlen(command); i++){ 
        path_and_command[i] = command[i];
    }
    path_and_command[i] = ' ';
    i++;
    for(j=0; j < simple_strlen(path); j++){
        path_and_command[i] = path[j];
        i++;
    }
    path_and_command[i] = '\0';
}


void create_list(int file_num, char* path){
    struct linux_dirent* l_dirent;
    int bytes_readed;
    char buff [1024];
    int curr_pos = 0;
    char d_type; /* from getdents man */
    int curr_name_len;
    int curr_path_len;
    int command_len;
    int new_file_num;
    int to_print;
    bytes_readed = system_call(SYS_GETDENTS, file_num, buff, 1024);
    if(bytes_readed == 0) return;
    if(bytes_readed < 0) { 
        exit_flag = 1;
        return;
    }
    while(curr_pos < bytes_readed){
        l_dirent = (struct linux_dirent*)(buff + curr_pos);
        d_type = *(buff + curr_pos +l_dirent->d_reclen -1);
        if( l_dirent -> d_name [0] == '.') {
            curr_pos = curr_pos + l_dirent -> d_reclen;
            continue;
        }
        curr_path_len = simple_strlen(path);
        curr_name_len = simple_strlen(l_dirent -> d_name);
        char new_path [curr_path_len + 1 + curr_name_len + 3];
        get_path(path, l_dirent -> d_name, new_path);
        if(e_flag == 0){
            to_print = ((n_flag == 1) && (simple_strcmp(name_to_find, l_dirent -> d_name)== 0)); /* must check if n_flag = 1 inorder to use srtcmp*/
            if(n_flag == 0 || to_print){
                system_call(SYS_WRITE, STDOUT,new_path,simple_strlen(new_path));
                system_call(SYS_WRITE, STDOUT,"\n",simple_strlen("\n"));
            }
        }
        if(d_type == DT_DIR){
            new_file_num = system_call(SYS_OPEN, new_path, 0, 0777);
            if(new_file_num < 0) {
                exit_flag = 1;
                return;
            }
            create_list(new_file_num, new_path);
        }
        
        else if(e_flag == 1 && simple_strcmp(name_to_find, l_dirent -> d_name)== 0){
            check_if_file_exists = 1;
            command_len = simple_strlen(command);
            char path_and_command [simple_strlen(new_path) + command_len + 1];
            append_command(command, new_path, path_and_command);
            simple_system(path_and_command);
        }
        curr_pos = curr_pos + l_dirent -> d_reclen;
    
    }
    
}


int main (int argc , char* argv[], char* envp[])
{
  int file_num;
  int i;
  for(i = 0; i < argc; i++){
      if(simple_strcmp(argv[i],"-n")==0){
          n_flag = 1;
          name_to_find = argv[i+1];
          break;
      }
      if(simple_strcmp(argv[i],"-e")==0){
          e_flag = 1;
          name_to_find = argv[i+1];
          command = argv[i+2];
          break;
      }
  }
  
  if(n_flag == 0 && e_flag == 0){
    if(system_call(SYS_WRITE, STDOUT, ".\n", simple_strlen(".\n"))<0) return 0x55;
  }
  file_num = system_call(SYS_OPEN, ".",0,0777);
  if(file_num < 0) return 0x55;
  create_list(file_num, ".");
  if( exit_flag == 1) return 0x55;
  if( e_flag == 1 && check_if_file_exists == 0){
      system_call(SYS_WRITE, STDOUT, "The file '", simple_strlen("The file '"));
      system_call(SYS_WRITE, STDOUT, name_to_find, simple_strlen(name_to_find));
      system_call(SYS_WRITE, STDOUT, "' Does not exist.\n", simple_strlen("' Does not exist.\n"));
  }
  return 0;
  
}
