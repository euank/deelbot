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

#include "include/module.h"
#include "list.h"

typedef struct {
  struct list_head list;
  void *module;
  char *moduleName;
} module_list_t;

module_list_t modules;
module_list_t *tmp;

typedef void (*moduleStub)(msg_type type, const char *from, const char *to, strarr_t *parts, const char *remainder, const char *raw);
typedef void (*moduleInit)(irc_session_t *session);

void loadModules(char *path) { //{{{
  char globPath[500]; //arbitrary length limit; maximum for one folder deep is really 510 I think
  globPath[0] = '\0';
  if(path == NULL) path = "./modules/";
  strcat(globPath, path);
  strcat(globPath, "*.so"); //Only load shared objects. Lets me keep *.c in there too
  glob_t dir;
  printf("Loading modules in %s\n", globPath);
  switch(glob(globPath, 0, NULL, &dir)) { //GLOB_NO_DOTDIRS isn't in my version of glob.h. :(
    case 0: break;
    case GLOB_NOSPACE: 
            printf("Out of memory reading glob\n");
            break;
    case GLOB_ABORTED:
            printf("Reading error\n");
            break;
    case GLOB_NOMATCH:
            printf("No files in module glob\n");
            break;
  }

  int i;
  module_list_t *tmp;
  for(i=0;i<dir.gl_pathc;i++) {
    void *mod = dlopen((dir.gl_pathv[i]), RTLD_NOW);
    if(mod == NULL) {
      fprintf(stderr, "Error loading %s and errno=%d, msg=%s\n", basename(dir.gl_pathv[i]), errno, dlerror());
    } else {
      printf("Successfully opened %s.\n", dir.gl_pathv[i]);
    }

    tmp = malloc(sizeof(module_list_t));
    tmp->module = mod;
    char *name = basename(dir.gl_pathv[i]); //Not a new string. BAD
    char *newStr = malloc(strlen(name) + 1); //null byte
    strcpy(newStr, name);
    tmp->moduleName = newStr;
    list_add(&(tmp->list), &modules.list);
  }
} //}}} 

void event_join(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  list_for_each_entry(tmp, &modules.list, list) {
    void *fun = dlsym(tmp->module, "onjoin");
    if(fun == NULL) continue;
    printf("Found a matching sig for onjoin in %s\n", tmp->moduleName);
    moduleStub ms = (moduleStub)fun;

    strarr_t *pars = malloc(sizeof(strarr_t));
    pars->strs = params;
    pars->length = count;
    ms(OTHER, NULL, params[0], pars, NULL, event);
  }
}//}}}


void event_channel(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  list_for_each_entry(tmp, &modules.list, list) {
    void *fun = dlsym(tmp->module, "onchannel");
    if(fun == NULL) continue;
    printf("Found a matching sig for onchannel in %s\n", tmp->moduleName);
    moduleStub ms = (moduleStub)fun;

    strarr_t *pars = malloc(sizeof(strarr_t));
    pars->strs = params;
    pars->length = count;
    ms(MSG, origin, params[0], pars, params[1], event);
  }
}//}}}

void event_connect(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) { //{{{
  list_for_each_entry(tmp, &modules.list, list) {
    void *fun = dlsym(tmp->module, "onconnect");
    if(fun == NULL) continue;
    printf("Found a matching sig for onconnect in %s\n", tmp->moduleName);
    moduleStub ms = (moduleStub)fun;

    strarr_t *pars = malloc(sizeof(strarr_t));
    pars->strs = params;
    pars->length = count;
    ms(CONNECT, origin, params[0], pars, NULL, event);
  }
}//}}}


void event_init(irc_session_t *session) {
  INIT_LIST_HEAD(&modules.list);
  loadModules(NULL);

  list_for_each_entry(tmp, &modules.list, list) {
    void *fun = dlsym(tmp->module, "init");
    if(fun == NULL) continue;
    printf("Found a matching sig for init in %s\n", tmp->moduleName);
    moduleInit mi = (moduleInit)fun;
    mi(session);
  }
}
