#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "strutils.h"
#include "remote.h"
#include "logging.h"
#include "miscutils.h"


int
socket_create(
  protocol_t protocol_type,
  int *socket_fd_ptr
) {
  int ret = 0;

  errno = 0;
  if (protocol_type == TCP) {
    *socket_fd_ptr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  } else if (protocol_type == UDP) {
    *socket_fd_ptr = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  } else {
    ret = -2;
  }
  if (!ret && ((*socket_fd_ptr) < 0)) {
    ret = -1;
    log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Setup Socket (socket)", strerror(errno));
  }

  return ret;
}

int
socket_bind(
  const socket_address_t *local_address,
  int socket_fd
) {
  int ret = 0;

  errno = 0;
  if (bind(socket_fd, (struct sockaddr *) local_address,
        sizeof(*local_address)) < 0) {
    ret = -1;
    log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Setup Socket (bind)", strerror(errno));
  }

  return ret;
}

int
socket_listen(
  int socket_fd
) {
  int ret = 0;

  errno = 0;
  if (listen(socket_fd, 5) < 0) {
    log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Setup Socket (listen)", strerror(errno));
    ret = -1;
  }
  return ret;
}

int
setup_passive_socket(
  protocol_t protocol_type,
  const socket_address_t *local_address,
  int *socket_fd_ptr
) {
  int ret = 0;

  ret = socket_create(protocol_type, socket_fd_ptr) < 0 ? -1 : 0;
  if (!ret) {
    ret = socket_bind(local_address, *socket_fd_ptr) < 0 ? -2 : 0;
  }
  if (!ret && protocol_type == TCP) {
    socket_listen(*socket_fd_ptr) < 0 ? -3 : 0;
  }

  return ret;
}

int
setup_active_socket(
  protocol_t protocol_type,
  int *socket_fd_ptr
) {
  socket_address_t local_address;
  int ret;

  /* {{{ Create the client address. */
  memset(&local_address, 0, sizeof(local_address));
  local_address.sin_family = AF_INET;
  local_address.sin_addr.s_addr = INADDR_ANY; /* Any IP address on this host. */
  local_address.sin_port = 0; /* Ephemere port. */
  /* }}} */

  ret = socket_create(protocol_type, socket_fd_ptr) < 0 ? -1 : 0;
  if (!ret) {
    ret = socket_bind(&local_address, *socket_fd_ptr) < 0 ? -2 : 0;
  }

  return ret;
}

int server_handler(
  int master_socket_fd,
  policy_t accept_error_policy,
  int (*fork_error_handler)(int, socket_address_t *),
  int (*slave_logic)(int, socket_address_t *)
) {
  int ret = 0;
  int accept_conn;
  socklen_t accept_conn_length = sizeof(accept_conn);
  int slave_socket_fd;
  socket_address_t client_address;
  socklen_t client_address_length = sizeof(client_address);
  pid_t slave_pid;

  /* {{{ Check if the socket is accepting connections. */
  errno = 0;
  if (getsockopt(master_socket_fd, SOL_SOCKET, SO_ACCEPTCONN, &accept_conn,
        &accept_conn_length) < 0) {
    ret = -1;
    log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Server Handler",
            "Cannot check if the socket is listening");
  } else if (accept_conn) {
    log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Server Handler",
            "The socket is listening");
  } else {
    ret = -2;
    log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Server Handler",
            "The socket isn't listening");
  }
  /* }}} */

  while(TRUE) {
    /* {{{ Accept a connection from a client. */
    errno = 0;
    slave_socket_fd = accept(master_socket_fd, (struct sockaddr*)
        &client_address, &client_address_length);
    if (slave_socket_fd < 0) {
      if (errno == EINTR) { /* System call interrupted. */
        log_msg(LOG_LVL_WRN, LOG_FAC_STD, "Server Handler",
                "A signal interrupted the accept");
        continue; /* The connection will be picked up in the next accept. */
      } else { /* A "real" error. */
        if (accept_error_policy == STOP) { /* STOP policy. */
          ret = -3;
          log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Server Handler",
                  "A bad accept caused the handler to stop");
          break;
        } else { /* CONTINUE policy (Skip the client connection). */
          log_msg(LOG_LVL_WRN, LOG_FAC_STD, "Server Handler", "Skipped the "
                  "current client connection because of a bad accept");
          continue;
        }
      }
    }
    /* }}} */

    /* {{{ Create a slave and give the client responsibility to the slave. */
    if (!ret) {
      errno = 0;
      slave_pid = fork();
      if (slave_pid < 0) {
        if ((*fork_error_handler)(slave_socket_fd, &client_address) < 0) {
          ret = -4;
          log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Server Handler",
              "Failed to handle the fork error");
        } else {
          log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Server Handler",
                  "Handled the fork error");
        }
      } else if (slave_pid == 0) {
        if (close(master_socket_fd) < 0) {
          log_msg(LOG_LVL_WRN, LOG_FAC_STD, "Slave", "Failed to close"
              " the master socket: fd=%d", master_socket_fd);
        }
        ret = (*slave_logic)(slave_socket_fd, &client_address);
        if (ret) {
          log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Slave",
                  "Failed to handle the client");
        }
        if (close(slave_socket_fd) < 0) {
          log_msg(LOG_LVL_WRN, LOG_FAC_STD, "Slave",
                  "Failed to close the slave socket: fd=%d", slave_socket_fd);
        }
        exit((ret == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
      } else {
        errno = 0;
        if (close(slave_socket_fd) < 0) {
          log_msg(LOG_LVL_WRN, LOG_FAC_STD, "Server Handler",
                  "Failed to close the slave socket: fd=%d", slave_socket_fd);
        }
      }
    }
    /* }}} */
  }

  if (close(master_socket_fd) < 0) {
    log_msg(LOG_LVL_WRN, LOG_FAC_STD, "Server Handler",
            "Failed to close the master socket: fd=%d", master_socket_fd);
  }

  return ret;
}

int
client_handler(
  int client_socket_fd,
  socket_address_t master_address,
  unsigned int retry_count
) {
  int ret = 0;
  int accept_conn;
  socklen_t accept_conn_length = sizeof(accept_conn);

  /* {{{ Check if the socket descriptor is valid. */
  errno = 0;
  if (getsockopt(client_socket_fd, SOL_SOCKET, SO_DEBUG, &accept_conn,
        &accept_conn_length) < 0) {
    ret = -1;
    log_msg(LOG_LVL_ERR, LOG_FAC_STD, "Client Handler", "Socket is invalid");
  } else {
    log_msg(LOG_LVL_DBG, LOG_FAC_STD, "Client Handler", "Socket is valid");
  }
  /* }}} */

  /* {{{ Connect to the master. */
  if (!ret) {
    errno = 0;
    ret = connect(client_socket_fd, (struct sockaddr *) &master_address,
                  sizeof(master_address));
    if (ret != 0) { /* connect() failed. */
      if (retry_count > 0 && errno == EINTR) {
        ret = client_handler(client_socket_fd, master_address, retry_count - 1);
      } else {
        ret = -2;
      }
    } else { /* connect() success. */
      log_msg(LOG_LVL_DBG, LOG_FAC_STD, "Client Handler",
              "Connected to the socket: %d:%d",
              master_address.sin_addr, master_address.sin_port);
    }
  }
  /* }}} */

  /* {{{ Call the client logic. */
  if (!ret) {
    /* TODO: implement. */
  }
  /* }}} */

  if (close(client_socket_fd) < 0) {
    log_msg(LOG_LVL_WRN, LOG_FAC_STD, "Client Handler",
            "Failed to close the client socket: fd=%d", client_socket_fd);
  }

  return ret;
}

