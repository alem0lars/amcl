#ifndef __REMOTE_TEST_H__
#define __REMOTE_TEST_H__


void test_socket_create(void);
void test_socket_bind(void);
void test_socket_listen(void);
void test_setup_passive_socket(void);
void test_setup_active_socket(void);

int suite_remote_init(void);
int suite_remote_cleanup(void);

CU_TestInfo testcases_remote[] = {
  { "test_socket_create", test_socket_create },
  { "test_socket_bind",   test_socket_bind },
  { "test_socket_listen", test_socket_listen },
  { "test_setup_passive_socket", test_setup_passive_socket },
  { "test_setup_active_socket", test_setup_active_socket },
  CU_TEST_INFO_NULL,
};


#endif

