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

volatile herrno_t * herrno_get(void);

const char *herrno_str(herrno_t error_number);
const char *herrno_curstr(void);


#ifdef  EOK
#define HEOK                    EOK
#else
#define HEOK                    0
#define HEOK_DEFINED            1
#endif
#ifdef  EPERM
#define HEPERM                  EPERM
#else
#define HEPERM                  1
#define HEPERM_DEFINED          1
#endif
#ifdef  ENOENT
#define HENOENT                 ENOENT
#else
#define HENOENT                 2
#define HENOENT_DEFINED         1
#endif

#ifdef  ESRCH
#define HESRCH                  ESRCH
#else
#define HESRCH                  3
#define HESRCH_DEFINED          1
#endif
#ifdef EINTR
#define HEINTR                  EINTR
#else
#define HEINTR                  4
#define HEINTR_DEFINED          1
#endif
#ifdef EIO
#define HEIO                    EIO
#else
#define HEIO                    5
#define HEIO_DEFINED            1
#endif
#ifdef ENXIO
#define HENXIO                  ENXIO
#else
#define HENXIO                  6
#define HENXIO_DEFINED          1
#endif
#ifdef E2BIG
#define HE2BIG                  E2BIG
#else
#define HE2BIG                  7
#define HE2BIG_DEFINED          1
#endif
#ifdef ENOEXEC
#define HENOEXEC                ENOEXEC
#else
#define HENOEXEC                8
#define HENOEXEC_DEFINED        1
#endif
#ifdef EBADF
#define HEBADF                  EBADF
#else
#define HEBADF                  9
#define HEBADF_DEFINED          1
#endif
#ifdef ECHILD
#define HECHILD                 ECHILD
#else
#define HECHILD                 10
#define HECHILD_DEFINED         1
#endif
#ifdef EAGAIN
#define HEAGAIN                 EAGAIN
#else
#define HEAGAIN                 11
#define HEAGAIN_DEFINED         1
#endif
#ifdef ENOMEM
#define HENOMEM                 ENOMEM
#else
#define HENOMEM                 12
#define HENOMEM_DEFINED         1
#endif
#ifdef EACCES
#define HEACCES                 EACCES
#else
#define HEACCES                 13
#define HEACCES_DEFINED         1
#endif
#ifdef EFAULT
#define HEFAULT                 EFAULT
#else
#define HEFAULT                 14
#define HEFAULT_DEFINED         1
#endif
#ifdef ENOTBLK
#define HENOTBLK                ENOTBLK
#else
#define HENOTBLK                15
#define HENOTBLK_DEFINED        1
#endif
#ifdef EBUSY
#define HEBUSY                  EBUSY
#else
#define HEBUSY                  16
#define HEBUSY_DEFINED          1
#endif
#ifdef EEXIST
#define HEEXIST                 EEXIST
#else
#define HEEXIST                 17
#define HEEXIST_DEFINED         1
#endif
#ifdef EXDEV
#define HEXDEV                  EXDEV
#else
#define HEXDEV                  18
#define HEXDEV_DEFINED          1
#endif
#ifdef ENODEV
#define HENODEV                 ENODEV
#else
#define HENODEV                 19
#define HENODEV_DEFINED         1
#endif
#ifdef ENOTDIR
#define HENOTDIR                ENOTDIR
#else
#define HENOTDIR                20
#define HENOTDIR_DEFINED        1
#endif
#ifdef EISDIR
#define HEISDIR                 EISDIR
#else
#define HEISDIR                 21
#define HEISDIR_DEFINED         1
#endif
#ifdef EINVAL
#define HEINVAL                 EINVAL
#else
#define HEINVAL                 22
#define HEINVAL_DEFINED         1
#endif
#ifdef ENFILE
#define HENFILE                 ENFILE
#else
#define HENFILE                 23
#define HENFILE_DEFINED         1
#endif
#ifdef EMFILE
#define HEMFILE                 EMFILE
#else
#define HEMFILE                 24
#define HEMFILE_DEFINED         1
#endif
#ifdef ENOTTY
#define HENOTTY                 ENOTTY
#else
#define HENOTTY                 25
#define HENOTTY_DEFINED         1
#endif
#ifdef ETXTBSY
#define HETXTBSY                ETXTBSY
#else
#define HETXTBSY                26
#define HETXTBSY_DEFINED        1
#endif
#ifdef EFBIG
#define HEFBIG                  EFBIG
#else
#define HEFBIG                  27
#define HEFBIG_DEFINED          1
#endif
#ifdef ENOSPC
#define HENOSPC                 ENOSPC
#else
#define HENOSPC                 28
#define HENOSPC_DEFINED         1
#endif
#ifdef ESPIPE
#define HESPIPE                 ESPIPE
#else
#define HESPIPE                 29
#define HESPIPE_DEFINED         1
#endif
#ifdef EROFS
#define HEROFS                  EROFS
#else
#define HEROFS                  30
#define HEROFS_DEFINED          1
#endif
#ifdef EMLINK
#define HEMLINK                 EMLINK
#else
#define HEMLINK                 31
#define HEMLINK_DEFINED         1
#endif
#ifdef EPIPE
#define HEPIPE                  EPIPE
#else
#define HEPIPE                  32
#define HEPIPE_DEFINED          1
#endif
#ifdef EDOM
#define HEDOM                   EDOM
#else
#define HEDOM                   33
#define HEDOM_DEFINED           1
#endif
#ifdef ERANGE
#define HERANGE                 ERANGE
#else
#define HERANGE                 34
#define HERANGE_DEFINED         1
#endif
#ifdef ENOMSG
#define HENOMSG                 ENOMSG
#else
#define HENOMSG                 35
#define HENOMSG_DEFINED         1
#endif
#ifdef EIDRM
#define HEIDRM                  EIDRM
#else
#define HEIDRM                  36
#define HEIDRM_DEFINED          1
#endif
#ifdef ECHRNG
#define HECHRNG                 ECHRNG
#else
#define HECHRNG                 37
#define HECHRNG_DEFINED         1
#endif
#ifdef EL2NSYNC
#define HEL2NSYNC               EL2NSYNC
#else
#define HEL2NSYNC               38
#define HEL2NSYNC_DEFINED       1
#endif
#ifdef EL3HLT
#define HEL3HLT                 EL3HLT
#else
#define HEL3HLT                 39
#define HEL3HLT_DEFINED         1
#endif
#ifdef EL3RST
#define HEL3RST                 EL3RST
#else
#define HEL3RST                 40
#define HEL3RST_DEFINED         1
#endif
#ifdef ELNRNG
#define HELNRNG                 ELNRNG
#else
#define HELNRNG                 41
#define HELNRNG_DEFINED         1
#endif
#ifdef EUNATCH
#define HEUNATCH EUNATCH
#else
#define HEUNATCH                42
#define HEUNATCH_DEFINED        1
#endif
#ifdef ENOCSI
#define HENOCSI                 ENOCSI
#else
#define HENOCSI                 43
#define HENOCSI_DEFINED         1
#endif
#ifdef EL2HLT
#define HEL2HLT                 EL2HLT
#else
#define HEL2HLT                 44
#define HEL2HLT_DEFINED         1
#endif
#ifdef EDEADLK
#define HEDEADLK                EDEADLK
#else
#define HEDEADLK                45
#define HEDEADLK_DEFINED        1
#endif
#ifdef ENOLCK
#define HENOLCK                 ENOLCK
#else
#define HENOLCK                 46
#define HENOLCK_DEFINED         1
#endif
#ifdef EBADE
#define HEBADE                  EBADE
#else
#define HEBADE                  50
#define HEBADE_DEFINED          1
#endif
#ifdef EBADR
#define HEBADR                  EBADR
#else
#define HEBADR                  51
#define HEBADR_DEFINED          1
#endif
#ifdef EXFULL
#define HEXFULL                 EXFULL
#else
#define HEXFULL                 52
#define HEXFULL_DEFINED         1
#endif
#ifdef ENOANO
#define HENOANO                 ENOANO
#else
#define HENOANO                 53
#define HENOANO_DEFINED         1
#endif
#ifdef EBADRQC
#define HEBADRQC                EBADRQC
#else
#define HEBADRQC                54
#define HEBADRQC_DEFINED        1
#endif
#ifdef EBADSLT
#define HEBADSLT                EBADSLT
#else
#define HEBADSLT                55
#define HEBADSLT_DEFINED        1
#endif
#ifdef EDEADLOCK
#define HEDEADLOCK              EDEADLOCK
#else
#define HEDEADLOCK              56
#define HEDEADLOCK_DEFINED      1
#endif
#ifdef EBFONT
#define HEBFONT                 EBFONT
#else
#define HEBFONT                 57
#define HEBFONT_DEFINED         1
#endif
#ifdef ENOSTR
#define HENOSTR                 ENOSTR
#else
#define HENOSTR                 60
#define HENOSTR_DEFINED         1
#endif
#ifdef ENODATA
#define HENODATA                ENODATA
#else
#define HENODATA                61
#define HENODATA_DEFINED        1
#endif
#ifdef ETIME
#define HETIME                  ETIME
#else
#define HETIME                  62
#define HETIME_DEFINED          1
#endif
#ifdef ENOSR
#define HENOSR                  ENOSR
#else
#define HENOSR                  63
#define HENOSR_DEFINED          1
#endif
#ifdef ENONET
#define HENONET                 ENONET
#else
#define HENONET                 64
#define HENONET_DEFINED         1
#endif
#ifdef ENOPKG
#define HENOPKG                 ENOPKG
#else
#define HENOPKG                 65
#define HENOPKG_DEFINED         1
#endif
#ifdef EREMOTE
#define HEREMOTE                EREMOTE
#else
#define HEREMOTE                66
#define HEREMOTE_DEFINED        1
#endif
#ifdef ENOLINK
#define HENOLINK                ENOLINK
#else
#define HENOLINK                67
#define HENOLINK_DEFINED        1
#endif
#ifdef EADV
#define HEADV                   EADV
#else
#define HEADV                   68
#define HEADV_DEFINED           1
#endif
#ifdef ESRMNT
#define HESRMNT                 ESRMNT
#else
#define HESRMNT                 69
#define HESRMNT_DEFINED         1
#endif
#ifdef ECOMM
#define HECOMM                  ECOMM
#else
#define HECOMM                  70
#define HECOMM_DEFINED          1
#endif
#ifdef EPROTO
#define HEPROTO                 EPROTO
#else
#define HEPROTO                 71
#define HEPROTO_DEFINED         1
#endif
#ifdef EMULTIHOP
#define HEMULTIHOP              EMULTIHOP
#else
#define HEMULTIHOP              74
#define HEMULTIHOP_DEFINED      1
#endif
#ifdef ELBIN
#define HELBIN                  ELBIN
#else
#define HELBIN                  75
#define HELBIN_DEFINED          1
#endif
#ifdef EDOTDOT
#define HEDOTDOT                EDOTDOT
#else
#define HEDOTDOT                76
#define HEDOTDOT_DEFINED        1
#endif
#ifdef EBADMSG
#define HEBADMSG                EBADMSG
#else
#define HEBADMSG                77
#define HEBADMSG_DEFINED        1
#endif
#ifdef EFTYPE
#define HEFTYPE                 EFTYPE
#else
#define HEFTYPE                 79
#define HEFTYPE_DEFINED         1
#endif
#ifdef ENOTUNIQ
#define HENOTUNIQ               ENOTUNIQ
#else
#define HENOTUNIQ               80
#define HENOTUNIQ_DEFINED       1
#endif
#ifdef EBADFD
#define HEBADFD                 EBADFD
#else
#define HEBADFD                 81
#define HEBADFD_DEFINED         1
#endif
#ifdef EREMCHG
#define HEREMCHG                EREMCHG
#else
#define HEREMCHG                82
#define HEREMCHG_DEFINED        1
#endif
#ifdef ELIBACC
#define HELIBACC                ELIBACC
#else
#define HELIBACC                83
#define HELIBACC_DEFINED        1
#endif
#ifdef ELIBBAD
#define HELIBBAD                ELIBBAD
#else
#define HELIBBAD                84
#define HELIBBAD_DEFINED        1
#endif
#ifdef ELIBSCN
#define HELIBSCN                ELIBSCN
#else
#define HELIBSCN                85
#define HELIBSCN_DEFINED        1
#endif
#ifdef ELIBMAX
#define HELIBMAX                ELIBMAX
#else
#define HELIBMAX                86
#define HELIBMAX_DEFINED        1
#endif
#ifdef ELIBEXEC
#define HELIBEXEC               ELIBEXEC
#else
#define HELIBEXEC               87
#define HELIBEXEC_DEFINED       1
#endif
#ifdef ENOSYS
#define HENOSYS                 ENOSYS
#else
#define HENOSYS                 88
#define HENOSYS_DEFINED         1
#endif
#ifdef ENOTEMPTY
#define HENOTEMPTY              ENOTEMPTY
#else
#define HENOTEMPTY              90
#define HENOTEMPTY_DEFINED      1
#endif
#ifdef ENAMETOOLONG
#define HENAMETOOLONG           ENAMETOOLONG
#else
#define HENAMETOOLONG           91
#define HENAMETOOLONG_DEFINED   1
#endif
#ifdef ELOOP
#define HELOOP                  ELOOP
#else
#define HELOOP                  92
#define HELOOP_DEFINED          1
#endif
#ifdef EOPNOTSUPP
#define HEOPNOTSUPP             EOPNOTSUPP
#else
#define HEOPNOTSUPP             95
#define HEOPNOTSUPP_DEFINED     1
#endif
#ifdef EPFNOSUPPORT
#define HEPFNOSUPPORT           EPFNOSUPPORT
#else
#define HEPFNOSUPPORT           96
#define HEPFNOSUPPORT_DEFINED   1
#endif
#ifdef ECONNRESET
#define HECONNRESET             ECONNRESET
#else
#define HECONNRESET             104
#define HECONNRESET_DEFINED     1
#endif
#ifdef ENOBUFS
#define HENOBUFS                ENOBUFS
#else
#define HENOBUFS                105
#define HENOBUFS_DEFINED        1
#endif
#ifdef EAFNOSUPPORT
#define HEAFNOSUPPORT           EAFNOSUPPORT
#else
#define HEAFNOSUPPORT           106
#define HEAFNOSUPPORT_DEFINED   1
#endif
#ifdef EPROTOTYPE
#define HEPROTOTYPE             EPROTOTYPE
#else
#define HEPROTOTYPE             107
#define HEPROTOTYPE_DEFINED     1
#endif
#ifdef ENOTSOCK
#define HENOTSOCK               ENOTSOCK
#else
#define HENOTSOCK               108
#define HENOTSOCK_DEFINED       1
#endif
#ifdef ENOPROTOOPT
#define HENOPROTOOPT            ENOPROTOOPT
#else
#define HENOPROTOOPT            109
#define HENOPROTOOPT_DEFINED    1
#endif
#ifdef ESHUTDOWN
#define HESHUTDOWN              ESHUTDOWN
#else
#define HESHUTDOWN              110
#define HESHUTDOWN_DEFINED      1
#endif
#ifdef ECONNREFUSED
#define HECONNREFUSED           ECONNREFUSED
#else
#define HECONNREFUSED           111
#define HECONNREFUSED_DEFINED   1
#endif
#ifdef EADDRINUSE
#define HEADDRINUSE             EADDRINUSE
#else
#define HEADDRINUSE             112
#define HEADDRINUSE_DEFINED     1
#endif
#ifdef ECONNABORTED
#define HECONNABORTED           ECONNABORTED
#else
#define HECONNABORTED           113
#define HECONNABORTED_DEFINED   1
#endif
#ifdef ENETUNREACH
#define HENETUNREACH            ENETUNREACH
#else
#define HENETUNREACH            114
#define HENETUNREACH_DEFINED    1
#endif
#ifdef ENETDOWN
#define HENETDOWN               ENETDOWN
#else
#define HENETDOWN               115
#define HENETDOWN_DEFINED       1
#endif
#ifdef ETIMEDOUT
#define HETIMEDOUT              ETIMEDOUT
#else
#define HETIMEDOUT              116
#define HETIMEDOUT_DEFINED      1
#endif
#ifdef EHOSTDOWN
#define HEHOSTDOWN              EHOSTDOWN
#else
#define HEHOSTDOWN              117
#define HEHOSTDOWN_DEFINED      1
#endif
#ifdef EHOSTUNREACH
#define HEHOSTUNREACH           EHOSTUNREACH
#else
#define HEHOSTUNREACH           118
#define HEHOSTUNREACH_DEFINED   1
#endif
#ifdef EINPROGRESS
#define HEINPROGRESS            EINPROGRESS
#else
#define HEINPROGRESS            119
#define HEINPROGRESS_DEFINED    1
#endif
#ifdef EALREADY
#define HEALREADY               EALREADY
#else
#define HEALREADY               120
#define HEALREADY_DEFINED       1
#endif
#ifdef EDESTADDRREQ
#define HEDESTADDRREQ           EDESTADDRREQ
#else
#define HEDESTADDRREQ           121
#define HEDESTADDRREQ_DEFINED   1
#endif
#ifdef EMSGSIZE
#define HEMSGSIZE               EMSGSIZE
#else
#define HEMSGSIZE               122
#define HEMSGSIZE_DEFINED       1
#endif
#ifdef EPROTONOSUPPORT
#define HEPROTONOSUPPORT        EPROTONOSUPPORT
#else
#define HEPROTONOSUPPORT        123
#define HEPROTONOSUPPORT_DEFINED1
#endif
#ifdef ESOCKTNOSUPPORT
#define HESOCKTNOSUPPORT        ESOCKTNOSUPPORT
#else
#define HESOCKTNOSUPPORT        124
#define HESOCKTNOSUPPORT_DEFINED1
#endif
#ifdef EADDRNOTAVAIL
#define HEADDRNOTAVAIL          EADDRNOTAVAIL
#else
#define HEADDRNOTAVAIL          125
#define HEADDRNOTAVAIL_DEFINED  1
#endif
#ifdef ENETRESET
#define HENETRESET              ENETRESET
#else
#define HENETRESET              126
#define HENETRESET_DEFINED      1
#endif
#ifdef EISCONN
#define HEISCONN                EISCONN
#else
#define HEISCONN                127
#define HEISCONN_DEFINED        1
#endif
#ifdef ENOTCONN
#define HENOTCONN               ENOTCONN
#else
#define HENOTCONN               128
#define HENOTCONN_DEFINED       1
#endif
#ifdef ETOOMANYREFS
#define HETOOMANYREFS           ETOOMANYREFS
#else
#define HETOOMANYREFS           129
#define HETOOMANYREFS_DEFINED   1
#endif
#ifdef EPROCLIM
#define HEPROCLIM               EPROCLIM
#else
#define HEPROCLIM               130
#define HEPROCLIM_DEFINED       1
#endif
#ifdef EUSERS
#define HEUSERS                 EUSERS
#else
#define HEUSERS                 131
#define HEUSERS_DEFINED         1
#endif
#ifdef EDQUOT
#define HEDQUOT                 EDQUOT
#else
#define HEDQUOT                 132
#define HEDQUOT_DEFINED         1
#endif
#ifdef ESTALE
#define HESTALE                 ESTALE
#else
#define HESTALE                 133
#define HESTALE_DEFINED         1
#endif
#ifdef ENOTSUP
#define HENOTSUP                ENOTSUP
#else
#define HENOTSUP                134
#define HENOTSUP_DEFINED        1
#endif
#ifdef ENOMEDIUM
#define HENOMEDIUM              ENOMEDIUM
#else
#define HENOMEDIUM              135
#define HENOMEDIUM_DEFINED      1
#endif
#ifdef EILSEQ
#define HEILSEQ                 EILSEQ
#else
#define HEILSEQ                 138
#define HEILSEQ_DEFINED         1
#endif
#ifdef EOVERFLOW
#define HEOVERFLOW              EOVERFLOW
#else
#define HEOVERFLOW              139
#define HEOVERFLOW_DEFINED      1
#endif
#ifdef ECANCELED
#define HECANCELED              ECANCELED
#else
#define HECANCELED              140
#define HECANCELED_DEFINED      1
#endif
#ifdef ENOTRECOVERABLE
#define HENOTRECOVERABLE        ENOTRECOVERABLE
#else
#define HENOTRECOVERABLE        141
#define HENOTRECOVERABLE_DEFINED1
#endif
#ifdef EOWNERDEAD
#define HEOWNERDEAD             EOWNERDEAD
#else
#define HEOWNERDEAD             142
#define HEOWNERDEAD_DEFINED     1
#endif
#ifdef ESTRPIPE
#define HESTRPIPE               ESTRPIPE
#else
#define HESTRPIPE               143
#define HESTRPIPE_DEFINED       1
#endif
#ifdef EHWPOISON
#define HEHWPOISON              EHWPOISON
#else
#define HEHWPOISON              144
#define HEHWPOISON_DEFINED      1
#endif
#ifdef EISNAM
#define                         HEISNAM EISNAM
#else
#define HEISNAM                 145
#define HEISNAM_DEFINED         1
#endif
#ifdef EKEYEXPIRED
#define HEKEYEXPIRED            EKEYEXPIRED
#else
#define HEKEYEXPIRED            146
#define HEKEYEXPIRED_DEFINED    1
#endif
#ifdef EKEYREJECTED
#define HEKEYREJECTED           EKEYREJECTED
#else
#define HEKEYREJECTED           147
#define HEKEYREJECTED_DEFINED   1
#endif
#ifdef EKEYREVOKED
#define HEKEYREVOKED            EKEYREVOKED
#else
#define HEKEYREVOKED            148
#define HEKEYREVOKED_DEFINED    1
#endif


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HERRNO_H__
