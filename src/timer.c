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


double get_tod_in_mill(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
}


char *get_datetime(){
    char *ret = malloc(30 * sizeof(char));
    struct tm tim;
    time_t now = time(NULL);
    tim = *(localtime(&now));
    strftime(ret,30,"%b %d, %Y; %H:%M:%S\n",&tim);
    return ret;
}
