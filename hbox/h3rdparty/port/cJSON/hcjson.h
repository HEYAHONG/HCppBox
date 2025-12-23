/***************************************************************
 * Name:      hcjson.h
 * Purpose:   声明hcjson接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HCJSON_H_INCLUDE__
#define __HCJSON_H_INCLUDE__
#include "h3rdparty_port_cJSON.h"
#include "stdbool.h"
#include "stdint.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hcjson;
typedef struct hcjson hcjson_t;

struct hcjson_hook;
typedef struct hcjson_hooks hcjson_hooks_t;
struct hcjson_hooks
{
    void *(*malloc_fn)(size_t sz);
    void (*free_fn)(void *ptr);
} ;


typedef int hcjson_bool_t;

const char*  hcjson_version(void);

void  hcjson_init_hooks(hcjson_hooks_t* hooks);


hcjson_t *  hcjson_parse(const char *value);
hcjson_t *  hcjson_parse_with_length(const char *value, size_t buffer_length);

hcjson_t *  hcjson_parse_with_opts(const char *value, const char **return_parse_end, hcjson_bool_t require_null_terminated);
hcjson_t *  hcjson_parse_with_length_opts(const char *value, size_t buffer_length, const char **return_parse_end, hcjson_bool_t require_null_terminated);

char *  hcjson_print(const hcjson_t *item);

char *  hcjson_print_unformatted(const hcjson_t *item);

char *  hcjson_print_buffered(const hcjson_t *item, int prebuffer, hcjson_bool_t fmt);

hcjson_bool_t  hcjson_print_preallocated(hcjson_t *item, char *buffer, const int length, const hcjson_bool_t format);

void  hcjson_delete(hcjson_t *item);


int  hcjson_get_array_size(const hcjson_t *array);

hcjson_t *  hcjson_get_array_item(const hcjson_t *array, int index);

hcjson_t *  hcjson_get_object_item(const hcjson_t * const object, const char * const string);
hcjson_t *  hcjson_get_object_item_casesensitive(const hcjson_t * const object, const char * const string);
hcjson_bool_t  hcjson_has_object_item(const hcjson_t *object, const char *string);

const char *  hcjson_get_error_ptr(void);


char *  hcjson_get_string_value(const hcjson_t * const item);
double  hcjson_get_number_value(const hcjson_t * const item);


hcjson_bool_t  hcjson_is_invalid(const hcjson_t * const item);
hcjson_bool_t  hcjson_is_false(const hcjson_t * const item);
hcjson_bool_t  hcjson_is_true(const hcjson_t * const item);
hcjson_bool_t  hcjson_is_bool(const hcjson_t * const item);
hcjson_bool_t  hcjson_is_null(const hcjson_t * const item);
hcjson_bool_t  hcjson_is_number(const hcjson_t * const item);
hcjson_bool_t  hcjson_is_string(const hcjson_t * const item);
hcjson_bool_t  hcjson_is_array(const hcjson_t * const item);
hcjson_bool_t  hcjson_is_object(const hcjson_t * const item);
hcjson_bool_t  hcjson_is_raw(const hcjson_t * const item);


hcjson_t *  hcjson_create_null(void);
hcjson_t *  hcjson_create_true(void);
hcjson_t *  hcjson_create_false(void);
hcjson_t *  hcjson_create_bool(hcjson_bool_t boolean);
hcjson_t *  hcjson_create_number(double num);
hcjson_t *  hcjson_create_string(const char *string);

hcjson_t *  hcjson_create_raw(const char *raw);
hcjson_t *  hcjson_create_array(void);
hcjson_t *  hcjson_create_object(void);


hcjson_t *  hcjson_create_string_reference(const char *string);

hcjson_t *  hcjson_create_object_reference(const hcjson_t *child);
hcjson_t *  hcjson_create_array_reference(const hcjson_t *child);


hcjson_t *  hcjson_create_int_array(const int *numbers, int count);
hcjson_t *  hcjson_create_float_array(const float *numbers, int count);
hcjson_t *  hcjson_create_double_array(const double *numbers, int count);
hcjson_t *  hcjson_create_string_array(const char *const *strings, int count);


hcjson_bool_t  hcjson_add_item_to_array(hcjson_t *array, hcjson_t *item);
hcjson_bool_t  hcjson_add_item_to_object(hcjson_t *object, const char *string, hcjson_t *item);

hcjson_bool_t  hcjson_add_item_to_object_CS(hcjson_t *object, const char *string, hcjson_t *item);

hcjson_bool_t  hcjson_add_item_reference_to_array(hcjson_t *array, hcjson_t *item);
hcjson_bool_t  hcjson_add_item_reference_to_object(hcjson_t *object, const char *string, hcjson_t *item);


hcjson_t *  hcjson_detach_item_via_pointer(hcjson_t *parent, hcjson_t * const item);
hcjson_t *  hcjson_detach_item_from_array(hcjson_t *array, int which);
void  hcjson_delete_item_from_array(hcjson_t *array, int which);
hcjson_t *  hcjson_detach_item_from_object(hcjson_t *object, const char *string);
hcjson_t *  hcjson_detach_item_from_object_casesensitive(hcjson_t *object, const char *string);
void  hcjson_delete_item_from_object(hcjson_t *object, const char *string);
void  hcjson_delete_item_from_object_casesensitive(hcjson_t *object, const char *string);


hcjson_bool_t  hcjson_insert_item_in_array(hcjson_t *array, int which, hcjson_t *newitem);
hcjson_bool_t  hcjson_replace_item_via_pointer(hcjson_t * const parent, hcjson_t * const item, hcjson_t * replacement);
hcjson_bool_t  hcjson_replace_item_in_array(hcjson_t *array, int which, hcjson_t *newitem);
hcjson_bool_t  hcjson_replace_item_in_object(hcjson_t *object,const char *string,hcjson_t *newitem);
hcjson_bool_t  hcjson_replace_item_in_object_casesensitive(hcjson_t *object,const char *string,hcjson_t *newitem);


hcjson_t *  hcjson_duplicate(const hcjson_t *item, hcjson_bool_t recurse);

hcjson_bool_t  hcjson_compare(const hcjson_t * const a, const hcjson_t * const b, const hcjson_bool_t case_sensitive);


void  hcjson_minify(char *json);


hcjson_t*  hcjson_add_null_to_object(hcjson_t * const object, const char * const name);
hcjson_t*  hcjson_add_true_to_object(hcjson_t * const object, const char * const name);
hcjson_t*  hcjson_add_false_to_object(hcjson_t * const object, const char * const name);
hcjson_t*  hcjson_add_bool_to_object(hcjson_t * const object, const char * const name, const hcjson_bool_t boolean);
hcjson_t*  hcjson_add_number_to_object(hcjson_t * const object, const char * const name, const double number);
hcjson_t*  hcjson_add_string_to_object(hcjson_t * const object, const char * const name, const char * const string);
hcjson_t*  hcjson_add_raw_to_object(hcjson_t * const object, const char * const name, const char * const raw);
hcjson_t*  hcjson_add_object_to_object(hcjson_t * const object, const char * const name);
hcjson_t*  hcjson_add_array_to_object(hcjson_t * const object, const char * const name);

int hcjson_set_int_value(hcjson_t * object,int number);
double  hcjson_set_number_helper(hcjson_t *object, double number);
double  hcjson_set_number_value(hcjson_t *object, double number);

hcjson_bool_t hcjson_set_bool_value(hcjson_t * object,bool boolValue);

char*  hcjson_set_valuestring(hcjson_t *object, const char *valuestring);


void hcjson_array_for_each(void (*on_element)(hcjson_t *,void *usr),hcjson_t * const array,void *usr);


void *  hcjson_malloc(size_t size);
void  hcjson_free(void *object);


hcjson_t *  hcjson_utils_get_pointer(hcjson_t * const object, const char *pointer);
hcjson_t *  hcjson_utils_get_pointer_casesensitive(hcjson_t * const object, const char *pointer);


hcjson_t *  hcjson_utils_generate_patches(hcjson_t * const from, hcjson_t * const to);
hcjson_t *  hcjson_utils_generate_patches_casesensitive(hcjson_t * const from, hcjson_t * const to);

void  hcjson_utils_add_patch_to_array(hcjson_t * const array, const char * const operation, const char * const path, const hcjson_t * const value);

int  hcjson_utils_apply_patches(hcjson_t * const object, const hcjson_t * const patches);
int  hcjson_utils_apply_patches_casesensitive(hcjson_t * const object, const hcjson_t * const patches);


hcjson_t *  hcjson_utils_merge_patch(hcjson_t *target, const hcjson_t * const patch);
hcjson_t *  hcjson_utils_merge_patch_casesensitive(hcjson_t *target, const hcjson_t * const patch);

hcjson_t *  hcjson_utils_generate_merge_patch(hcjson_t * const from, hcjson_t * const to);
hcjson_t *  hcjson_utils_generate_merge_patch_casesensitive(hcjson_t * const from, hcjson_t * const to);

char *  hcjson_utils_find_pointer_from_object_to(const hcjson_t * const object, const hcjson_t * const target);

void  hcjson_utils_sort_bject(hcjson_t * const object);
void  hcjson_utils_sort_object_casesensitive(hcjson_t * const object);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HCJSON_H_INCLUDE__
