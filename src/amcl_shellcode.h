#ifndef __AMCL_SHELLCODE_H__
#define __AMCL_SHELLCODE_H__

/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * Execute the provided shellcode.
 *
 * @param shellcode The shellcode (as bytestring of opcodes) to be executed.
 */
void
exec_shellcode(
  const char *shellcode
);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

#endif
