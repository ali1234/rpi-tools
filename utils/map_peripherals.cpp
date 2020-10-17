#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <hexdump.h>
#include <bcm_host.h>

#include "map_peripherals.h"

void open_peripherals(struct peripherals &handle) {
  int fd = open("/dev/mem", O_RDWR);
  if (fd < 0) {
    perror("unable to open /dev/mem");
    exit(1);
  }
  uint32_t arm_phys = bcm_host_get_peripheral_address();
  printf("arm physical is at 0x%x\n", arm_phys);
  uint32_t *rawaddr = (uint32_t*)mmap(NULL, 16 * 1024 * 1024, PROT_READ, MAP_SHARED, fd, arm_phys);
  if (rawaddr == MAP_FAILED) {
    perror("unable to mmap");
    exit(2);
  }
  handle.fd = fd;
  handle.peripherals_start = rawaddr;
  handle.physical_start = arm_phys;
}
