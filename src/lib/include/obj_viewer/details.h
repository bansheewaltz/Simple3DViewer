#ifndef DETAILS_H_
#define DETAILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void objviewer_sort_lines_index_arr(unsigned int* arr, size_t len);
unsigned int* objviewer_delete_index_duplicates(unsigned int* arr, size_t len,
                                                size_t* newlen);
void objviewer_flip_line_indices(unsigned int* arr, size_t len);

#ifdef __cplusplus
}
#endif

#endif  // DETAILS_H_
