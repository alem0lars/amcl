#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "logging.h"
#include "remote.h"


int main(int argc, char** argv) {
  int ret = 0;

  printf("This is the NMLabs Stdlib C\n");
  printf("\tAuthor: Alessandro Molari (molari.alessandro@gmail.com)\n");
  printf("\tLicense: GPLv3\n");
  exit((ret == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}

