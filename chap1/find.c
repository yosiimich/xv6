#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\x00', DIRSIZ-strlen(p));
  return buf;
}

void
check_filename(char *path, char *filenames[])
{
  //printf("CHECK FILENAME START\n");
  char *name = fmtname(path);
  for (int i = 0; filenames[i]; i++) {
    //printf("[COMPARE] %s %s\n", name, filenames[i]);
    if (!strcmp(name, filenames[i])) {
      printf("%s\n", path);
      break;
    }
  }
}

void
find(char *path, char *filenames[]){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    check_filename(path,filenames);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    //printf("[TEST buf] %s\n",buf);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if (de.inum == 0){
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      //printf("[TEST] %s\n", p);
      
      //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, (int) st.size);
      if(!strcmp(p,".") || !strcmp(p,"..")){
        continue;
      }
      else{
        find(buf, filenames);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;
  char *new_argv[argc - 1];
  for(i = 2; i < argc; i++){
    new_argv[i - 2] = argv[i];
  }    
  find(argv[1], new_argv);
  exit(0);
}