// omni.h - single-header C utility library
//
// Combines what used to be dynarr.h (dynamic arrays), sv.h (string views),
// hashmap.h (string-keyed hashmaps) and mio.h (typesafe printf-alternative)
// into one file, in traditional single-header style:
//
//     #define OMNI_IMPLEMENTATION
//     #include "omni.h"
//
// in exactly ONE translation unit before including omni.h; every other file
// just does a plain `#include "omni.h"`.
//
// All public identifiers live under the omni_/OMNI_ prefix. By default this
// file ALSO defines short, unprefixed aliases (da_push, sv_eq, hm_set,
// print, ...) matching the ergonomics of the original standalone headers.
// If those short names collide with something else in your project,
// define OMNI_UNSTRIP_PREFIX before including omni.h to skip creating them --
// only the omni_/OMNI_-prefixed names are ever required.

#ifndef OMNI_H
#define OMNI_H
#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS (1)
#endif // _CRT_SECURE_NO_WARNINGS
#endif //  _WIN32

// OMNIDEF controls the linkage of every public function in this file.
// Left empty (the default) each function has ordinary external linkage:
// define OMNI_IMPLEMENTATION in exactly one translation unit and every other
// file that includes omni.h links against that one. Define OMNIDEF as
// `static` yourself before including omni.h if you'd rather each
// translation unit get its own private copy of the implementation.
#ifndef OMNIDEF
#define OMNIDEF
#endif // OMNIDEF

// The _Generic tag functions behind {} formatting (omni_mio_tag_char and
// friends) must be `static` and defined directly in the declaration section
// below rather than behind OMNI_IMPLEMENTATION -- every translation unit that
// expands the print()/mio_printf()-style macros needs its own copy, since
// _Generic has to resolve to a real function in that TU, whether or not
// that TU is the one with OMNI_IMPLEMENTATION. A translation unit that never
// happens to use those macros -- e.g. it only uses the dynamic array part
// of this file -- would otherwise get a harmless-but-noisy "defined but not
// used" warning under -Wall/-Wextra for each one.
#if defined(__GNUC__) || defined(__clang__)
#define OMNI_MAYBE_UNUSED __attribute__((unused))
#else
#define OMNI_MAYBE_UNUSED
#endif

#ifdef __cplusplus
#include <type_traits>
#endif // __cplusplus

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* -------- GENERAL MACROS -------- */

#define OMNI_UNUSED(value) (void)(value)

#define OMNI_TODO(message)                                                     \
    do {                                                                       \
        omni_mio_fprintf(STDERR_FILENO, "{}:{}: TODO: {}\n", __FILE__,         \
                         __LINE__, message);                                   \
        abort();                                                               \
    } while (0)

#define OMNI_UNREACHABLE(message)                                              \
    do {                                                                       \
        omni_mio_fprintf(STDERR_FILENO, "{}:{}: UNREACHABLE: {}\n", __FILE__,  \
                         __LINE__, message);                                   \
        abort();                                                               \
    } while (0)

#define OMNI_ARRAY_LEN(array) (sizeof(array) / sizeof((array)[0]))

#define OMNI_ARRAY_GET(array, index)                                           \
    (assert((size_t)(index) < OMNI_ARRAY_LEN(array)), (array)[(size_t)(index)])

#define omni_shift(xs, xs_sz) (assert((xs_sz) > 0), (xs_sz)--, *(xs)++)

#define omni_shift_args(argc, argv) omni_shift(*argv, *argc)

/* ---------- DYNAMIC ARRAYS (omni_da_*) ---------- */

#ifndef OMNI_DA_INIT_CAP
#define OMNI_DA_INIT_CAP 256
#endif // OMNI_DA_INIT_CAP

#ifdef __cplusplus
// decltype(T) is a reference type whenever T is already parenthesized
// (which happens routinely here -- macros forward their argument as
// "(arg)" into other macros) -- strip that back off, since a prvalue
// void* can't be cast to e.g. "char *&".
#define OMNI_DECLTYPE_CAST(T)                                                  \
    (typename std::remove_reference<decltype(T)>::type)
#else
#define OMNI_DECLTYPE_CAST(T)
#endif

typedef struct {
    size_t count;
    size_t capacity;
} OMNI_DAHeader;

OMNIDEF void *omni_da_alloc(size_t element_size, size_t capacity);
OMNIDEF void *omni_da_grow(void *arr, size_t element_size, size_t min_capacity);

#define omni_da_len(arr) (((OMNI_DAHeader *)(arr) - 1)->count)

#define omni_da_capacity(arr) (((OMNI_DAHeader *)(arr) - 1)->capacity)

#define omni_da_reserve(arr, _da_capacity)                                     \
    do {                                                                       \
        if ((arr) == NULL)                                                     \
            (arr) = OMNI_DECLTYPE_CAST(arr)                                    \
                omni_da_alloc(sizeof(*(arr)), OMNI_DA_INIT_CAP);               \
                                                                               \
        if ((arr) != NULL) {                                                   \
            OMNI_DAHeader *h = (OMNI_DAHeader *)(arr) - 1;                     \
            if ((_da_capacity) > h->capacity) {                                \
                void *_new_arr =                                               \
                    omni_da_grow((arr), sizeof(*(arr)), (_da_capacity));       \
                if (_new_arr != NULL)                                          \
                    (arr) = OMNI_DECLTYPE_CAST(arr) _new_arr;                  \
            }                                                                  \
        }                                                                      \
    } while (0)

#define omni_da_push(arr, value)                                               \
    do {                                                                       \
        if ((arr) == NULL)                                                     \
            omni_da_reserve((arr), OMNI_DA_INIT_CAP);                          \
        else if (omni_da_len(arr) >= omni_da_capacity(arr))                    \
            omni_da_reserve((arr), omni_da_capacity(arr) + 1);                 \
                                                                               \
        (arr)[omni_da_len(arr)++] = (value);                                   \
    } while (0)

#define omni_da_pop(arr)                                                       \
    (assert(omni_da_len(arr) > 0), (arr)[--omni_da_len(arr)])

#define omni_da_first(arr) ((arr)[0])

#define omni_da_last(arr) ((arr)[omni_da_len(arr) - 1])

#define omni_da_peek(arr) ((arr)[omni_da_len(arr) - 1])

#define omni_da_remove_unordered(arr, i)                                       \
    do {                                                                       \
        size_t j = (i);                                                        \
        assert(j < omni_da_len(arr));                                          \
        (arr)[j] = (arr)[--omni_da_len(arr)];                                  \
    } while (0)

#define omni_da_remove(arr, i)                                                 \
    do {                                                                       \
        size_t j = (i);                                                        \
        assert(j < omni_da_len(arr));                                          \
        memmove((arr) + j, (arr) + j + 1,                                      \
                (omni_da_len(arr) - j - 1) * sizeof(*(arr)));                  \
        --omni_da_len(arr);                                                    \
    } while (0)

#define omni_da_push_many(arr, items, count)                                   \
    do {                                                                       \
        size_t c = (count);                                                    \
        omni_da_reserve((arr), omni_da_len((arr)) + c);                        \
        memcpy((arr) + omni_da_len((arr)), (items), c * sizeof(*(arr)));       \
        omni_da_len((arr)) += c;                                               \
    } while (0)

#define omni_da_resize(arr, size)                                              \
    do {                                                                       \
        omni_da_reserve((arr), (size));                                        \
        omni_da_len((arr)) = (size);                                           \
    } while (0)

#define omni_da_free(arr)                                                      \
    do {                                                                       \
        if ((arr) != NULL) {                                                   \
            free((OMNI_DAHeader *)(arr) - 1);                                  \
            (arr) = NULL;                                                      \
        }                                                                      \
    } while (0)

#define omni_da_foreach(Type, it, arr)                                         \
    for (Type *it = (arr); it < (arr) + omni_da_len(arr); ++it)

#define omni_da_clear(arr) (omni_da_len(arr) = 0)

#define omni_dynarr(Type) Type *

/* ---------- STRING VIEWS (omni_sv_*) ---------- */

typedef struct {
    size_t count;
    union {
        const char *data;
        const char *items;
    };
} omni_string;

// A flat, growable byte buffer -- NOT an array of strings. Wraps a
// omni_dynarr(char) (`.items`: a `char *` with a hidden OMNI_DAHeader right
// before it, exactly like any other dynarr) in a one-member struct so it
// has its own distinct C type. That's what lets a bare `omni_string_builder`
// be passed straight into omni_mio_printf/print's `{}` machinery and get
// printed by its tracked length -- a raw `char *` can't be told apart from
// a NUL-terminated C string by `_Generic`, so this couldn't work if
// omni_string_builder were still just `omni_dynarr(char)` (a bare `char *`),
// which is what it used to be.
//
// See the "STRING BUILDER" section below (after omni_mio_*) for the omni_sb_*
// operations built on top of it -- use those (or `.items`) rather than the
// plain omni_da_* macros directly, since those expect their argument to
// literally be the pointer they grow, not a struct wrapping one.
//
// Initialize with `= {0}` (portable) or `= {}` (GNU/C23 extension, same as
// before) -- both zero out `.items` to NULL, same as `omni_dynarr(char) sb =
// NULL;` did previously.
typedef struct {
    char *items;
} omni_string_builder;

#define OMNI_SV_FMT "%.*s"
#define OMNI_SV_ARG(sv) (int)(sv).count, (sv).data

// String view built directly from a C string *literal*, at zero runtime
// cost (the length comes from sizeof, not a strlen-style scan). Only valid
// for actual string literals -- for a `const char *` that isn't necessarily
// a literal, use omni_sv() instead.
#ifdef __cplusplus
#define OMNI_SVLIT(s) (omni_string{sizeof(s) - 1, {(s)}})
#else
#define OMNI_SVLIT(s) ((omni_string){.count = sizeof(s) - 1, .data = (s)})
#endif // __cplusplus

OMNIDEF size_t omni_cstr_len(const char *cstr);

OMNIDEF omni_string omni_sv(const char *cstr);

OMNIDEF void omni_sv_chop_left(omni_string *sv, size_t n);
OMNIDEF void omni_sv_chop_right(omni_string *sv, size_t n);

OMNIDEF void omni_sv_trim_left(omni_string *sv);
OMNIDEF void omni_sv_trim_right(omni_string *sv);
OMNIDEF void omni_sv_trim(omni_string *sv);

OMNIDEF bool omni_sv_eq(omni_string a, omni_string b);

OMNIDEF bool omni_sv_eq_ignorecase(omni_string a, omni_string b);

OMNIDEF bool omni_sv_starts_with(omni_string sv, omni_string prefix);

OMNIDEF bool omni_sv_ends_with(omni_string sv, omni_string suffix);

OMNIDEF int omni_sv_cmp(omni_string a, omni_string b);

OMNIDEF omni_string omni_sv_chop_by_delim(omni_string *sv, char delim);
OMNIDEF omni_string omni_sv_chop_if(omni_string *sv, int (*pred)(int c));

OMNIDEF void omni__sv_to_cstr(omni_string sv, char *out);
OMNIDEF const char *omni_sv_to_cstr(omni_string sv);

/* ---------- HASHMAPS (omni_hm_*) ---------- */

#ifndef OMNI_HM_LOAD_FACTOR
#define OMNI_HM_LOAD_FACTOR 0.75
#endif // OMNI_HM_LOAD_FACTOR

#ifndef OMNI_HM_INIT_CAP
#define OMNI_HM_INIT_CAP 8
#endif // OMNI_HM_INIT_CAP

#define omni_hashmap(V)                                                        \
    struct {                                                                   \
        omni_string key;                                                       \
        V value;                                                               \
    } *

#define omni_hashmap_type(V) typedef omni_hashmap(V)

typedef struct {
    size_t count;    // live entries
    size_t capacity; // total slots
} OMNI_HMHeader;

OMNIDEF void *omni_hm__alloc(size_t stride, size_t capacity);
OMNIDEF size_t omni_hm__probe(const void *entries, size_t stride,
                              size_t capacity, omni_string key);
OMNIDEF void omni_hm__rehash(const void *old_entries, size_t old_capacity,
                             void *new_entries, size_t stride,
                             size_t new_capacity, size_t *count);
OMNIDEF bool omni_hm__remove(void *entries, size_t stride, size_t capacity,
                             omni_string key, size_t *count);

OMNIDEF uint64_t omni_hm_hash(omni_string key);

#define omni_hm__header(m) ((OMNI_HMHeader *)(m) - 1)

#define omni_hm_len(m) ((m) == NULL ? 0 : omni_hm__header(m)->count)

#define omni_hm_cap(m) ((m) == NULL ? 0 : omni_hm__header(m)->capacity)

#define omni_hm_reserve(m, new_cap)                                            \
    do {                                                                       \
        size_t _hm_stride = sizeof(*(m));                                      \
        size_t _hm_old_cap = omni_hm_cap(m);                                   \
        size_t _hm_new_cap = (new_cap);                                        \
        void *_hm_old = (m);                                                   \
        (m) = OMNI_DECLTYPE_CAST(m) omni_hm__alloc(_hm_stride, _hm_new_cap);   \
        if ((m) != NULL) {                                                     \
            if (_hm_old != NULL) {                                             \
                omni_hm__rehash(_hm_old, _hm_old_cap, (m), _hm_stride,         \
                                _hm_new_cap, &omni_hm__header(m)->count);      \
                free(omni_hm__header(_hm_old));                                \
            }                                                                  \
        } else {                                                               \
            (m) = OMNI_DECLTYPE_CAST(m) _hm_old;                               \
        }                                                                      \
    } while (0)

#define omni_hm_set(m, k, v)                                                   \
    do {                                                                       \
        assert((k).data != NULL);                                              \
        if ((m) == NULL)                                                       \
            omni_hm_reserve((m), OMNI_HM_INIT_CAP);                            \
        else if ((double)(omni_hm_len(m) + 1) / omni_hm_cap(m) >               \
                 OMNI_HM_LOAD_FACTOR)                                          \
            omni_hm_reserve((m), omni_hm_cap(m) * 2);                          \
        size_t _hm_idx =                                                       \
            omni_hm__probe((m), sizeof(*(m)), omni_hm_cap(m), (k));            \
        if ((m)[_hm_idx].key.data == NULL) {                                   \
            (m)[_hm_idx].key = (k);                                            \
            omni_hm__header(m)->count++;                                       \
        }                                                                      \
        (m)[_hm_idx].value = (v);                                              \
    } while (0)

#define omni_hm_get(m, k)                                                      \
    ((m) == NULL ||                                                            \
             (m)[omni_hm__probe((m), sizeof(*(m)), omni_hm_cap(m), (k))]       \
                     .key.data == NULL                                         \
         ? NULL                                                                \
         : &(m)[omni_hm__probe((m), sizeof(*(m)), omni_hm_cap(m), (k))].value)

#define omni_hm_has(m, k)                                                      \
    ((m) != NULL &&                                                            \
     (m)[omni_hm__probe((m), sizeof(*(m)), omni_hm_cap(m), (k))].key.data !=   \
         NULL)

#define omni_hm_remove(m, k)                                                   \
    ((m) == NULL ? false                                                       \
                 : omni_hm__remove((m), sizeof(*(m)), omni_hm_cap(m), (k),     \
                                   &omni_hm__header(m)->count))

#define omni_hm_free(m)                                                        \
    do {                                                                       \
        if ((m) != NULL) {                                                     \
            free(omni_hm__header(m));                                          \
            (m) = NULL;                                                        \
        }                                                                      \
    } while (0)

/* ---------- PRINTING (omni_mio_*) ---------- */

typedef enum {
    OMNI_MIO_DECIMAL,
    OMNI_MIO_HEX_LOW,
    OMNI_MIO_HEX_UPPER,
    OMNI_MIO_OCTAL
} OMNI_MioDisplayType;

OMNIDEF int omni_mio_putc(int fd, char c);
OMNIDEF int omni_mio_putcstr(int fd, const char *cstr, int len);
OMNIDEF int omni_mio_putsv(int fd, omni_string sv);

OMNIDEF void omni_mio_putcstr_padded(int fd, const char *cstr, int len,
                                     int width, int left_align, char pad_char);
OMNIDEF void omni_mio_putsv_padded(int fd, omni_string sv, int width,
                                   int left_align, char pad_char);

OMNIDEF int omni_mio_itos(int64_t n, char *buf);
OMNIDEF int omni_mio_utos(uint64_t n, char *buf);
OMNIDEF int omni_mio_itoh(uint64_t n, char *buf, bool uppercase);
OMNIDEF int omni_mio_itoo(uint64_t n, char *buf);
OMNIDEF int omni_mio_ftos(double n, char *buf, int precision);

OMNIDEF void omni_mio_printchar(int fd, char c, OMNI_MioDisplayType disp_type,
                                bool left_align, int width, int precision,
                                bool zero_pad, bool display_plus);
OMNIDEF void omni_mio_printbool(int fd, bool val, OMNI_MioDisplayType disp_type,
                                bool left_align, int width, int precision,
                                bool zero_pad, bool display_plus);
OMNIDEF void omni_mio_printint(int fd, int64_t val,
                               OMNI_MioDisplayType disp_type, bool left_align,
                               int width, int precision, bool zero_pad,
                               bool display_plus);
OMNIDEF void omni_mio_printuint(int fd, uint64_t val,
                                OMNI_MioDisplayType disp_type, bool left_align,
                                int width, int precision, bool zero_pad,
                                bool display_plus);
OMNIDEF void omni_mio_printfloat(int fd, double val,
                                 OMNI_MioDisplayType disp_type, bool left_align,
                                 int width, int precision, bool zero_pad,
                                 bool display_plus);
OMNIDEF void omni_mio_printptr(int fd, const void *ptr,
                               OMNI_MioDisplayType disp_type, bool left_align,
                               int width, int precision, bool zero_pad,
                               bool display_plus);
OMNIDEF void omni_mio_printcstr(int fd, const char *cstr,
                                OMNI_MioDisplayType disp_type, bool left_align,
                                int width, int precision, bool zero_pad,
                                bool display_plus);
OMNIDEF void omni_mio_printsv(int fd, omni_string sv,
                              OMNI_MioDisplayType disp_type, bool left_align,
                              int width, int precision, bool zero_pad,
                              bool display_plus);
OMNIDEF void omni_mio_printsb(int fd, omni_string_builder sb,
                              OMNI_MioDisplayType disp_type, bool left_align,
                              int width, int precision, bool zero_pad,
                              bool display_plus);

#ifdef __cplusplus
// C++ has no _Generic. omni_mio_printtype_impl (defined below, once
// OMNI_MioArg and OMNI_MIO_TAG exist) plus the omni_mio_tag_dispatch
// overload set stand in for it -- see the comment above OMNI_MIO_TAG.
#define omni_mio_printtype(fd, val, ...)                                       \
    omni_mio_printtype_impl((fd), OMNI_MIO_TAG(val), __VA_ARGS__)
#else
#define omni_mio_printtype(fd, val, ...)                                       \
    _Generic((val),                                                            \
        char: omni_mio_printchar,                                              \
        _Bool: omni_mio_printbool,                                             \
                                                                               \
        signed char: omni_mio_printint,                                        \
        short: omni_mio_printint,                                              \
        int: omni_mio_printint,                                                \
        long: omni_mio_printint,                                               \
        long long: omni_mio_printint,                                          \
                                                                               \
        unsigned char: omni_mio_printuint,                                     \
        unsigned short: omni_mio_printuint,                                    \
        unsigned int: omni_mio_printuint,                                      \
        unsigned long: omni_mio_printuint,                                     \
        unsigned long long: omni_mio_printuint,                                \
                                                                               \
        float: omni_mio_printfloat,                                            \
        double: omni_mio_printfloat,                                           \
        long double: omni_mio_printfloat,                                      \
                                                                               \
        char *: omni_mio_printcstr,                                            \
        const char *: omni_mio_printcstr,                                      \
        omni_string: omni_mio_printsv,                                         \
        omni_string_builder: omni_mio_printsb,                                 \
                                                                               \
        default: omni_mio_printptr)(fd, (val), __VA_ARGS__)
#endif // __cplusplus

typedef enum {
    OMNI_MIO_CHAR,
    OMNI_MIO_BOOL,
    OMNI_MIO_INT,
    OMNI_MIO_UINT,
    OMNI_MIO_FLOAT,
    OMNI_MIO_PTR,
    OMNI_MIO_CSTR,
    OMNI_MIO_SV,
} OMNI_MioArgType;

typedef struct {
    OMNI_MioArgType type;
    union {
        char c;
        bool b;
        int64_t i;
        uint64_t u;
        double f;
        const void *p;
        const char *s;
        omni_string sv;
    } as;
} OMNI_MioArg;

// Captures both a value and its type, at the call site, while the compiler
// still knows what type was actually written there -- this is the part a
// va_list can't do, since by the time a value is inside one, its type has
// already been erased. Mirrors omni_mio_printtype's type table.
//
// NOTE: each _Generic branch below must be a plain function name, not an
// expression built from `x` directly (e.g. NOT `(OMNI_MioArg){.as.s = (x)}`).
// Since OMNI_MIO_TAG is a macro, `x` gets substituted into every branch's
// text, not just the one that ends up selected -- and every branch is still
// type-checked by the compiler even though only one is used. Embedding `x`
// straight into a union-member initializer would mean the compiler has to
// accept e.g. assigning an int into `.as.s` (a const char *) for the string
// branch, which isn't valid C and (unlike the pointer branches) is a hard
// error for the `omni_string`/struct branch. Routing through same-shaped
// helper functions (all "OMNI_MioArg(T)") sidesteps this exactly the way
// omni_mio_printtype sidesteps it: _Generic only ever picks between function
// names, and `x` is only ever type-checked once, against the one function
// actually called.
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_char(char c) {
    OMNI_MioArg arg;
    arg.type = OMNI_MIO_CHAR;
    arg.as.c = c;
    return arg;
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_bool(bool b) {
    OMNI_MioArg arg;
    arg.type = OMNI_MIO_BOOL;
    arg.as.b = b;
    return arg;
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_int(int64_t i) {
    OMNI_MioArg arg;
    arg.type = OMNI_MIO_INT;
    arg.as.i = i;
    return arg;
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_uint(uint64_t u) {
    OMNI_MioArg arg;
    arg.type = OMNI_MIO_UINT;
    arg.as.u = u;
    return arg;
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_float(double f) {
    OMNI_MioArg arg;
    arg.type = OMNI_MIO_FLOAT;
    arg.as.f = f;
    return arg;
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_cstr(const char *s) {
    OMNI_MioArg arg;
    arg.type = OMNI_MIO_CSTR;
    arg.as.s = s;
    return arg;
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_sv(omni_string sv) {
    OMNI_MioArg arg;
    arg.type = OMNI_MIO_SV;
    arg.as.sv = sv;
    return arg;
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_sb(omni_string_builder sb) {
    OMNI_MioArg arg;
    arg.type = OMNI_MIO_SV;
    arg.as.sv.count = omni_da_len(sb.items);
    arg.as.sv.data = sb.items;
    return arg;
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_ptr(const void *p) {
    OMNI_MioArg arg;
    arg.type = OMNI_MIO_PTR;
    arg.as.p = p;
    return arg;
}

#ifdef __cplusplus
// _Generic doesn't exist in C++; ordinary overload resolution does the same
// job -- one overload per branch of the C _Generic table above, plus a
// function template standing in for the `default:` pointer catch-all.
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(char c) {
    return omni_mio_tag_char(c);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(bool b) {
    return omni_mio_tag_bool(b);
}

static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(signed char i) {
    return omni_mio_tag_int(i);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(short i) {
    return omni_mio_tag_int(i);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(int i) {
    return omni_mio_tag_int(i);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(long i) {
    return omni_mio_tag_int(i);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(long long i) {
    return omni_mio_tag_int(i);
}

static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(unsigned char u) {
    return omni_mio_tag_uint(u);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(unsigned short u) {
    return omni_mio_tag_uint(u);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(unsigned int u) {
    return omni_mio_tag_uint(u);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(unsigned long u) {
    return omni_mio_tag_uint(u);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg
omni_mio_tag_dispatch(unsigned long long u) {
    return omni_mio_tag_uint(u);
}

static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(float f) {
    return omni_mio_tag_float(f);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(double f) {
    return omni_mio_tag_float(f);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(long double f) {
    return omni_mio_tag_float((double)f);
}

static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(char *s) {
    return omni_mio_tag_cstr(s);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(const char *s) {
    return omni_mio_tag_cstr(s);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(omni_string sv) {
    return omni_mio_tag_sv(sv);
}
static OMNI_MAYBE_UNUSED OMNI_MioArg
omni_mio_tag_dispatch(omni_string_builder sb) {
    return omni_mio_tag_sb(sb);
}

static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(decltype(nullptr)) {
    return omni_mio_tag_ptr(NULL);
}

// Matches _Generic's `default` branch: any pointer type not listed above
// (including plain `void *`) prints/tags as a pointer.
template <typename OMNI_MioT>
static OMNI_MAYBE_UNUSED OMNI_MioArg omni_mio_tag_dispatch(const OMNI_MioT *p) {
    return omni_mio_tag_ptr((const void *)p);
}

#define OMNI_MIO_TAG(x) omni_mio_tag_dispatch(x)

// Backs omni_mio_printtype(fd, val, ...) (see its #ifdef above): tags `val`
// exactly like OMNI_MIO_TAG would, then unpacks the tag to call the one
// omni_mio_print* function _Generic would have picked for it in C.
static OMNI_MAYBE_UNUSED void
omni_mio_printtype_impl(int fd, OMNI_MioArg arg, OMNI_MioDisplayType disp_type,
                        bool left_align, int width, int precision,
                        bool zero_pad, bool display_plus) {
    switch (arg.type) {
    case OMNI_MIO_CHAR:
        omni_mio_printchar(fd, arg.as.c, disp_type, left_align, width,
                           precision, zero_pad, display_plus);
        break;
    case OMNI_MIO_BOOL:
        omni_mio_printbool(fd, arg.as.b, disp_type, left_align, width,
                           precision, zero_pad, display_plus);
        break;
    case OMNI_MIO_INT:
        omni_mio_printint(fd, arg.as.i, disp_type, left_align, width, precision,
                          zero_pad, display_plus);
        break;
    case OMNI_MIO_UINT:
        omni_mio_printuint(fd, arg.as.u, disp_type, left_align, width,
                           precision, zero_pad, display_plus);
        break;
    case OMNI_MIO_FLOAT:
        omni_mio_printfloat(fd, arg.as.f, disp_type, left_align, width,
                            precision, zero_pad, display_plus);
        break;
    case OMNI_MIO_PTR:
        omni_mio_printptr(fd, arg.as.p, disp_type, left_align, width, precision,
                          zero_pad, display_plus);
        break;
    case OMNI_MIO_CSTR:
        omni_mio_printcstr(fd, arg.as.s, disp_type, left_align, width,
                           precision, zero_pad, display_plus);
        break;
    case OMNI_MIO_SV:
        omni_mio_printsv(fd, arg.as.sv, disp_type, left_align, width, precision,
                         zero_pad, display_plus);
        break;
    }
}
#else
#define OMNI_MIO_TAG(x)                                                        \
    _Generic((x),                                                              \
        char: omni_mio_tag_char,                                               \
        _Bool: omni_mio_tag_bool,                                              \
                                                                               \
        signed char: omni_mio_tag_int,                                         \
        short: omni_mio_tag_int,                                               \
        int: omni_mio_tag_int,                                                 \
        long: omni_mio_tag_int,                                                \
        long long: omni_mio_tag_int,                                           \
                                                                               \
        unsigned char: omni_mio_tag_uint,                                      \
        unsigned short: omni_mio_tag_uint,                                     \
        unsigned int: omni_mio_tag_uint,                                       \
        unsigned long: omni_mio_tag_uint,                                      \
        unsigned long long: omni_mio_tag_uint,                                 \
                                                                               \
        float: omni_mio_tag_float,                                             \
        double: omni_mio_tag_float,                                            \
        long double: omni_mio_tag_float,                                       \
                                                                               \
        char *: omni_mio_tag_cstr,                                             \
        const char *: omni_mio_tag_cstr,                                       \
        omni_string: omni_mio_tag_sv,                                          \
        omni_string_builder: omni_mio_tag_sb,                                  \
                                                                               \
        default: omni_mio_tag_ptr)(x)
#endif // __cplusplus

// -- Preprocessor machinery to apply OMNI_MIO_TAG to every argument of a
// variadic macro call (OMNI_MIO_FOR_EACH) and to count how many were passed
// (OMNI_MIO_ARG_COUNT). Supports 0-16 arguments; to raise that, extend both
// numbered lists below in lockstep (the OMNI_MIO_FE_N chain and the two
// literal countdown lists) and add matching OMNI_MIO_FE_N definitions.

#define OMNI_MIO_CONCAT_(a, b) a##b
#define OMNI_MIO_CONCAT(a, b) OMNI_MIO_CONCAT_(a, b)

#define OMNI_MIO_ARG_COUNT_(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11,  \
                            _12, _13, _14, _15, _16, N, ...)                   \
    N
#define OMNI_MIO_ARG_COUNT(anchor, ...)                                        \
    OMNI_MIO_ARG_COUNT_(anchor, ##__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9,  \
                        8, 7, 6, 5, 4, 3, 2, 1, 0)

#define OMNI_MIO_FE_0(f)
#define OMNI_MIO_FE_1(f, x1) f(x1)
#define OMNI_MIO_FE_2(f, x1, ...) f(x1), OMNI_MIO_FE_1(f, __VA_ARGS__)
#define OMNI_MIO_FE_3(f, x1, ...) f(x1), OMNI_MIO_FE_2(f, __VA_ARGS__)
#define OMNI_MIO_FE_4(f, x1, ...) f(x1), OMNI_MIO_FE_3(f, __VA_ARGS__)
#define OMNI_MIO_FE_5(f, x1, ...) f(x1), OMNI_MIO_FE_4(f, __VA_ARGS__)
#define OMNI_MIO_FE_6(f, x1, ...) f(x1), OMNI_MIO_FE_5(f, __VA_ARGS__)
#define OMNI_MIO_FE_7(f, x1, ...) f(x1), OMNI_MIO_FE_6(f, __VA_ARGS__)
#define OMNI_MIO_FE_8(f, x1, ...) f(x1), OMNI_MIO_FE_7(f, __VA_ARGS__)
#define OMNI_MIO_FE_9(f, x1, ...) f(x1), OMNI_MIO_FE_8(f, __VA_ARGS__)
#define OMNI_MIO_FE_10(f, x1, ...) f(x1), OMNI_MIO_FE_9(f, __VA_ARGS__)
#define OMNI_MIO_FE_11(f, x1, ...) f(x1), OMNI_MIO_FE_10(f, __VA_ARGS__)
#define OMNI_MIO_FE_12(f, x1, ...) f(x1), OMNI_MIO_FE_11(f, __VA_ARGS__)
#define OMNI_MIO_FE_13(f, x1, ...) f(x1), OMNI_MIO_FE_12(f, __VA_ARGS__)
#define OMNI_MIO_FE_14(f, x1, ...) f(x1), OMNI_MIO_FE_13(f, __VA_ARGS__)
#define OMNI_MIO_FE_15(f, x1, ...) f(x1), OMNI_MIO_FE_14(f, __VA_ARGS__)
#define OMNI_MIO_FE_16(f, x1, ...) f(x1), OMNI_MIO_FE_15(f, __VA_ARGS__)

#define OMNI_MIO_FOR_EACH_(N, f, ...)                                          \
    OMNI_MIO_CONCAT(OMNI_MIO_FE_, N)(f, ##__VA_ARGS__)
#define OMNI_MIO_FOR_EACH(f, ...)                                              \
    OMNI_MIO_FOR_EACH_(OMNI_MIO_ARG_COUNT(f, ##__VA_ARGS__), f, ##__VA_ARGS__)

// The runtime half: walks fmt, and for each {...} spec pulls the next
// OMNI_MioArg and dispatches on its .type -- no va_list involved, so no need
// for a type letter in the format string; OMNI_MIO_TAG already recorded the
// type.
OMNIDEF int omni_mio_printf_args(int fd, const char *fmt,
                                 const OMNI_MioArg *args, int argc);

// fmt grammar per {...} spec:
//   (empty)|X|x|o  <- display type (only honored when the arg is unsigned)
//   (empty)|+      <- display plus
//   (empty)|-      <- left align
//   (empty)|0      <- zero pad
//   (empty)|int    <- width
//   (empty)|.int   <- precision
// "{{" prints a literal '{'.
#ifdef __cplusplus
// See the comment on OMNI_DECLTYPE_CAST/OMNI_MIO_TAG above: a C++ array
// compound literal is a temporary that can't decay to a pointer here, so
// build the args array as a named local instead (GNU statement-expression).
#define omni_mio_fprintf(fd, fmt, ...)                                         \
    ({                                                                         \
        const OMNI_MioArg omni_mio__args[] = {                                 \
            OMNI_MIO_FOR_EACH(OMNI_MIO_TAG, ##__VA_ARGS__)};                   \
        omni_mio_printf_args((fd), (fmt), omni_mio__args,                      \
                             OMNI_MIO_ARG_COUNT(fmt, ##__VA_ARGS__));          \
    })
#else
#define omni_mio_fprintf(fd, fmt, ...)                                         \
    omni_mio_printf_args(                                                      \
        (fd), (fmt),                                                           \
        (OMNI_MioArg[]){OMNI_MIO_FOR_EACH(OMNI_MIO_TAG, ##__VA_ARGS__)},       \
        OMNI_MIO_ARG_COUNT(fmt, ##__VA_ARGS__))
#endif // __cplusplus

#define omni_mio_printf(fmt, ...)                                              \
    omni_mio_fprintf(STDOUT_FILENO, (fmt), ##__VA_ARGS__)

/* ---------- STRING BUILDER (omni_sb_*) ---------- */
//
// omni_string_builder wraps a flat, growable byte buffer (`.items`, a
// omni_dynarr(char)) in a one-member struct -- see the struct's own doc
// comment above for why. These are thin conveniences on top of
// omni_da_*/omni_mio_* for the common case of building up a string (or
// arbitrary binary blob) incrementally. Anything omni_da_* offers
// (omni_da_len, omni_da_free, omni_da_foreach, omni_da_clear, ...) works on
// `sb.items` -- the macros below just save typing for the string-shaped
// operations, and read a little more naturally than spelling out `.items`
// everywhere.
//
// A bare omni_string_builder can also be passed directly to
// print/omni_mio_printf's `{}` machinery, or to omni_mio_printtype -- it
// prints by its tracked length, the same as a omni_string does:
//
//   omni_string_builder sb = {0};
//   sb_append_cstr(sb, "count=");
//   print("{}\n", sb); // -> count=
//
// omni_sb_append/_buf/_sv/_cstr/_null, omni_sb_free and omni_sb_clear are
// macros that (like omni_da_push et al.) take the builder itself -- `sb`,
// not `&sb`. omni_sb_appendf, omni_sb_pad_align and omni_sb_to_sv are real
// functions and so need a pointer to the builder -- `&sb` -- since a plain
// C function can't grow the caller's variable otherwise:
//
//   omni_string_builder sb = {0};
//   omni_sb_append_cstr(sb, "count=");     // takes sb
//   omni_sb_appendf(&sb, "{}\n", 42);      // takes &sb
//   omni_string view = omni_sb_to_sv(&sb);  // takes &sb

// Appends a single byte.
#define omni_sb_append(sb, c) omni_da_push((sb).items, (c))

// Appends `size` raw bytes from `buf`.
#define omni_sb_append_buf(sb, buf, size)                                      \
    omni_da_push_many((sb).items, (buf), (size))

// Appends the bytes a omni_string currently views.
#define omni_sb_append_sv(sb, sv)                                              \
    omni_sb_append_buf((sb), (sv).data, (sv).count)

// Appends a NUL-terminated C string's bytes (not including the terminator).
#define omni_sb_append_cstr(sb, cstr)                                          \
    omni_sb_append_buf((sb), (cstr), omni_cstr_len(cstr))

// Appends a single '\0'. This does not "seal" the builder -- appending more
// afterward just grows past it like any other byte. Handy right before
// treating the builder's contents as a C string, e.g.:
//   omni_sb_append_null(sb); some_c_api(sb.items);
#define omni_sb_append_null(sb) omni_da_push((sb).items, '\0')

// Frees the builder's storage and resets it to NULL/empty (same as
// omni_da_free -- provided under the sb_ name for discoverability).
#define omni_sb_free(sb) omni_da_free((sb).items)

// Resets the builder to empty without freeing its storage, so it can be
// reused for the next build without a fresh allocation.
#define omni_sb_clear(sb) omni_da_clear((sb).items)

// Pads `*sb` with zero bytes until its length is a multiple of `size`.
// Useful when building up binary data with alignment requirements. Does
// nothing if `*sb` is already aligned (including when it's empty).
OMNIDEF void omni_sb_pad_align(omni_string_builder *sb, size_t size);

// A read-only view over `sb`'s current contents. The view aliases `sb`'s
// storage -- it becomes invalid the moment `sb` is appended to again (which
// may reallocate), same as any other omni_string that borrows someone else's
// memory instead of owning its own copy.
OMNIDEF omni_string omni_sb_to_sv(omni_string_builder *sb);

// Formats `fmt` (the same {}-based mini-language as omni_mio_printf -- see
// its comment above) and appends the result onto `*sb`, growing it as
// needed. Returns the number of {...} specs actually filled from the
// arguments, same convention as omni_mio_printf_args/omni_mio_fprintf.
OMNIDEF int omni_sb_printf_args(omni_string_builder *sb, const char *fmt,
                                const OMNI_MioArg *args, int argc);

#ifdef __cplusplus
#define omni_sb_appendf(sb, fmt, ...)                                          \
    ({                                                                         \
        const OMNI_MioArg omni_mio__args[] = {                                 \
            OMNI_MIO_FOR_EACH(OMNI_MIO_TAG, ##__VA_ARGS__)};                   \
        omni_sb_printf_args((sb), (fmt), omni_mio__args,                       \
                            OMNI_MIO_ARG_COUNT(fmt, ##__VA_ARGS__));           \
    })
#else
#define omni_sb_appendf(sb, fmt, ...)                                          \
    omni_sb_printf_args(                                                       \
        (sb), (fmt),                                                           \
        (OMNI_MioArg[]){OMNI_MIO_FOR_EACH(OMNI_MIO_TAG, ##__VA_ARGS__)},       \
        OMNI_MIO_ARG_COUNT(fmt, ##__VA_ARGS__))
#endif // __cplusplus

// omni_sb_print(sb) / omni_sb_fprint(fd, sb) print a builder's current
// contents with no format string needed -- shorthand for
// `print("{}", sb)` / `fprint(fd, "{}", sb)`. Take `sb` itself, not `&sb`,
// matching the append-family macros above.
#define omni_sb_print(sb)                                                      \
    omni_mio_printsb(STDOUT_FILENO, (sb), OMNI_MIO_DECIMAL, false, 0, 0,       \
                     false, false)
#define omni_sb_fprint(fd, sb)                                                 \
    omni_mio_printsb((fd), (sb), OMNI_MIO_DECIMAL, false, 0, 0, false, false)

/* LINKED LISTS */

#define omni_ll_type_concat_(a, b) a##b
#define omni_ll_type_concat(a, b) omni_ll_type_concat_(a, b)
#define omni_ll_type_tag(type)                                                 \
    omni_ll_type_concat(omni_ll_type_concat(ll_node_, type),                   \
                        omni_ll_type_concat(_, __LINE__))

#define omni_ll_type(type)                                                     \
    struct {                                                                   \
        struct omni_ll_type_tag(type) {                                        \
            type value;                                                        \
            uintptr_t xored;                                                   \
        } *begin;                                                              \
        struct omni_ll_type_tag(type) * end;                                   \
    }

#define omni_ll_node(ll) __typeof__((ll)->begin)

#define omni_ll_node_create(ll, val)                                           \
    ({                                                                         \
        omni_ll_node(ll) node = (omni_ll_node(ll))malloc(sizeof(*node));       \
        assert(node != NULL);                                                  \
        node->value = (val);                                                   \
        node->xored = 0;                                                       \
        node;                                                                  \
    })

#define omni_ll_append(ll, value)                                              \
    do {                                                                       \
        omni_ll_node(ll) node = omni_ll_node_create(ll, value);                \
                                                                               \
        if ((ll)->end == NULL) {                                               \
            assert((ll)->begin == NULL);                                       \
            (ll)->begin = node;                                                \
            (ll)->end = node;                                                  \
        } else {                                                               \
            node->xored = (uintptr_t)(ll)->end;                                \
            (ll)->end->xored ^= (uintptr_t)node;                               \
            (ll)->end = node;                                                  \
        }                                                                      \
    } while (0)

#define omni_ll_prepend(ll, value)                                             \
    do {                                                                       \
        omni_ll_node(ll) node = omni_ll_node_create(ll, value);                \
                                                                               \
        if ((ll)->begin == NULL) {                                             \
            assert((ll)->end == NULL);                                         \
            (ll)->begin = node;                                                \
            (ll)->end = node;                                                  \
        } else {                                                               \
            node->xored = (uintptr_t)(ll)->begin;                              \
            (ll)->begin->xored ^= (uintptr_t)node;                             \
            (ll)->begin = node;                                                \
        }                                                                      \
    } while (0)

#define omni_ll_next(node, prev)                                               \
    ((__typeof__(node))((node)->xored ^ (uintptr_t)(prev)))

#define omni_ll_prev(node, next)                                               \
    ((__typeof__(node))((node)->xored ^ (uintptr_t)(next)))

#define omni_ll_foreach(ll, node)                                              \
    for (omni_ll_node(ll)                                                      \
             node = (ll)->begin,                                               \
             omni_ll_type_concat(ll_foreach_prev_, __LINE__) = NULL,           \
             omni_ll_type_concat(ll_foreach_next_, __LINE__);                  \
         node != NULL                                                          \
             ? (omni_ll_type_concat(ll_foreach_next_, __LINE__) =              \
                    omni_ll_next(node, omni_ll_type_concat(ll_foreach_prev_,   \
                                                           __LINE__)),         \
                                                      1)                       \
             : 0;                                                              \
         omni_ll_type_concat(ll_foreach_prev_, __LINE__) = node,               \
             node = omni_ll_type_concat(ll_foreach_next_, __LINE__))

#define omni_ll_foreach_reverse(ll, node)                                      \
    for (omni_ll_node(ll)                                                      \
             node = (ll)->end,                                                 \
             omni_ll_type_concat(ll_foreach_next_, __LINE__) = NULL,           \
             omni_ll_type_concat(ll_foreach_prev_, __LINE__);                  \
         node != NULL                                                          \
             ? (omni_ll_type_concat(ll_foreach_prev_, __LINE__) =              \
                    omni_ll_prev(node, omni_ll_type_concat(ll_foreach_next_,   \
                                                           __LINE__)),         \
                                                      1)                       \
             : 0;                                                              \
         omni_ll_type_concat(ll_foreach_next_, __LINE__) = node,               \
             node = omni_ll_type_concat(ll_foreach_prev_, __LINE__))

#define omni_ll_pop_back(ll)                                                   \
    do {                                                                       \
        assert((ll) != NULL);                                                  \
        assert((ll)->end != NULL);                                             \
                                                                               \
        omni_ll_node(ll) node = (ll)->end;                                     \
        omni_ll_node(ll) prev = omni_ll_prev(node, NULL);                      \
                                                                               \
        if (prev == NULL) {                                                    \
            assert((ll)->begin == node);                                       \
            (ll)->begin = NULL;                                                \
            (ll)->end = NULL;                                                  \
        } else {                                                               \
            prev->xored ^= (uintptr_t)node;                                    \
            (ll)->end = prev;                                                  \
        }                                                                      \
                                                                               \
        free(node);                                                            \
    } while (0)

#define omni_ll_pop_front(ll)                                                  \
    do {                                                                       \
        assert((ll) != NULL);                                                  \
        assert((ll)->begin != NULL);                                           \
                                                                               \
        omni_ll_node(ll) node = (ll)->begin;                                   \
        omni_ll_node(ll) next = omni_ll_next(node, NULL);                      \
                                                                               \
        if (next == NULL) {                                                    \
            assert((ll)->end == node);                                         \
            (ll)->begin = NULL;                                                \
            (ll)->end = NULL;                                                  \
        } else {                                                               \
            next->xored ^= (uintptr_t)node;                                    \
            (ll)->begin = next;                                                \
        }                                                                      \
                                                                               \
        free(node);                                                            \
    } while (0)

#define omni_ll_clear(ll)                                                      \
    do {                                                                       \
        omni_ll_node(ll) prev = NULL;                                          \
        omni_ll_node(ll) node = (ll)->begin;                                   \
                                                                               \
        while (node != NULL) {                                                 \
            omni_ll_node(ll) next = omni_ll_next(node, prev);                  \
                                                                               \
            free(node);                                                        \
                                                                               \
            prev = node;                                                       \
            node = next;                                                       \
        }                                                                      \
                                                                               \
        (ll)->begin = NULL;                                                    \
        (ll)->end = NULL;                                                      \
    } while (0)

#define omni_ll_len(ll)                                                        \
    ({                                                                         \
        size_t count = 0;                                                      \
        omni_ll_node(ll) prev = NULL;                                          \
        omni_ll_node(ll) node = (ll)->begin;                                   \
                                                                               \
        while (node != NULL) {                                                 \
            omni_ll_node(ll) next = omni_ll_next(node, prev);                  \
                                                                               \
            ++count;                                                           \
            prev = node;                                                       \
            node = next;                                                       \
        }                                                                      \
                                                                               \
        count;                                                                 \
    })

/* ---------- LOGGING (omni_log_*) ---------- */

typedef enum {
    OMNI_TRACE,
    OMNI_DEBUG,
    OMNI_INFO,
    OMNI_WARN,
    OMNI_ERROR,
    OMNI_FATAL,
    OMNI_NO_LOGS
} OmniLogLevel;

// Messages below this level are dropped by omni_log -- e.g. setting this to
// OMNI_WARN silences OMNI_TRACE/OMNI_DEBUG/OMNI_INFO but still logs
// OMNI_WARN/OMNI_ERROR/OMNI_FATAL. OMNI_NO_LOGS silences omni_log entirely.
// Defaults to OMNI_TRACE (nothing is filtered).
extern OmniLogLevel omni_minimum_log_level;

extern bool omni_colorful_logs;

// Not meant to be called directly -- go through the omni_log macro below,
// which fills in `file`/`line` from the call site and tags `args`/`argc`
// for you, the same way omni_mio_fprintf/omni_sb_appendf do.
OMNIDEF void omni_log_impl(OmniLogLevel level, const char *file, int line,
                           const char *fmt, const OMNI_MioArg *args, int argc);

// Logs `fmt` (same {}-based mini-language as omni_mio_printf -- see its
// comment above) at `level`, together with any {}-tagged arguments, and
// terminates the line for you -- unlike omni_mio_printf/omni_sb_appendf,
// `fmt` should NOT end in "\n". OMNI_TRACE/OMNI_ERROR/OMNI_FATAL are
// additionally prefixed with the call site's `file:line`; every level is
// prefixed with its name. Writes to stdout, except OMNI_ERROR/OMNI_FATAL
// which go to stderr. No-op for levels below omni_minimum_log_level.
#ifdef __cplusplus
#define omni_log(level, fmt, ...)                                              \
    ({                                                                         \
        const OMNI_MioArg omni_mio__args[] = {                                 \
            OMNI_MIO_FOR_EACH(OMNI_MIO_TAG, ##__VA_ARGS__)};                   \
        omni_log_impl((level), __FILE__, __LINE__, (fmt), omni_mio__args,      \
                      OMNI_MIO_ARG_COUNT(fmt, ##__VA_ARGS__));                 \
    })
#else
#define omni_log(level, fmt, ...)                                              \
    omni_log_impl(                                                             \
        (level), __FILE__, __LINE__, (fmt),                                    \
        (OMNI_MioArg[]){OMNI_MIO_FOR_EACH(OMNI_MIO_TAG, ##__VA_ARGS__)},       \
        OMNI_MIO_ARG_COUNT(fmt, ##__VA_ARGS__))
#endif // __cplusplus

#endif // OMNI_H

#ifdef OMNI_IMPLEMENTATION

/* ---------- DYNAMIC ARRAYS impl ---------- */

OMNIDEF void *omni_da_alloc(size_t element_size, size_t capacity) {
    OMNI_DAHeader *header = (OMNI_DAHeader *)malloc(sizeof(OMNI_DAHeader) +
                                                    element_size * capacity);

    if (header == NULL)
        return NULL;

    header->count = 0;
    header->capacity = capacity;

    return header + 1;
}

OMNIDEF void *omni_da_grow(void *arr, size_t element_size,
                           size_t min_capacity) {
    OMNI_DAHeader *header = (OMNI_DAHeader *)arr - 1;
    size_t capacity = header->capacity;

    while (capacity < min_capacity)
        capacity *= 2;

    header = (OMNI_DAHeader *)realloc(header, sizeof(OMNI_DAHeader) +
                                                  element_size * capacity);

    if (header == NULL)
        return NULL;

    header->capacity = capacity;
    return header + 1;
}

/* ---------- STRING VIEWS impl ---------- */

OMNIDEF size_t omni_cstr_len(const char *cstr) {
    size_t c = 0;
    while (*cstr != '\0') {
        c++;
        cstr++;
    }
    return c;
}

OMNIDEF omni_string omni_sv(const char *cstr) {
    omni_string sv;
    sv.count = omni_cstr_len(cstr);
    sv.data = cstr;
    return sv;
}

OMNIDEF void omni_sv_chop_left(omni_string *sv, size_t n) {
    if (n > sv->count)
        n = sv->count;
    sv->count -= n;
    sv->data += n;
}

OMNIDEF void omni_sv_chop_right(omni_string *sv, size_t n) {
    if (n > sv->count)
        n = sv->count;
    sv->count -= n;
}

OMNIDEF void omni_sv_trim_left(omni_string *sv) {
    while (sv->count > 0 && isspace((unsigned char)sv->data[0])) {
        omni_sv_chop_left(sv, 1);
    }
}

OMNIDEF void omni_sv_trim_right(omni_string *sv) {
    while (sv->count > 0 && isspace((unsigned char)sv->data[sv->count - 1])) {
        omni_sv_chop_right(sv, 1);
    }
}

OMNIDEF void omni_sv_trim(omni_string *sv) {
    omni_sv_trim_left(sv);
    omni_sv_trim_right(sv);
}

OMNIDEF bool omni_sv_eq(omni_string a, omni_string b) {
    if (a.count != b.count)
        return false;

    for (size_t i = 0; i < a.count; i++) {
        if (a.data[i] != b.data[i])
            return false;
    }

    return true;
}

OMNIDEF bool omni_sv_eq_ignorecase(omni_string a, omni_string b) {
    if (a.count != b.count)
        return false;

    for (size_t i = 0; i < a.count; i++) {
        if (tolower((unsigned char)a.data[i]) !=
            tolower((unsigned char)b.data[i]))
            return false;
    }

    return true;
}

OMNIDEF bool omni_sv_starts_with(omni_string sv, omni_string prefix) {
    if (prefix.count > sv.count)
        return false;

    for (size_t i = 0; i < prefix.count; i++) {
        if (sv.data[i] != prefix.data[i])
            return false;
    }

    return true;
}

OMNIDEF bool omni_sv_ends_with(omni_string sv, omni_string suffix) {
    if (suffix.count > sv.count)
        return false;

    size_t offset = sv.count - suffix.count;

    for (size_t i = 0; i < suffix.count; i++) {
        if (sv.data[offset + i] != suffix.data[i])
            return false;
    }

    return true;
}

OMNIDEF int omni_sv_cmp(omni_string a, omni_string b) {
    size_t count = a.count < b.count ? a.count : b.count;

    for (size_t i = 0; i < count; i++) {
        unsigned char ac = (unsigned char)a.data[i];
        unsigned char bc = (unsigned char)b.data[i];

        if (ac < bc)
            return -1;

        if (ac > bc)
            return 1;
    }

    if (a.count < b.count)
        return -1;

    if (a.count > b.count)
        return 1;

    return 0;
}

OMNIDEF omni_string omni_sv_chop_by_delim(omni_string *sv, char delim) {
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delim) {
        i++;
    }

    if (i < sv->count) {
        omni_string result;
        result.count = i;
        result.data = sv->data;
        omni_sv_chop_left(sv, i + 1);
        return result;
    }

    omni_string result = *sv;
    omni_sv_chop_left(sv, sv->count);
    return result;
}

OMNIDEF omni_string omni_sv_chop_if(omni_string *sv, int (*pred)(int c)) {
    size_t i = 0;
    while (i < sv->count && !pred(sv->data[i])) {
        i++;
    }

    if (i < sv->count) {
        omni_string result;
        result.count = i;
        result.data = sv->data;
        omni_sv_chop_left(sv, i + 1);
        return result;
    }

    omni_string result = *sv;
    omni_sv_chop_left(sv, sv->count);
    return result;
}

OMNIDEF void omni__sv_to_cstr(omni_string sv, char *out) {
    for (size_t i = 0; i < sv.count; i++) {
        *out = sv.data[i];
        out++;
    }
    *out = 0;
}

OMNIDEF const char *omni_sv_to_cstr(omni_string sv) {
    char *c = (char *)malloc(sv.count + 1);
    omni__sv_to_cstr(sv, c);
    return c;
}

/* ---------- HASHMAPS impl ---------- */

OMNIDEF uint64_t omni_hm_hash(omni_string key) {
    uint64_t result = 14695981039346656037ULL;
    for (size_t i = 0; i < key.count; ++i) {
        result ^= (uint8_t)key.data[i];
        result *= 1099511628211ULL;
    }
    return result;
}

OMNIDEF void *omni_hm__alloc(size_t stride, size_t capacity) {
    OMNI_HMHeader *header =
        (OMNI_HMHeader *)malloc(sizeof(OMNI_HMHeader) + stride * capacity);

    if (header == NULL)
        return NULL;

    header->count = 0;
    header->capacity = capacity;

    void *entries = header + 1;
    memset(entries, 0, stride * capacity);

    return entries;
}

OMNIDEF size_t omni_hm__probe(const void *entries, size_t stride,
                              size_t capacity, omni_string key) {
    size_t index = omni_hm_hash(key) % capacity;

    for (;;) {
        const omni_string *slot_key =
            (const omni_string *)((const char *)entries + index * stride);

        if (slot_key->data == NULL || omni_sv_eq(*slot_key, key))
            return index;

        index = (index + 1) % capacity;
    }
}

OMNIDEF void omni_hm__rehash(const void *old_entries, size_t old_capacity,
                             void *new_entries, size_t stride,
                             size_t new_capacity, size_t *count) {
    for (size_t i = 0; i < old_capacity; ++i) {
        const omni_string *key =
            (const omni_string *)((const char *)old_entries + i * stride);

        if (key->data == NULL)
            continue;

        size_t index = omni_hm__probe(new_entries, stride, new_capacity, *key);

        memcpy((char *)new_entries + index * stride,
               (const char *)old_entries + i * stride, stride);

        (*count)++;
    }
}

OMNIDEF bool omni_hm__remove(void *entries, size_t stride, size_t capacity,
                             omni_string key, size_t *count) {
    size_t index = omni_hm_hash(key) % capacity;

    for (;;) {
        omni_string *slot_key =
            (omni_string *)((char *)entries + index * stride);

        if (slot_key->data == NULL)
            return false;

        if (omni_sv_eq(*slot_key, key))
            break;

        index = (index + 1) % capacity;
    }

    ((omni_string *)((char *)entries + index * stride))->data = NULL;
    (*count)--;

#ifdef __cplusplus
    // A VLA is a GNU/C99 feature, not standard C++ -- allocate on the heap
    // here instead. (The C path below still uses the VLA, unchanged.)
    char *scratch = (char *)malloc(stride);
    assert(scratch != NULL);
#else
    char scratch[stride];
#endif
    size_t next = (index + 1) % capacity;

    while (((omni_string *)((char *)entries + next * stride))->data != NULL) {
        memcpy(scratch, (char *)entries + next * stride, stride);
        ((omni_string *)((char *)entries + next * stride))->data = NULL;
        (*count)--;

        omni_string entry_key = *(omni_string *)scratch;
        size_t new_index = omni_hm_hash(entry_key) % capacity;

        while (((omni_string *)((char *)entries + new_index * stride))->data !=
               NULL)
            new_index = (new_index + 1) % capacity;

        memcpy((char *)entries + new_index * stride, scratch, stride);
        (*count)++;

        next = (next + 1) % capacity;
    }

#ifdef __cplusplus
    free(scratch);
#endif
    return true;
}

/* ---------- PRINTING impl ---------- */

static void omni_mio_reverse_cstr(char *str) {
    int len = 0, i;
    while (str[len])
        len++;
    for (i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

// Small local stand-ins for strlen/strcpy so this section doesn't pull in
// more of <string.h> than the rest of the file already needs.
static int omni_mio_strlen(const char *s) {
    int n = 0;
    while (s[n])
        n++;
    return n;
}

static char *omni_mio_strcpy(char *dst, const char *src) {
    char *d = dst;
    while ((*d++ = *src++))
        ;
    return dst;
}

OMNIDEF int omni_mio_putc(int fd, char c) { return write(fd, &c, 1); }

OMNIDEF int omni_mio_putcstr(int fd, const char *cstr, int len) {
    return write(fd, cstr, len);
}

OMNIDEF int omni_mio_putsv(int fd, omni_string sv) {
    return omni_mio_putcstr(fd, sv.data, sv.count);
}

// -- Output sinks --------------------------------------------------------
//
// Everything from omni_mio_putcstr_padded down through omni_mio_printf_args
// ultimately just does two things: convert a value to text, and write that
// text out somewhere with padding. "Somewhere" used to always mean a raw
// fd. To let omni_sb_appendf reuse the exact same {}-format/padding/type
// logic to append into a omni_string_builder instead of writing to an fd,
// each of those functions is a thin fd-shaped public wrapper (unchanged
// signature -- still just takes `int fd`, exactly as before) around a
// private "sink"-shaped core that can target either. OMNI_MioSink and the
// omni_mio__sink_* helpers below are purely an implementation detail: no
// public macro or prototype ever mentions them.
typedef enum { OMNI_MIO__SINK_FD, OMNI_MIO__SINK_SB } OMNI_MioSinkKind;

typedef struct {
    OMNI_MioSinkKind kind;
    union {
        int fd;
        omni_string_builder *sb;
    } as;
} OMNI_MioSink;

static OMNI_MioSink omni_mio__sink_fd(int fd) {
    OMNI_MioSink sink;
    sink.kind = OMNI_MIO__SINK_FD;
    sink.as.fd = fd;
    return sink;
}

static OMNI_MioSink omni_mio__sink_sb(omni_string_builder *sb) {
    OMNI_MioSink sink;
    sink.kind = OMNI_MIO__SINK_SB;
    sink.as.sb = sb;
    return sink;
}

static int omni_mio__sink_putc(OMNI_MioSink sink, char c) {
    if (sink.kind == OMNI_MIO__SINK_FD)
        return omni_mio_putc(sink.as.fd, c);
    omni_da_push(sink.as.sb->items, c);
    return 1;
}

static int omni_mio__sink_putcstr(OMNI_MioSink sink, const char *cstr,
                                  int len) {
    if (sink.kind == OMNI_MIO__SINK_FD)
        return omni_mio_putcstr(sink.as.fd, cstr, len);
    omni_da_push_many(sink.as.sb->items, cstr, (size_t)len);
    return len;
}

static void omni_mio__sink_putcstr_padded(OMNI_MioSink sink, const char *cstr,
                                          int len, int width, int left_align,
                                          char pad_char) {
    int pad = (width > len) ? width - len : 0;

    if (!left_align) {
        for (int i = 0; i < pad; i++)
            omni_mio__sink_putc(sink, pad_char);
    }

    omni_mio__sink_putcstr(sink, cstr, len);

    if (left_align) {
        for (int i = 0; i < pad; i++)
            omni_mio__sink_putc(sink, pad_char);
    }
}

OMNIDEF void omni_mio_putcstr_padded(int fd, const char *cstr, int len,
                                     int width, int left_align, char pad_char) {
    omni_mio__sink_putcstr_padded(omni_mio__sink_fd(fd), cstr, len, width,
                                  left_align, pad_char);
}

OMNIDEF void omni_mio_putsv_padded(int fd, omni_string sv, int width,
                                   int left_align, char pad_char) {
    omni_mio_putcstr_padded(fd, sv.data, sv.count, width, left_align, pad_char);
}

OMNIDEF int omni_mio_itos(int64_t n, char *buf) {
    int i = 0, sign = 0;

    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        buf[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        buf[i++] = '-';
    buf[i] = '\0';
    omni_mio_reverse_cstr(buf);
    return i;
}

OMNIDEF int omni_mio_utos(uint64_t n, char *buf) {
    int i = 0;
    if (n == 0)
        buf[i++] = '0';
    while (n != 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }
    buf[i] = '\0';
    omni_mio_reverse_cstr(buf);
    return i;
}

OMNIDEF int omni_mio_itoh(uint64_t n, char *buf, bool uppercase) {
    const char *hex = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    int i = 0;
    do {
        buf[i++] = hex[n % 16];
        n /= 16;
    } while (n);
    buf[i] = '\0';
    omni_mio_reverse_cstr(buf);
    return i;
}

OMNIDEF int omni_mio_itoo(uint64_t n, char *buf) {
    int i = 0;
    do {
        buf[i++] = (n % 8) + '0';
        n /= 8;
    } while (n);
    buf[i] = '\0';
    omni_mio_reverse_cstr(buf);
    return i;
}

OMNIDEF int omni_mio_ftos(double n, char *buf, int precision) {
    bool negative = n < 0;
    if (negative)
        n = -n;

    int int_part = (int)n;
    double frac_part = n - int_part;

    int int_len = 0;
    if (negative)
        buf[int_len++] = '-';
    int_len += omni_mio_itos(int_part, buf + int_len);

    int frac_len = 0;

    if (precision > 0) {
        char *p = buf + int_len;
        *p++ = '.';
        frac_len = 1;

        for (int i = 0; i < precision; i++) {
            frac_part *= 10;
        }

        char frac_digits[32];
        // +0.5 to round to nearest instead of truncating toward zero --
        // without it, e.g. 1.2 (stored as 1.199999...) renders as "1.1999"
        // instead of "1.2000".
        int frac_digit_count =
            omni_mio_utos((uint64_t)(frac_part + 0.5), frac_digits);

        // Zero-pad on the left up to `precision` digits, e.g. 0.05 with
        // precision=2 must render "05", not "5".
        for (int i = frac_digit_count; i < precision; i++) {
            *p++ = '0';
            frac_len++;
        }

        for (int i = 0; i < frac_digit_count; i++)
            *p++ = frac_digits[i];
        *p = '\0';
        frac_len += frac_digit_count;
    }

    return int_len + frac_len;
}

static void omni_mio__sink_printchar(OMNI_MioSink sink, char c,
                                     OMNI_MioDisplayType disp_type,
                                     bool left_align, int width, int precision,
                                     bool zero_pad, bool display_plus) {
    (void)disp_type;    // not meaningful for a single character
    (void)precision;    // not meaningful for a single character
    (void)display_plus; // not meaningful for a single character
    // NOTE: matching printf's %c, zero_pad is ignored here -- padding is
    // always done with spaces, only width/left_align apply.
    (void)zero_pad;
    omni_mio__sink_putcstr_padded(sink, &c, 1, width, left_align, ' ');
}

OMNIDEF void omni_mio_printchar(int fd, char c, OMNI_MioDisplayType disp_type,
                                bool left_align, int width, int precision,
                                bool zero_pad, bool display_plus) {
    omni_mio__sink_printchar(omni_mio__sink_fd(fd), c, disp_type, left_align,
                             width, precision, zero_pad, display_plus);
}

static void omni_mio__sink_printbool(OMNI_MioSink sink, bool val,
                                     OMNI_MioDisplayType disp_type,
                                     bool left_align, int width, int precision,
                                     bool zero_pad, bool display_plus) {
    (void)disp_type;    // not meaningful for "true"/"false"
    (void)precision;    // not meaningful for "true"/"false"
    (void)zero_pad;     // matches printf's %s-like behavior: only space pad
    (void)display_plus; // not meaningful for "true"/"false"

    const char *s = val ? "true" : "false";
    omni_mio__sink_putcstr_padded(sink, s, omni_mio_strlen(s), width,
                                  left_align, ' ');
}

OMNIDEF void omni_mio_printbool(int fd, bool val, OMNI_MioDisplayType disp_type,
                                bool left_align, int width, int precision,
                                bool zero_pad, bool display_plus) {
    omni_mio__sink_printbool(omni_mio__sink_fd(fd), val, disp_type, left_align,
                             width, precision, zero_pad, display_plus);
}

static void omni_mio__sink_printint(OMNI_MioSink sink, int64_t val,
                                    OMNI_MioDisplayType disp_type,
                                    bool left_align, int width, int precision,
                                    bool zero_pad, bool display_plus) {
    // Signed integers are always printed in decimal -- HEX_LOW/HEX_UPPER/
    // OCTAL are only offered on omni_mio_printuint, matching how real printf
    // only allows %x/%o on unsigned conversions.
    (void)disp_type;

    char buffer[1024];
    int len = omni_mio_itos(val, buffer);

    bool negative = buffer[0] == '-';
    char sign_char = negative ? '-' : (display_plus ? '+' : 0);
    const char *digits = negative ? buffer + 1 : buffer;
    int digit_len = negative ? len - 1 : len;

    int total_pad = 0;
    if (precision > 0 && precision > digit_len)
        total_pad = precision - digit_len;

    if (sign_char && zero_pad && !left_align) {
        omni_mio__sink_putc(sink, sign_char);
        omni_mio__sink_putcstr_padded(sink, digits, digit_len, width - 1, 0,
                                      '0');
    } else {
        char temp[1024];
        int idx = 0;
        if (sign_char)
            temp[idx++] = sign_char;
        for (int i = 0; i < total_pad; i++)
            temp[idx++] = '0';
        omni_mio_strcpy(temp + idx, digits);
        omni_mio__sink_putcstr_padded(sink, temp, omni_mio_strlen(temp), width,
                                      left_align,
                                      zero_pad && !left_align ? '0' : ' ');
    }
}

OMNIDEF void omni_mio_printint(int fd, int64_t val,
                               OMNI_MioDisplayType disp_type, bool left_align,
                               int width, int precision, bool zero_pad,
                               bool display_plus) {
    omni_mio__sink_printint(omni_mio__sink_fd(fd), val, disp_type, left_align,
                            width, precision, zero_pad, display_plus);
}

static void omni_mio__sink_printuint(OMNI_MioSink sink, uint64_t val,
                                     OMNI_MioDisplayType disp_type,
                                     bool left_align, int width, int precision,
                                     bool zero_pad, bool display_plus) {
    // '+' has no meaning for an unsigned value, and real printf ignores the
    // flag for %u/%x/%o too.
    (void)display_plus;

    char buffer[1024];
    int len;
    switch (disp_type) {
    case OMNI_MIO_HEX_LOW:
        len = omni_mio_itoh(val, buffer, false);
        break;
    case OMNI_MIO_HEX_UPPER:
        len = omni_mio_itoh(val, buffer, true);
        break;
    case OMNI_MIO_OCTAL:
        len = omni_mio_itoo(val, buffer);
        break;
    case OMNI_MIO_DECIMAL:
    default:
        len = omni_mio_utos(val, buffer);
        break;
    }

    int total_pad = 0;
    if (precision > 0 && precision > len)
        total_pad = precision - len;

    char temp[1024];
    int idx = 0;
    for (int i = 0; i < total_pad; i++)
        temp[idx++] = '0';
    omni_mio_strcpy(temp + idx, buffer);

    omni_mio__sink_putcstr_padded(sink, temp, omni_mio_strlen(temp), width,
                                  left_align,
                                  zero_pad && !left_align ? '0' : ' ');
}

OMNIDEF void omni_mio_printuint(int fd, uint64_t val,
                                OMNI_MioDisplayType disp_type, bool left_align,
                                int width, int precision, bool zero_pad,
                                bool display_plus) {
    omni_mio__sink_printuint(omni_mio__sink_fd(fd), val, disp_type, left_align,
                             width, precision, zero_pad, display_plus);
}

static void omni_mio__sink_printfloat(OMNI_MioSink sink, double val,
                                      OMNI_MioDisplayType disp_type,
                                      bool left_align, int width, int precision,
                                      bool zero_pad, bool display_plus) {
    // Floating point values are always printed in decimal.
    (void)disp_type;

    char buffer[1024];
    int len = omni_mio_ftos(val, buffer, precision);

    bool negative = buffer[0] == '-';
    char sign_char = negative ? '-' : (display_plus ? '+' : 0);
    const char *digits = negative ? buffer + 1 : buffer;
    int digit_len = negative ? len - 1 : len;

    if (sign_char && zero_pad && !left_align) {
        omni_mio__sink_putc(sink, sign_char);
        omni_mio__sink_putcstr_padded(sink, digits, digit_len, width - 1, 0,
                                      '0');
    } else {
        char temp[1024];
        int idx = 0;
        if (sign_char)
            temp[idx++] = sign_char;
        omni_mio_strcpy(temp + idx, digits);
        omni_mio__sink_putcstr_padded(sink, temp, omni_mio_strlen(temp), width,
                                      left_align,
                                      zero_pad && !left_align ? '0' : ' ');
    }
}

OMNIDEF void omni_mio_printfloat(int fd, double val,
                                 OMNI_MioDisplayType disp_type, bool left_align,
                                 int width, int precision, bool zero_pad,
                                 bool display_plus) {
    omni_mio__sink_printfloat(omni_mio__sink_fd(fd), val, disp_type, left_align,
                              width, precision, zero_pad, display_plus);
}

static void omni_mio__sink_printptr(OMNI_MioSink sink, const void *ptr,
                                    OMNI_MioDisplayType disp_type,
                                    bool left_align, int width, int precision,
                                    bool zero_pad, bool display_plus) {
    // %p is always hex with an automatic "0x" prefix; precision has no
    // defined meaning here (glibc ignores it too), and a pointer has no
    // sign.
    (void)disp_type;
    (void)precision;
    (void)display_plus;

    if (ptr == NULL) {
        omni_mio__sink_putcstr_padded(sink, "(nil)", 5, width, left_align, ' ');
        return;
    }

    char digits[1024];
    int digit_len = omni_mio_itoh((uint64_t)(uintptr_t)ptr, digits, false);

    if (zero_pad && !left_align) {
        omni_mio__sink_putcstr(sink, "0x", 2);
        omni_mio__sink_putcstr_padded(sink, digits, digit_len, width - 2, 0,
                                      '0');
    } else {
        char temp[1024];
        temp[0] = '0';
        temp[1] = 'x';
        omni_mio_strcpy(temp + 2, digits);
        omni_mio__sink_putcstr_padded(sink, temp, omni_mio_strlen(temp), width,
                                      left_align, ' ');
    }
}

OMNIDEF void omni_mio_printptr(int fd, const void *ptr,
                               OMNI_MioDisplayType disp_type, bool left_align,
                               int width, int precision, bool zero_pad,
                               bool display_plus) {
    omni_mio__sink_printptr(omni_mio__sink_fd(fd), ptr, disp_type, left_align,
                            width, precision, zero_pad, display_plus);
}

static void omni_mio__sink_printcstr(OMNI_MioSink sink, const char *cstr,
                                     OMNI_MioDisplayType disp_type,
                                     bool left_align, int width, int precision,
                                     bool zero_pad, bool display_plus) {
    // None of these are meaningful for a string -- matches printf, which
    // ignores the 0 flag and disallows +/hex conversions for %s.
    (void)disp_type;
    (void)zero_pad;
    (void)display_plus;

    int len = omni_mio_strlen(cstr);
    if (precision > 0 && precision < len)
        len = precision;

    omni_mio__sink_putcstr_padded(sink, cstr, len, width, left_align, ' ');
}

OMNIDEF void omni_mio_printcstr(int fd, const char *cstr,
                                OMNI_MioDisplayType disp_type, bool left_align,
                                int width, int precision, bool zero_pad,
                                bool display_plus) {
    omni_mio__sink_printcstr(omni_mio__sink_fd(fd), cstr, disp_type, left_align,
                             width, precision, zero_pad, display_plus);
}

static void omni_mio__sink_printsv(OMNI_MioSink sink, omni_string sv,
                                   OMNI_MioDisplayType disp_type,
                                   bool left_align, int width, int precision,
                                   bool zero_pad, bool display_plus) {
    (void)disp_type;
    (void)zero_pad;
    (void)display_plus;

    int len = (int)sv.count;
    if (precision > 0 && precision < len)
        len = precision;

    omni_mio__sink_putcstr_padded(sink, sv.data, len, width, left_align, ' ');
}

OMNIDEF void omni_mio_printsv(int fd, omni_string sv,
                              OMNI_MioDisplayType disp_type, bool left_align,
                              int width, int precision, bool zero_pad,
                              bool display_plus) {
    omni_mio__sink_printsv(omni_mio__sink_fd(fd), sv, disp_type, left_align,
                           width, precision, zero_pad, display_plus);
}

OMNIDEF void omni_mio_printsb(int fd, omni_string_builder sb,
                              OMNI_MioDisplayType disp_type, bool left_align,
                              int width, int precision, bool zero_pad,
                              bool display_plus) {
    omni_string view;
    view.count = omni_da_len(sb.items);
    view.data = sb.items;
    omni_mio_printsv(fd, view, disp_type, left_align, width, precision,
                     zero_pad, display_plus);
}

static int omni_mio_parse_uint_from_fmt(const char *fmt, int *i) {
    int n = 0;
    while (fmt[*i] >= '0' && fmt[*i] <= '9') {
        n = n * 10 + (fmt[*i] - '0');
        (*i)++;
    }
    return n;
}

// The shared core behind both omni_mio_printf_args (sink = an fd) and
// omni_sb_printf_args (sink = a omni_string_builder). Walks fmt exactly as
// before; only every actual "write this out" call changed from the old
// fd-only omni_mio_put*/omni_mio_print* to their omni_mio__sink_* twins.
static int omni_mio__printf_sink(OMNI_MioSink sink, const char *fmt,
                                 const OMNI_MioArg *args, int argc) {
    int run_start = 0;
    int i = 0;
    int argi = 0;

    while (fmt[i] != '\0') {
        if (fmt[i] != '{') {
            i++;
            continue;
        }

        // "{{" -> literal '{'
        if (fmt[i + 1] == '{') {
            omni_mio__sink_putcstr(sink, fmt + run_start, i - run_start + 1);
            i += 2;
            run_start = i;
            continue;
        }

        if (i > run_start)
            omni_mio__sink_putcstr(sink, fmt + run_start, i - run_start);

        int spec_start = i;
        i++; // skip '{'

        OMNI_MioDisplayType disp_type = OMNI_MIO_DECIMAL;
        if (fmt[i] == 'X') {
            disp_type = OMNI_MIO_HEX_UPPER;
            i++;
        } else if (fmt[i] == 'x') {
            disp_type = OMNI_MIO_HEX_LOW;
            i++;
        } else if (fmt[i] == 'o') {
            disp_type = OMNI_MIO_OCTAL;
            i++;
        }

        bool display_plus = false;
        if (fmt[i] == '+') {
            display_plus = true;
            i++;
        }

        bool left_align = false;
        if (fmt[i] == '-') {
            left_align = true;
            i++;
        }

        bool zero_pad = false;
        if (fmt[i] == '0') {
            zero_pad = true;
            i++;
        }

        int width = omni_mio_parse_uint_from_fmt(fmt, &i);

        int precision = 0;
        if (fmt[i] == '.') {
            i++;
            precision = omni_mio_parse_uint_from_fmt(fmt, &i);
        }

        if (fmt[i] != '}') {
            // Malformed spec -- print it verbatim rather than guessing.
            omni_mio__sink_putcstr(sink, fmt + spec_start, i - spec_start);
            run_start = i;
            continue;
        }
        i++; // skip '}'

        if (argi >= argc) {
            // Ran out of arguments for the specs in fmt -- print the spec
            // verbatim instead of reading past the end of args[].
            omni_mio__sink_putcstr(sink, fmt + spec_start, i - spec_start);
            run_start = i;
            continue;
        }

        OMNI_MioArg arg = args[argi++];
        switch (arg.type) {
        case OMNI_MIO_CHAR:
            omni_mio__sink_printchar(sink, arg.as.c, disp_type, left_align,
                                     width, precision, zero_pad, display_plus);
            break;
        case OMNI_MIO_BOOL:
            omni_mio__sink_printbool(sink, arg.as.b, disp_type, left_align,
                                     width, precision, zero_pad, display_plus);
            break;
        case OMNI_MIO_INT:
            omni_mio__sink_printint(sink, arg.as.i, disp_type, left_align,
                                    width, precision, zero_pad, display_plus);
            break;
        case OMNI_MIO_UINT:
            omni_mio__sink_printuint(sink, arg.as.u, disp_type, left_align,
                                     width, precision, zero_pad, display_plus);
            break;
        case OMNI_MIO_FLOAT:
            omni_mio__sink_printfloat(sink, arg.as.f, disp_type, left_align,
                                      width, precision, zero_pad, display_plus);
            break;
        case OMNI_MIO_CSTR:
            omni_mio__sink_printcstr(sink, arg.as.s, disp_type, left_align,
                                     width, precision, zero_pad, display_plus);
            break;
        case OMNI_MIO_SV:
            omni_mio__sink_printsv(sink, arg.as.sv, disp_type, left_align,
                                   width, precision, zero_pad, display_plus);
            break;
        case OMNI_MIO_PTR:
        default:
            omni_mio__sink_printptr(sink, arg.as.p, disp_type, left_align,
                                    width, precision, zero_pad, display_plus);
            break;
        }

        run_start = i;
    }

    if (i > run_start)
        omni_mio__sink_putcstr(sink, fmt + run_start, i - run_start);

    return argi; // number of {...} specs actually filled from args[]
}

OMNIDEF int omni_mio_printf_args(int fd, const char *fmt,
                                 const OMNI_MioArg *args, int argc) {
    return omni_mio__printf_sink(omni_mio__sink_fd(fd), fmt, args, argc);
}

/* ---------- STRING BUILDER impl ---------- */

OMNIDEF int omni_sb_printf_args(omni_string_builder *sb, const char *fmt,
                                const OMNI_MioArg *args, int argc) {
    return omni_mio__printf_sink(omni_mio__sink_sb(sb), fmt, args, argc);
}

OMNIDEF void omni_sb_pad_align(omni_string_builder *sb, size_t size) {
    assert(size > 0);
    size_t len = omni_da_len(sb->items);
    size_t rem = len % size;
    if (rem == 0)
        return;
    size_t pad = size - rem;
    for (size_t i = 0; i < pad; i++)
        omni_da_push(sb->items, 0);
}

OMNIDEF omni_string omni_sb_to_sv(omni_string_builder *sb) {
    omni_string result;
    result.count = omni_da_len(sb->items);
    result.data = sb->items;
    return result;
}

/* ---------- LOGGING impl ---------- */

OmniLogLevel omni_minimum_log_level = OMNI_TRACE;
bool omni_colorful_logs = false;

OMNIDEF void omni_log_impl(OmniLogLevel level, const char *file, int line,
                           const char *fmt, const OMNI_MioArg *args, int argc) {
    if ((int)level < (int)omni_minimum_log_level)
        return;

    int fd = STDOUT_FILENO;
    bool show_line_info = false;
    const char *prefix = NULL;
    const char *color_code = "";

    switch (level) {
    case OMNI_TRACE:
        prefix = "[TRACE]";
        show_line_info = true;
        if (omni_colorful_logs)
            color_code = "\x1b[3m"; // italic
        break;
    case OMNI_DEBUG:
        prefix = "[DEBUG]";
        show_line_info = false;
        if (omni_colorful_logs)
            color_code = "\x1b[36m"; // cyan
        break;
    case OMNI_INFO:
        prefix = " [INFO]";
        show_line_info = false;
        if (omni_colorful_logs)
            color_code = "\x1b[32m"; // green
        break;
    case OMNI_WARN:
        prefix = " [WARN]";
        show_line_info = false;
        if (omni_colorful_logs)
            color_code = "\x1b[35m"; // magenta
        break;
    case OMNI_ERROR:
        prefix = "[ERROR]";
        show_line_info = true;
        if (omni_colorful_logs)
            color_code = "\x1b[3m\x1b[31m"; // italic red
        fd = STDERR_FILENO;
        break;
    case OMNI_FATAL:
        prefix = "[FATAL]";
        show_line_info = true;
        if (omni_colorful_logs)
            color_code = "\x1b[1m\x1b[31m"; // bold red
        fd = STDERR_FILENO;
        break;
    default:
        break;
    }

    if (show_line_info)
        omni_mio_fprintf(fd, "{}:{}: ", file, line);

    const char *clear_ansi = "\x1b[0m";

    omni_mio_fprintf(fd, "{}{} ", color_code, prefix);
    omni_mio_printf_args(fd, fmt, args, argc);
    if (omni_colorful_logs)
        omni_mio_fprintf(fd, "{}", clear_ansi);
    omni_mio_fprintf(fd, "\n");
}

#endif // OMNI_IMPLEMENTATION

/* ---------- PREFIX STRIPPING ---------- */
//
// By default the short, unprefixed names below (matching the original
// standalone dynarr.h/sv.h/hashmap.h/mio.h APIs) are made available as
// aliases for their omni_/OMNI_-prefixed counterparts. Define
// OMNI_UNSTRIP_PREFIX before including omni.h to skip all of this and use
// only the prefixed names -- handy if e.g. your own code already has a
// `string` or `sv` of its own.
//
// This block deliberately lives after both the declaration section (guarded
// by OMNI_H) and the implementation section (guarded by OMNI_IMPLEMENTATION):
// omni.h's own internals are written entirely in terms of the omni_/OMNI_
// names, but several of the short names being aliased here (`sv` above all)
// are also common parameter/field names inside this file's own
// implementation. Defining these aliases any earlier would have the
// preprocessor rewrite omni.h's own source (e.g. turning the union access
// `arg.as.sv` into `arg.as.omni_sv`, which doesn't exist) instead of just
// affecting code written after the #include. Guarded separately from OMNI_H
// so it still only runs once per translation unit even if omni.h ends up
// included more than once.
#if !defined(OMNI_UNSTRIP_PREFIX) && !defined(OMNI_H_STRIPPED_ALIASES)
#define OMNI_H_STRIPPED_ALIASES

// general
#define TODO OMNI_TODO
#define UNREACHABLE OMNI_UNREACHABLE
#define ARRAY_LEN OMNI_ARRAY_LEN
#define ARRAY_GET OMNI_ARRAY_GET
#define shift omni_shift
#define shift_args omni_shift_args

// dynamic arrays
#define dynarr omni_dynarr
#define da_len omni_da_len
#define da_capacity omni_da_capacity
#define da_reserve omni_da_reserve
#define da_push omni_da_push
#define da_pop omni_da_pop
#define da_first omni_da_first
#define da_last omni_da_last
#define da_peek omni_da_peek
#define da_remove_unordered omni_da_remove_unordered
#define da_remove omni_da_remove
#define da_push_many omni_da_push_many
#define da_resize omni_da_resize
#define da_free omni_da_free
#define da_foreach omni_da_foreach
#define da_clear omni_da_clear

// string views
#define string omni_string
#define string_builder omni_string_builder
#define SV_FMT OMNI_SV_FMT
#define SV_ARG OMNI_SV_ARG
#define SVLIT OMNI_SVLIT
#define cstr_len omni_cstr_len
#define sv omni_sv
#define sv_chop_left omni_sv_chop_left
#define sv_chop_right omni_sv_chop_right
#define sv_trim_left omni_sv_trim_left
#define sv_trim_right omni_sv_trim_right
#define sv_trim omni_sv_trim
#define sv_eq omni_sv_eq
#define sv_eq_ignorecase omni_sv_eq_ignorecase
#define sv_starts_with omni_sv_starts_with
#define sv_ends_with omni_sv_ends_with
#define sv_cmp omni_sv_cmp
#define sv_chop_by_delim omni_sv_chop_by_delim
#define sv_chop_if omni_sv_chop_if
#define _sv_to_cstr omni__sv_to_cstr
#define sv_to_cstr omni_sv_to_cstr

// hashmaps
#define hashmap omni_hashmap
#define hashmap_type omni_hashmap_type
#define hm_hash omni_hm_hash
#define hm_len omni_hm_len
#define hm_cap omni_hm_cap
#define hm_reserve omni_hm_reserve
#define hm_set omni_hm_set
#define hm_get omni_hm_get
#define hm_has omni_hm_has
#define hm_remove omni_hm_remove
#define hm_free omni_hm_free

// printing
#define MioDisplayType OMNI_MioDisplayType
#define DECIMAL OMNI_MIO_DECIMAL
#define HEX_LOW OMNI_MIO_HEX_LOW
#define HEX_UPPER OMNI_MIO_HEX_UPPER
#define OCTAL OMNI_MIO_OCTAL
#define mio_putc omni_mio_putc
#define mio_putcstr omni_mio_putcstr
#define mio_putsv omni_mio_putsv
#define mio_putcstr_padded omni_mio_putcstr_padded
#define mio_putsv_padded omni_mio_putsv_padded
#define mio_itos omni_mio_itos
#define mio_utos omni_mio_utos
#define mio_itoh omni_mio_itoh
#define mio_itoo omni_mio_itoo
#define mio_ftos omni_mio_ftos
#define mio_printchar omni_mio_printchar
#define mio_printbool omni_mio_printbool
#define mio_printint omni_mio_printint
#define mio_printuint omni_mio_printuint
#define mio_printfloat omni_mio_printfloat
#define mio_printptr omni_mio_printptr
#define mio_printcstr omni_mio_printcstr
#define mio_printsv omni_mio_printsv
#define mio_printsb omni_mio_printsb
#define mio_printtype omni_mio_printtype
#define MioArgType OMNI_MioArgType
#define MIO_CHAR OMNI_MIO_CHAR
#define MIO_BOOL OMNI_MIO_BOOL
#define MIO_INT OMNI_MIO_INT
#define MIO_UINT OMNI_MIO_UINT
#define MIO_FLOAT OMNI_MIO_FLOAT
#define MIO_PTR OMNI_MIO_PTR
#define MIO_CSTR OMNI_MIO_CSTR
#define MIO_SV OMNI_MIO_SV
#define MioArg OMNI_MioArg
#define mio_printf_args omni_mio_printf_args
#define mio_fprintf omni_mio_fprintf
#define mio_printf omni_mio_printf
#define fprint omni_mio_fprintf
#define print omni_mio_printf

// string builder
#define sb_append omni_sb_append
#define sb_append_buf omni_sb_append_buf
#define sb_append_sv omni_sb_append_sv
#define sb_append_cstr omni_sb_append_cstr
#define sb_append_null omni_sb_append_null
#define sb_free omni_sb_free
#define sb_clear omni_sb_clear
#define sb_pad_align omni_sb_pad_align
#define sb_to_sv omni_sb_to_sv
#define sb_printf_args omni_sb_printf_args
#define sb_appendf omni_sb_appendf
#define sb_print omni_sb_print
#define sb_fprint omni_sb_fprint

// linked lists
#define ll_type omni_ll_type
#define ll_node omni_ll_node
#define ll_node_create omni_ll_node_create
#define ll_append omni_ll_append
#define ll_prepend omni_ll_prepend
#define ll_next omni_ll_next
#define ll_prev omni_ll_prev
#define ll_foreach omni_ll_foreach
#define ll_foreach_reverse omni_ll_foreach_reverse
#define ll_pop_back omni_ll_pop_back
#define ll_pop_front omni_ll_pop_front
#define ll_clear omni_ll_clear
#define ll_len omni_ll_len

// logging
#define LogLevel OmniLogLevel
#define TRACE OMNI_TRACE
#define DEBUG OMNI_DEBUG
#define INFO OMNI_INFO
#define WARN OMNI_WARN
#define ERROR OMNI_ERROR
#define FATAL OMNI_FATAL
#define NO_LOGS OMNI_NO_LOGS
#define minimum_log_level omni_minimum_log_level
#define colorful_logs omni_colorful_logs
#define log_impl omni_log_impl
#define LOG omni_log

#endif // !OMNI_UNSTRIP_PREFIX && !OMNI_H_STRIPPED_ALIASES
