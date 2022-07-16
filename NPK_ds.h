/*
Copyright 2021 Nikolaos Panagiotis Koukouras.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#if !defined NPK_DS_H_
    #define NPK_DS_H_

    // The headers of the C Standard Library
    // You can include them from here directly, since this header will be included everywhere anyway
    // #include <assert.h>
    // #include <complex.h> // Optional ISO C header
    // #include <ctype.h>
    // #include <errno.h>
    #include <fenv.h>
    // #include <float.h>
    #include <inttypes.h>
    // #include <iso646.h>
    // #include <limits.h>
    // #include <locale.h>
    #include <math.h>
    // #include <setjmp.h>
    // #include <signal.h>
    // #include <stdalign.h>
    // #include <stdarg.h>
    // #include <stdatomic.h> // Optional ISO C header
    #include <stdbool.h>
    // #include <stddef.h>
    // #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>
    // #include <stdnoreturn.h>
    #include <string.h>
    // #include <tgmath.h> // Overrides <math.h> and <complex.h>
    // #include <threads.h> // Optional ISO C header
    // #include <time.h>
    // #include <uchar.h>
    // #include <wchar.h>
    // #include <wctype.h>

    // Contains alloca macro
    #if defined _MSC_VER
        #include <malloc.h>
    #endif

    // Contains alloca macro
    #if defined __linux__
        #include <alloca.h>
    #endif

    // =============
    // NPK INTERFACE
    // =============

    // Definitions of our generic containers
    typedef struct {
        int len;
        int maxlen;
        void *data;
    } NPK_array;

    typedef NPK_array NPK_string;

    typedef struct {
        NPK_array keys;
        NPK_array values;
    } NPK_map;

    // Loop that appears very frequently, it almost feels like a chore not having it
    #define NPK_count_up(var, from, to) for (int var = from; var < to; var++)

    // count_down is exactly the same with count_up but in reverse order
    #define NPK_count_down(var, from, to) for (int var = from - 1; var >= to; var--)

    // A loop on steroids that traverses all elements of the array except if it is a string
    // In this case, it omits the nul byte :)
    #define NPK_for_each(type, var, arr) for (type *var = (arr)->data; var - NPK_array_data(type, arr) < _Generic(var, char *: NPK_is_string_(arr) ? NPK_string_length(arr) : NPK_array_length(arr), default: NPK_array_length(arr)); var++)

    // The syntax for a cast is very hard to look for and code smells usually include casts
    // So here is a cast that is simulating the equivalent syntax of the casts in C++
    #define NPK_cast(type, expr) ((type) (expr))

    // Allows us to omit declaring a variable, just to take its address
    #define NPK_rvalue_addr(type, expr) ((type []) {expr})

    // A generic swap
    #define NPK_swap(type, p1, p2) NPK_swap_(sizeof(type), p1, p2)

    // Writes to a file all the bytes of the array
    void NPK_file_write_bytes(const char *fname, const NPK_array *arr);

    // Reads a file byte by byte and stores its contents into an array
    NPK_array NPK_file_read_bytes_new(const char *fname);

    // Writes in a file each string of the array to its own line
    void NPK_file_write(const char *fname, const NPK_array *lines);

    // Reads a file and loads its contents in a array
    // Every line corresponds to a single string
    NPK_array NPK_file_read_new(const char *fname);

    // Character handling macros
    #define NPK_char_to_lower(c) (c + ' ')
    #define NPK_char_to_upper(c) (c - ' ')
    #define NPK_char_is_lower(c) (c >= 'a' && c <= 'z')
    #define NPK_char_is_upper(c) (c >= 'A' && c <= 'Z')
    #define NPK_char_is_blank(c) (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
    #define NPK_char_is_digit(c) (c >= '0' && c <= '9')
    #define NPK_char_is_letter(c) (NPK_char_is_lower(c) || NPK_char_is_upper(c))
    #define NPK_char_is_punct(c) ((c >= '!' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~'))

    // Allocates an array with two initial elements
    #define NPK_array_new(type) NPK_array_new_(sizeof(type))

    // Allocates an array with "maxlen" initial elements
    #define NPK_array_sized_new(type, maxlen) NPK_array_sized_new_(sizeof(type), maxlen)

    // Allocates a new array that is a deep copy of "arr"
    #define NPK_array_copy_new(type, arr) NPK_array_copy_new_(sizeof(type), arr, 1)
    #define NPK_array_dim_copy_new(type, arr, dim) NPK_array_copy_new_(sizeof(type), arr, dim)

    // Splits an array in parts, based on a "delim" delimiter
    #define NPK_array_split_new(type, arr, delim, eq) NPK_array_split_new_(sizeof(type), arr, NPK_rvalue_addr(type, delim), eq)
    #define NPK_array_dim_split_new(type, arr, delim, eq, dim) NPK_array_dim_split_new_(sizeof(type), arr, delim, eq, dim)

    // Checks if two arrays are equal
    #define NPK_array_equals(type, arr1, arr2, eq) NPK_array_equals_(sizeof(type), arr1, arr2, eq, 1)
    #define NPK_array_dim_equals(type, arr1, arr2, eq, dim) NPK_array_equals_(sizeof(type), arr1, arr2, eq, dim)

    // Checks if the array contains "val"
    #define NPK_array_contains(type, arr, val, eq) NPK_array_contains_(sizeof(type), arr, NPK_rvalue_addr(type, val), eq)
    #define NPK_array_dim_contains(type, arr, val, eq, dim) NPK_array_dim_contains_(sizeof(type), arr, val, eq, dim)

    // Provides typed access to the array data
    #define NPK_array_data(type, arr) NPK_cast(type *, (arr)->data)

    // Checks if the array is empty
    #define NPK_array_is_empty(arr) (NPK_array_length(arr) == 0)

    // Provides the length of the array
    #define NPK_array_length(arr) ((arr)->len + 0)

    // Adds to the array a shallow copy of "val"
    #define NPK_array_add(type, arr, val) NPK_array_add_(sizeof(type), arr, NPK_rvalue_addr(type, val))

    // Replaces every occurence of a "from" in the array with "to"
    #define NPK_array_replace(type, arr, from, to, eq) NPK_array_replace_(sizeof(type), arr, NPK_rvalue_addr(type, from), NPK_rvalue_addr(type, to), eq)
    #define NPK_array_dim_replace(type, arr, from, to, eq, dim) NPK_array_dim_replace_(sizeof(type), arr, from, to, eq, dim)

    // Sorts the array in "cmp" order
    #define NPK_array_sort(type, arr, cmp) NPK_array_sort_(sizeof(type), arr, cmp, 1)
    #define NPK_array_dim_sort(type, arr, cmp, dim) NPK_array_sort_(sizeof(type), arr, cmp, dim)

    // Reverses the order of the elements in the array
    #define NPK_array_reverse(type, arr) NPK_array_reverse_(sizeof(type), arr, NPK_array_length(arr))

    // Removes the element at "idx" from the array
    #define NPK_array_remove_at(type, arr, idx) NPK_array_remove_at_(sizeof(type), arr, idx)

    // Removes every occurence of a "val" from the array
    #define NPK_array_remove(type, arr, val, eq) NPK_array_remove_(sizeof(type), arr, NPK_rvalue_addr(type, val), eq)
    #define NPK_array_dim_remove(type, arr, val, eq, dim) NPK_array_dim_remove_(sizeof(type), arr, val, eq, dim)

    // Deallocates the array
    #define NPK_array_del(arr) NPK_array_dim_del(arr, 1)
    void NPK_array_dim_del(NPK_array *arr, int dim);

    // Allocates a string with a cstring "s"
    NPK_string NPK_string_new(const char *s);

    // Allocates a string with "maxlen" initial elements
    NPK_string NPK_string_sized_new(int maxlen);

    // Allocates a new string that is a deep copy of "str"
    #define NPK_string_copy_new(str) NPK_array_copy_new(char, str)

    // Splits a string in parts, based on a cstring "delim"
    NPK_array NPK_string_split_new(const NPK_string *str, const char *delim);

    // Checks if two strings are equal
    #define NPK_string_equals(str, s) (strcmp((str)->data, s) == 0)

    // Checks if the string contains a cstring "s"
    #define NPK_string_contains(str, s) (strstr((str)->data, s) != NULL)

    // Provides typed access to the string data
    #define NPK_string_data(str) NPK_array_data(char, str)

    // Checks if the string is empty
    #define NPK_string_is_empty(str) (NPK_string_length(str) == 0)

    // Provides the length of the string
    #define NPK_string_length(str) (NPK_array_length(str) - 1)

    // Checks if the string starts with a cstring
    #define NPK_string_starts(str, s) (strncmp((str)->data, s, strlen(s)) == 0)

    // Checks if the string ends with a cstring
    #define NPK_string_ends(str, s) (strcmp(NPK_string_data(str) + NPK_string_length(str) - strlen(s), s) == 0)

    // Appends to the string a cstring "s"
    void NPK_string_append(NPK_string *str, const char *s);

    // Adds "c" to the string, taking the nul byte into account
    void NPK_string_add(NPK_string *str, char c);

    // Replaces every occurence of a "from" in the string with a "to"
    void NPK_string_replace(NPK_string *str, const char *from, const char *to);

    // Sorts the string in "cmp" order
    #define NPK_string_sort(str, cmp) qsort((str)->data, NPK_string_length(str), 1, cmp)

    // Reverses the order of the elements in the string
    #define NPK_string_reverse(str) NPK_array_reverse_(1, str, NPK_string_length(str))

    // Converts a string to lowercase
    void NPK_string_to_lower(NPK_string *str);

    // Converts a string to uppercase
    void NPK_string_to_upper(NPK_string *str);

    // Removes the element at "idx" from the string
    #define NPK_string_remove_at(str, idx) NPK_array_remove_at(char, str, idx)

    // Removes every occurence of a cstring "s" from the string
    void NPK_string_remove(NPK_string *str, const char *s);

    // Deallocates a string
    #define NPK_string_del(str) NPK_array_del(str)

    // Allocates a map of nine initial key-value pairs
    #define NPK_map_new(type) NPK_map_new_(sizeof(type))

    // Allocates a map with "maxlen" initial key-value pairs
    #define NPK_map_sized_new(type, maxlen) NPK_map_sized_new_(sizeof(type), maxlen)

    // Allocates a new map that is a deep copy of "m"
    #define NPK_map_copy_new(type, m) NPK_map_copy_new_(sizeof(type), m, 1)
    #define NPK_map_dim_copy_new(type, m, dim) NPK_map_copy_new_(sizeof(type), m, dim)

    // Allocates an array with all the keys that exist in "m"
    NPK_array NPK_map_keys_new(const NPK_map *m);

    // Allocates an array with a deep copy of all the values that exist in "m"
    #define NPK_map_values_new(type, m) NPK_map_values_new_(sizeof(type), m, 1)
    #define NPK_map_dim_values_new(type, m, dim) NPK_map_values_new_(sizeof(type), m, dim)

    // Returns the value that corresponds to the key in the map
    #define NPK_map_get(type, m, key) NPK_cast(type *, NPK_map_get_(sizeof(type), m, key))

    // Checks if two maps are equal
    #define NPK_map_equals(type, m1, m2, eq) NPK_map_equals_(sizeof(type), m1, m2, eq)

    // Checks if the map contains the "key"
    bool NPK_map_contains_key(const NPK_map *m, const void *key);

    // Checks if the map contains the "val"
    #define NPK_map_contains_value(type, m, val, eq) NPK_map_contains_value_(sizeof(type), m, NPK_rvalue_addr(type, val), eq, 1)
    #define NPK_map_dim_contains_value(type, m, val, eq) NPK_map_contains_value_(sizeof(type), m, val, eq, dim)

    // Checks if the map is empty
    #define NPK_map_is_empty(m) (NPK_map_length(m) == 0)

    // Provides the number of the pairs in the map as an rvalue
    #define NPK_map_length(m) (NPK_array_length(&(m)->keys))

    // Adds to the map a key-value pair
    #define NPK_map_add(type, m, key, val) NPK_map_add_(sizeof(type), m, key, NPK_rvalue_addr(type, val))

    // Removes a key-value pair from the map using "key"
    #define NPK_map_remove(m, key) NPK_map_remove_(m, key, 1)
    #define NPK_map_dim_remove(m, key, dim) NPK_map_remove_(m, key, dim)

    // Deallocates the map
    #define NPK_map_del(m) NPK_map_dim_del(m, 1)
    void NPK_map_dim_del(NPK_map *m, int dim);

    // Equality functions are important when dealing with objects
    // Here, the equality functions for primitive types are provided
    bool NPK_uchar_equals(const void *p1, const void *p2);
    bool NPK_ushrt_equals(const void *p1, const void *p2);
    bool NPK_uint_equals(const void *p1, const void *p2);
    bool NPK_ulong_equals(const void *p1, const void *p2);
    bool NPK_ullong_equals(const void *p1, const void *p2);

    bool NPK_schar_equals(const void *p1, const void *p2);
    bool NPK_shrt_equals(const void *p1, const void *p2);
    bool NPK_int_equals(const void *p1, const void *p2);
    bool NPK_long_equals(const void *p1, const void *p2);
    bool NPK_llong_equals(const void *p1, const void *p2);

    bool NPK_bool_equals(const void *p1, const void *p2);
    bool NPK_char_equals(const void *p1, const void *p2);
    bool NPK_flt_equals(const void *p1, const void *p2);
    bool NPK_dbl_equals(const void *p1, const void *p2);
    bool NPK_ldbl_equals(const void *p1, const void *p2);

    bool NPK_uint8_equals(const void *p1, const void *p2);
    bool NPK_uint16_equals(const void *p1, const void *p2);
    bool NPK_uint32_equals(const void *p1, const void *p2);
    bool NPK_uint64_equals(const void *p1, const void *p2);

    bool NPK_int8_equals(const void *p1, const void *p2);
    bool NPK_int16_equals(const void *p1, const void *p2);
    bool NPK_int32_equals(const void *p1, const void *p2);
    bool NPK_int64_equals(const void *p1, const void *p2);

    // Comparison functions are important when dealing with objects
    // Here, the comparison functions for primitive types are provided
    #define compare(order)                                  \
    int NPK_uchar_##order(const void *p1, const void *p2);  \
    int NPK_ushrt_##order(const void *p1, const void *p2);  \
    int NPK_uint_##order(const void *p1, const void *p2);   \
    int NPK_ulong_##order(const void *p1, const void *p2);  \
    int NPK_ullong_##order(const void *p1, const void *p2); \
                                                            \
    int NPK_schar_##order(const void *p1, const void *p2);  \
    int NPK_shrt_##order(const void *p1, const void *p2);   \
    int NPK_int_##order(const void *p1, const void *p2);    \
    int NPK_long_##order(const void *p1, const void *p2);   \
    int NPK_llong_##order(const void *p1, const void *p2);  \
                                                            \
    int NPK_bool_##order(const void *p1, const void *p2);   \
    int NPK_char_##order(const void *p1, const void *p2);   \
    int NPK_flt_##order(const void *p1, const void *p2);    \
    int NPK_dbl_##order(const void *p1, const void *p2);    \
    int NPK_ldbl_##order(const void *p1, const void *p2);   \
                                                            \
    int NPK_uint8_##order(const void *p1, const void *p2);  \
    int NPK_uint16_##order(const void *p1, const void *p2); \
    int NPK_uint32_##order(const void *p1, const void *p2); \
    int NPK_uint64_##order(const void *p1, const void *p2); \
                                                            \
    int NPK_int8_##order(const void *p1, const void *p2);   \
    int NPK_int16_##order(const void *p1, const void *p2);  \
    int NPK_int32_##order(const void *p1, const void *p2);  \
    int NPK_int64_##order(const void *p1, const void *p2);

    compare(asc) compare(desc)
    #undef compare

    // Converts a bool to a cstring
    #define NPK_bool_to_cstring(flag) (flag) ? NPK_cast(const char *, "True") : NPK_cast(const char *, "False")

    // Converts a value to a string
    NPK_string NPK_uchar_to_string_new(unsigned char val);
    NPK_string NPK_ushrt_to_string_new(unsigned short val);
    NPK_string NPK_uint_to_string_new(unsigned int val);
    NPK_string NPK_ulong_to_string_new(unsigned long val);
    NPK_string NPK_ullong_to_string_new(unsigned long long val);

    NPK_string NPK_schar_to_string_new(signed char val);
    NPK_string NPK_shrt_to_string_new(short val);
    NPK_string NPK_int_to_string_new(int val);
    NPK_string NPK_long_to_string_new(long val);
    NPK_string NPK_llong_to_string_new(long long val);

    NPK_string NPK_dbl_to_string_new(double val);
    NPK_string NPK_ldbl_to_string_new(long double val);

    NPK_string NPK_uint8_to_string_new(uint8_t val);
    NPK_string NPK_uint16_to_string_new(uint16_t val);
    NPK_string NPK_uint32_to_string_new(uint32_t val);
    NPK_string NPK_uint64_to_string_new(uint64_t val);

    NPK_string NPK_int8_to_string_new(int8_t val);
    NPK_string NPK_int16_to_string_new(int16_t val);
    NPK_string NPK_int32_to_string_new(int32_t val);
    NPK_string NPK_int64_to_string_new(int64_t val);

    // Converts a cstring to a bool
    #define NPK_cstring_to_bool(s) (strcmp(s, "True") == 0)

    // Functions that convert a string to a value
    unsigned char NPK_string_to_uchar(const NPK_string *str);
    unsigned short NPK_string_to_ushrt(const NPK_string *str);
    unsigned int NPK_string_to_uint(const NPK_string *str);
    unsigned long NPK_string_to_ulong(const NPK_string *str);
    unsigned long long NPK_string_to_ullong(const NPK_string *str);

    signed char NPK_string_to_schar(const NPK_string *str);
    short NPK_string_to_shrt(const NPK_string *str);
    int NPK_string_to_int(const NPK_string *str);
    long NPK_string_to_long(const NPK_string *str);
    long long NPK_string_to_llong(const NPK_string *str);

    float NPK_string_to_flt(const NPK_string *str);
    double NPK_string_to_dbl(const NPK_string *str);
    long double NPK_string_to_ldbl(const NPK_string *str);

    uint8_t NPK_string_to_uint8(const NPK_string *str);
    uint16_t NPK_string_to_uint16(const NPK_string *str);
    uint32_t NPK_string_to_uint32(const NPK_string *str);
    uint64_t NPK_string_to_uint64(const NPK_string *str);

    int8_t NPK_string_to_int8(const NPK_string *str);
    int16_t NPK_string_to_int16(const NPK_string *str);
    int32_t NPK_string_to_int32(const NPK_string *str);
    int64_t NPK_string_to_int64(const NPK_string *str);

    // =============
    // NPK INTERNALS
    // =============

    #define NPK_is_string_(arr) (NPK_array_data(char, arr)[NPK_string_length(arr)] == '\0')
    void NPK_swap_(int size, void *p1, void *p2);

    #define NPK_array_new_(size) NPK_array_sized_new_(size, 2)
    NPK_array NPK_array_sized_new_(int size, int maxlen);
    NPK_array NPK_array_copy_new_(int size, const NPK_array *arr, int dim);
    NPK_array NPK_array_split_new_(int size, const NPK_array *arr, const void *delim, bool (*type_equals)(const void *p1, const void *p2));
    NPK_array NPK_array_dim_split_new_(int size, const NPK_array *arr, const void *delim, bool (*type_equals)(const void *p1, const void *p2), int dim);
    bool NPK_array_equals_(int size, const NPK_array *arr1, const NPK_array *arr2, bool (*type_equals)(const void *p1, const void *p2), int dim);
    bool NPK_array_contains_(int size, const NPK_array *arr, const void *val, bool (*type_equals)(const void *p1, const void *p2));
    bool NPK_array_dim_contains_(int size, const NPK_array *arr, const void *val, bool (*type_equals)(const void *p1, const void *p2), int dim);
    void NPK_array_add_(int size, NPK_array *arr, const void *val);
    void NPK_array_replace_(int size, NPK_array *arr, const void *from, const void *to, bool (*type_equals)(const void *p1, const void *p2));
    void NPK_array_dim_replace_(int size, NPK_array *arr, const void *from, const void *to, bool (*type_equals)(const void *p1, const void *p2), int dim);
    void NPK_array_sort_(int size, NPK_array *arr, int (*type_compare)(const void *p1, const void *p2), int dim);
    void NPK_array_reverse_(int size, NPK_array *arr, int len);
    void NPK_array_remove_at_(int size, NPK_array *arr, int idx);
    void NPK_array_dim_remove_at(NPK_array *arr, int idx, int dim);
    void NPK_array_remove_(int size, NPK_array *arr, const void *val, bool (*type_equals)(const void *p1, const void *p2));
    void NPK_array_dim_remove_(int size, NPK_array *arr, const void *val, bool (*type_equals)(const void *p1, const void *p2), int dim);

    #define NPK_map_new_(size) NPK_map_sized_new_(size, 9)
    NPK_map NPK_map_sized_new_(int size, int maxlen);
    NPK_map NPK_map_copy_new_(int size, const NPK_map *m, int dim);
    NPK_array NPK_map_values_new_(int size, const NPK_map *m, int dim);
    void NPK_map_add_(int size, NPK_map *m, void *key, const void *val);
    void NPK_map_remove_(NPK_map *m, const void *key, int dim);
    void *NPK_map_get_(int size, const NPK_map *m, const void *key);
    bool NPK_map_equals_(int size, const NPK_map *m1, const NPK_map *m2, bool (*type_equals)(const void *p1, const void *p2));
    bool NPK_map_contains_value_(int size, const NPK_map *m, const void *val, bool (*type_equals)(const void *p1, const void *p2), int dim);
#endif

#if defined NPK_DS_IMPL
    // ==================
    // NPK IMPLEMENTATION
    // ==================

    void NPK_swap_(int size, void *p1, void *p2) {
        void *tmp = alloca(size);

        memcpy(tmp, p1, size);
        memcpy(p1, p2, size);
        memcpy(p2, tmp, size);
    }

    void NPK_file_write_bytes(const char *fname, const NPK_array *arr) {
        FILE *f = fopen(fname, "wb");

        fwrite(arr->data, 1, NPK_array_length(arr), f);
        fclose(f);
    }

    NPK_array NPK_file_read_bytes_new(const char *fname) {
        NPK_array arr = NPK_array_new(uint8_t);
        FILE *f = fopen(fname, "rb");
        uint8_t b;

        while (fread(&b, 1, 1, f) != 0) {
            NPK_array_add(uint8_t, &arr, b);
        }

        fclose(f);

        return arr;
    }

    void NPK_file_write(const char *fname, const NPK_array *lines) {
        FILE *f = fopen(fname, "w");

        NPK_for_each (NPK_string, line, lines) {
            fprintf(f, "%s\n", NPK_string_data(line));
        }

        fclose(f);
    }

    NPK_array NPK_file_read_new(const char *fname) {
        FILE *f = fopen(fname, "r");
        NPK_array lines = NPK_array_new(NPK_string);
        NPK_string line = NPK_string_new("");

        for (int c = fgetc(f); c != EOF; c = fgetc(f)) {
            if (c != '\n') {
                NPK_string_add(&line, c);
            } else {
                NPK_array_add(NPK_string, &lines, line);
                line = NPK_string_new("");
            }
        }

        NPK_array_add(NPK_string, &lines, line);
        fclose(f);

        return lines;
    }

    NPK_array NPK_array_sized_new_(int size, int maxlen) {
        NPK_array arr;

        arr.len = 0;
        arr.maxlen = maxlen;
        arr.data = malloc(maxlen * size);

        return arr;
    }

    NPK_array NPK_array_copy_new_(int size, const NPK_array *arr, int dim) {
        NPK_array copy;

        if (dim > 1) {
            copy = NPK_array_sized_new(NPK_array, arr->maxlen);

            NPK_for_each (NPK_array, elem, arr) {
                NPK_array lcopy = NPK_array_copy_new_(size, elem, dim - 1);

                NPK_array_add(NPK_array, &copy, lcopy);
            }
        } else {
            copy = NPK_array_sized_new_(size, arr->maxlen);
            copy.len = NPK_array_length(arr);
            memcpy(copy.data, arr->data, arr->maxlen * size);
        }

        return copy;
    }

    NPK_array NPK_array_split_new_(int size, const NPK_array *arr, const void *delim, bool (*type_equals) (const void *p1, const void *p2)) {
        NPK_array parts = NPK_array_new(NPK_array);
        NPK_array part = NPK_array_new_(size);

        NPK_count_up (i, 0, NPK_array_length(arr)) {
            void *cur = NPK_array_data(uint8_t, arr) + i * size;

            if (type_equals(cur, delim) == false) {
                NPK_array_add_(size, &part, cur);
            } else {
                NPK_array_add(NPK_array, &parts, part);
                part = NPK_array_new_(size);
            }
        }

        NPK_array_add(NPK_array, &parts, part);

        return parts;
    }

    NPK_array NPK_array_dim_split_new_(int size, const NPK_array *arr, const void *delim, bool (*type_equals) (const void *p1, const void *p2), int dim) {
        NPK_array parts = NPK_array_new(NPK_array);
        NPK_array part = NPK_array_new(NPK_array);

        NPK_count_up (i, 0, NPK_array_length(arr)) {
            NPK_array *cur = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr) + i * sizeof(NPK_array));

            if (NPK_array_equals_(size, cur, delim, type_equals, dim - 1) == false) {
                NPK_array copy = NPK_array_copy_new_(size, cur, dim - 1);

                NPK_array_add(NPK_array, &part, copy);
            } else {
                NPK_array_add(NPK_array, &parts, part);
                part = NPK_array_new(NPK_array);
            }
        }

        NPK_array_add(NPK_array, &parts, part);

        return parts;
    }

    bool NPK_array_equals_(int size, const NPK_array *arr1, const NPK_array *arr2, bool (*type_equals)(const void *p1, const void *p2), int dim) {
        if (NPK_array_length(arr1) != NPK_array_length(arr2)) {
            return false;
        } else {
            NPK_count_up (i, 0, NPK_array_length(arr1)) {
                if (dim > 1) {
                    NPK_array *cur1 = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr1) + i * sizeof(NPK_array));
                    NPK_array *cur2 = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr2) + i * sizeof(NPK_array));

                    if (NPK_array_equals_(size, cur1, cur2, type_equals, dim - 1) == false) {
                        return false;
                    }
                } else {
                    void *cur1 = NPK_array_data(uint8_t, arr1) + i * size;
                    void *cur2 = NPK_array_data(uint8_t, arr2) + i * size;

                    if (type_equals(cur1, cur2) == false) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    bool NPK_array_contains_(int size, const NPK_array *arr, const void *val, bool (*type_equals)(const void *p1, const void *p2)) {
        NPK_count_up (i, 0, NPK_array_length(arr)) {
            void *cur = NPK_array_data(uint8_t, arr) + i * size;

            if (type_equals(cur, val)) {
                return true;
            }
        }

        return false;
    }

    bool NPK_array_dim_contains_(int size, const NPK_array *arr, const void *val, bool (*type_equals)(const void *p1, const void *p2), int dim) {
        NPK_count_up (i, 0, NPK_array_length(arr)) {
            NPK_array *cur = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr) + i * sizeof(NPK_array));

            if (NPK_array_equals_(size, cur, val, type_equals, dim - 1)) {
                return true;
            }
        }

        return false;
    }

    void NPK_array_add_(int size, NPK_array *arr, const void *val) {
        if (NPK_array_length(arr) == arr->maxlen) {
            arr->maxlen *= 1.5;
            arr->data = realloc(arr->data, arr->maxlen * size);
        }

        void *cur = NPK_array_data(uint8_t, arr) + NPK_array_length(arr) * size;

        memcpy(cur, val, size);
        arr->len++;
    }

    void NPK_array_replace_(int size, NPK_array *arr, const void *from, const void *to, bool (*type_equals)(const void *p1, const void *p2)) {
        NPK_count_up (i, 0, NPK_array_length(arr)) {
            void *cur = NPK_array_data(uint8_t, arr) + i * size;

            if (type_equals(cur, from)) {
                memcpy(cur, to, size);
            }
        }
    }

    void NPK_array_dim_replace_(int size, NPK_array *arr, const void *from, const void *to, bool (*type_equals)(const void *p1, const void *p2), int dim) {
        NPK_count_up (i, 0, NPK_array_length(arr)) {
            NPK_array *cur = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr) + i * sizeof(NPK_array));

            if (NPK_array_equals_(size, cur, from, type_equals, dim - 1)) {
                NPK_array_dim_del(cur, dim - 1);

                NPK_array copy = NPK_array_copy_new_(size, to, dim - 1);

                memcpy(cur, &copy, sizeof(NPK_array));
            }
        }
    }

    void NPK_array_sort_(int size, NPK_array *arr, int (*type_compare)(const void *p1, const void *p2), int dim) {
        NPK_count_up(i, 0, NPK_array_length(arr)) {
            if (dim > 1) {
                NPK_array *cur = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr) + i * sizeof(NPK_array));

                NPK_array_sort_(size, cur, type_compare, dim - 1);
            } else {
                qsort(arr->data, NPK_array_length(arr), size, type_compare);
            }
        }
    }

    void NPK_array_reverse_(int size, NPK_array *arr, int len) {
        NPK_count_up (i, 0, len) {
            if (i < len - 1 - i) {
                void *p1 = NPK_array_data(uint8_t, arr) + i * size;
                void *p2 = NPK_array_data(uint8_t, arr) + (len - 1 - i) * size;

                NPK_swap_(size, p1, p2);
            } else {
                break;
            }
        }
    }

    void NPK_array_remove_at_(int size, NPK_array *arr, int idx) {
        NPK_count_up (i, idx, NPK_array_length(arr) - 1) {
            void *cur = NPK_array_data(uint8_t, arr) + i * size;
            void *next = NPK_array_data(uint8_t, arr) + (i + 1) * size;

            memcpy(cur, next, size);
        }

        (arr)->len--;
    }

    void NPK_array_dim_remove_at(NPK_array *arr, int idx, int dim) {
        NPK_array *idx_loc = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr) + idx * sizeof(NPK_array));

        NPK_array_dim_del(idx_loc, dim - 1);

        NPK_count_up (i, idx, NPK_array_length(arr) - 1) {
            NPK_array *cur = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr) + i * sizeof(NPK_array));
            NPK_array *next = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr) + (i + 1) * sizeof(NPK_array));

            memcpy(cur, next, sizeof(NPK_array));
        }

        (arr)->len--;
    }

    void NPK_array_remove_(int size, NPK_array *arr, const void *val, bool (*type_equals)(const void *p1, const void *p2)) {
        NPK_count_up (i, 0, NPK_array_length(arr)) {
            void *cur = NPK_array_data(uint8_t, arr) + i * size;

            if (type_equals(cur, val)) {
                NPK_array_remove_at_(size, arr, i);
                i--;
            }
        }
    }

    void NPK_array_dim_remove_(int size, NPK_array *arr, const void *val, bool (*type_equals)(const void *p1, const void *p2), int dim) {
        NPK_count_up (i, 0, NPK_array_length(arr)) {
            NPK_array *cur = NPK_cast(NPK_array *, NPK_array_data(uint8_t, arr) + i * sizeof(NPK_array));

            if (NPK_array_equals_(size, cur, val, type_equals, dim - 1)) {
                NPK_array_dim_remove_at(arr, i, dim);
                i--;
            }
        }
    }

    void NPK_array_dim_del(NPK_array *arr, int dim) {
        if (dim > 1) {
            NPK_for_each (NPK_array, elem, arr) {
                NPK_array_dim_del(elem, dim - 1);
            }
        } else {
            free(arr->data);
            return;
        }

        free(arr->data);
    }

    NPK_string NPK_string_new(const char *s) {
        NPK_string str;

        if (*s == '\0') {
            str = NPK_string_sized_new(2);
        } else {
            str = NPK_string_sized_new(strlen(s) +1);
            NPK_string_append(&str, s);
        }

        return str;
    }

    NPK_string NPK_string_sized_new(int maxlen) {
        NPK_array arr = NPK_array_sized_new(char, maxlen);

        NPK_array_add(char, &arr, '\0');

        return arr;
    }

    NPK_array NPK_string_split_new(const NPK_string *str, const char *delim) {
        NPK_array parts = NPK_array_new(NPK_string);
        int len = strlen(delim);
        const char *begin = str->data;
        const char *end = strstr(str->data, delim);

        while (end != NULL) {
            NPK_string part = NPK_string_new("");

            while (begin != end) {
                NPK_string_add(&part, *begin);
                begin++;
            }

            NPK_array_add(NPK_string, &parts, part);
            begin += len;
            end = strstr(begin, delim);
        }

        NPK_string part = NPK_string_new(begin);

        NPK_array_add(NPK_string, &parts, part);

        return parts;
    }

    void NPK_string_append(NPK_string *str, const char *s) {
        for (; *s != '\0'; s++) {
            NPK_string_add(str, *s);
        }
    }

    void NPK_string_add(NPK_string *str, char c) {
        NPK_string_data(str)[NPK_string_length(str)] = c;
        NPK_array_add(char, str, '\0');
    }

    void NPK_string_replace(NPK_string *str, const char *from, const char *to) {
        NPK_string nstr = NPK_string_new("");
        int len = strlen(from);
        const char *begin = str->data;
        const char *end = strstr(str->data, from);

        while (end != NULL) {
            while (begin != end) {
                NPK_string_add(&nstr, *begin);
                begin++;
            }

            NPK_string_append(&nstr, to);
            begin += len;
            end = strstr(begin, from);
        }

        NPK_string_append(&nstr, begin);
        NPK_string_del(str);
        *str = nstr;
    }

    void NPK_string_to_lower(NPK_string *str) {
        NPK_for_each (char, c, str) {
            if (NPK_char_is_upper(*c)) {
                *c = NPK_char_to_lower(*c);
            }
        }
    }

    void NPK_string_to_upper(NPK_string *str) {
        NPK_for_each (char, c, str) {
            if (NPK_char_is_lower(*c)) {
                *c = NPK_char_to_upper(*c);
            }
        }
    }

    void NPK_string_remove(NPK_string *str, const char *s) {
        const char *pos = strstr(str->data, s);
        int len = strlen(s);

        while (pos != NULL) {
            NPK_count_up (i, 0, len) {
                NPK_string_remove_at(str, pos - NPK_string_data(str));
            }

            pos = strstr(str->data, s);
        }
    }

    #define map_maxlength(m) ((m)->keys.maxlen + 0)

    NPK_map NPK_map_sized_new_(int size, int maxlen) {
        NPK_map m;

        m.keys = NPK_array_sized_new(void *, maxlen);
        m.values = NPK_array_sized_new_(size, maxlen);
        memset(m.keys.data, 0, maxlen * sizeof(void *));

        return m;
    }

    NPK_map NPK_map_copy_new_(int size, const NPK_map *m, int dim) {
        NPK_map copy;

        copy.keys = NPK_array_copy_new(void *, &m->keys);
        copy.values = NPK_array_copy_new_(size, &m->values, dim);

        return copy;
    }

    NPK_array NPK_map_keys_new(const NPK_map *m) {
        NPK_array keys = NPK_array_new(void *);

        NPK_count_up (i, 0, map_maxlength(m)) {
            void *key = NPK_array_data(void *, &m->keys)[i];

            if (key != NULL) {
                NPK_array_add(void *, &keys, key);
            }
        }

        return keys;
    }

    NPK_array NPK_map_values_new_(int size, const NPK_map *m, int dim) {
        NPK_array vals;
        NPK_array keys = NPK_map_keys_new(m);

        if (dim > 1) {
            vals = NPK_array_new(NPK_array);
        } else {
            vals = NPK_array_new_(size);
        }

        NPK_for_each (void *, key, &keys) {
            if (dim > 1) {
                NPK_array *cur = NPK_map_get(NPK_array, m, *key);
                NPK_array copy = NPK_array_copy_new_(size, cur, dim - 1);

                NPK_array_add(NPK_array, &vals, copy);
            } else {
                void *cur = NPK_map_get_(size, m, *key);

                NPK_array_add_(size, &vals, cur);
            }
        }

        NPK_array_del(&keys);

        return vals;
    }

    void *NPK_map_get_(int size, const NPK_map *m, const void *key) {
        int idx = NPK_cast(int64_t, key) % map_maxlength(m);

        NPK_count_up (i, idx, idx + map_maxlength(m)) {
            void *cur_key = NPK_array_data(void *, &m->keys)[i % map_maxlength(m)];

            if (cur_key == NULL) {
                return NULL;
            } else {
                if (cur_key == key) {
                    return NPK_array_data(uint8_t, &m->values) + (i % map_maxlength(m)) * size;
                }
            }
        }

        return NULL;
    }

    bool NPK_map_equals_(int size, const NPK_map *m1, const NPK_map *m2, bool (*type_equals)(const void *p1, const void *p2)) {
        if (map_maxlength(m1) != map_maxlength(m2)) {
            return false;
        }

        NPK_count_up (i, 0, map_maxlength(m1)) {
            void *key1 = NPK_array_data(void *, &m1->keys)[i];
            void *key2 = NPK_array_data(void *, &m2->keys)[i];

            if (key1 != key2) {
                return false;
            } else {
                if (key1 != NULL) {
                    if (type_equals(NPK_map_get_(size, m1, key1), NPK_map_get_(size, m2, key2)) == false) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    bool NPK_map_contains_key(const NPK_map *m, const void *key) {
        int idx = NPK_cast(int64_t, key) % map_maxlength(m);

        NPK_count_up (i, idx, idx + map_maxlength(m)) {
            void *cur_key = NPK_array_data(void *, &m->keys)[i % map_maxlength(m)];

            if (cur_key == NULL) {
                return false;
            } else {
                if (cur_key == key) {
                    return true;
                }
            }
        }

        return false;
    }

    bool NPK_map_contains_value_(int size, const NPK_map *m, const void *val, bool (*type_equals) (const void *p1, const void *p2), int dim) {
        NPK_array vals = NPK_map_values_new_(size, m, dim);

        if (NPK_array_contains_(size, &vals, val, type_equals)) {
            NPK_array_dim_del(&vals, dim);

            return true;
        } else {
            NPK_array_dim_del(&vals, dim);

            return false;
        }
    }

    void NPK_map_add_(int size, NPK_map *m, void *key, const void *val) {
        int idx = NPK_cast(int64_t, key) % map_maxlength(m);

        NPK_count_up (i, idx, idx + map_maxlength(m)) {
            void *cur_key = NPK_array_data(void *, &m->keys)[i % map_maxlength(m)];
            void *cur_val = NPK_array_data(uint8_t, &m->values) + (i % map_maxlength(m)) * size;

            if (cur_key == NULL) {
                NPK_array_data(void *, &m->keys)[i % map_maxlength(m)] = key;
                memcpy(cur_val, val, size);
                m->keys.len++;
                m->values.len++;

                if (NPK_cast(double, NPK_map_length(m)) / NPK_cast(double, map_maxlength(m)) >= 0.5) {
                    NPK_map nm = NPK_map_sized_new_(size, map_maxlength(m) * 1.5);
                    NPK_array keys = NPK_map_keys_new(m);

                    NPK_for_each (void *, old_key, &keys) {
                        void *old_val = NPK_map_get_(size, m, *old_key);

                        NPK_map_add_(size, &nm, *old_key, old_val);
                    }

                    NPK_array_del(&keys);
                    NPK_map_dim_del(m, 1);
                    *m = nm;
                }

                break;
            } else {
                if (cur_key == key) {
                    memcpy(cur_val, val, size);
                    break;
                }
            }
        }
    }

    void NPK_map_remove_(NPK_map *m, const void *key, int dim) {
        int idx = NPK_cast(int64_t, key) % map_maxlength(m);

        NPK_count_up (i, idx, idx + map_maxlength(m)) {
            void *cur_key = NPK_array_data(void *, &m->keys)[i % map_maxlength(m)];

            if (cur_key == key) {
                if (dim > 1) {
                    NPK_array *cur_val = NPK_map_get(NPK_array, m, key);

                    NPK_array_dim_del(cur_val, dim - 1);
                }

                NPK_array_data(void *, &m->keys)[i % map_maxlength(m)] = NULL;
                m->keys.len--;
                m->values.len--;

                return;
            }
        }
    }

    void NPK_map_dim_del(NPK_map *m, int dim) {
        if (dim > 1) {
            NPK_array keys = NPK_map_keys_new(m);

            NPK_for_each (void *, key, &keys) {
                NPK_array arr = *NPK_map_get(NPK_array, m, *key);

                NPK_array_dim_del(&arr, dim - 1);
            }

            NPK_array_del(&keys);
        }

        NPK_array_del(&m->keys);
        NPK_array_dim_del(&m->values, 1);
    }

    #undef map_maxlength

    bool NPK_uchar_equals(const void *p1, const void *p2) { return *NPK_cast(const unsigned char *, p1) == *NPK_cast(const unsigned char *, p2); }
    bool NPK_ushrt_equals(const void *p1, const void *p2) { return *NPK_cast(const unsigned short *, p1) == *NPK_cast(const unsigned short *, p2); }
    bool NPK_uint_equals(const void *p1, const void *p2) { return *NPK_cast(const unsigned int *, p1) == *NPK_cast(const unsigned int *, p2); }
    bool NPK_ulong_equals(const void *p1, const void *p2) { return *NPK_cast(const unsigned long *, p1) == *NPK_cast(const unsigned long *, p2); }
    bool NPK_ullong_equals(const void *p1, const void *p2) { return *NPK_cast(const unsigned long long *, p1) == *NPK_cast(const unsigned long long *, p2); }

    bool NPK_schar_equals(const void *p1, const void *p2) { return *NPK_cast(const signed char *, p1) == *NPK_cast(const signed char *, p2); }
    bool NPK_shrt_equals(const void *p1, const void *p2) { return *NPK_cast(const short *, p1) == *NPK_cast(const short *, p2); }
    bool NPK_int_equals(const void *p1, const void *p2) { return *NPK_cast(const int *, p1) == *NPK_cast(const int *, p2); }
    bool NPK_long_equals(const void *p1, const void *p2) { return *NPK_cast(const long *, p1) == *NPK_cast(const long *, p2); }
    bool NPK_llong_equals(const void *p1, const void *p2) { return *NPK_cast(const long long *, p1) == *NPK_cast(const long long *, p2); }

    bool NPK_bool_equals(const void *p1, const void *p2) { return *NPK_cast(const bool *, p1) == *NPK_cast(const bool *, p2); }
    bool NPK_char_equals(const void *p1, const void *p2) { return *NPK_cast(const char *, p1) == *NPK_cast(const char *, p2); }
    bool NPK_flt_equals(const void *p1, const void *p2) { return fabsf(*NPK_cast(const float *, p1) - *NPK_cast(const float *, p2)) < FLT_EPSILON; }
    bool NPK_dbl_equals(const void *p1, const void *p2) { return fabs(*NPK_cast(const double *, p1) - *NPK_cast(const double *, p2)) < DBL_EPSILON; }
    bool NPK_ldbl_equals(const void *p1, const void *p2) { return fabsl(*NPK_cast(const long double *, p1) - *NPK_cast(const long double *, p2)) < LDBL_EPSILON; }

    bool NPK_uint8_equals(const void *p1, const void *p2) { return *NPK_cast(const uint8_t *, p1) == *NPK_cast(const uint8_t *, p2); }
    bool NPK_uint16_equals(const void *p1, const void *p2) { return *NPK_cast(const uint16_t *, p1) == *NPK_cast(const uint16_t *, p2); }
    bool NPK_uint32_equals(const void *p1, const void *p2) { return *NPK_cast(const uint32_t *, p1) == *NPK_cast(const uint32_t *, p2); }
    bool NPK_uint64_equals(const void *p1, const void *p2) { return *NPK_cast(const uint64_t *, p1) == *NPK_cast(const uint64_t *, p2); }

    bool NPK_int8_equals(const void *p1, const void *p2) { return *NPK_cast(const int8_t *, p1) == *NPK_cast(const int8_t *, p2); }
    bool NPK_int16_equals(const void *p1, const void *p2) { return *NPK_cast(const int16_t *, p1) == *NPK_cast(const int16_t *, p2); }
    bool NPK_int32_equals(const void *p1, const void *p2) { return *NPK_cast(const int32_t *, p1) == *NPK_cast(const int32_t *, p2); }
    bool NPK_int64_equals(const void *p1, const void *p2) { return *NPK_cast(const int64_t *, p1) == *NPK_cast(const int64_t *, p2); }

    #define compare(type, name, order, val)                     \
    int NPK_##name##_##order(const void *p1, const void *p2) {  \
        const type *elem1 = p1;                                 \
        const type *elem2 = p2;                                 \
                                                                \
        if (NPK_##name##_equals(elem1, elem2)) {                \
            return 0;                                           \
        } else if (*elem1 > *elem2) {                           \
            return val;                                         \
        } else {                                                \
            return -val;                                        \
        }                                                       \
    }

    compare(unsigned char, uchar, asc, 1)
    compare(unsigned short, ushrt, asc, 1)
    compare(unsigned int, uint, asc, 1)
    compare(unsigned long, ulong, asc, 1)
    compare(unsigned long long, ullong, asc, 1)

    compare(signed char, schar, asc, 1)
    compare(short, shrt, asc, 1)
    compare(int, int, asc, 1)
    compare(long, long, asc, 1)
    compare(long long, llong, asc, 1)

    compare(bool, bool, asc, 1)
    compare(char, char, asc, 1)
    compare(float, flt, asc, 1)
    compare(double, dbl, asc, 1)
    compare(long double, ldbl, asc, 1)

    compare(uint8_t, uint8, asc, 1)
    compare(uint16_t, uint16, asc, 1)
    compare(uint32_t, uint32, asc, 1)
    compare(uint64_t, uint64, asc, 1)

    compare(int8_t, int8, asc, 1)
    compare(int16_t, int16, asc, 1)
    compare(int32_t, int32, asc, 1)
    compare(int64_t, int64, asc, 1)

    compare(unsigned char, uchar, desc, -1)
    compare(unsigned short, ushrt, desc, -1)
    compare(unsigned int, uint, desc, -1)
    compare(unsigned long, ulong, desc, -1)
    compare(unsigned long long, ullong, desc, -1)

    compare(signed char, schar, desc, -1)
    compare(short, shrt, desc, -1)
    compare(int, int, desc, -1)
    compare(long, long, desc, -1)
    compare(long long, llong, desc, -1)

    compare(bool, bool, desc, -1)
    compare(char, char, desc, -1)
    compare(float, flt, desc, -1)
    compare(double, dbl, desc, -1)
    compare(long double, ldbl, desc, -1)

    compare(uint8_t, uint8, desc, -1)
    compare(uint16_t, uint16, desc, -1)
    compare(uint32_t, uint32, desc, -1)
    compare(uint64_t, uint64, desc, -1)

    compare(int8_t, int8, desc, -1)
    compare(int16_t, int16, desc, -1)
    compare(int32_t, int32, desc, -1)
    compare(int64_t, int64, desc, -1)

    #undef compare

    #define to_string(type, name, format)                   \
    NPK_string NPK_##name##_to_string_new(type val) {       \
        int len = snprintf(NULL, 0, format, val);           \
        NPK_array arr = NPK_array_sized_new(char, len +1);  \
                                                            \
        snprintf(arr.data, len +1, format, val);            \
        arr.len = len +1;                                   \
                                                            \
        return arr;                                         \
    }

    to_string(unsigned char, uchar, "%u")
    to_string(unsigned short, ushrt, "%u")
    to_string(unsigned int, uint, "%u")
    to_string(unsigned long, ulong, "%lu")
    to_string(unsigned long long, ullong, "%llu")

    to_string(signed char, schar, "%i")
    to_string(short, shrt, "%i")
    to_string(int, int, "%i")
    to_string(long, long, "%li")
    to_string(long long, llong, "%lli")

    to_string(double, dbl, "%f")
    to_string(long double, ldbl, "%Lf")

    to_string(uint8_t, uint8, "%"PRIu8)
    to_string(uint16_t, uint16, "%"PRIu16)
    to_string(uint32_t, uint32, "%"PRIu32)
    to_string(uint64_t, uint64, "%"PRIu64)

    to_string(int8_t, int8, "%"PRIi8)
    to_string(int16_t, int16, "%"PRIi16)
    to_string(int32_t, int32, "%"PRIi32)
    to_string(int64_t, int64, "%"PRIi64)

    #undef to_string

    #define from_string(type, name, format)             \
    type NPK_string_to_##name(const NPK_string *str) {  \
        type val;                                       \
                                                        \
        sscanf(str->data, format, &val);                \
                                                        \
        return val;                                     \
    }

    from_string(unsigned char, uchar, "%hhu")
    from_string(unsigned short, ushrt, "%hu")
    from_string(unsigned int, uint, "%u")
    from_string(unsigned long, ulong, "%lu")
    from_string(unsigned long long, ullong, "%llu")

    from_string(signed char, schar, "%hhi")
    from_string(short, shrt, "%hi")
    from_string(int, int, "%i")
    from_string(long, long, "%li")
    from_string(long long, llong, "%lli")

    from_string(float, flt, "%f")
    from_string(double, dbl, "%lf")
    from_string(long double, ldbl, "%Lf")

    from_string(uint8_t, uint8, "%"SCNu8)
    from_string(uint16_t, uint16, "%"SCNu16)
    from_string(uint32_t, uint32, "%"SCNu32)
    from_string(uint64_t, uint64, "%"SCNu64)

    from_string(int8_t, int8, "%"SCNi8)
    from_string(int16_t, int16, "%"SCNi16)
    from_string(int32_t, int32, "%"SCNi32)
    from_string(int64_t, int64, "%"SCNi64)

    #undef from_string
#endif