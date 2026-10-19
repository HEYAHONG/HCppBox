#ifndef __HTTPD_H__
#define __HTTPD_H__

#include "HCPPBox.h"

#ifdef __cplusplus
extern "C"
{
#endif

void httpd_set_index_file_name(const char *file_name);

void httpd_set_root_path(const char * path);

const hhttp_tinyserver_service_t * httpd_service(void);


#ifdef __cplusplus
}
#endif

#endif // __HTTPD_H__
