/* MIT License
 *
 * Copyright (c) 2021 Tyge Løvset, NORCE, www.norceresearch.no
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef STC_FORWARD_H_INCLUDED
#define STC_FORWARD_H_INCLUDED

#define forward_carray2(TAG, VAL) _c_carray2_types(carray2##TAG, VAL)
#define forward_carray3(TAG, VAL) _c_carray3_types(carray2##TAG, VAL)
#define forward_cdeq(TAG, VAL) _c_cdeq_types(cdeq_##TAG, VAL)
#define forward_clist(TAG, VAL) _c_clist_types(clist_##TAG, VAL)
#define forward_cmap(TAG, KEY, VAL) _c_chash_types(csmap_##TAG, cmap, KEY, VAL)
#define forward_csmap(TAG, KEY, VAL) _c_aatree_types(csmap_##TAG, csmap, KEY, VAL)
#define forward_cset(TAG, KEY) _c_chash_types(cset_##TAG, cset, KEY, KEY)
#define forward_csset(TAG, KEY) _c_aatree_types(csset_##TAG, csset, KEY, KEY)
#define forward_csptr(TAG, VAL) _csptr_types(csptr_##TAG, VAL)
#define forward_cpque(TAG, ctype) _c_cpque_types(cpque_##TAG, ctype)
#define forward_cqueue(TAG, ctype) _c_cqueue_types(cqueue_##TAG, ctype)
#define forward_cstack(TAG, ctype) _c_cstack_types(cstack_##TAG, ctype)
#define forward_cvec(TAG, VAL) _c_cvec_types(cvec_##TAG, VAL)

#define SET_ONLY_cmap_(...)
#define MAP_ONLY_cmap_(...) __VA_ARGS__
#define SET_ONLY_cset_(...) __VA_ARGS__
#define MAP_ONLY_cset_(...)

#define SET_ONLY_csmap_(...)
#define MAP_ONLY_csmap_(...) __VA_ARGS__
#define SET_ONLY_csset_(...) __VA_ARGS__
#define MAP_ONLY_csset_(...)

#ifndef MAP_SIZE_T
#define MAP_SIZE_T uint32_t
#endif

#define _c_carray2_types(SELF, VAL) \
    typedef VAL SELF##_value_t; \
    typedef struct { SELF##_value_t *ref; } SELF##_iter_t; \
    typedef struct { SELF##_value_t **data; size_t xdim, ydim; } SELF

#define _c_carray3_types(SELF, VAL) \
    typedef VAL SELF##_value_t; \
    typedef struct { SELF##_value_t *ref; } SELF##_iter_t; \
    typedef struct { SELF##_value_t ***data; size_t xdim, ydim, zdim; } SELF

#define _c_cdeq_types(SELF, VAL) \
    typedef VAL SELF##_value_t; \
    typedef struct {SELF##_value_t *ref; } SELF##_iter_t; \
    typedef struct {SELF##_value_t *_base, *data;} SELF

#define _c_clist_types(SELF, VAL) \
    typedef VAL SELF##_value_t; \
    typedef struct SELF##_node_t SELF##_node_t; \
\
    typedef struct { \
        SELF##_node_t *const *_last, *prev; \
        SELF##_value_t *ref; \
    } SELF##_iter_t; \
\
    typedef struct { \
        SELF##_node_t *last; \
    } SELF

#define _c_chash_types(SELF, C, KEY, VAL) \
    typedef KEY SELF##_key_t; \
    typedef VAL SELF##_mapped_t; \
    typedef MAP_SIZE_T SELF##_size_t; \
\
    typedef SET_ONLY_##C( SELF##_key_t ) \
            MAP_ONLY_##C( struct SELF##_value_t ) \
    SELF##_value_t; \
\
    typedef struct { \
        SELF##_value_t *ref; \
        bool inserted; \
    } SELF##_result_t; \
\
    typedef struct { \
        SELF##_value_t *ref; \
        uint8_t* _hx; \
    } SELF##_iter_t; \
\
    typedef struct { \
        SELF##_value_t* table; \
        uint8_t* _hashx; \
        SELF##_size_t size, bucket_count; \
        float max_load_factor; \
    } SELF

#define _c_aatree_types(SELF, C, KEY, VAL) \
    typedef KEY SELF##_key_t; \
    typedef VAL SELF##_mapped_t; \
    typedef MAP_SIZE_T SELF##_size_t; \
\
    typedef SET_ONLY_##C( SELF##_key_t ) \
            MAP_ONLY_##C( struct SELF##_value_t ) \
    SELF##_value_t; \
\
    typedef struct { \
        SELF##_value_t *ref; \
        bool inserted; \
    } SELF##_result_t; \
\
    typedef struct SELF##_node_t SELF##_node_t; \
\
    typedef struct { \
        SELF##_value_t *ref; \
        SELF##_node_t *_d; \
        int _top; \
        SELF##_size_t _tn, _st[36]; \
    } SELF##_iter_t; \
\
    typedef struct { \
        SELF##_node_t *nodes; \
    } SELF

#define _csptr_types(SELF, VAL) \
    typedef VAL SELF##_value_t; \
\
    typedef struct { \
        SELF##_value_t* get; \
        long* use_count; \
    } SELF

#define _c_cpque_types(SELF, ctype) \
    typedef ctype##_value_t SELF##_value_t; \
    typedef ctype##_rawvalue_t SELF##_rawvalue_t; \
    typedef ctype SELF

#define _c_cqueue_types(SELF, ctype) \
    typedef ctype##_value_t SELF##_value_t; \
    typedef ctype##_rawvalue_t SELF##_rawvalue_t; \
    typedef ctype##_iter_t SELF##_iter_t; \
    typedef struct { ctype rep; size_t size; } SELF

#define _c_cstack_types(SELF, ctype) \
    typedef ctype##_value_t SELF##_value_t; \
    typedef ctype##_rawvalue_t SELF##_rawvalue_t; \
    typedef ctype##_iter_t SELF##_iter_t; \
    typedef ctype SELF

#define _c_cvec_types(SELF, VAL) \
    typedef VAL SELF##_value_t; \
    typedef struct { SELF##_value_t *ref; } SELF##_iter_t; \
    typedef struct { SELF##_value_t *data; } SELF

#endif // STC_FORWARD_H_INCLUDED