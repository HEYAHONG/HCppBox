/***************************************************************
 * Name:      tcp.h
 * Purpose:   声明TCP兼容接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT AND BSD-3-Clause
 **************************************************************/
/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)tcp.h	8.1 (Berkeley) 6/10/93
 */

#ifndef __HALIBC_POSIX_SOCKET_NETINET_TCP_H__
#define __HALIBC_POSIX_SOCKET_NETINET_TCP_H__

#ifndef _NETINET_TCP_H
#define _NETINET_TCP_H 1
#endif

#ifndef HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS
#define HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS 0
#endif

#if HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS

#include "stdint.h"
#include "endian.h"
#include "sys/socket.h"

/*
 * User-settable options (used with setsockopt).
 */
#define	TCP_NODELAY		 1  /* Don't delay send to coalesce packets  */
#define	TCP_MAXSEG		 2  /* Set maximum segment size  */
#define TCP_CORK		 3  /* Control sending of partial frames  */
#define TCP_KEEPIDLE		 4  /* Start keeplives after this period */
#define TCP_KEEPINTVL		 5  /* Interval between keepalives */
#define TCP_KEEPCNT		 6  /* Number of keepalives before death */
#define TCP_SYNCNT		 7  /* Number of SYN retransmits */
#define TCP_LINGER2		 8  /* Life time of orphaned FIN-WAIT-2 state */
#define TCP_DEFER_ACCEPT	 9  /* Wake up listener only when data arrive */
#define TCP_WINDOW_CLAMP	 10 /* Bound advertised window */
#define TCP_INFO		 11 /* Information about this connection. */
#define	TCP_QUICKACK		 12 /* Bock/re-enable quick ACKs.  */
#define TCP_CONGESTION		 13 /* Congestion control algorithm.  */
#define TCP_MD5SIG		 14 /* TCP MD5 Signature (RFC2385) */
#define TCP_COOKIE_TRANSACTIONS	 15 /* TCP Cookie Transactions */
#define TCP_THIN_LINEAR_TIMEOUTS 16 /* Use linear timeouts for thin streams*/
#define TCP_THIN_DUPACK		 17 /* Fast retrans. after 1 dupack */
#define TCP_USER_TIMEOUT	 18 /* How long for loss retry before timeout */
#define TCP_REPAIR		 19 /* TCP sock is under repair right now */
#define TCP_REPAIR_QUEUE	 20 /* Set TCP queue to repair */
#define TCP_QUEUE_SEQ		 21 /* Set sequence number of repaired queue. */
#define TCP_REPAIR_OPTIONS	 22 /* Repair TCP connection options */
#define TCP_FASTOPEN		 23 /* Enable FastOpen on listeners */
#define TCP_TIMESTAMP		 24 /* TCP time stamp */
#define TCP_NOTSENT_LOWAT	 25 /* Limit number of unsent bytes in
				       write queue.  */
#define TCP_CC_INFO		 26 /* Get Congestion Control
				       (optional) info.  */
#define TCP_SAVE_SYN		 27 /* Record SYN headers for new
				       connections.  */
#define TCP_SAVED_SYN		 28 /* Get SYN headers recorded for
				       connection.  */
#define TCP_REPAIR_WINDOW	 29 /* Get/set window parameters.  */
#define TCP_FASTOPEN_CONNECT	 30 /* Attempt FastOpen with connect.  */
#define TCP_ULP			 31 /* Attach a ULP to a TCP connection.  */
#define TCP_MD5SIG_EXT		 32 /* TCP MD5 Signature with extensions.  */
#define TCP_FASTOPEN_KEY	 33 /* Set the key for Fast Open (cookie).  */
#define TCP_FASTOPEN_NO_COOKIE	 34 /* Enable TFO without a TFO cookie.  */
#define TCP_ZEROCOPY_RECEIVE	 35
#define TCP_INQ			 36 /* Notify bytes available to read
				       as a cmsg on read.  */
#define TCP_CM_INQ		 TCP_INQ
#define TCP_TX_DELAY		 37 /* Delay outgoing packets by XX usec.  */

#define TCP_REPAIR_ON		 1
#define TCP_REPAIR_OFF		 0
#define TCP_REPAIR_OFF_NO_WP	 -1

typedef	uint32_t tcp_seq;

/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
struct tcphdr
{
    __extension__ union
    {
	struct
	{
	    uint16_t th_sport;	/* source port */
	    uint16_t th_dport;	/* destination port */
	    tcp_seq th_seq;	/* sequence number */
	    tcp_seq th_ack;	/* acknowledgement number */
#if _BYTE_ORDER == _LITTLE_ENDIAN
	    uint8_t th_x2:4;	/* (unused) */
	    uint8_t th_off:4;	/* data offset */
#endif
#if _BYTE_ORDER == _BIG_ENDIAN
	    uint8_t th_off:4;	/* data offset */
	    uint8_t th_x2:4;	/* (unused) */
#endif
	    uint8_t th_flags;
#define TH_FIN	0x01
#define TH_SYN	0x02
#define TH_RST	0x04
#define TH_PUSH	0x08
#define TH_ACK	0x10
#define TH_URG	0x20
	    uint16_t th_win;	/* window */
	    uint16_t th_sum;	/* checksum */
	    uint16_t th_urp;	/* urgent pointer */
	};
	struct
	{
	    uint16_t source;
	    uint16_t dest;
	    uint32_t seq;
	    uint32_t ack_seq;
#if _BYTE_ORDER == _LITTLE_ENDIAN
	    uint16_t res1:4;
	    uint16_t doff:4;
	    uint16_t fin:1;
	    uint16_t syn:1;
	    uint16_t rst:1;
	    uint16_t psh:1;
	    uint16_t ack:1;
	    uint16_t urg:1;
	    uint16_t res2:2;
#elif _BYTE_ORDER == _BIG_ENDIAN
	    uint16_t doff:4;
	    uint16_t res1:4;
	    uint16_t res2:2;
	    uint16_t urg:1;
	    uint16_t ack:1;
	    uint16_t psh:1;
	    uint16_t rst:1;
	    uint16_t syn:1;
	    uint16_t fin:1;
#else
#error "Adjust your <endian.h> defines"
#endif
	    uint16_t window;
	    uint16_t check;
	    uint16_t urg_ptr;
	};
    };
};

enum
{
    TCP_ESTABLISHED = 1,
    TCP_SYN_SENT,
    TCP_SYN_RECV,
    TCP_FIN_WAIT1,
    TCP_FIN_WAIT2,
    TCP_TIME_WAIT,
    TCP_CLOSE,
    TCP_CLOSE_WAIT,
    TCP_LAST_ACK,
    TCP_LISTEN,
    TCP_CLOSING
};

#define TCPOPT_EOL		0
#define TCPOPT_NOP		1
#define TCPOPT_MAXSEG		2
#define TCPOLEN_MAXSEG		4
#define TCPOPT_WINDOW		3
#define TCPOLEN_WINDOW		3
#define TCPOPT_SACK_PERMITTED	4
#define TCPOLEN_SACK_PERMITTED	2
#define TCPOPT_SACK		5
#define TCPOPT_TIMESTAMP	8
#define TCPOLEN_TIMESTAMP	10
#define TCPOLEN_TSTAMP_APPA	(TCPOLEN_TIMESTAMP+2)

#define TCPOPT_TSTAMP_HDR	\
    (TCPOPT_NOP<<24|TCPOPT_NOP<<16|TCPOPT_TIMESTAMP<<8|TCPOLEN_TIMESTAMP)

#define TCP_MSS	512

#define TCP_MAXWIN	65535

#define TCP_MAX_WINSHIFT	14

#ifndef SOL_TCP
#define SOL_TCP		6	/* TCP level */
#endif

#define TCPI_OPT_TIMESTAMPS	1
#define TCPI_OPT_SACK		2
#define TCPI_OPT_WSCALE	4
#define TCPI_OPT_ECN		8
#define TCPI_OPT_ECN_SEEN	16
#define TCPI_OPT_SYN_DATA	32
#define TCPI_OPT_USEC_TS	64
#define TCPI_OPT_TFO_CHILD	128

/* Values for tcpi_state.  */
enum tcp_ca_state
{
    TCP_CA_Open = 0,
    TCP_CA_Disorder = 1,
    TCP_CA_CWR = 2,
    TCP_CA_Recovery = 3,
    TCP_CA_Loss = 4
};

struct tcp_info
{
    uint8_t	tcpi_state;
    uint8_t	tcpi_ca_state;
    uint8_t	tcpi_retransmits;
    uint8_t	tcpi_probes;
    uint8_t	tcpi_backoff;
    uint8_t	tcpi_options;
    uint8_t	tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4;

    uint32_t	tcpi_rto;
    uint32_t	tcpi_ato;
    uint32_t	tcpi_snd_mss;
    uint32_t	tcpi_rcv_mss;

    uint32_t	tcpi_unacked;
    uint32_t	tcpi_sacked;
    uint32_t	tcpi_lost;
    uint32_t	tcpi_retrans;
    uint32_t	tcpi_fackets;

    /* Times. */
    uint32_t	tcpi_last_data_sent;
    uint32_t	tcpi_last_ack_sent;
    uint32_t	tcpi_last_data_recv;
    uint32_t	tcpi_last_ack_recv;

    /* Metrics. */
    uint32_t	tcpi_pmtu;
    uint32_t	tcpi_rcv_ssthresh;
    uint32_t	tcpi_rtt;
    uint32_t	tcpi_rttvar;
    uint32_t	tcpi_snd_ssthresh;
    uint32_t	tcpi_snd_cwnd;
    uint32_t	tcpi_advmss;
    uint32_t	tcpi_reordering;

    uint32_t	tcpi_rcv_rtt;
    uint32_t	tcpi_rcv_space;

    uint32_t	tcpi_total_retrans;

    uint64_t	tcpi_pacing_rate;
    uint64_t	tcpi_max_pacing_rate;
    uint64_t	tcpi_bytes_acked;
    uint64_t	tcpi_bytes_received;
    uint32_t	tcpi_segs_out;
    uint32_t	tcpi_segs_in;

    uint32_t	tcpi_notsent_bytes;
    uint32_t	tcpi_min_rtt;
    uint32_t	tcpi_data_segs_in;
    uint32_t	tcpi_data_segs_out;

    uint64_t	tcpi_delivery_rate;

    uint64_t	tcpi_busy_time;
    uint64_t	tcpi_rwnd_limited;
    uint64_t	tcpi_sndbuf_limited;

    uint32_t	tcpi_delivered;
    uint32_t	tcpi_delivered_ce;

    uint64_t	tcpi_bytes_sent;
    uint64_t	tcpi_bytes_retrans;
    uint32_t	tcpi_dsack_dups;
    uint32_t	tcpi_reord_seen;

    uint32_t	tcpi_rcv_ooopack;
    uint32_t	tcpi_snd_wnd;
    uint32_t	tcpi_rcv_wnd;

    uint32_t	tcpi_rehash;
    uint16_t	tcpi_total_rto;
    uint16_t	tcpi_total_rto_recoveries;
    uint32_t	tcpi_total_rto_time;
    uint32_t	tcpi_received_ce;
    uint32_t	tcpi_delivered_e1_bytes;
    uint32_t	tcpi_delivered_e0_bytes;
    uint32_t	tcpi_delivered_ce_bytes;
    uint32_t	tcpi_received_e1_bytes;
    uint32_t	tcpi_received_e0_bytes;
    uint32_t	tcpi_received_ce_bytes;
    uint16_t	tcpi_accecn_fail_mode;
    uint16_t	tcpi_accecn_opt_seen;
};

/* Netlink attributes types for SCM_TIMESTAMPING_OPT_STATS */
enum
{
    TCP_NLA_PAD,
    TCP_NLA_BUSY,
    TCP_NLA_RWND_LIMITED,
    TCP_NLA_SNDBUF_LIMITED,
    TCP_NLA_DATA_SEGS_OUT,
    TCP_NLA_TOTAL_RETRANS,
    TCP_NLA_PACING_RATE,
    TCP_NLA_DELIVERY_RATE,
    TCP_NLA_SND_CWND,
    TCP_NLA_REORDERING,
    TCP_NLA_MIN_RTT,
    TCP_NLA_RECUR_RETRANS,
    TCP_NLA_DELIVERY_RATE_APP_LMT,
    TCP_NLA_SNDQ_SIZE,
    TCP_NLA_CA_STATE,
    TCP_NLA_SND_SSTHRESH,
    TCP_NLA_DELIVERED,
    TCP_NLA_DELIVERED_CE,
    TCP_NLA_BYTES_SENT,
    TCP_NLA_BYTES_RETRANS,
    TCP_NLA_DSACK_DUPS,
    TCP_NLA_REORD_SEEN,
    TCP_NLA_SRTT,
    TCP_NLA_TIMEOUT_REHASH,
    TCP_NLA_BYTES_NOTSENT,
    TCP_NLA_EDT,
    TCP_NLA_TTL,
    TCP_NLA_REHASH,
};

/* For TCP_MD5SIG socket option.  */
#define TCP_MD5SIG_MAXKEYLEN	80

/* tcp_md5sig extension flags for TCP_MD5SIG_EXT.  */
#define TCP_MD5SIG_FLAG_PREFIX	1
#define TCP_MD5SIG_FLAG_IFINDEX	2

struct tcp_md5sig
{
    struct sockaddr_storage tcpm_addr;		/* Address associated.  */
    uint8_t	tcpm_flags;			/* Extension flags.  */
    uint8_t	tcpm_prefixlen;			/* Address prefix.  */
    uint16_t	tcpm_keylen;			/* Key length.  */
    int		tcpm_ifindex;			/* Device index for scope.  */
    uint8_t	tcpm_key[TCP_MD5SIG_MAXKEYLEN];	/* Key (binary).  */
};

/* INET_DIAG_MD5SIG */
struct tcp_diag_md5sig
{
    uint8_t   tcpm_family;
    uint8_t   tcpm_prefixlen;
    uint16_t  tcpm_keylen;
    uint32_t  tcpm_addr[4];
    uint8_t   tcpm_key[TCP_MD5SIG_MAXKEYLEN];
};

/* For socket repair options.  */
struct tcp_repair_opt
{
    uint32_t	opt_code;
    uint32_t	opt_val;
};

/* Queue to repair, for TCP_REPAIR_QUEUE.  */
enum
{
    TCP_NO_QUEUE,
    TCP_RECV_QUEUE,
    TCP_SEND_QUEUE,
    TCP_QUEUES_NR,
};

/* For cookie transactions socket options.  */
#define TCP_COOKIE_MIN		8
#define TCP_COOKIE_MAX		16
#define TCP_COOKIE_PAIR_SIZE	(2*TCP_COOKIE_MAX)

#define TCP_COOKIE_IN_ALWAYS	(1 << 0)
#define TCP_COOKIE_OUT_NEVER	(1 << 1)

#define TCP_S_DATA_IN		(1 << 2)
#define TCP_S_DATA_OUT		(1 << 3)

#define TCP_MSS_DEFAULT		 536U
#define TCP_MSS_DESIRED		1220U

struct tcp_cookie_transactions
{
    uint16_t	tcpct_flags;
    uint8_t	__tcpct_pad1;
    uint8_t	tcpct_cookie_desired;
    uint16_t	tcpct_s_data_desired;
    uint16_t	tcpct_used;
    uint8_t	tcpct_value[TCP_MSS_DEFAULT];
};

/* For use with TCP_REPAIR_WINDOW.  */
struct tcp_repair_window
{
    uint32_t snd_wl1;
    uint32_t snd_wnd;
    uint32_t max_window;
    uint32_t rcv_wnd;
    uint32_t rcv_wup;
};

/* For use with TCP_ZEROCOPY_RECEIVE.  */
struct tcp_zerocopy_receive
{
    uint64_t address;
    uint32_t length;
    uint32_t recv_skip_hint;
    uint32_t inq;
    int32_t err;
    uint64_t copybuf_address;
    int32_t copybuf_len;
    uint32_t flags;
    uint64_t msg_control;
    uint64_t msg_controllen;
    uint32_t msg_flags;
    uint32_t reserved;
};

#endif /* HALIBC_POSIX_SOCKET_ENABLE_EXTENSIONS */

#endif /* __HALIBC_POSIX_SOCKET_NETINET_TCP_H__ */
