#ifndef __REMOTE_H__
#define __REMOTE_H__


/**
 * Possible protocols that can be used
 */
typedef enum protocol {
  TCP,  /**<TCP Protocol */
  UDP   /**<UDP Protocol */
} protocol_t;

/**
 * The socket address
 */
typedef struct sockaddr_in socket_address_t;

/**
 * The policy to be used in an exceptional case
 */
typedef enum policy {
  STOP,     /**< Stop the execution */
  CONTINUE  /**< Continue the execution */
} policy_t;


/**
 * Create a new socket
 *
 * @param protocol_type The protocol that must be used
 * @param socket_fd_ptr The socket file description pointer. It is
 *   set if the socket creation was successful, otherwise the pointer
 *   is set to NULL
 *
 * @return Status
 *   - 0 => Success
 *   - -1 => Socket creation error (errno indicates the error kind)
 *   - -2 => Invalid protocol type
 */
int socket_create(protocol_t protocol_type, int* socket_fd_ptr);

/**
 * Bind the local_address to the socket
 *
 * @param socket_fd The descriptor for the socket to be binded
 *   with the local_address
 * @param local_address The local address for the socket
 *
 * @return Status
 *   - 0 => Success
 *   - -1 => Error while binding the address to the socket
 *       (errno indicates the error kind)
 */
int socket_bind(const socket_address_t* local_address, int socket_fd);

/**
 * Puts the socket in the listening state
 * 
 * @param socket_fd The descriptor for the socket to bring in
 *   the listening state
 *
 * @return Status
 *   - 0 => Success
 *   - -1 => Error while trying to enter in the listen state
 *       (errno indicates the error kind)
 */
int socket_listen(int socket_fd);

/**
 * Setup a new "passive" socket:
 *   - Create the socket
 *   - Bind it to the local address
 *   - Put the socket in the listening state
 *
 * @param protocol_type The protocol that must be used
 * @param server_address The local address to bind to the socket
 * @param socket_fd_ptr Output argument set to the socket descriptor,
 *   if the socket has been created successfully, otherwise to NULL
 * @return Status
 *   - 0 => Success
 *   - -1 => Error while creating the socket
 *   - -2 => Error while binding the local address to the socket
 *   - -3 => Error while trying to put the socket in the listen state
 */
int setup_passive_socket(protocol_t protocol_type,
    const socket_address_t* local_address, int* socket_fd_ptr);

/**
 * Setup a new "active" socket
 *   - Create the socket
 *   - Bind it to an ephemera port
 */
int setup_active_socket(protocol_t protocol_type, int* socket_fd_ptr);

/**
 * Handler for client connection requests
 *
 * @param master_socket_fd The socket descriptor for the master
 *   (the receptionist)
 * @param accept_error_policy The policy that should be taken when
 *   bad things happen while accepting a client connection
 * @param fork_error_handler The function handler that handles
 *   the slave creation failure
 * @param slave_logic The slave business-logic which handles the interaction
 *   with the client
 *
 * @return Status
 *   - 0 => Success
 *   - -1 => Cannot check if the socket is listening
 *   - -2 => The socket isn't listening
 *   - -3 => A bad accept caused the handler to stop
 *   - -4 => The fork_error_handler failed
 */
int server_handler(int master_socket_fd, policy_t accept_error_policy,
    int (*fork_error_handler)(int, socket_address_t*),
    int (*slave_logic)(int, socket_address_t*));
/**
 * Handle the communication with a server
 * 
 * @param client_socket_fd The socket descriptor for the client
 * @param master_address The address for the master
 *   (the "receptionist" in the server-side)
 * @param retry_count The number of times that the client retries to
 *   connect if there are some "soft" connection errors
 *
 * @return Status
 *   - 0 => Success
 *   - -1 => The socket is invalid
 *   - -2 => Cannot connect the socket to the master_address
 */
int client_handler(int client_socket_fd, socket_address_t master_address,
    unsigned int retry_count);


#endif

