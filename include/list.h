#ifndef _LIST_H
#define _LIST_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

#define LIST_REMOVE(type, arr, n)                                       \
{                                                                       \
    if (n != 0)                                                         \
    {                                                                   \
        memmove(arr->data, arr->data, n * sizeof(type));                \
    }                                                                   \
                                                                        \
    if (n != (arr->size - 1))                                           \
    {                                                                   \
        memmove(arr->data + n, arr->data + n + 1, (arr->size - n - 1) * sizeof(type)); \
        arr->size -= 1;                                                 \
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