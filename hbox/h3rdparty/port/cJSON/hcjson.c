/***************************************************************
 * Name:      hcjson.c
 * Purpose:   实现hcjson接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-12-22
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hcjson.h"
#ifndef H3RDPARTY_USING_SYSTEM_CJSON
/*
 * 隐藏cJSON符号
 */
#define  CJSON_HIDE_SYMBOLS 1
#include "h3rdparty/3rdparty/cJSON/cJSON.h"
#include "h3rdparty/3rdparty/cJSON/cJSON_Utils.h"
#else
#include   "cJSON.h"
#include   "cJSON_Utils.h"
#endif

const char*  hcjson_version(void)
{
    return cJSON_Version();
}

void  hcjson_init_hooks(hcjson_hooks_t* hooks)
{
    if(hooks==NULL)
    {
        return;
    }
    struct cJSON_Hooks cjsonhooks;
    cjsonhooks.malloc_fn=hooks->malloc_fn;
    cjsonhooks.free_fn=hooks->free_fn;
    cJSON_InitHooks(&cjsonhooks);
}


hcjson_t *  hcjson_parse(const char *value)
{
    return (hcjson_t *)cJSON_Parse(value);
}
hcjson_t *  hcjson_parse_with_length(const char *value, size_t buffer_length)
{
    return (hcjson_t *)cJSON_ParseWithLength(value,buffer_length);
}

hcjson_t *  hcjson_parse_with_opts(const char *value, const char **return_parse_end, hcjson_bool_t require_null_terminated)
{
    return (hcjson_t *)cJSON_ParseWithOpts(value,return_parse_end,require_null_terminated);
}
hcjson_t *  hcjson_parse_with_length_opts(const char *value, size_t buffer_length, const char **return_parse_end, hcjson_bool_t require_null_terminated)
{
    return (hcjson_t *)cJSON_ParseWithLengthOpts(value,buffer_length,return_parse_end,require_null_terminated);
}

char *  hcjson_print(const hcjson_t *item)
{
    return cJSON_Print((const cJSON *)item);
}

char *  hcjson_print_unformatted(const hcjson_t *item)
{
    return cJSON_PrintUnformatted((const cJSON *)item);
}

char *  hcjson_print_buffered(const hcjson_t *item, int prebuffer, hcjson_bool_t fmt)
{
    return cJSON_PrintBuffered((const cJSON *)item,prebuffer,fmt);
}

hcjson_bool_t  hcjson_print_preallocated(hcjson_t *item, char *buffer, const int length, const hcjson_bool_t format)
{
    return cJSON_PrintPreallocated((cJSON *)item,buffer,length,format);
}

void  hcjson_delete(hcjson_t *item)
{
    cJSON_Delete((cJSON *)item);
}


int  hcjson_get_array_size(const hcjson_t *array)
{
    return cJSON_GetArraySize((const cJSON *)array);
}

hcjson_t *  hcjson_get_array_item(const hcjson_t *array, int index)
{
    return (hcjson_t *)cJSON_GetArrayItem((const cJSON *) array,index);
}

hcjson_t *  hcjson_get_object_item(const hcjson_t * const object, const char * const string)
{
    return (hcjson_t *)cJSON_GetObjectItem((const cJSON * const)object,string);
}
hcjson_t *  hcjson_get_object_item_casesensitive(const hcjson_t * const object, const char * const string)
{
    return (hcjson_t *)cJSON_GetObjectItemCaseSensitive((const cJSON * const)object,string);
}
hcjson_bool_t  hcjson_has_object_item(const hcjson_t *object, const char *string)
{
    return cJSON_HasObjectItem((const cJSON *)object,string);
}

const char *  hcjson_get_error_ptr(void)
{
    return cJSON_GetErrorPtr();
}


char *  hcjson_get_string_value(const hcjson_t * const item)
{
    return cJSON_GetStringValue((const cJSON * const)item);
}
double  hcjson_get_number_value(const hcjson_t * const item)
{
    return cJSON_GetNumberValue((const cJSON * const)item);
}


hcjson_bool_t  hcjson_is_invalid(const hcjson_t * const item)
{
    return cJSON_IsInvalid((const cJSON *const)item);
}
hcjson_bool_t  hcjson_is_false(const hcjson_t * const item)
{
    return cJSON_IsFalse((const cJSON *const)item);
}
hcjson_bool_t  hcjson_is_true(const hcjson_t * const item)
{
    return cJSON_IsTrue((const cJSON *const)item);
}
hcjson_bool_t  hcjson_is_bool(const hcjson_t * const item)
{
    return cJSON_IsBool((const cJSON *const)item);
}
hcjson_bool_t  hcjson_is_null(const hcjson_t * const item)
{
    return cJSON_IsNull((const cJSON *const)item);
}
hcjson_bool_t  hcjson_is_number(const hcjson_t * const item)
{
    return cJSON_IsNumber((const cJSON *const)item);
}
hcjson_bool_t  hcjson_is_string(const hcjson_t * const item)
{
    return cJSON_IsString((const cJSON *const)item);
}
hcjson_bool_t  hcjson_is_array(const hcjson_t * const item)
{
    return cJSON_IsArray((const cJSON *const)item);
}
hcjson_bool_t  hcjson_is_object(const hcjson_t * const item)
{
    return cJSON_IsObject((const cJSON *const)item);
}
hcjson_bool_t  hcjson_is_raw(const hcjson_t * const item)
{
    return cJSON_IsRaw((const cJSON *const)item);
}


hcjson_t *  hcjson_create_null(void)
{
    return (hcjson_t *)cJSON_CreateNull();
}
hcjson_t *  hcjson_create_true(void)
{
    return (hcjson_t *)cJSON_CreateTrue();
}
hcjson_t *  hcjson_create_false(void)
{
    return (hcjson_t *)cJSON_CreateFalse();
}
hcjson_t *  hcjson_create_bool(hcjson_bool_t boolean)
{
    return (hcjson_t *)cJSON_CreateBool(boolean);
}
hcjson_t *  hcjson_create_number(double num)
{
    return (hcjson_t *)cJSON_CreateNumber(num);
}
hcjson_t *  hcjson_create_string(const char *string)
{
    return (hcjson_t *)cJSON_CreateString(string);
}

hcjson_t *  hcjson_create_raw(const char *raw)
{
    return (hcjson_t *)cJSON_CreateRaw(raw);
}
hcjson_t *  hcjson_create_array(void)
{
    return (hcjson_t *)cJSON_CreateArray();
}
hcjson_t *  hcjson_create_object(void)
{
    return (hcjson_t *)cJSON_CreateObject();
}


hcjson_t *  hcjson_create_string_reference(const char *string)
{
    return (hcjson_t*)cJSON_CreateStringReference(string);
}

hcjson_t *  hcjson_create_object_reference(const hcjson_t *child)
{
    return (hcjson_t *)cJSON_CreateObjectReference((const cJSON *)child);
}
hcjson_t *  hcjson_create_array_reference(const hcjson_t *child)
{
    return (hcjson_t *)cJSON_CreateArrayReference((const cJSON *)child);
}


hcjson_t *  hcjson_create_int_array(const int *numbers, int count)
{
    return (hcjson_t *)cJSON_CreateIntArray(numbers,count);
}
hcjson_t *  hcjson_create_float_array(const float *numbers, int count)
{
    return (hcjson_t *)cJSON_CreateFloatArray(numbers,count);
}
hcjson_t *  hcjson_create_double_array(const double *numbers, int count)
{
    return (hcjson_t *)cJSON_CreateDoubleArray(numbers,count);
}
hcjson_t *  hcjson_create_string_array(const char *const *strings, int count)
{
    return (hcjson_t *)cJSON_CreateStringArray(strings,count);
}


hcjson_bool_t  hcjson_add_item_to_array(hcjson_t *array, hcjson_t *item)
{
    return cJSON_AddItemToArray((cJSON *)array,(cJSON *)item);
}
hcjson_bool_t  hcjson_add_item_to_object(hcjson_t *object, const char *string, hcjson_t *item)
{
    return cJSON_AddItemToObject((cJSON *)object,string,(cJSON *)item);
}

hcjson_bool_t  hcjson_add_item_to_object_CS(hcjson_t *object, const char *string, hcjson_t *item)
{
    return cJSON_AddItemToObjectCS((cJSON *)object,string,(cJSON *)item);
}

hcjson_bool_t  hcjson_add_item_reference_to_array(hcjson_t *array, hcjson_t *item)
{
    return cJSON_AddItemReferenceToArray((cJSON *)array,(cJSON *)item);
}
hcjson_bool_t  hcjson_add_item_reference_to_object(hcjson_t *object, const char *string, hcjson_t *item)
{
    return cJSON_AddItemReferenceToObject((cJSON *)object,string,(cJSON *)item);
}


hcjson_t *  hcjson_detach_item_via_pointer(hcjson_t *parent, hcjson_t * const item)
{
    return (hcjson_t *)cJSON_DetachItemViaPointer((cJSON *)parent,(cJSON * const)item);
}
hcjson_t *  hcjson_detach_item_from_array(hcjson_t *array, int which)
{
    return (hcjson_t *)cJSON_DetachItemFromArray((cJSON *)array,which);
}
void  hcjson_delete_item_from_array(hcjson_t *array, int which)
{
    cJSON_DeleteItemFromArray((cJSON *)array,which);
}
hcjson_t *  hcjson_detach_item_from_object(hcjson_t *object, const char *string)
{
    return (hcjson_t *)cJSON_DetachItemFromObject((cJSON *)object,string);
}
hcjson_t *  hcjson_detach_item_from_object_casesensitive(hcjson_t *object, const char *string)
{
    return (hcjson_t *)cJSON_DetachItemFromObjectCaseSensitive((cJSON *)object,string);
}
void  hcjson_delete_item_from_object(hcjson_t *object, const char *string)
{
    cJSON_DeleteItemFromObject((cJSON *)object,string);
}
void  hcjson_delete_item_from_object_casesensitive(hcjson_t *object, const char *string)
{
    cJSON_DeleteItemFromObjectCaseSensitive((cJSON *)object,string);
}


hcjson_bool_t  hcjson_insert_item_in_array(hcjson_t *array, int which, hcjson_t *newitem)
{
    return cJSON_InsertItemInArray((cJSON *)array,which,(cJSON *)newitem);
}
hcjson_bool_t  hcjson_replace_item_via_pointer(hcjson_t * const parent, hcjson_t * const item, hcjson_t * replacement)
{
    return cJSON_ReplaceItemViaPointer((cJSON *const)parent,(cJSON *const)item,(cJSON *)replacement);
}
hcjson_bool_t  hcjson_replace_item_in_array(hcjson_t *array, int which, hcjson_t *newitem)
{
    return cJSON_ReplaceItemInArray((cJSON *)array,which,(cJSON *)newitem);
}
hcjson_bool_t  hcjson_replace_item_in_object(hcjson_t *object,const char *string,hcjson_t *newitem)
{
    return cJSON_ReplaceItemInObject((cJSON *)object,string,(cJSON *)newitem);
}
hcjson_bool_t  hcjson_replace_item_in_object_casesensitive(hcjson_t *object,const char *string,hcjson_t *newitem)
{
    return cJSON_ReplaceItemInObjectCaseSensitive((cJSON *)object,string,(cJSON *)newitem);
}


hcjson_t *  hcjson_duplicate(const hcjson_t *item, hcjson_bool_t recurse)
{
    return (hcjson_t *)cJSON_Duplicate((const cJSON *)item,recurse);
}

hcjson_bool_t  hcjson_compare(const hcjson_t * const a, const hcjson_t * const b, const hcjson_bool_t case_sensitive)
{
    return cJSON_Compare((const cJSON *const)a,(const cJSON *const)b,case_sensitive);
}


void  hcjson_minify(char *json)
{
    cJSON_Minify(json);
}


hcjson_t*  hcjson_add_null_to_object(hcjson_t * const object, const char * const name)
{
    return (hcjson_t *)cJSON_AddNullToObject((cJSON * const)object,name);
}
hcjson_t*  hcjson_add_true_to_object(hcjson_t * const object, const char * const name)
{
    return (hcjson_t *)cJSON_AddTrueToObject((cJSON * const)object,name);
}
hcjson_t*  hcjson_add_false_to_object(hcjson_t * const object, const char * const name)
{
    return (hcjson_t *)cJSON_AddFalseToObject((cJSON * const)object,name);
}
hcjson_t*  hcjson_add_bool_to_object(hcjson_t * const object, const char * const name, const hcjson_bool_t boolean)
{
    return (hcjson_t *)cJSON_AddBoolToObject((cJSON * const)object,name,boolean);
}
hcjson_t*  hcjson_add_number_to_object(hcjson_t * const object, const char * const name, const double number)
{
    return (hcjson_t *)cJSON_AddNumberToObject((cJSON * const)object,name,number);
}
hcjson_t*  hcjson_add_string_to_object(hcjson_t * const object, const char * const name, const char * const string)
{
    return (hcjson_t *)cJSON_AddStringToObject((cJSON * const)object,name,string);
}
hcjson_t*  hcjson_add_raw_to_object(hcjson_t * const object, const char * const name, const char * const raw)
{
    return (hcjson_t *)cJSON_AddRawToObject((cJSON * const)object,name,raw);
}
hcjson_t*  hcjson_add_object_to_object(hcjson_t * const object, const char * const name)
{
    return (hcjson_t *)cJSON_AddObjectToObject((cJSON * const)object,name);
}
hcjson_t*  hcjson_add_array_to_object(hcjson_t * const object, const char * const name)
{
    return (hcjson_t *)cJSON_AddArrayToObject((cJSON * const)object,name);
}

int hcjson_set_int_value(hcjson_t * object,int number)
{
    return cJSON_SetIntValue(((cJSON *)object),number);
}

double  hcjson_set_number_helper(hcjson_t *object, double number)
{
    return cJSON_SetNumberHelper((cJSON *)object,number);
}

double  hcjson_set_number_value(hcjson_t *object, double number)
{
    return cJSON_SetNumberValue(((cJSON *)object),number);
}

hcjson_bool_t hcjson_set_bool_value(hcjson_t * object,bool boolValue)
{
    return cJSON_SetBoolValue(((cJSON *)object),boolValue);
}

char*  hcjson_set_valuestring(hcjson_t *object, const char *valuestring)
{
    return cJSON_SetValuestring((cJSON *)object,valuestring);
}

void hcjson_array_for_each(void (*on_element)(hcjson_t *),hcjson_t * const array)
{
    if(on_element==NULL)
    {
        return;
    }
    {
        cJSON *element=NULL;
        cJSON_ArrayForEach(element, ((cJSON *const)array))
        {
            on_element((hcjson_t *)element);
        }
    }
}

void *  hcjson_malloc(size_t size)
{
    return cJSON_malloc(size);
}
void  hcjson_free(void *object)
{
    cJSON_free(object);
}

hcjson_t *  hcjson_utils_get_pointer(hcjson_t * const object, const char *pointer)
{
    return (hcjson_t *)cJSONUtils_GetPointer((cJSON *const)object,pointer);
}
hcjson_t *  hcjson_utils_get_pointer_casesensitive(hcjson_t * const object, const char *pointer)
{
    return (hcjson_t *)cJSONUtils_GetPointerCaseSensitive((cJSON *const)object,pointer);
}


hcjson_t *  hcjson_utils_generate_patches(hcjson_t * const from, hcjson_t * const to)
{
    return (hcjson_t *)cJSONUtils_GeneratePatches((cJSON *const)from,(cJSON * const)to);
}
hcjson_t *  hcjson_utils_generate_patches_casesensitive(hcjson_t * const from, hcjson_t * const to)
{
    return (hcjson_t *)cJSONUtils_GeneratePatchesCaseSensitive((cJSON *const)from,(cJSON * const)to);
}

void  hcjson_utils_add_patch_to_array(hcjson_t * const array, const char * const operation, const char * const path, const hcjson_t * const value)
{
    cJSONUtils_AddPatchToArray((cJSON *const)array,operation,path,(const cJSON * const)value);
}

int  hcjson_utils_apply_patches(hcjson_t * const object, const hcjson_t * const patches)
{
    return cJSONUtils_ApplyPatches((cJSON *const)object,(const cJSON *const)patches);
}
int  hcjson_utils_apply_patches_sasesensitive(hcjson_t * const object, const hcjson_t * const patches)
{
    return cJSONUtils_ApplyPatchesCaseSensitive((cJSON *const)object,(const cJSON *const)patches);
}


hcjson_t *  hcjson_utils_merge_patch(hcjson_t *target, const hcjson_t * const patch)
{
    return (hcjson_t *)cJSONUtils_MergePatch((cJSON *)target,(const cJSON * const)patch);
}
hcjson_t *  hcjson_utils_merge_patch_casesensitive(hcjson_t *target, const hcjson_t * const patch)
{
    return (hcjson_t *)cJSONUtils_MergePatchCaseSensitive((cJSON *)target,(const cJSON * const)patch);
}

hcjson_t *  hcjson_utils_generate_merge_patch(hcjson_t * const from, hcjson_t * const to)
{
    return (hcjson_t *)cJSONUtils_GenerateMergePatch((cJSON *const )from,(cJSON *const)to);
}
hcjson_t *  hcjson_utils_generate_merge_patch_casesensitive(hcjson_t * const from, hcjson_t * const to)
{
    return (hcjson_t *)cJSONUtils_GenerateMergePatchCaseSensitive((cJSON *const )from,(cJSON *const)to);
}

char *  hcjson_utils_find_pointer_from_object_to(const hcjson_t * const object, const hcjson_t * const target)
{
    return cJSONUtils_FindPointerFromObjectTo((const cJSON *const)object,(const cJSON *const )target);
}

void  hcjson_utils_sort_bject(hcjson_t * const object)
{
    cJSONUtils_SortObject((cJSON *const)object);
}
void  hcjson_utils_sort_object_casesensitive(hcjson_t * const object)
{
    cJSONUtils_SortObjectCaseSensitive((cJSON *const)object);
}

