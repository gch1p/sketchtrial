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
static int nstime_calls_left = 3;
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
    if (result == 0 && !done) {
        pthread_mutex_lock(&mutex);
        if (nstime_calls_left > 0) {
            void *callstack[128];
            int frames = backtrace(callstack, 128);
            char **strs = backtrace_symbols(callstack, frames);
            
            bool is_sketch = false, is_nstime = false, is_cfnet = false, is_nt = false;
            int i;

            for (i = 0; i < frames; ++i) {
                parse_bt_source(strs[i], buf, 32);
                if (!is_nstime && strstr(strs[i], "NSDate") != NULL) {
                    is_nstime = true;
                }
                if (!is_nt && strstr(strs[i], "NSNotification") != NULL) {
                    is_nt = true;
                }
                if (!is_sketch && !strcmp(buf, "Sketch")) {
                    is_sketch = true;
                }
                if (!is_cfnet && !strcmp(buf, "CFNetwork")) {
                    is_cfnet = true;
                }
            }

            if (is_sketch && is_nstime && !is_cfnet && !is_nt) {
                tp->tv_sec += days * 86400;
                tp->tv_usec = tp->tv_sec * 1000000;
                nstime_calls_left--;

                printf("spoofed time on call stack:\n");
                for (i = 0; i < frames; i++) {
                    printf("    %s\n", strs[i]);
                }
                printf("\n");
            }
            
            free(strs);
            if (!nstime_calls_left) {
                done = true;
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    return result;
}
