#pragma once


/*
 * return from checkpassword
*/
#define PASS_NOT_S 0
#define PASS_LEN 1
#define PASS_N 2
#define PASS_M 3
#define OK_PASSWORD 4

/*
 * parameter
*/

#define MIN_NUMM 1
#define MIN_NUMN 4
#define MINSTRLEN 8

int checkpassword(const char *, const char *);

