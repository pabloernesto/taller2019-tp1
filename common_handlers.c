#include "handlers.h"

#include <search.h>
#include <stdlib.h>

Handler *get_handler(const char *key) {
  ENTRY *ret = hsearch((ENTRY){ .key=(char*) key }, FIND);
  return ret
    ? *(Handler **)(ret->data)
    : NULL;
}
