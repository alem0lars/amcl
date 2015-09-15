#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../src/amcl_remote.h"
#include "../src/amcl_log.h"


int
suite_remote_init(
) {
  return 0;
}

int
suite_remote_cleanup(
) {
  return 0;
}

void
test_socket_create(
) {
  int socket_fd;
  /* {{{ Success with TCP. */
  CU_ASSERT(socket_create(TCP, &socket_fd) == 0);
  close(socket_fd);
  /* }}} */
  /* {{{ Success with UDP. */
  CU_ASSERT(socket_create(UDP, &socket_fd) == 0);
  close(socket_fd);
  /* }}} */
  /* {{{ Fail with invalid protocol_type. */
  CU_ASSERT(socket_create(-1, &socket_fd) == -2);
  close(socket_fd);
  /* }}} */
}

void
test_socket_bind(
) {
  int socket_fd;
  socket_address_t address, address2;
  memset(&address, 0, sizeof(address));
  memset(&address2, 0, sizeof(address2));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = 0;

  /* {{{ Success with valid TCP socket and valid address. */
  if (socket_create(TCP, &socket_fd)) {
    CU_ASSERT(socket_bind(&address, socket_fd) == 0);
    close(socket_fd);
  }
  /* }}} */
  /* {{{ Success with valid UDP socket and valid address. */
  if (socket_create(UDP, &socket_fd)) {
    CU_ASSERT(socket_bind(&address, socket_fd) == 0);
    close(socket_fd);
  }
  /* }}} */
  /* {{{ Fail with invalid address but valid TCP socket. */
  if (socket_create(TCP, &socket_fd)) {
    memset(&address2, 0, sizeof(address2));
    CU_ASSERT(socket_bind(&address, socket_fd) == -1);
    close(socket_fd);
  }
  /* }}} */
  /* {{{ Fail with invalid address but valid UDP socket. */
  if (socket_create(UDP, &socket_fd)) {
    CU_ASSERT(socket_bind(&address2, socket_fd) == -1);
    close(socket_fd);
  }
  /* }}} */
  /* {{{ Fail with invalid socket descriptor. */
  close(socket_fd);
  CU_ASSERT(socket_bind(&address, socket_fd) == -1);
  /* }}} */
}

void
test_socket_listen(
) {
  int socket_fd;
  socket_address_t address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = 0;

  /* {{{ Success with valid TCP socket and explicit bind. */
  if (!socket_create(TCP, &socket_fd)
      && !socket_bind(&address, socket_fd)) {
    CU_ASSERT(socket_listen(socket_fd) == 0);
    close(socket_fd);
  }
  /* }}} */
  /* {{{ Fail with valid UDP socket and explicit bind. */
  if (!socket_create(UDP, &socket_fd)
      && !socket_bind(&address, socket_fd)) {
    CU_ASSERT(socket_listen(socket_fd) == -1);
    close(socket_fd);
  }
  /* }}} */
  /* {{{ Success with valid TCP socket and implicit bind. */
  if (!socket_create(TCP, &socket_fd)) {
    CU_ASSERT(socket_listen(socket_fd) == 0);
    close(socket_fd);
  }
  /* }}} */
  /* {{{ Fail with valid UDP socket and implicit bind. */
  if (!socket_create(UDP, &socket_fd)) {
    CU_ASSERT(socket_listen(socket_fd) == -1);
    close(socket_fd);
  }
  /* }}} */
  /* {{{ Fail with invalid socket descriptor. */
  close(socket_fd);
  CU_ASSERT(socket_listen(socket_fd) == -1);
  /* }}} */
}

void
test_setup_passive_socket(
) {
  int socket_fd;
  socket_address_t address, address2;
  memset(&address, 0, sizeof(address));
  memset(&address2, 0, sizeof(address2));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = 0;

  /* {{{ Fail the socket create with invalid protocol. */
  CU_ASSERT(setup_passive_socket(-1, &address2, &socket_fd) == -1);
  close(socket_fd);
  /* }}} */
  /* {{{ Fail the bind with TCP and invalid address. */
  CU_ASSERT(setup_passive_socket(TCP, &address2, &socket_fd) == -2);
  close(socket_fd);
  /* }}} */
  /* {{{ Fail the bind with UDP and invalid address. */
  CU_ASSERT(setup_passive_socket(UDP, &address2, &socket_fd) == -2);
  close(socket_fd);
  /* }}} */
}

void
test_setup_active_socket(
) {
  int socket_fd;

  /* {{{ Success with TCP. */
  CU_ASSERT(setup_active_socket(TCP, &socket_fd) == 0);
  close(socket_fd);
  /* }}} */
  /* {{{ Success with TCP. */
  CU_ASSERT(setup_active_socket(UDP, &socket_fd) == 0);
  close(socket_fd);
  /* }}} */
  /* {{{ Fail with invalid protocol. */
  CU_ASSERT(setup_active_socket(-1, &socket_fd) == -1);
  close(socket_fd);
  /* }}} */
}

/**
 * Slave business-logic.
 *
 * @note This isn't a test method, but it's used inside test_tcp().
 */
int
slave_logic(
  int slave_socket_fd,
  socket_address_t *slave_address
) {
  log_msg(LOG_LVL_DBG, LOG_FAC_STD, "Slave", "Handling a client...");
  return 0;
}

int
test_tcp(
) {
  int ret = 0;
  pid_t server_pid;
  socket_address_t server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY; /* Any IP addr on this host. */
  server_address.sin_port = htons(34732);

  log_msg(LOG_LVL_DBG, LOG_FAC_STD, "test_tcp()", "Started");

  server_pid = fork();
  if (server_pid < 0) {
    ret = 1;
  } else if (server_pid == 0) {
    int master_socket_fd;
    ret =  setup_passive_socket(TCP, &server_address, &master_socket_fd);
    if (!ret) { /* Test address already in use. */
      sleep(1);
      ret =  setup_passive_socket(TCP, &server_address, &master_socket_fd);
    }
    if (!ret) { /* Test server handler. */
      ret = server_handler(master_socket_fd, STOP, &slave_logic, &slave_logic);
    }
    exit((ret == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
  }

  int client_socket_fd;
  ret =  setup_active_socket(TCP, &client_socket_fd)
      || client_handler(client_socket_fd, server_address, 0);

  log_msg(LOG_LVL_DBG, LOG_FAC_STD, "test_tcp()", "Finished");
  return ret;
}
