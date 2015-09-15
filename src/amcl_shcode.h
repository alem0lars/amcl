#ifndef __AMCL_SHCODE_H__
#define __AMCL_SHCODE_H__

/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * Execute the provided shellcode.
 *
 * @param shellcode The shellcode (as bytestring of opcodes) to be executed.
 */
void
exec_shcode(
  const char *shcode
);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

#endif
