/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --output-file=../../../src/credential-helper/lmd_connections.c ../../../src/credential-helper/lmd_connections.gpf  */
/* Computed positions: -k'2' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "../../../src/credential-helper/lmd_connections.gpf"

#include "lmd_connections.h"

#include <string.h>
#include <errno.h>

#include "lmd_google_connections.h"
#include "lmd_github_connections.h"
#include "lmd_ms_connections.h"

/**
 * name service callback
 */
typedef struct _service_entry service_entry;


static service_entry*
in_word_set(
    const char* name,
    size_t length);

/**
 * set additional parameter and load discovery document
 */
static int
lmd_google_load_discovery_document(
    lmd* obj);

/**
 * set additional parameter and load discovery document
 */
static int
lmd_ms_load_discovery_document(
    lmd* obj);
#line 41 "../../../src/credential-helper/lmd_connections.gpf"
struct _service_entry {
    /**
     * name
     */
    const char* name;
    /**
     * fill endpoint urls for oauth token 
     */
    int (*set_endpoint_urls)(lmd*);
};

#define TOTAL_KEYWORDS 3
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 9
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 10
/* maximum key range = 5, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11,  1, 11, 11, 11, 11,
      11,  0, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11
    };
  return len + asso_values[(unsigned char)str[1]];
}

struct _service_entry *
in_word_set (register const char *str, register size_t len)
{
  static unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  6,  6,  0,  0,  9
    };
  static struct _service_entry wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""},
#line 52 "../../../src/credential-helper/lmd_connections.gpf"
      {"google",lmd_google_connections_load_discovery_document},
#line 54 "../../../src/credential-helper/lmd_connections.gpf"
      {"github",lmd_github_connections_load_oauth_endpoint_urls},
      {""}, {""},
#line 53 "../../../src/credential-helper/lmd_connections.gpf"
      {"microsoft",lmd_ms_load_discovery_document}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
#line 55 "../../../src/credential-helper/lmd_connections.gpf"

/**
 * load discovery document
 */
int
lmd_connections_load_discovery_document(
    lmd* obj,
    const char* token_service)
{
    int result;
    result = 0;
    if (obj && token_service) {
        service_entry* entry;
        entry = in_word_set(token_service, strlen(token_service));
        if (entry) {
            result = entry->set_endpoint_urls(obj);
        } else {
            result = -1;
            errno = EINVAL;
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set additional parameter and load discovery document
 */
static int
lmd_ms_load_discovery_document(
    lmd* obj)
{
    int result;
    result = 0;

    if (obj) {
        const char* tenant;
        tenant = lmd_get_ms_tenant_ref(obj);
        if (!tenant) {
            result = lmd_set_ms_tenant(obj, "common");
        }
        if (result == 0) {
            result = lmd_ms_connections_load_discovery_document(obj);
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}

/**
 * set additional parameter and load discovery document
 */
static int
lmd_google_load_discovery_document(
    lmd* obj)
{
    int result;
    result = 0;

    if (obj) {
        const char* client_secret;
        client_secret = lmd_get_client_secret_ref(obj);
        result = client_secret ? 0 : -1;
        if (result == 0) {
            result = lmd_google_connections_load_discovery_document(obj);
        }
    } else {
        result = -1;
        errno = EINVAL;
    }
    return result;
}
/* vi: se ts=4 sw=4 et: */
