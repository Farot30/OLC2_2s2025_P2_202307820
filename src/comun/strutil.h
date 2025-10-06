#ifndef STRUTIL_H
#define STRUTIL_H
#include <string.h>
#include <stdlib.h>
static inline char* dupstr_local(const char* s){
    if(!s) return NULL;
    size_t n = strlen(s);
    char* p = (char*)malloc(n+1);
    if(p) memcpy(p,s,n+1);
    return p;
}
#ifndef strdup
#define strdup dupstr_local
#endif
#endif