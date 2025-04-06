#ifndef _LIST_H
#define _LIST_H

#include <stddef.h>

#define LIST(type, arr, n)                                              \
{                                                                       \
    arr = malloc(sizeof(type));                                         \
    if (arr != NULL)                                                    \
    {                                                                   \
        arr->data = calloc(n, sizeof(type*));                           \
        arr->size = n;                                                  \
    }                                                                   \
}                                                                       \

#define LIST_GET(arr, n, ret)                                           \
{                                                                       \
    if (arr != NULL && n < arr->size)                                   \
        ret = arr->data[n];                                             \
}                                                                       \

#define LIST_SET(type, arr, n, value)                                   \
{                                                                       \
    if (arr != NULL)                                                    \
    {                                                                   \
        if (n >= arr->size)                                             \
        {                                                               \
            type** data = realloc(arr->data,                            \
                                  (arr->size + 1) * sizeof(type*));     \
            if (data != NULL)                                           \
            {                                                           \
                arr->data = data;                                       \
                arr->size += 1;                                         \
            }                                                           \
        }                                                               \
        arr->data[n - 1] = value;                                       \
    }                                                                   \
}                                                                       \

#define LIST_FREE(arr)                                                  \
{                                                                       \
    if (arr != NULL)                                                    \
    {                                                                   \
        for (size_t i = 0; i < arr->size; ++i)                          \
            free(arr->data[i]);                                         \
                                                                        \
        free(arr->data);                                                \
        free(arr);                                                      \
    }                                                                   \
}                                                                       \

#endif