#include <module.h> //All modules use this

void onchannel(msg_type type, const char *from, const char *to, strarr_t *parts, const char *remainder, const char *raw) {
  irc_cmd_msg(session, to, remainder);
}
