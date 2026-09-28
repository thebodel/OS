#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char separators[] = " -\r\t\n./,";

void
sixfive(int fd)
{
  char c;
  int n;
  int num;
  int in_number;

  num = 0;
  in_number = 0;

  while ((n = read(fd, &c, 1)) == 1) {
    if (c >= '0' && c <= '9') {
      num = num * 10 + (c - '0');
      in_number = 1;
    } else if (strchr(separators, c)) {
      if (in_number) {
        if (num % 5 == 0 || num % 6 == 0)
          printf("%d\n", num);
        num = 0;
        in_number = 0;
      }
    } else {
      num = 0;
      in_number = 0;
    }
  }

  if (in_number) {
    if (num % 5 == 0 || num % 6 == 0)
      printf("%d\n", num);
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if (argc == 1) {
    sixfive(0);
    exit(0);
  }

  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], O_RDONLY)) < 0) {
      fprintf(2, "error: cannot open %s\n", argv[i]);
      continue;
    }
    sixfive(fd);
    close(fd);
  }

  exit(0);
}
