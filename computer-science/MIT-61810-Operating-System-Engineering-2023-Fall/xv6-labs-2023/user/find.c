#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int find(char *path, char *filename) {
  char buf[512];
  struct stat st;
  int fd;
  struct dirent de;

  // open and check whether path is a directory
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return -1;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return -1;
  }

  if (st.type != T_DIR) {
    fprintf(2, "find: `%s` is not a directory\n", path);
    return -1;
  }

  // add '/' to the last character of the path
  memcpy(buf, path, strlen(path));
  char *p = buf + strlen(path);
  if (*(p - 1) != '/') *p++ = '/';

  // read directory entries
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    
    // read path/de.name
    memcpy(p, de.name, sizeof(de.name));
    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      return -1;
    }

    if (st.type == T_FILE && strcmp(filename, de.name) == 0) { // if it is a file and has the same file name, print
      fprintf(1, "%s\n", buf);
    } else if (st.type == T_DIR) { // if it is a directory, recursively find
      find(buf, filename);
    }
  }

  return 0;
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(1, "Usage: find dir_name file_name");
    exit(0);
  }

  if (find(argv[1], argv[2]) == -1) {
    fprintf(2, "find: error");
    exit(1);
  }

  exit(0);
}
