#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dirent.h>
#include <glob.h>
#include <string.h>
#include <dlfcn.h>
#include <libgen.h>
#include <errno.h>

#include <libircclient/libircclient.h>

irc_session_t *session; //Global. Modules need this too

/* This is the stub for all events. All events except CTCP RELOAD are passively passed through.
 * Reload is actively used to reload all event modules before it passes through as well
 */
typedef void (*eventInitStub)(irc_session_t *session);
typedef void (*eventStub)(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count);
void *evso;

void loadEventModules(char *name) { //{{{
  if(name == NULL)
    name = "events.so"; //poor man's default values :D
  void *m = dlopen(name, RTLD_NOW);
  if(m == NULL) {
    fprintf(stderr, "I can't find events.so or couldn't resolve all the symbols.");
    return;
  } else {
    printf("Successfully loaded the event.so. Pray it has everyhting right :D\n");
  }
  evso = m;
} //}}}

void addlog (const char * fmt, ...) //{{{
{
  FILE * fp; 
  char buf[1024];
  va_list va_alist;

  va_start (va_alist, fmt);
  vsnprintf (buf, sizeof(buf), fmt, va_alist);
  va_end (va_alist);

  printf ("%s\n", buf);

  if ( (fp = fopen ("irctest.log", "ab")) != 0 ) 
  {   
    fprintf (fp, "%s\n", buf);
    fclose (fp);
  }   
} //}}}

void dump_event (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) { //{{{
  char buf[512];
  int cnt;
  buf[0] = '\0';
  for ( cnt = 0; cnt < count; cnt++ ) {
    if ( cnt )
      strcat (buf, "|");
    strcat (buf, params[cnt]);
  }
  addlog ("Event \"%s\", origin: \"%s\", params: %d [%s]", event, origin ? origin : "NULL", cnt, buf);
} //}}}

void event_connect(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_connect");
  if(evfun == NULL) 
    printf("Event connect not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_join(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_join");
  if(evfun == NULL) 
    printf("Event join not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_nick(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_nick");
  if(evfun == NULL) 
    printf("Event nick not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_quit(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_quit");
  if(evfun == NULL) 
    printf("Event quit not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_part(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_part");
  if(evfun == NULL) 
    printf("Event part not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_mode(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_mode");
  if(evfun == NULL) 
    printf("Event mode not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_topic(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_topic");
  if(evfun == NULL) 
    printf("Event topic not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_kick(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_kick");
  if(evfun == NULL) 
    printf("Event kick not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_channel(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_channel");
  if(evfun == NULL) 
    printf("Event channel not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_privmsg(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_privmsg");
  if(evfun == NULL) 
    printf("Event privmsg not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_notice(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_notice");
  if(evfun == NULL) 
    printf("Event notice not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_invite(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_invite");
  if(evfun == NULL) 
    printf("Event invite not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_umode(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_umode");
  if(evfun == NULL) 
    printf("Event umode not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_ctcp_req(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  if(strcmp(params[0], "RELOAD") == 0) {
    printf("Reloading the main module and calling its init\n");
    loadEventModules(NULL);
    void *initFuns = dlsym(evso, "event_init");
    if(initFuns != NULL) {
      eventInitStub eis = (eventInitStub)initFuns;
      eis(session);
    }
  }



  void *evfun = dlsym(evso, "event_ctcp_req");
  if(evfun == NULL) 
    printf("Event ctcp_req not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_ctcp_rep(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_ctcp_rep");
  printf("Param[0] = %s\n", params[0]);
  if(evfun == NULL) 
    printf("Event ctcp_rep not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_ctcp_action(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_ctcp_action");
  if(evfun == NULL) 
    printf("Event ctcp_action not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}
void event_unknown(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  void *evfun = dlsym(evso, "event_unknown");
  if(evfun == NULL) 
    printf("Event unknown not found in events.so\n");
  else {
    eventStub es = (eventStub)evfun;
    es(session, event, origin, params, count);
  }
} //}}}

int main(int argc, char **argv) { //{{{
  if(argc == 1) {
    fprintf(stderr, "Must supply an irc server to connect to\n");
    return 1;
  }

  loadEventModules(NULL);
  irc_callbacks_t callbacks;
  memset(&callbacks, 0, sizeof(callbacks));

  callbacks.event_connect      = event_connect;
  callbacks.event_join         = event_join;
  callbacks.event_nick         = event_nick;
  callbacks.event_quit         = event_quit;
  callbacks.event_part         = event_part;
  callbacks.event_mode         = event_mode;
  callbacks.event_topic        = event_topic;
  callbacks.event_kick         = event_kick;
  callbacks.event_channel      = event_channel;
  callbacks.event_privmsg      = event_privmsg;
  callbacks.event_notice       = event_notice;
  callbacks.event_invite       = event_invite;
  callbacks.event_umode        = event_umode;
  callbacks.event_ctcp_req     = event_ctcp_req;
  callbacks.event_ctcp_rep     = event_ctcp_rep;
  callbacks.event_ctcp_action  = event_ctcp_action;
  callbacks.event_unknown      = event_unknown;
  //callbacks.event_numeric      = dump_event; //These events are of a different format
  //callbacks.event_dcc_chat_req = dump_event;
  //callbacks.event_dcc_send_req = dump_event;

  session = irc_create_session(&callbacks);
  if(!session) {
    fprintf(stderr, "Couldn't create session\n");
    return 1;
  }

  if(irc_connect(session, argv[1], 6667, 0, "K", "K", "K")) {
    fprintf(stderr, "Could not connect: %s\n", irc_strerror(irc_errno(session)));
    return 1;
  }


  void *initFuns = dlsym(evso, "event_init");
  if(initFuns != NULL) {
    eventInitStub eis = (eventInitStub)initFuns;
    eis(session);
  }
  irc_run(session);
  return 0;
} //}}}

