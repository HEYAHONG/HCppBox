/***************************************************************
 * Name:      herrno.c
 * Purpose:   实现herrno接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "herrno.h"
#include "hdefaults.h"
#include "errno.h"

volatile herrno_t * herrno_get()
{
#if defined(HERRNO)
    return (volatile herrno_t *)&(HERRNO);
#else
    return (volatile herrno_t *)&(errno);
#endif
}

static struct
{
    herrno_t        error_number;
    const char *    error_string;
}   herrno_string_table[]=
{
    {HEOK,"EOK"},
    {HEPERM,"EPERM"},
    {HENOENT,"ENOENT"},
    {HESRCH,"ESRCH"},
    {HEINTR,"EINTR"},
    {HEIO,"EIO"},
    {HENXIO,"ENXIO"},
    {HE2BIG,"E2BIG"},
    {HENOEXEC,"ENOEXEC"},
    {HEBADF,"EBADF"},
    {HECHILD,"ECHILD"},
    {HEAGAIN,"EAGAIN"},
    {HENOMEM,"ENOMEM"},
    {HEACCES,"EACCES"},
    {HEFAULT,"EFAULT"},
    {HENOTBLK,"ENOTBLK"},
    {HEBUSY,"EBUSY"},
    {HEEXIST,"EEXIST"},
    {HEXDEV,"EXDEV"},
    {HENODEV,"ENODEV"},
    {HENOTDIR,"ENOTDIR"},
    {HEISDIR,"EISDIR"},
    {HEINVAL,"EINVAL"},
    {HENFILE,"ENFILE"},
    {HEMFILE,"EMFILE"},
    {HENOTTY,"ENOTTY"},
    {HETXTBSY,"ETXTBSY"},
    {HEFBIG,"EFBIG"},
    {HENOSPC,"ENOSPC"},
    {HESPIPE,"ESPIPE"},
    {HEROFS,"EROFS"},
    {HEMLINK,"EMLINK"},
    {HEPIPE,"EPIPE"},
    {HEDOM,"EDOM"},
    {HERANGE,"ERANGE"},
    {HENOMSG,"ENOMSG"},
    {HEIDRM,"EIDRM"},
    {HECHRNG,"ECHRNG"},
    {HEL2NSYNC,"EL2NSYNC"},
    {HEL3HLT,"EL3HLT"},
    {HEL3RST,"EL3RST"},
    {HELNRNG,"ELNRNG"},
    {HEUNATCH,"EUNATCH"},
    {HENOCSI,"ENOCSI"},
    {HEL2HLT,"EL2HLT"},
    {HEDEADLK,"EDEADLK"},
    {HENOLCK,"ENOLCK"},
    {HEBADE,"EBADE"},
    {HEBADR,"EBADR"},
    {HEXFULL,"EXFULL"},
    {HENOANO,"ENOANO"},
    {HEBADRQC,"EBADRQC"},
    {HEBADSLT,"EBADSLT"},
    {HEDEADLOCK,"EDEADLOCK"},
    {HEBFONT,"EBFONT"},
    {HENOSTR,"ENOSTR"},
    {HENODATA,"ENODATA"},
    {HETIME,"ETIME"},
    {HENOSR,"ENOSR"},
    {HENONET,"ENONET"},
    {HENOPKG,"ENOPKG"},
    {HEREMOTE,"EREMOTE"},
    {HENOLINK,"ENOLINK"},
    {HEADV,"EADV"},
    {HESRMNT,"ESRMNT"},
    {HECOMM,"ECOMM"},
    {HEPROTO,"EPROTO"},
    {HEMULTIHOP,"EMULTIHOP"},
    {HELBIN,"ELBIN"},
    {HEDOTDOT,"EDOTDOT"},
    {HEBADMSG,"EBADMSG"},
    {HEFTYPE,"EFTYPE"},
    {HENOTUNIQ,"ENOTUNIQ"},
    {HEBADFD,"EBADFD"},
    {HEREMCHG,"EREMCHG"},
    {HELIBACC,"ELIBACC"},
    {HELIBBAD,"ELIBBAD"},
    {HELIBSCN,"ELIBSCN"},
    {HELIBMAX,"ELIBMAX"},
    {HELIBEXEC,"ELIBEXEC"},
    {HENOSYS,"ENOSYS"},
    {HENOTEMPTY,"ENOTEMPTY"},
    {HENAMETOOLONG,"ENAMETOOLONG"},
    {HELOOP,"ELOOP"},
    {HEOPNOTSUPP,"EOPNOTSUPP"},
    {HEPFNOSUPPORT,"EPFNOSUPPORT"},
    {HECONNRESET,"ECONNRESET"},
    {HENOBUFS,"ENOBUFS"},
    {HEAFNOSUPPORT,"EAFNOSUPPORT"},
    {HEPROTOTYPE,"EPROTOTYPE"},
    {HENOTSOCK,"ENOTSOCK"},
    {HENOPROTOOPT,"ENOPROTOOPT"},
    {HESHUTDOWN,"ESHUTDOWN"},
    {HECONNREFUSED,"ECONNREFUSED"},
    {HEADDRINUSE,"EADDRINUSE"},
    {HECONNABORTED,"ECONNABORTED"},
    {HENETUNREACH,"ENETUNREACH"},
    {HENETDOWN,"ENETDOWN"},
    {HETIMEDOUT,"ETIMEDOUT"},
    {HEHOSTDOWN,"EHOSTDOWN"},
    {HEHOSTUNREACH,"EHOSTUNREACH"},
    {HEINPROGRESS,"EINPROGRESS"},
    {HEALREADY,"EALREADY"},
    {HEDESTADDRREQ,"EDESTADDRREQ"},
    {HEMSGSIZE,"EMSGSIZE"},
    {HEPROTONOSUPPORT,"EPROTONOSUPPORT"},
    {HESOCKTNOSUPPORT,"ESOCKTNOSUPPORT"},
    {HEADDRNOTAVAIL,"EADDRNOTAVAIL"},
    {HENETRESET,"ENETRESET"},
    {HEISCONN,"EISCONN"},
    {HENOTCONN,"ENOTCONN"},
    {HETOOMANYREFS,"ETOOMANYREFS"},
    {HEPROCLIM,"EPROCLIM"},
    {HEUSERS,"EUSERS"},
    {HEDQUOT,"EDQUOT"},
    {HESTALE,"ESTALE"},
    {HENOTSUP,"ENOTSUP"},
    {HENOMEDIUM,"ENOMEDIUM"},
    {HEILSEQ,"EILSEQ"},
    {HEOVERFLOW,"EOVERFLOW"},
    {HECANCELED,"ECANCELED"},
    {HENOTRECOVERABLE,"ENOTRECOVERABLE"},
    {HEOWNERDEAD,"EOWNERDEAD"},
    {HESTRPIPE,"ESTRPIPE"},
    {HEHWPOISON,"EHWPOISON"},
    {HEISNAM,"EISNAM"},
    {HEKEYEXPIRED,"EKEYEXPIRED"},
    {HEKEYREJECTED,"EKEYREJECTED"},
    {HEKEYREVOKED,"EKEYREVOKED"}
};

const char *herrno_str(herrno_t error_number)
{
    for(size_t i=0; i<sizeof(herrno_string_table)/sizeof(herrno_string_table[0]); i++)
    {
        if(herrno_string_table[i].error_number==error_number)
        {
            return herrno_string_table[i].error_string;
        }
    }
    return "";
}
const char *herrno_curstr(void)
{
    return herrno_str(herrno);
}
