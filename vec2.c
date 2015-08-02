/****************************************************************************/
/* vec2.c --- vec2 (fixed block vector for C language)                      */
/****************************************************************************/
/* [History]                                                                */
/* 2015.07.28: katahiromz creates v0.                                       */
/* 2015.07.31: katahiromz creates v1.                                       */
/* 2015.08.02: katahiromz creates v2.                                       */
/****************************************************************************/

#include "vec2.h"

/****************************************************************************/
/* status checking */

#ifndef vec2_status_bad
    #define vec2_status_bad(pv)    assert(0)
#endif

/****************************************************************************/
/* C/C++ switching */

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************/
/* functions */

bool vec2_valid(const VEC2 *pv)
{
    bool ret;

    if ((pv == NULL) || (pv->num_items > pv->capacity))
    {
        ret = false;
    }
    else if ((pv->num_items != 0U) && (pv->items == NULL))
    {
        ret = false;
    }
    else if (pv->size_per_item == 0U)
    {
        ret = false;
    }
    else
    {
        ret = true;
    }

    return ret;
} /* vec2_valid */

#ifndef NDEBUG
    void *vec2_item(PVEC2 pv, size_t index0)
    {
        char *p;
        assert(vec2_valid(pv));
        assert(index0 <= vec2_size(pv));
        p = (char *)pv->items;
        return (void *)(p + index0 * pv->size_per_item);
    } /* vec2_item */

    const void *vec2_const_item(const VEC2 *pv, size_t index0)
    {
        const char *p;
        assert(vec2_valid(pv));
        assert(index0 <= vec2_size(pv));
        p = (const char *)pv->items;
        return (const void *)(p + index0 * pv->size_per_item);
    } /* vec2_const_item */
#endif  /* ndef NDEBUG */

vec2_bool
vec2_construct(PVEC2 pv, size_t size_per_item,
               size_t capacity, void *items, size_t num_items)
{
    VEC2_STATUS_INIT(ret, true);
    assert(items != NULL);

    /* NOTE: vec2 doesn't allocate memory. Just weakly refers. */
    pv->items = items;
    pv->size_per_item = size_per_item;
    pv->num_items = num_items;
    pv->capacity = capacity;

    assert(vec2_valid(pv));

    VEC2_STATUS_RETURN(ret);
} /* vec2_construct */

void vec2_destroy(PVEC2 pv)
{
    assert(vec2_valid(pv));
    /* NOTE: vec2 doesn't free memory. */
} /* vec2_destroy */

void vec2_clear(PVEC2 pv)
{
    assert(vec2_valid(pv));
    vec2_destroy(pv);
    pv->num_items = 0;
} /* vec2_clear */

void *vec2_get_at(PVEC2 pv, size_t index0)
{
    char *ptr;
    void *p = NULL;
    assert(vec2_valid(pv));
    assert(index0 <= pv->num_items);
    if (index0 <= pv->num_items)
    {
        ptr = (char *)pv->items;
        p = &ptr[index0 * pv->size_per_item];
    }
    return p;
} /* vec2_get_at */

void vec2_set_at(PVEC2 pv, size_t index0, const void *pitem)
{
    void *p;
    assert(vec2_valid(pv));
    p = vec2_get_at(pv, index0);
    if (p != NULL)
    {
        memcpy(p, pitem, pv->size_per_item);
    }
    assert(vec2_valid(pv));
} /* vec2_set_at */

#ifndef MISRA_C
    /* NOTE: vec2_bsearch() and vec2_sort() aren't available in MISRA-C. */

    void *
    vec2_bsearch(PVEC2 pv, const void *pitem, VEC2_ITEM_COMPARE_FN compare)
    {
        assert(vec2_valid(pv));
        assert(pitem != NULL);
        assert(compare != NULL);
        return bsearch(pitem, pv->items, pv->num_items,
                       pv->size_per_item, compare);
    } /* vec2_bsearch */

    void vec2_sort(PVEC2 pv, VEC2_ITEM_COMPARE_FN compare)
    {
        assert(vec2_valid(pv));
        assert(compare != NULL);
        qsort(pv->items, pv->num_items, pv->size_per_item, compare);
    } /* vec2_sort */
#endif  /* ndef MISRA_C */

vec2_bool vec2_reserve(PVEC2 pv, size_t capacity)
{
    VEC2_STATUS_INIT(ret, true);

    assert(vec2_valid(pv));

    if (capacity > pv->capacity)
    {
        VEC2_STATUS_SET(ret, false);
        /* status bad */
        vec2_status_bad(pv);
    }

    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_reserve */

void vec2_shrink_to_fit(PVEC2 pv)
{
    /* does nothing */
    assert(vec2_valid(pv));
} /* vec2_shrink_to_fit */

vec2_bool vec2_erase(PVEC2 pv, size_t index0)
{
    char *ptr;
    VEC2_STATUS_INIT(ret, false);

    assert(vec2_valid(pv));
    assert(pv->num_items > 0U);
    assert(index0 < pv->num_items);

    if (index0 < pv->num_items)
    {
        ptr = (char *)pv->items;
        memmove(
            &ptr[index0 * pv->size_per_item],
            &ptr[(index0 + 1) * pv->size_per_item],
            ((pv->num_items - index0) - 1U) * pv->size_per_item);
        pv->num_items -= 1U;
        VEC2_STATUS_SET(ret, true);
    }

    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_erase */

vec2_bool vec2_push_back(PVEC2 pv, const void *pitem)
{
    char *ptr;
    VEC2_STATUS_INIT(ret, false);

    assert(vec2_valid(pv));
    assert(pitem);

#ifdef VEC2_QUICK_BUT_RISKY
    vec2_reserve(pv, pv->num_items + 1U);
#else
    if (vec2_reserve(pv, pv->num_items + 1U))
#endif
    {
        ptr = (char *)pv->items;
        memcpy(
            &ptr[pv->num_items * pv->size_per_item],
            pitem,
            pv->size_per_item);
        pv->num_items += 1U;
        VEC2_STATUS_SET(ret, true);
    }

    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_push_back */

vec2_bool vec2_pop_back(PVEC2 pv)
{
    VEC2_STATUS_INIT(ret, false);
    assert(vec2_valid(pv));
    if (pv->num_items > 0U)
    {
        pv->num_items -= 1U;
        VEC2_STATUS_SET(ret, true);
    }
    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_pop_back */

void vec2_swap(PVEC2 pv1, PVEC2 pv2)
{
    VEC2 v;

    assert(vec2_valid(pv1));
    assert(vec2_valid(pv2));

    if (pv1 != pv2)
    {
        v = *pv1;
        *pv1 = *pv2;
        *pv2 = v;
    }

    assert(vec2_valid(pv1));
    assert(vec2_valid(pv2));
} /* vec2_swap */

vec2_bool vec2_copy(PVEC2 dest, const VEC2 *src)
{
    VEC2_STATUS_INIT(ret, false);

    assert(vec2_valid(dest));
    assert(vec2_valid(src));
    assert(dest->size_per_item == src->size_per_item);

    if (dest != src)
    {
        assert(dest->items != src->items);

#ifdef VEC2_QUICK_BUT_RISKY
        vec2_reserve_2(dest, src->num_items, src->size_per_item);
#else
        ret = vec2_reserve_2(dest, src->num_items, src->size_per_item);
        if (ret)
#endif
        {
            memcpy(dest->items, src->items,
                   src->num_items * dest->size_per_item);
            dest->num_items = src->num_items;
        }
    }

    assert(vec2_valid(dest));
    VEC2_STATUS_RETURN(ret);
} /* vec2_copy */

vec2_bool vec2_resize(PVEC2 pv, size_t count, const void *pitem)
{
    char *p;
    size_t i, old_num_items = pv->num_items;
    VEC2_STATUS_INIT(ret, false);

    assert(vec2_valid(pv));

#ifdef VEC2_QUICK_BUT_RISKY
    vec2_reserve(pv, count);
#else
    if (vec2_reserve(pv, count))
#endif
    {
        if (count > old_num_items)
        {
            p = (char *)pv->items;
            if (pitem != NULL)
            {
                for (i = old_num_items; i < count; ++i)
                {
                    memcpy(&p[i * pv->size_per_item], pitem,
                           pv->size_per_item);
                }
            }
            else
            {
#ifdef VEC2_NO_ZERO_INIT
                ;
#else
                memset(&p[old_num_items * pv->size_per_item], 0,
                       (count - old_num_items) * pv->size_per_item);
#endif
            }
        }
        pv->num_items = count;
        VEC2_STATUS_SET(ret, true);
    }

    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_resize */

vec2_bool vec2_assign(PVEC2 pv, size_t count, const void *pitem,
                      size_t size_per_item)
{
    size_t i;
    char *ptr;
    VEC2_STATUS_INIT(ret, false);

    assert(vec2_valid(pv));

#ifdef VEC2_QUICK_BUT_RISKY
    vec2_reserve_2(pv, count, size_per_item);
#else
    if (vec2_reserve_2(pv, count, size_per_item))
#endif
    {
        if (pitem != NULL)
        {
            ptr = (char *)pv->items;
            for (i = 0; i < count; ++i)
            {
                memcpy(&ptr[i * size_per_item], pitem, size_per_item);
            }
        }
        else
        {
#ifdef VEC2_NO_ZERO_INIT
            ;
#else
            memset(pv->items, 0, size_per_item * count);
#endif
        }
        pv->num_items = count;
        VEC2_STATUS_SET(ret, true);
    }

    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_assign */

void vec2_foreach(PVEC2 pv, VEC2_FOREACH_FN fn)
{
    char *ptr;
    size_t i, count, size_per_item;

    assert(vec2_valid(pv));
    assert(fn != NULL);

    count = pv->num_items;
    if (count > 0U)
    {
        size_per_item = pv->size_per_item;
        ptr = (char *)pv->items;
        for (i = 0; i < count; ++i)
        {
            if ((*fn)(i, &ptr[i * size_per_item]) == false)
            {
                break;
            }
        }
    }

    assert(vec2_valid(pv));
} /* vec2_foreach */

void vec2_foreach_reverse(PVEC2 pv, VEC2_FOREACH_FN fn)
{
    char *ptr;
    size_t i, count, size_per_item;

    assert(vec2_valid(pv));
    assert(fn != NULL);

    count = pv->num_items;
    if (count > 0U)
    {
        size_per_item = pv->size_per_item;
        ptr = (char *)pv->items;
        for (i = count - 1; i < count; --i)
        {
            if ((*fn)(i, &ptr[i * size_per_item]) == false)
            {
                break;
            }
        }
    }

    assert(vec2_valid(pv));
} /* vec2_foreach_reverse */

void vec2_foreach_range(PVEC2 pv, VEC2_FOREACH_FN fn, size_t index0, size_t count)
{
    char *ptr;
    size_t i, size_per_item;

    assert(vec2_valid(pv));
    assert(fn != NULL);
    assert(index0 < pv->num_items);
    assert((index0 + count) <= pv->num_items);

    if (count > 0U)
    {
        size_per_item = pv->size_per_item;

        ptr = (char *)pv->items;
        ptr = &ptr[index0 * pv->size_per_item];
        for (i = 0; i < count; ++i)
        {
            if ((*fn)(index0 + i, &ptr[i * size_per_item]) == false)
            {
                break;
            }
        }
    }

    assert(vec2_valid(pv));
} /* vec2_foreach_range */

void *vec2_find(PVEC2 pv, const void *pitem, VEC2_ITEM_COMPARE_FN compare)
{
    char *ptr;
    size_t i, count;
    void *p;
    void *ret;

    assert(vec2_valid(pv));
    assert(pitem != NULL);
    assert(compare != NULL);

    count = pv->num_items;
    ptr = (char *)pv->items;
    ret = NULL;
    for (i = 0; i < count; ++i)
    {
        p = (void *)&ptr[i * pv->size_per_item];
        if ((*compare)(pitem, p) == 0)
        {
            ret = p;
            break;
        }
    }
    return ret;
} /* vec2_find */

vec2_bool vec2_reserve_2(PVEC2 pv, size_t capacity, size_t size_per_item)
{
    size_t new_size;
    VEC2_STATUS_INIT(ret, true);

    assert(vec2_valid(pv));

    new_size = capacity * size_per_item;
    if (new_size > pv->capacity * pv->size_per_item)
    {
        VEC2_STATUS_SET(ret, false);
        /* status bad */
        vec2_status_bad(pv);
    }
    else
    {
        pv->num_items *= pv->size_per_item;
        pv->num_items /= size_per_item;
        pv->capacity = capacity;
        pv->size_per_item = size_per_item;
    }

    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_reserve_2 */

vec2_bool vec2_insert(PVEC2 pv, size_t index0, size_t count, const void *pitem)
{
    char *ptr;
    size_t i;
    VEC2_STATUS_INIT(ret, false);

    assert(vec2_valid(pv));
    assert(index0 <= vec2_size(pv));

#ifdef VEC2_QUICK_BUT_RISKY
    vec2_reserve(pv, pv->num_items + count);
#else
    if (vec2_reserve(pv, pv->num_items + count))
#endif
    {
        ptr = (char *)pv->items;

        memmove(
            &ptr[(index0 + count) * pv->size_per_item],
            &ptr[index0 * pv->size_per_item],
            (pv->num_items - index0) * pv->size_per_item);
        for (i = index0; i < index0 + count; ++i)
        {
            memcpy(&ptr[i * pv->size_per_item], pitem, pv->size_per_item);
        }
        pv->num_items += count;
        VEC2_STATUS_SET(ret, true);
    }

    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_insert */

vec2_bool vec2_insert_sub(PVEC2 pv, size_t index0, const VEC2 *psubvec)
{
    char *ptr;
    size_t count = vec2_size(psubvec);
    VEC2_STATUS_INIT(ret, false);

    assert(vec2_valid(pv));
    assert(vec2_valid(psubvec));
    assert(index0 <= vec2_size(pv));

#ifdef VEC2_QUICK_BUT_RISKY
    vec2_reserve(pv, pv->num_items + count);
#else
    if (vec2_reserve(pv, pv->num_items + count))
#endif
    {
        ptr = (char *)pv->items;

        memmove(
            &ptr[(index0 + count) * pv->size_per_item],
            &ptr[index0 * pv->size_per_item],
            (pv->num_items - index0) * pv->size_per_item);
        memcpy(&ptr[index0 * pv->size_per_item], psubvec->items,
               count * pv->size_per_item);
        pv->num_items += count;
        VEC2_STATUS_SET(ret, true);
    }

    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_insert_sub */

vec2_bool vec2_erase_range(PVEC2 pv, size_t index0, size_t count)
{
    char *ptr;
    VEC2_STATUS_INIT(ret, false);

    assert(vec2_valid(pv));
    assert(pv->num_items >= count);
    assert(index0 < pv->num_items);
    assert((index0 + count) <= pv->num_items);

    if (count != 0U)
    {
        if ((index0 + count) <= pv->num_items)
        {
            ptr = (char *)pv->items;
            memmove(
                &ptr[index0 * pv->size_per_item],
                &ptr[(index0 + count) * pv->size_per_item],
                ((pv->num_items - index0) - count) * pv->size_per_item);
            pv->num_items -= count;
            VEC2_STATUS_SET(ret, true);
        }
    }

    assert(vec2_valid(pv));
    VEC2_STATUS_RETURN(ret);
} /* vec2_erase_range */

/****************************************************************************/
/* C/C++ switching */

#ifdef __cplusplus
} /* extern "C" */
#endif

/****************************************************************************/
/* test and sample */

/* #define VEC2_TEST */

#ifdef VEC2_TEST
    #include <stdio.h>

    bool print_foreach(size_t index0, void *ptr)
    {
        printf("[%d] %ld ", (int)index0, *(long *)ptr);
        return true;
    }

    int long_compare(const void *x, const void *y)
    {
        const long *a = (const long *)x;
        const long *b = (const long *)y;
        if (*a < *b)
        {
            return -1;
        }
        if (*a > *b)
        {
            return 1;
        }
        return 0;
    }

    int main(void)
    {
        VEC2 vec1, vec2;
        long n = 4;
        size_t siz = sizeof(long);
        static long items1[100], items2[100];

        /* NOTE: You cannot use vec2_init(). Use vec2_construct() instead. */
        vec2_construct(&vec1, siz, 100, items1, 0);
        vec2_construct(&vec2, siz, 100, items2, 0);

        n = 1;
        vec2_push_back(&vec1, &n);
        assert(vec2_size(&vec1) == 1);
        printf("vec1: ");
        vec2_foreach(&vec1, print_foreach);
        printf("\n");

        n = 2;
        vec2_push_back(&vec1, &n);
        assert(vec2_size(&vec1) == 2);
        printf("vec1: ");
        vec2_foreach(&vec1, print_foreach);
        printf("\n");

        n = 3;
        vec2_push_back(&vec1, &n);
        assert(vec2_size(&vec1) == 3);
        printf("vec1: ");
        vec2_foreach(&vec1, print_foreach);
        printf("\n");

        n = 1;
        assert(memcmp(vec2_item(&vec1, 0), &n, siz) == 0);
        n = 2;
        assert(memcmp(vec2_item(&vec1, 1), &n, siz) == 0);
        n = 3;
        assert(memcmp(vec2_item(&vec1, 2), &n, siz) == 0);

        assert(vec2_size(&vec2) == 0);

        n = -1;
        vec2_push_back(&vec2, &n);
        assert(vec2_size(&vec2) == 1);

        vec2_insert_sub(&vec1, 1, &vec2);
        printf("vec1: ");
        vec2_foreach(&vec1, print_foreach);
        printf("\n");

        n = 1;
        assert(memcmp(vec2_item(&vec1, 0), &n, siz) == 0);
        n = -1;
        assert(memcmp(vec2_item(&vec1, 1), &n, siz) == 0);
        n = 2;
        assert(memcmp(vec2_item(&vec1, 2), &n, siz) == 0);
        n = 3;
        assert(memcmp(vec2_item(&vec1, 3), &n, siz) == 0);

        vec2_sort(&vec1, long_compare);
        printf("vec1: ");
        vec2_foreach(&vec1, print_foreach);
        printf("\n");

        vec2_erase(&vec1, 0);

        n = 1;
        assert(memcmp(vec2_item(&vec1, 0), &n, siz) == 0);
        n = 2;
        assert(memcmp(vec2_item(&vec1, 1), &n, siz) == 0);
        n = 3;
        assert(memcmp(vec2_item(&vec1, 2), &n, siz) == 0);

        n = 4;
        vec2_insert(&vec1, 1, 1, &n);
        printf("vec1: ");
        vec2_foreach(&vec1, print_foreach);
        printf("\n");

        n = 1;
        assert(memcmp(vec2_item(&vec1, 0), &n, siz) == 0);
        n = 4;
        assert(memcmp(vec2_item(&vec1, 1), &n, siz) == 0);
        n = 2;
        assert(memcmp(vec2_item(&vec1, 2), &n, siz) == 0);
        n = 3;
        assert(memcmp(vec2_item(&vec1, 3), &n, siz) == 0);

        vec2_erase_range(&vec1, 1, 2);
        vec2_shrink_to_fit(&vec1);
        printf("vec1: ");
        vec2_foreach(&vec1, print_foreach);
        printf("\n");

        n = 1;
        assert(memcmp(vec2_item(&vec1, 0), &n, siz) == 0);
        n = 3;
        assert(memcmp(vec2_item(&vec1, 1), &n, siz) == 0);

        vec2_copy(&vec2, &vec1);
        printf("vec2: ");
        vec2_foreach(&vec2, print_foreach);
        printf("\n");

        vec2_destroy(&vec1);
        vec2_destroy(&vec2);

        return 0;
    } /* main */
#endif  /* def VEC2_TEST */

/****************************************************************************/
