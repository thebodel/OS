#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data, int len);

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("Example 1:\n");
    int a[2] = {61810, 2026};
    memdump("ii", (char *)a, sizeof(a));

    printf("Example 2:\n");
    memdump("S", "a string", sizeof("a string"));

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *)&s, sizeof(s));

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char *)&example, sizeof(example));

    printf("Example 5:\n");
    memdump("sccccc", (char *)&example, sizeof(example));
  } else if (argc == 2) {
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while (n < sizeof(data)) {
      int nn = read(0, data + n, sizeof(data) - n);
      if (nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data, n);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data, int len)
{
  uchar *p = (uchar *)data;
  int left = len;

  for (; *fmt; fmt++) {
    if (*fmt == 'i') {
      uint x = 0;

      if (left < 4) {
        printf("memdump: not enough data for '%c'\n", *fmt);
        return;
      }

      x = (uint)p[0];
      x |= (uint)p[1] << 8;
      x |= (uint)p[2] << 16;
      x |= (uint)p[3] << 24;
      printf("%d\n", (int)x);

      p += 4;
      left -= 4;
    } else if (*fmt == 'p') {
      uint64 x = 0;
      int i;

      if (left < 8) {
        printf("memdump: not enough data for '%c'\n", *fmt);
        return;
      }

      for (i = 0; i < 8; i++)
        x |= (uint64)p[i] << (8 * i);
      printf("%lx\n", x);

      p += 8;
      left -= 8;
    } else if (*fmt == 'h') {
      ushort x = 0;

      if (left < 2) {
        printf("memdump: not enough data for '%c'\n", *fmt);
        return;
      }

      x = (ushort)p[0];
      x |= (ushort)p[1] << 8;
      printf("%d\n", x);

      p += 2;
      left -= 2;
    } else if (*fmt == 'c') {
      if (left < 1) {
        printf("memdump: not enough data for '%c'\n", *fmt);
        return;
      }

      printf("%c\n", p[0]);
      p += 1;
      left -= 1;
    } else if (*fmt == 's') {
      uint64 x = 0;
      int i;

      if (left < 8) {
        printf("memdump: not enough data for '%c'\n", *fmt);
        return;
      }

      for (i = 0; i < 8; i++)
        x |= (uint64)p[i] << (8 * i);
      printf("%s\n", (char *)x);

      p += 8;
      left -= 8;
    } else if (*fmt == 'S') {
      while (left > 0 && *p != '\0') {
        printf("%c", *p);
        p++;
        left--;
      }
      printf("\n");

      if (left > 0) {
        p++;
        left--;
      }
    }
  }
}
