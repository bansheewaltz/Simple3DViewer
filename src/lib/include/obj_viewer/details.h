#ifndef DETAILS_H_
#define DETAILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void objviewer_iarr_lines_sort(unsigned int* arr, size_t len);
unsigned int* objviewer_iarr_lines_clean(unsigned int* arr, size_t len,
                                         size_t* newlen);
void objviewer_iarr_lines_flip(unsigned int* arr, size_t len);

#ifdef __cplusplus
}
#endif

#endif  // DETAILS_H_
