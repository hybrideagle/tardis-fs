#ifndef LOG_H
#define LOG_H

void init_logging();

FILE* log_file;

// Logging macros and constants
//#define START(args...) printf("(%s)[START]", __FILE__);printf(args);fflush(stdout);printf("\n");
#define START(args...) 
#define END(args...)   printf("[END]");printf(args);fflush(stdout);printf("\n");
#define LOG(args...)   printf("\t");printf(args);fflush(stdout);printf("\n");
#define LOG1(args...)  printf("\t\t");printf(args);fflush(stdout);printf("\n");

//Delimited assert
#define assertd(x) if(!(x))printf("\n\n###########\n");assert(x);
//assertation that prints out a numeric value on crash
#define numassert(x, num) if(!(x)) {printf("\n\n###########\n"); printf("%s:%d:assertion failed(%s:[%s=%d])", __FILE__, __LINE__, #x, #num, num); abort();}

#endif //LOG.H
