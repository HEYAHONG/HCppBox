#include "../runtime/criticalsection.h"
#include "../runtime/libc.h"

#if !defined(LIBMONO_SHELL_SHELL_H)
#define LIBMONO_SHELL_SHELL_H

#if !defined(LIBMONO_SHELL_BUFFER_LENGTH)
#define LIBMONO_SHELL_BUFFER_LENGTH         32  /**< shell缓存大小*/
#endif

/*
 * 注意：shell默认使用putchar输出，如需控制输出请配置putchar移植
 */

/** \brief shell处理
 *
 * \param ch int 输出shell的字符串，仅0～255有效，其余参数将忽略
 *
 */
void libmono_shell_process(int ch) LIBMONO_FUNCTION_ATTRIBUTE;

struct libmono_shell_command
{
    void (* entry)(char *cmd);
    const char *name;
    const char *help;
};

typedef struct libmono_shell_command libmono_shell_command_t;

#endif

