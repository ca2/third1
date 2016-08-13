#pragma once


#ifndef HAVE_ARPA_NAMESER_COMPAT_H
#include <netinet/in.h>

/* Android NDK is missing some of these headers */

#define ns_c_in      1
#define ns_t_srv     33 
#define C_IN         ns_c_in
#define T_SRV        ns_t_srv
#define QFIXEDSZ    NS_QFIXEDSZ
#define NS_QFIXEDSZ 4
#define GETSHORT        NS_GET16
#define NS_INT32SZ    4
#define NS_INT16SZ    2


typedef struct {
    unsigned    id :16;     /*%< query identification number */
#if BYTE_ORDER == BIG_ENDIAN
            /* fields in third byte */
    unsigned    qr: 1;      /*%< response flag */
    unsigned    opcode: 4;  /*%< purpose of message */
    unsigned    aa: 1;      /*%< authoritive answer */
    unsigned    tc: 1;      /*%< truncated message */
    unsigned    rd: 1;      /*%< recursion desired */
            /* fields in fourth byte */
    unsigned    ra: 1;      /*%< recursion available */
    unsigned    unused :1;  /*%< unused bits (MBZ as of 4.9.3a3) */
    unsigned    ad: 1;      /*%< authentic data from named */
    unsigned    cd: 1;      /*%< checking disabled by resolver */
    unsigned    rcode :4;   /*%< response code */
#endif
#if BYTE_ORDER == LITTLE_ENDIAN || BYTE_ORDER == PDP_ENDIAN
            /* fields in third byte */
    unsigned    rd :1;      /*%< recursion desired */
    unsigned    tc :1;      /*%< truncated message */
    unsigned    aa :1;      /*%< authoritive answer */
    unsigned    opcode :4;  /*%< purpose of message */
    unsigned    qr :1;      /*%< response flag */
            /* fields in fourth byte */
    unsigned    rcode :4;   /*%< response code */
    unsigned    cd: 1;      /*%< checking disabled by resolver */
    unsigned    ad: 1;      /*%< authentic data from named */
    unsigned    unused :1;  /*%< unused bits (MBZ as of 4.9.3a3) */
    unsigned    ra :1;      /*%< recursion available */
#endif
            /* remaining bytes */
    unsigned    qdcount :16;    /*%< number of question entries */
    unsigned    ancount :16;    /*%< number of answer entries */
    unsigned    nscount :16;    /*%< number of authority entries */
    unsigned    arcount :16;    /*%< number of resource entries */
} HEADER;

int dn_expand (const unsigned char * srv, const unsigned char * end, const unsigned char * pos, char * name, const unsigned int n);
int res_init (void);
int res_query(const char *dname, int class, int type, unsigned char *answer, int anslen); 

#define NS_GET16(s, cp) do { \
    register const u_char *t_cp = (const u_char *)(cp); \
    (s) = ((u_int16_t)t_cp[0] << 8) \
        | ((u_int16_t)t_cp[1]) \
        ; \
    (cp) += NS_INT16SZ; \
} while (0)

#endif