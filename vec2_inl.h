/****************************************************************************/
/* vec2_inl.h --- inline functions                                          */
/****************************************************************************/

VEC2_INLINE bool vec2_valid(const VEC2 *pv)
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

VEC2_INLINE vec2_bool
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

VEC2_INLINE void vec2_destroy(PVEC2 pv)
{
    assert(vec2_valid(pv));
    /* NOTE: vec2 doesn't free memory. */
} /* vec2_destroy */

VEC2_INLINE void vec2_clear(PVEC2 pv)
{
    assert(vec2_valid(pv));
    vec2_destroy(pv);
    pv->num_items = 0;
} /* vec2_clear */

VEC2_INLINE void *vec2_get_at(PVEC2 pv, size_t index0)
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

VEC2_INLINE void vec2_set_at(PVEC2 pv, size_t index0, const void *pitem)
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

    VEC2_INLINE void *
    vec2_bsearch(PVEC2 pv, const void *pitem, VEC2_ITEM_COMPARE_FN compare)
    {
        assert(vec2_valid(pv));
        assert(pitem != NULL);
        assert(compare != NULL);
        return bsearch(pitem, pv->items, pv->num_items,
                       pv->size_per_item, compare);
    } /* vec2_bsearch */

    VEC2_INLINE void vec2_sort(PVEC2 pv, VEC2_ITEM_COMPARE_FN compare)
    {
        assert(vec2_valid(pv));
        assert(compare != NULL);
        qsort(pv->items, pv->num_items, pv->size_per_item, compare);
    } /* vec2_sort */
#endif  /* ndef MISRA_C */

VEC2_INLINE vec2_bool vec2_reserve(PVEC2 pv, size_t capacity)
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

VEC2_INLINE void vec2_shrink_to_fit(PVEC2 pv)
{
    /* does nothing */
    assert(vec2_valid(pv));
} /* vec2_shrink_to_fit */

VEC2_INLINE vec2_bool vec2_erase(PVEC2 pv, size_t index0)
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

VEC2_INLINE vec2_bool vec2_push_back(PVEC2 pv, const void *pitem)
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

VEC2_INLINE vec2_bool vec2_pop_back(PVEC2 pv)
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

VEC2_INLINE void vec2_swap(PVEC2 pv1, PVEC2 pv2)
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

/****************************************************************************/
