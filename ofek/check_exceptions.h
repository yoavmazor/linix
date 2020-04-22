#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


#define NO_ERROR_OCCURRED (EXIT_SUCCESS)
#define ERROR_OCCURRED (EXIT_FAILURE)

#define CHECK_TRUE(exp) do { if (!(exp)) { printf("ERROR: file - %s; line -%d\n", __FILE__, __LINE__); goto exit; } } while (false)

#define CHECK_AND_PASS(exp) do { if ((exp) == ERROR_OCCURRED) { printf("ERROR: file - %s; line -%d\n", __FILE__, __LINE__); goto exit; } } while (false)

typedef int ERROR_STATUS;


#endif

