#ifndef IRCBOT_MODULE_H
#define IRCBOT_MODULE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dirent.h>
#include <string.h>
//Buggy irc lib needs some of those to be included before it

#include <libircclient/libircclient.h>

extern irc_session_t *session;

typedef struct {
   char 	* channel;
   char 	* nick;

} irc_ctx_t;

typedef struct {
   const char **strs;
   unsigned int length;
} strarr_t;

typedef enum {
   PM,
   NOTICE,
   MSG,
   CONNECT,
   OTHER
} msg_type;

#endif
