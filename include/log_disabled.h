#ifndef LOG_H
#define LOG_H

void init_logging();

FILE* log_file;

// Logging macros and constants
#define START(args...)
#define END(args...)
#define LOG(args...)
#define LOG1(args...) 

//Delimited assert
#define assertd(x) if(!(x))printf("\n\n###########\n");assert(x);
//assertation that prints out a numeric value on crash
#define numassert(x, num) if(!(x)) {printf("\n\n###########\n"); printf("%s:%d:assertion failed(%s:[%s=%d])", __FILE__, __LINE__, #x, #num, num); abort();}

#endif //LOG.H
