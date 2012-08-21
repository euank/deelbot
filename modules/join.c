#include <module.h> //All modules use this

void onconnect(msg_type type, const char *from, const char *to, strarr_t *parts, const char *remainder, const char *raw) {
  irc_cmd_join(session, "#deelbot", NULL);
}
