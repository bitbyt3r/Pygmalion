#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void print_trace (void)
{
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);

  printf ("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
     printf ("%s\n", strings[i]);

  free (strings);
}

void pack16(uint16_t val, unsigned char *buf) {
    buf[0] = val & 0xff;
    buf[1] = (val >> 8) & 0xff;
}

void pack32(uint32_t val, unsigned char *buf) {
    buf[0] = val & 0xff;
    buf[1] = (val >> 8) & 0xff;
    buf[2] = (val >> 16) & 0xff;
    buf[3] = (val >> 24) & 0xff;
}

uint32_t unpack32(unsigned char *buf) {
    uint32_t val = buf[3];
    val = (val << 8) + buf[2];
    val = (val << 8) + buf[1];
    val = (val << 8) + buf[0];
    return val;
}

uint16_t unpack16(unsigned char *buf) {
    uint16_t val = buf[1];
    val = (val << 8) + buf[0];
    return val;
}