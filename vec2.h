/****************************************************************************/
/* vec2.h --- vec2 (fixed block vector for C language)                      */
/****************************************************************************/

#ifndef KATAHIROMZ_VEC2_H
#define KATAHIROMZ_VEC2_H    2  /* Version 2 */

#ifdef __cplusplus
    #include <cstdlib>
    #include <cstring>
    #include <cassert>
#else
    #include <stdlib.h>
    #include <string.h>
    #include <assert.h>
    #include <stdbool.h>    /* for bool, false, true */
#endif

/****************************************************************************/
/* types */

/* VEC2 and PVEC2 */
typedef struct VEC2
{
    void *  items;          /* Not malloc'ed. It's a fixed block. */
    size_t  num_items;      /* number of items alive */
    size_t  capacity;       /* number of items allocated */
    size_t  size_per_item;  /* the size of one item */
} VEC2, *PVEC2;

/* NOTE: VEC2_FOREACH_FN returns false to cancel operation. */
typedef bool (*VEC2_FOREACH_FN)(size_t index0, void *pitem);

/* NOTE: VEC2_ITEM_COMPARE_FN returns 0 if equal; -1 if less; 1 if greater. */
typedef int (*VEC2_ITEM_COMPARE_FN)(const void *pitem1, const void *pitem2);

/****************************************************************************/
/* Do you wanna status return? */

#ifdef VEC2_QUICK_BUT_RISKY
    /* speedy but dangerous */
    typedef void vec2_bool;
    #define VEC2_STATUS_INIT(ret,value)  /* empty */
    #define VEC2_STATUS_SET(ret,value)   /* empty */
    #define VEC2_STATUS_RETURN(ret)      /* empty */
#else
    /* safer but slow */
    typedef bool vec2_bool;
    #define VEC2_STATUS_INIT(ret,value)  vec2_bool ret = (value)
    #define VEC2_STATUS_SET(ret,value)   ret = (value)
    #define VEC2_STATUS_RETURN(ret)      return ret
#endif

/****************************************************************************/
/* C/C++ switching */

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************/
/* functions */

#define vec2_init(pv,spi)   ERROR_You_cannot_use_vec2_init_You_lose

vec2_bool vec2_construct(PVEC2 pv, size_t size_per_item,
                         size_t capacity, void *items, size_t num_items);
void vec2_destroy(PVEC2 pv);
void vec2_clear(PVEC2 pv);

/* NOTE: You cannot use vec2_new(). */
#define vec2_new(spi)       ERROR_You_cannot_use_vec2_new_You_lose
/* NOTE: You cannot use vec2_delete(). */
#define vec2_delete(pv)     ERROR_You_cannot_use_vec2_delete_You_lose

void *vec2_get_at(PVEC2 pv, size_t index0);
void vec2_set_at(PVEC2 pv, size_t index0, const void *pitem);

vec2_bool vec2_copy(PVEC2 dest, const VEC2 *src);

vec2_bool vec2_assign(PVEC2 pv, size_t count, const void *pitem,
                      size_t size_per_item);

vec2_bool vec2_resize(PVEC2 pv, size_t count, const void *pitem);
vec2_bool vec2_reserve(PVEC2 pv, size_t capacity);
vec2_bool vec2_reserve_2(PVEC2 pv, size_t capacity, size_t size_per_item);
void vec2_shrink_to_fit(PVEC2 pv);

void vec2_foreach(PVEC2 pv, VEC2_FOREACH_FN fn);
void vec2_foreach_reverse(PVEC2 pv, VEC2_FOREACH_FN fn);

void
vec2_foreach_range(PVEC2 pv, VEC2_FOREACH_FN fn, size_t index0, size_t count);

void *vec2_find(PVEC2 pv, const void *pitem, VEC2_ITEM_COMPARE_FN compare);

#ifndef MISRA_C
    /* NOTE: vec2_bsearch() and vec2_sort() aren't available in MISRA-C. */
    void *
    vec2_bsearch(PVEC2 pv, const void *pitem, VEC2_ITEM_COMPARE_FN compare);

    void vec2_sort(PVEC2 pv, VEC2_ITEM_COMPARE_FN compare);
#endif  /* ndef MISRA_C */

vec2_bool vec2_insert(PVEC2 pv, size_t index0, size_t count, const void *pitem);
vec2_bool vec2_insert_sub(PVEC2 pv, size_t index0, const VEC2 *psubvec);

vec2_bool vec2_erase(PVEC2 pv, size_t index0);
vec2_bool vec2_erase_range(PVEC2 pv, size_t index0, size_t count);

vec2_bool vec2_push_back(PVEC2 pv, const void *pitem);
vec2_bool vec2_pop_back(PVEC2 pv);

void vec2_swap(PVEC2 pv1, PVEC2 pv2);

/* validation for debugging */
bool vec2_valid(const VEC2 *pv);

#ifndef NDEBUG
    void *vec2_item(PVEC2 pv, size_t index0);
    const void *vec2_const_item(const VEC2 *pv, size_t index0);
#endif

/****************************************************************************/
/* function macros */

#define vec2_data(pv)              ((pv)->items)
#define vec2_empty(pv)             ((pv)->num_items == 0)
#define vec2_size(pv)              ((pv)->num_items)
/* NOTE: vec2's capacity is not changable. */
#define vec2_capacity(pv)          (*(const size_t *)&(pv)->capacity)
#define vec2_size_per_item(pv)     ((pv)->size_per_item)

#ifdef NDEBUG
    #define vec2_item(pv,index0) ( \
        (void *)( \
            ((char *)(pv)->items) + (index0) * (pv)->size_per_item \
        ) \
    )
#endif

#define vec2_front(pv)             ((pv)->items)
#define vec2_back(pv)              vec2_item((pv), vec2_size(pv) - 1)

/*
 * read-only accesses
 */
#define vec2_const_data(pv)        ((const void *)(pv)->items)

#ifdef NDEBUG
    #define vec2_const_item(pv,index0) ( \
        (const void *)( \
            ((const char *)(pv)->items) + (index0) * (pv)->size_per_item \
        ) \
    )
#endif

#define vec2_const_front(pv)       ((const void *)(pv)->items)
#define vec2_const_back(pv)        vec2_const_item((pv), vec2_size(pv) - 1)

/****************************************************************************/
/* status checking */

#ifndef vec2_status_bad
    #define vec2_status_bad(pv)    assert(0)
#endif

/****************************************************************************/
/* inline functions */

#ifndef VEC2_NO_INLINING
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
        /* NOTE: C11 can use inline functions */
        #ifndef VEC2_ENABLE_INLINING
            #define VEC2_ENABLE_INLINING
        #endif
    #endif

    #ifdef __GNUC__
        /* NOTE: modern GNU C can use inline functions */
        #ifndef VEC2_ENABLE_INLINING
            #define VEC2_ENABLE_INLINING
        #endif
    #endif

    #ifdef _MSC_VER
        /* NOTE: modern Microsoft C can use inline functions */
        #ifndef VEC2_ENABLE_INLINING
            #define VEC2_ENABLE_INLINING
        #endif
    #endif

    #ifdef __DMC__
        /* NOTE: modern Digital Mars C can use inline functions */
        #ifndef VEC2_ENABLE_INLINING
            #define VEC2_ENABLE_INLINING
        #endif
    #endif
#endif  /* ndef VEC_NO_INLINING */

#ifdef VEC2_ENABLE_INLINING
    #undef VEC2_INLINE
    #define VEC2_INLINE  inline
    #include "vec2_inl.h"
#endif

/****************************************************************************/
/* C/C++ switching */

#ifdef __cplusplus
} /* extern "C" */
#endif

/****************************************************************************/

#endif  /* ndef KATAHIROMZ_VEC2_H */
