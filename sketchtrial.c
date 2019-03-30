#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int calls_left = 9;
static bool done = false;
static char buf[32];
static long days = -5000;

// pointer to the original function
int (*orig_gettimeofday) (struct timeval *restrict tp, void *restrict tzp) = NULL;

void parse_bt_source(char* src, char* dst, int len) {
    int pos = 0;
    int state = 0;
    for (char *c = src; *c != '\0' && pos < len-1; c++) {
        if (!state) {
            if (*c == ' ') {
                state = 1;
            }
            continue;
        }
        if (state == 1) {
            if (*c != ' ') {
                state = 2;
            }
        }
        if (state == 2) {
            if (*c != ' ') {
                dst[pos++] = *c;
            } else {
                dst[pos] = '\0';
                break;
            }
        }
    }
}

int gettimeofday(struct timeval *restrict tp, void *restrict tzp) {
    if (!orig_gettimeofday) {
        orig_gettimeofday = dlsym(RTLD_NEXT, "gettimeofday");
    }

    int result = orig_gettimeofday(tp, tzp);
    if (result != 0 || done) {
        return result;
    }

    pthread_mutex_lock(&mutex);
    if (!calls_left) {
        goto end;
    }

    void *callstack[128];
    int frames = backtrace(callstack, 128);
    char **strs = backtrace_symbols(callstack, frames);
    int i;
    bool call_sketch = false;
    bool call_nsdate = false;

    for (i = 0; i < frames; ++i) {
        parse_bt_source(strs[i], buf, 32);
        if (strstr(strs[i], "NSNotification") != NULL || !strcmp(buf, "CFNetwork")) {
            call_sketch = false;
            break;
        }
        if (!call_nsdate && strstr(strs[i], "NSDate") != NULL) {
            call_nsdate = true;
        }
        if (!call_sketch && !strcmp(buf, "Sketch")) {
            call_sketch = true;
        }
    }

    if (call_sketch && call_nsdate) {
        tp->tv_sec += days * 86400;
        tp->tv_usec = tp->tv_sec * 1000000;
        calls_left--;

        printf("spoofed time on call stack:\n");
        for (i = 0; i < frames; i++) {
            printf("    %s\n", strs[i]);
        }
        printf("\n");
    }

    free(strs);
    if (!calls_left) {
        done = true;
    }

end:
    pthread_mutex_unlock(&mutex);

    return result;
}
