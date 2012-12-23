#include <stdlib.h>
#include <CUnit/CUnit.h>

#include "../src/string.h"


int suite_string_init() {
  return 0;
}

int suite_string_cleanup() {
  return 0;
}

void test_get_string_length() {
  char* str1 = "Hello World!";
  char str2[8] = "Hello";
  char* str3 = malloc(16 * sizeof(char));
  str3 = "Hello World C";

  CU_ASSERT(12 == get_string_length(str1));
  CU_ASSERT(5 == get_string_length(str2));
  CU_ASSERT(13 == get_string_length(str3));
}
