#include <sys/time.h>
#include "covidTrace.h"

struct timeval tic(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}


double toc(struct timeval begin){
    struct timeval end;
    gettimeofday(&end, NULL);
    double stime = ((double)(end.tv_sec - begin.tv_sec) * 1000) +
                   ((double)(end.tv_usec - begin.tv_usec) / 1000);
    stime = stime / 1000;
    return (stime);
}


int get_seconds_of_tod(){
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec;
}


int get_useconds_of_tod(){
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec;
}