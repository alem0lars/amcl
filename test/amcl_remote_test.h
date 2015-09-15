#ifndef __AMCL_REMOTE_TEST_H__
#define __AMCL_REMOTE_TEST_H__

/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

int
suite_remote_init(
  void
);

int
suite_remote_cleanup(
  void
);

void
test_socket_create(
  void
);

void
test_socket_bind(
  void
);

void
test_socket_listen(
  void
);

void
test_setup_passive_socket(
  void
);

void
test_setup_active_socket(
  void
);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * Test-cases definition for the 'amcl_remote' module.
 */
CU_TestInfo testcases_remote[] = {
  { "test_socket_create",        test_socket_create },
  { "test_socket_bind",          test_socket_bind },
  { "test_socket_listen",        test_socket_listen },
  { "test_setup_passive_socket", test_setup_passive_socket },
  { "test_setup_active_socket",  test_setup_active_socket },
  CU_TEST_INFO_NULL,
};

#endif
