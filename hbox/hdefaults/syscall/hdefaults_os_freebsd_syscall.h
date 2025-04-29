/*
 * System call numbers.
 *
 * DO NOT EDIT-- this file is automatically @generated.
 */

#define	HDEFAULTS_OS_FREEBSD_SYSCALL_syscall	0
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_exit	1
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fork	2
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_read	3
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_write	4
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_open	5
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_close	6
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_wait4	7
/* 8 is old creat */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_link	9
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_unlink	10
/* 11 is obsolete execv */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_chdir	12
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fchdir	13
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_mknod	14
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_chmod	15
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_chown	16
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_break	17
/* 18 is freebsd4 getfsstat */
/* 19 is old lseek */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getpid	20
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mount	21
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_unmount	22
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setuid	23
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getuid	24
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_geteuid	25
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ptrace	26
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_recvmsg	27
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sendmsg	28
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_recvfrom	29
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_accept	30
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getpeername	31
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getsockname	32
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_access	33
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_chflags	34
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fchflags	35
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sync	36
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kill	37
/* 38 is old stat */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getppid	39
/* 40 is old lstat */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_dup	41
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd10_pipe	42
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getegid	43
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_profil	44
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ktrace	45
/* 46 is old sigaction */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getgid	47
/* 48 is old sigprocmask */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getlogin	49
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setlogin	50
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_acct	51
/* 52 is old sigpending */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigaltstack	53
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ioctl	54
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_reboot	55
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_revoke	56
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_symlink	57
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_readlink	58
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_execve	59
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_umask	60
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_chroot	61
/* 62 is old fstat */
/* 63 is old getkerninfo */
/* 64 is old getpagesize */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_msync	65
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_vfork	66
/* 67 is obsolete vread */
/* 68 is obsolete vwrite */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sbrk	69
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sstk	70
/* 71 is old mmap */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_vadvise	72
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_munmap	73
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mprotect	74
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_madvise	75
/* 76 is obsolete vhangup */
/* 77 is obsolete vlimit */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mincore	78
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getgroups	79
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setgroups	80
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getpgrp	81
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setpgid	82
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setitimer	83
/* 84 is old wait */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_swapon	85
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getitimer	86
/* 87 is old gethostname */
/* 88 is old sethostname */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getdtablesize	89
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_dup2	90
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fcntl	92
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_select	93
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fsync	95
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setpriority	96
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_socket	97
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_connect	98
/* 99 is old accept */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getpriority	100
/* 101 is old send */
/* 102 is old recv */
/* 103 is old sigreturn */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_bind	104
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setsockopt	105
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_listen	106
/* 107 is obsolete vtimes */
/* 108 is old sigvec */
/* 109 is old sigblock */
/* 110 is old sigsetmask */
/* 111 is old sigsuspend */
/* 112 is old sigstack */
/* 113 is old recvmsg */
/* 114 is old sendmsg */
/* 115 is obsolete vtrace */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_gettimeofday	116
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getrusage	117
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getsockopt	118
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_readv	120
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_writev	121
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_settimeofday	122
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fchown	123
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fchmod	124
/* 125 is old recvfrom */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setreuid	126
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setregid	127
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rename	128
/* 129 is old truncate */
/* 130 is old ftruncate */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_flock	131
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mkfifo	132
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sendto	133
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_shutdown	134
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_socketpair	135
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mkdir	136
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rmdir	137
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_utimes	138
/* 139 is obsolete sigreturn */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_adjtime	140
/* 141 is old getpeername */
/* 142 is old gethostid */
/* 143 is old sethostid */
/* 144 is old getrlimit */
/* 145 is old setrlimit */
/* 146 is old killpg */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setsid	147
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_quotactl	148
/* 149 is old quota */
/* 150 is old getsockname */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_nlm_syscall	154
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_nfssvc	155
/* 156 is old getdirentries */
/* 157 is freebsd4 statfs */
/* 158 is freebsd4 fstatfs */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_lgetfh	160
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getfh	161
/* 162 is freebsd4 getdomainname */
/* 163 is freebsd4 setdomainname */
/* 164 is freebsd4 uname */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sysarch	165
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rtprio	166
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_semsys	169
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_msgsys	170
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_shmsys	171
/* 173 is freebsd6 pread */
/* 174 is freebsd6 pwrite */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setfib	175
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ntp_adjtime	176
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setgid	181
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setegid	182
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_seteuid	183
/* 184 is obsolete lfs_bmapv */
/* 185 is obsolete lfs_markv */
/* 186 is obsolete lfs_segclean */
/* 187 is obsolete lfs_segwait */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_stat	188
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_fstat	189
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_lstat	190
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_pathconf	191
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fpathconf	192
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getrlimit	194
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setrlimit	195
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_getdirentries	196
/* 197 is freebsd6 mmap */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___syscall	198
/* 199 is freebsd6 lseek */
/* 200 is freebsd6 truncate */
/* 201 is freebsd6 ftruncate */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___sysctl	202
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mlock	203
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_munlock	204
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_undelete	205
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_futimes	206
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getpgid	207
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_poll	209
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd7___semctl	220
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_semget	221
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_semop	222
/* 223 is obsolete semconfig */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd7_msgctl	224
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_msgget	225
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_msgsnd	226
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_msgrcv	227
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_shmat	228
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd7_shmctl	229
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_shmdt	230
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_shmget	231
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_clock_gettime	232
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_clock_settime	233
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_clock_getres	234
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ktimer_create	235
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ktimer_delete	236
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ktimer_settime	237
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ktimer_gettime	238
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ktimer_getoverrun	239
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_nanosleep	240
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ffclock_getcounter	241
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ffclock_setestimate	242
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ffclock_getestimate	243
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_clock_nanosleep	244
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_clock_getcpuclockid2	247
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ntp_gettime	248
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_minherit	250
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rfork	251
/* 252 is obsolete openbsd_poll */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_issetugid	253
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_lchown	254
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_read	255
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_write	256
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_lio_listio	257
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_getdents	272
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_lchmod	274
/* 275 is obsolete netbsd_lchown */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_lutimes	276
/* 277 is obsolete netbsd_msync */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_nstat	278
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_nfstat	279
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_nlstat	280
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_preadv	289
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_pwritev	290
/* 297 is freebsd4 fhstatfs */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fhopen	298
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_fhstat	299
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_modnext	300
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_modstat	301
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_modfnext	302
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_modfind	303
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kldload	304
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kldunload	305
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kldfind	306
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kldnext	307
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kldstat	308
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kldfirstmod	309
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getsid	310
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setresuid	311
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setresgid	312
/* 313 is obsolete signanosleep */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_return	314
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_suspend	315
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_cancel	316
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_error	317
/* 318 is freebsd6 aio_read */
/* 319 is freebsd6 aio_write */
/* 320 is freebsd6 lio_listio */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_yield	321
/* 322 is obsolete thr_sleep */
/* 323 is obsolete thr_wakeup */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mlockall	324
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_munlockall	325
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___getcwd	326
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sched_setparam	327
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sched_getparam	328
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sched_setscheduler	329
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sched_getscheduler	330
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sched_yield	331
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sched_get_priority_max	332
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sched_get_priority_min	333
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sched_rr_get_interval	334
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_utrace	335
/* 336 is freebsd4 sendfile */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kldsym	337
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_jail	338
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_nnpfs_syscall	339
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigprocmask	340
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigsuspend	341
/* 342 is freebsd4 sigaction */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigpending	343
/* 344 is freebsd4 sigreturn */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigtimedwait	345
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigwaitinfo	346
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_get_file	347
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_set_file	348
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_get_fd	349
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_set_fd	350
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_delete_file	351
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_delete_fd	352
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_aclcheck_file	353
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_aclcheck_fd	354
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattrctl	355
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_set_file	356
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_get_file	357
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_delete_file	358
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_waitcomplete	359
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getresuid	360
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getresgid	361
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kqueue	362
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_kevent	363
/* 364 is obsolete __cap_get_proc */
/* 365 is obsolete __cap_set_proc */
/* 366 is obsolete __cap_get_fd */
/* 367 is obsolete __cap_get_file */
/* 368 is obsolete __cap_set_fd */
/* 369 is obsolete __cap_set_file */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_set_fd	371
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_get_fd	372
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_delete_fd	373
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___setugid	374
/* 375 is obsolete nfsclnt */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_eaccess	376
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_afs3_syscall	377
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_nmount	378
/* 379 is obsolete kse_exit */
/* 380 is obsolete kse_wakeup */
/* 381 is obsolete kse_create */
/* 382 is obsolete kse_thr_interrupt */
/* 383 is obsolete kse_release */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_get_proc	384
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_set_proc	385
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_get_fd	386
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_get_file	387
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_set_fd	388
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_set_file	389
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kenv	390
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_lchflags	391
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_uuidgen	392
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sendfile	393
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mac_syscall	394
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_getfsstat	395
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_statfs	396
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_fstatfs	397
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_fhstatfs	398
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_close	400
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_post	401
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_wait	402
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_trywait	403
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_init	404
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_open	405
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_unlink	406
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_getvalue	407
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_destroy	408
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_get_pid	409
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_get_link	410
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_set_link	411
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_set_link	412
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_get_link	413
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_delete_link	414
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___mac_execve	415
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigaction	416
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigreturn	417
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getcontext	421
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setcontext	422
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_swapcontext	423
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd13_swapoff	424
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_get_link	425
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_set_link	426
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_delete_link	427
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___acl_aclcheck_link	428
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigwait	429
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_thr_create	430
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_thr_exit	431
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_thr_self	432
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_thr_kill	433
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd10__umtx_lock	434
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd10__umtx_unlock	435
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_jail_attach	436
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_list_fd	437
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_list_file	438
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_extattr_list_link	439
/* 440 is obsolete kse_switchin */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ksem_timedwait	441
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_thr_suspend	442
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_thr_wake	443
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kldunloadf	444
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_audit	445
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_auditon	446
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getauid	447
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setauid	448
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getaudit	449
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setaudit	450
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getaudit_addr	451
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setaudit_addr	452
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_auditctl	453
#define	HDEFAULTS_OS_FREEBSD_SYSCALL__umtx_op	454
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_thr_new	455
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigqueue	456
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kmq_open	457
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kmq_setattr	458
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kmq_timedreceive	459
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kmq_timedsend	460
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kmq_notify	461
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kmq_unlink	462
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_abort2	463
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_thr_set_name	464
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_fsync	465
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rtprio_thread	466
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sctp_peeloff	471
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sctp_generic_sendmsg	472
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sctp_generic_sendmsg_iov	473
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sctp_generic_recvmsg	474
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_pread	475
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_pwrite	476
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mmap	477
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_lseek	478
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_truncate	479
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ftruncate	480
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_thr_kill2	481
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd12_shm_open	482
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_shm_unlink	483
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cpuset	484
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cpuset_setid	485
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cpuset_getid	486
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cpuset_getaffinity	487
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cpuset_setaffinity	488
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_faccessat	489
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fchmodat	490
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fchownat	491
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fexecve	492
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_fstatat	493
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_futimesat	494
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_linkat	495
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mkdirat	496
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mkfifoat	497
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd11_mknodat	498
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_openat	499
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_readlinkat	500
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_renameat	501
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_symlinkat	502
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_unlinkat	503
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_posix_openpt	504
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_gssd_syscall	505
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_jail_get	506
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_jail_set	507
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_jail_remove	508
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_freebsd12_closefrom	509
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___semctl	510
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_msgctl	511
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_shmctl	512
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_lpathconf	513
/* 514 is obsolete cap_new */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___cap_rights_get	515
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cap_enter	516
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cap_getmode	517
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_pdfork	518
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_pdkill	519
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_pdgetpid	520
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_pselect	522
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getloginclass	523
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_setloginclass	524
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rctl_get_racct	525
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rctl_get_rules	526
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rctl_get_limits	527
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rctl_add_rule	528
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rctl_remove_rule	529
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_posix_fallocate	530
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_posix_fadvise	531
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_wait6	532
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cap_rights_limit	533
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cap_ioctls_limit	534
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cap_ioctls_get	535
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cap_fcntls_limit	536
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cap_fcntls_get	537
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_bindat	538
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_connectat	539
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_chflagsat	540
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_accept4	541
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_pipe2	542
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_mlock	543
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_procctl	544
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_ppoll	545
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_futimens	546
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_utimensat	547
/* 548 is obsolete numa_getaffinity */
/* 549 is obsolete numa_setaffinity */
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fdatasync	550
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fstat	551
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fstatat	552
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fhstat	553
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getdirentries	554
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_statfs	555
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fstatfs	556
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getfsstat	557
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fhstatfs	558
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_mknodat	559
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kevent	560
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cpuset_getdomain	561
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_cpuset_setdomain	562
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getrandom	563
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getfhat	564
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fhlink	565
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fhlinkat	566
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fhreadlink	567
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_funlinkat	568
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_copy_file_range	569
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___sysctlbyname	570
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_shm_open2	571
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_shm_rename	572
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sigfastblock	573
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___realpathat	574
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_close_range	575
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_rpctls_syscall	576
#define	HDEFAULTS_OS_FREEBSD_SYSCALL___specialfd	577
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_writev	578
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_aio_readv	579
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_fspacectl	580
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_sched_getcpu	581
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_swapoff	582
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kqueuex	583
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_membarrier	584
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_timerfd_create	585
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_timerfd_gettime	586
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_timerfd_settime	587
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_kcmp	588
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_getrlimitusage	589
#define	HDEFAULTS_OS_FREEBSD_SYSCALL_MAXSYSCALL	590
