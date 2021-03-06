#ifndef HEADER_CURL_IMAP_H
#define HEADER_CURL_IMAP_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 2009 - 2013, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#include "pingpong.h"

/****************************************************************************
 * IMAP unique setup
 ***************************************************************************/
typedef enum {
  IMAP_STOP,         /* do nothing state, stops the state machine */
  IMAP_SERVERGREET,  /* waiting for the initial greeting immediately after
                        a connect */
  IMAP_CAPABILITY,
  IMAP_STARTTLS,
  IMAP_UPGRADETLS,   /* asynchronously upgrade the connection to SSL/TLS
                       (multi mode only) */
  IMAP_AUTHENTICATE_PLAIN,
  IMAP_AUTHENTICATE_LOGIN,
  IMAP_AUTHENTICATE_LOGIN_PASSWD,
  IMAP_AUTHENTICATE_CRAMMD5,
  IMAP_AUTHENTICATE_DIGESTMD5,
  IMAP_AUTHENTICATE_DIGESTMD5_RESP,
  IMAP_AUTHENTICATE_NTLM,
  IMAP_AUTHENTICATE_NTLM_TYPE2MSG,
  IMAP_AUTHENTICATE,
  IMAP_LOGIN,
  IMAP_SELECT,
  IMAP_FETCH,
  IMAP_LOGOUT,
  IMAP_LAST          /* never used */
} imapstate;

/* This IMAP struct is used in the SessionHandle. All IMAP data that is
   connection-oriented must be in imap_conn to properly deal with the fact that
   perhaps the SessionHandle is changed between the times the connection is
   used. */
struct IMAP {
  curl_off_t *bytecountp;
  curl_pp_transfer transfer;
  char *mailbox;          /* Mailbox to select */
  char *uidvalidity;      /* UIDVALIDITY to check in select */
  char *uid;              /* Message UID to fetch */
  char *section;          /* Message SECTION to fetch */
};

/* imap_conn is used for struct connection-oriented data in the connectdata
   struct */
struct imap_conn {
  struct pingpong pp;
  unsigned int authmechs; /* Accepted authentication mechanisms */
  unsigned int authused;  /* Auth mechanism used for the connection */
  imapstate state;        /* Always use imap.c:state() to change state! */
  int cmdid;              /* Last used command ID */
  char resptag[5];        /* Response tag to wait for */
  bool ssldone;           /* Is connect() over SSL done? */
  bool tls_supported;     /* StartTLS capability supported by server */
  bool login_disabled;    /* LOGIN command explicitly disabled by server */
  bool ir_supported;      /* Initial response supported by server */
};

extern const struct Curl_handler Curl_handler_imap;
extern const struct Curl_handler Curl_handler_imaps;

#endif /* HEADER_CURL_IMAP_H */
