#ifndef HEADERS_H
#define HEADERS_H
#include "../src/headers.h"
#endif

typedef char* FTint;
typedef char* FTlong_int;
typedef char* FTshort_int;
typedef char* FTfloat;
typedef char* FTdouble;
typedef char* FTstring;

typedef struct FTpointers_list FTpointers_list;

FT_pointers_list* crFT_pointers_list(void* ptr/* , char* ptr_type */);
void drFT_pointers_list(FT_pointers_list* list);

FTint wrap_int(int, FTpointers_list*);
FTlong_int wrap_long_int(long, FTpointers_list*);
FTfloat wrap_float(float, FTpointers_list*);
FTdouble wrap_double(double, FTpointers_list*);

int unwrap_int(FTint*);
long unwrap_long_int(FTlong_int*);
float unwrap_float(FTfloat*);
double unwrap_double(FTdouble*);
