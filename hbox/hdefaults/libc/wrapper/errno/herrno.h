/***************************************************************
 * Name:      herrno.h
 * Purpose:   声明herrno接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-01
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HERRNO_H__
#define __HERRNO_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdio.h"
#include "errno.h"

typedef int herrno_t;

#define herrno (*herrno_get())

herrno_t * herrno_get(void);

#ifdef  EOK
#define HEOK                    EOK
#else
#define HEOK                    0
#endif
#ifdef  EPERM
#define HEPERM                  EPERM
#else
#define HEPERM                  1
#endif
#ifdef  ENOENT
#define HENOENT                 ENOENT
#else
#define HENOENT                 2
#endif

#ifdef  ESRCH
#define HESRCH                  ESRCH
#else
#define HESRCH                  3
#endif
#ifdef EINTR
#define HEINTR                  EINTR
#else
#define HEINTR                  4
#endif
#ifdef EIO
#define HEIO                    EIO
#else
#define HEIO                    5
#endif
#ifdef ENXIO
#define HENXIO                  ENXIO
#else
#define HENXIO                  6
#endif
#ifdef E2BIG
#define HE2BIG                  E2BIG
#else
#define HE2BIG                  7
#endif
#ifdef ENOEXEC
#define HENOEXEC                ENOEXEC
#else
#define HENOEXEC                8
#endif
#ifdef EBADF
#define HEBADF                  EBADF
#else
#define HEBADF                  9
#endif
#ifdef ECHILD
#define HECHILD                 ECHILD
#else
#define HECHILD                 10
#endif
#ifdef EAGAIN
#define HEAGAIN                 EAGAIN
#else
#define HEAGAIN                 11
#endif
#ifdef ENOMEM
#define HENOMEM                 ENOMEM
#else
#define HENOMEM                 12
#endif
#ifdef EACCES
#define HEACCES                 EACCES
#else
#define HEACCES                 13
#endif
#ifdef EFAULT
#define HEFAULT                 EFAULT
#else
#define HEFAULT                 14
#endif
#ifdef ENOTBLK
#define HENOTBLK                ENOTBLK
#else
#define HENOTBLK                15
#endif
#ifdef EBUSY
#define HEBUSY                  EBUSY
#else
#define HEBUSY                  16
#endif
#ifdef EEXIST
#define HEEXIST                 EEXIST
#else
#define HEEXIST                 17
#endif
#ifdef EXDEV
#define HEXDEV                  EXDEV
#else
#define HEXDEV                  18
#endif
#ifdef ENODEV
#define HENODEV                 ENODEV
#else
#define HENODEV                 19
#endif
#ifdef ENOTDIR
#define HENOTDIR                ENOTDIR
#else
#define HENOTDIR                20
#endif
#ifdef EISDIR
#define HEISDIR                 EISDIR
#else
#define HEISDIR                 21
#endif
#ifdef EINVAL
#define HEINVAL                 EINVAL
#else
#define HEINVAL                 22
#endif
#ifdef ENFILE
#define HENFILE                 ENFILE
#else
#define HENFILE                 23
#endif
#ifdef EMFILE
#define HEMFILE                 EMFILE
#else
#define HEMFILE                 24
#endif
#ifdef ENOTTY
#define HENOTTY                 ENOTTY
#else
#define HENOTTY                 25
#endif
#ifdef ETXTBSY
#define HETXTBSY                ETXTBSY
#else
#define HETXTBSY                26
#endif
#ifdef EFBIG
#define HEFBIG                  EFBIG
#else
#define HEFBIG                  27
#endif
#ifdef ENOSPC
#define HENOSPC                 ENOSPC
#else
#define HENOSPC                 28
#endif
#ifdef ESPIPE
#define HESPIPE                 ESPIPE
#else
#define HESPIPE                 29
#endif
#ifdef EROFS
#define HEROFS                  EROFS
#else
#define HEROFS                  30
#endif
#ifdef EMLINK
#define HEMLINK                 EMLINK
#else
#define HEMLINK                 31
#endif
#ifdef EPIPE
#define HEPIPE                  EPIPE
#else
#define HEPIPE                  32
#endif
#ifdef EDOM
#define HEDOM                   EDOM
#else
#define HEDOM                   33
#endif
#ifdef ERANGE
#define HERANGE                 ERANGE
#else
#define HERANGE                 34
#endif
#ifdef ENOMSG
#define HENOMSG                 ENOMSG
#else
#define HENOMSG                 35
#endif
#ifdef EIDRM
#define HEIDRM                  EIDRM
#else
#define HEIDRM                  36
#endif
#ifdef ECHRNG
#define HECHRNG                 ECHRNG
#else
#define HECHRNG                 37
#endif
#ifdef EL2NSYNC
#define HEL2NSYNC               EL2NSYNC
#else
#define HEL2NSYNC               38
#endif
#ifdef EL3HLT
#define HEL3HLT                 EL3HLT
#else
#define HEL3HLT                 39
#endif
#ifdef EL3RST
#define HEL3RST                 EL3RST
#else
#define HEL3RST                 40
#endif
#ifdef ELNRNG
#define HELNRNG                 ELNRNG
#else
#define HELNRNG                 41
#endif
#ifdef EUNATCH
#define HEUNATCH EUNATCH
#else
#define HEUNATCH                42
#endif
#ifdef ENOCSI
#define HENOCSI                 ENOCSI
#else
#define HENOCSI                 43
#endif
#ifdef EL2HLT
#define HEL2HLT                 EL2HLT
#else
#define HEL2HLT                 44
#endif
#ifdef EDEADLK
#define HEDEADLK                EDEADLK
#else
#define HEDEADLK                45
#endif
#ifdef ENOLCK
#define HENOLCK                 ENOLCK
#else
#define HENOLCK                 46
#endif
#ifdef EBADE
#define HEBADE                  EBADE
#else
#define HEBADE                  50
#endif
#ifdef EBADR
#define HEBADR                  EBADR
#else
#define HEBADR                  51
#endif
#ifdef EXFULL
#define HEXFULL                 EXFULL
#else
#define HEXFULL                 52
#endif
#ifdef ENOANO
#define HENOANO                 ENOANO
#else
#define HENOANO                 53
#endif
#ifdef EBADRQC
#define HEBADRQC                EBADRQC
#else
#define HEBADRQC                54
#endif
#ifdef EBADSLT
#define HEBADSLT                EBADSLT
#else
#define HEBADSLT                55
#endif
#ifdef EDEADLOCK
#define HEDEADLOCK              EDEADLOCK
#else
#define HEDEADLOCK              56
#endif
#ifdef EBFONT
#define HEBFONT                 EBFONT
#else
#define HEBFONT                 57
#endif
#ifdef ENOSTR
#define HENOSTR                 ENOSTR
#else
#define HENOSTR                 60
#endif
#ifdef ENODATA
#define HENODATA                ENODATA
#else
#define HENODATA                61
#endif
#ifdef ETIME
#define HETIME                  ETIME
#else
#define HETIME                  62
#endif
#ifdef ENOSR
#define HENOSR                  ENOSR
#else
#define HENOSR                  63
#endif
#ifdef ENONET
#define HENONET                 ENONET
#else
#define HENONET                 64
#endif
#ifdef ENOPKG
#define HENOPKG                 ENOPKG
#else
#define HENOPKG                 65
#endif
#ifdef EREMOTE
#define HEREMOTE                EREMOTE
#else
#define HEREMOTE                66
#endif
#ifdef ENOLINK
#define HENOLINK                ENOLINK
#else
#define HENOLINK                67
#endif
#ifdef EADV
#define HEADV                   EADV
#else
#define HEADV                   68
#endif
#ifdef ESRMNT
#define HESRMNT                 ESRMNT
#else
#define HESRMNT                 69
#endif
#ifdef ECOMM
#define HECOMM                  ECOMM
#else
#define HECOMM                  70
#endif
#ifdef EPROTO
#define HEPROTO                 EPROTO
#else
#define HEPROTO                 71
#endif
#ifdef EMULTIHOP
#define HEMULTIHOP              EMULTIHOP
#else
#define HEMULTIHOP              74
#endif
#ifdef ELBIN
#define HELBIN                  ELBIN
#else
#define HELBIN                  75
#endif
#ifdef EDOTDOT
#define HEDOTDOT                EDOTDOT
#else
#define HEDOTDOT                76
#endif
#ifdef EBADMSG
#define HEBADMSG                EBADMSG
#else
#define HEBADMSG                77
#endif
#ifdef EFTYPE
#define HEFTYPE                 EFTYPE
#else
#define HEFTYPE                 79
#endif
#ifdef ENOTUNIQ
#define HENOTUNIQ               ENOTUNIQ
#else
#define HENOTUNIQ               80
#endif
#ifdef EBADFD
#define HEBADFD                 EBADFD
#else
#define HEBADFD                 81
#endif
#ifdef EREMCHG
#define HEREMCHG                EREMCHG
#else
#define HEREMCHG                82
#endif
#ifdef ELIBACC
#define HELIBACC                ELIBACC
#else
#define HELIBACC                83
#endif
#ifdef ELIBBAD
#define HELIBBAD                ELIBBAD
#else
#define HELIBBAD                84
#endif
#ifdef ELIBSCN
#define HELIBSCN                ELIBSCN
#else
#define HELIBSCN                85
#endif
#ifdef ELIBMAX
#define HELIBMAX                ELIBMAX
#else
#define HELIBMAX                86
#endif
#ifdef ELIBEXEC
#define HELIBEXEC               ELIBEXEC
#else
#define HELIBEXEC               87
#endif
#ifdef ENOSYS
#define HENOSYS                 ENOSYS
#else
#define HENOSYS                 88
#endif
#ifdef ENOTEMPTY
#define HENOTEMPTY              ENOTEMPTY
#else
#define HENOTEMPTY              90
#endif
#ifdef ENAMETOOLONG
#define HENAMETOOLONG           ENAMETOOLONG
#else
#define HENAMETOOLONG           91
#endif
#ifdef ELOOP
#define HELOOP                  ELOOP
#else
#define HELOOP                  92
#endif
#ifdef EOPNOTSUPP
#define HEOPNOTSUPP             EOPNOTSUPP
#else
#define HEOPNOTSUPP             95
#endif
#ifdef EPFNOSUPPORT
#define HEPFNOSUPPORT           EPFNOSUPPORT
#else
#define HEPFNOSUPPORT           96
#endif
#ifdef ECONNRESET
#define HECONNRESET             ECONNRESET
#else
#define HECONNRESET             104
#endif
#ifdef ENOBUFS
#define HENOBUFS                ENOBUFS
#else
#define HENOBUFS                105
#endif
#ifdef EAFNOSUPPORT
#define HEAFNOSUPPORT           EAFNOSUPPORT
#else
#define HEAFNOSUPPORT           106
#endif
#ifdef EPROTOTYPE
#define HEPROTOTYPE             EPROTOTYPE
#else
#define HEPROTOTYPE             107
#endif
#ifdef ENOTSOCK
#define HENOTSOCK               ENOTSOCK
#else
#define HENOTSOCK               108
#endif
#ifdef ENOPROTOOPT
#define HENOPROTOOPT            ENOPROTOOPT
#else
#define HENOPROTOOPT            109
#endif
#ifdef ESHUTDOWN
#define HESHUTDOWN              ESHUTDOWN
#else
#define HESHUTDOWN              110
#endif
#ifdef ECONNREFUSED
#define HECONNREFUSED           ECONNREFUSED
#else
#define HECONNREFUSED           111
#endif
#ifdef EADDRINUSE
#define HEADDRINUSE             EADDRINUSE
#else
#define HEADDRINUSE             112
#endif
#ifdef ECONNABORTED
#define HECONNABORTED           ECONNABORTED
#else
#define HECONNABORTED           113
#endif
#ifdef ENETUNREACH
#define HENETUNREACH            ENETUNREACH
#else
#define HENETUNREACH            114
#endif
#ifdef ENETDOWN
#define HENETDOWN               ENETDOWN
#else
#define HENETDOWN               115
#endif
#ifdef ETIMEDOUT
#define HETIMEDOUT              ETIMEDOUT
#else
#define HETIMEDOUT              116
#endif
#ifdef EHOSTDOWN
#define HEHOSTDOWN              EHOSTDOWN
#else
#define HEHOSTDOWN              117
#endif
#ifdef EHOSTUNREACH
#define HEHOSTUNREACH           EHOSTUNREACH
#else
#define HEHOSTUNREACH           118
#endif
#ifdef EINPROGRESS
#define HEINPROGRESS            EINPROGRESS
#else
#define HEINPROGRESS            119
#endif
#ifdef EALREADY
#define HEALREADY               EALREADY
#else
#define HEALREADY               120
#endif
#ifdef EDESTADDRREQ
#define HEDESTADDRREQ           EDESTADDRREQ
#else
#define HEDESTADDRREQ           121
#endif
#ifdef EMSGSIZE
#define HEMSGSIZE               EMSGSIZE
#else
#define HEMSGSIZE               122
#endif
#ifdef EPROTONOSUPPORT
#define HEPROTONOSUPPORT        EPROTONOSUPPORT
#else
#define HEPROTONOSUPPORT        123
#endif
#ifdef ESOCKTNOSUPPORT
#define HESOCKTNOSUPPORT        ESOCKTNOSUPPORT
#else
#define HESOCKTNOSUPPORT        124
#endif
#ifdef EADDRNOTAVAIL
#define HEADDRNOTAVAIL          EADDRNOTAVAIL
#else
#define HEADDRNOTAVAIL          125
#endif
#ifdef ENETRESET
#define HENETRESET              ENETRESET
#else
#define HENETRESET              126
#endif
#ifdef EISCONN
#define HEISCONN                EISCONN
#else
#define HEISCONN                127
#endif
#ifdef ENOTCONN
#define HENOTCONN               ENOTCONN
#else
#define HENOTCONN               128
#endif
#ifdef ETOOMANYREFS
#define HETOOMANYREFS           ETOOMANYREFS
#else
#define HETOOMANYREFS           129
#endif
#ifdef EPROCLIM
#define HEPROCLIM               EPROCLIM
#else
#define HEPROCLIM               130
#endif
#ifdef EUSERS
#define HEUSERS                 EUSERS
#else
#define HEUSERS                 131
#endif
#ifdef EDQUOT
#define HEDQUOT                 EDQUOT
#else
#define HEDQUOT                 132
#endif
#ifdef ESTALE
#define HESTALE                 ESTALE
#else
#define HESTALE                 133
#endif
#ifdef ENOTSUP
#define HENOTSUP                ENOTSUP
#else
#define HENOTSUP                134
#endif
#ifdef ENOMEDIUM
#define HENOMEDIUM              ENOMEDIUM
#else
#define HENOMEDIUM              135
#endif
#ifdef EILSEQ
#define HEILSEQ                 EILSEQ
#else
#define HEILSEQ                 138
#endif
#ifdef EOVERFLOW
#define HEOVERFLOW              EOVERFLOW
#else
#define HEOVERFLOW              139
#endif
#ifdef ECANCELED
#define HECANCELED              ECANCELED
#else
#define HECANCELED              140
#endif
#ifdef ENOTRECOVERABLE
#define HENOTRECOVERABLE        ENOTRECOVERABLE
#else
#define HENOTRECOVERABLE        141
#endif
#ifdef EOWNERDEAD
#define HEOWNERDEAD             EOWNERDEAD
#else
#define HEOWNERDEAD             142
#endif
#ifdef ESTRPIPE
#define HESTRPIPE               ESTRPIPE
#else
#define HESTRPIPE               143
#endif
#ifdef EHWPOISON
#define HEHWPOISON              EHWPOISON
#else
#define HEHWPOISON              144
#endif
#ifdef EISNAM
#define                         HEISNAM EISNAM
#else
#define HEISNAM                 145
#endif
#ifdef EKEYEXPIRED
#define HEKEYEXPIRED            EKEYEXPIRED
#else
#define HEKEYEXPIRED            146
#endif
#ifdef EKEYREJECTED
#define HEKEYREJECTED           EKEYREJECTED
#else
#define HEKEYREJECTED           147
#endif
#ifdef EKEYREVOKED
#define HEKEYREVOKED            EKEYREVOKED
#else
#define HEKEYREVOKED            148
#endif


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HERRNO_H__
