#include "libdbc.h"
#include "__libdbc.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define PARSE_VERSION_DEFAULT ("")

typedef enum
{
    PARSE_ERR_SUCCESS,
    PARSE_ERR_MALFORMED,
    PARSE_ERR_CRITICAL
} parse_err_t;

static bool __dbc_valid_cexpr(const char* str, const size_t len) {
    if (!(str[0] == '_' || isalpha(str[0]))) {
        return false;
    }

    for (size_t i = 1; i < len; i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }

    return true;
}

static bool __dbc_valid_char_str(const char* str, const size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '"') {
            return false;
        }
    }

    return true;
}

static parse_err_t __dbc_parse_version(dbc_t dbc, char* str) {
    // ['VERSION' '"' { CANdb_version_string } '"' ]
    const char* const first_quote = strchr(str, '"');

    const char* const content_begin = first_quote + 1;
    // Empty version string if <VERSION "">
    if (unlikely(*content_begin == '"'))
    {
        dbc_set_version(dbc, "");
        return PARSE_ERR_SUCCESS;
    }

    // Malformed if <VERSION ">
    // TODO: Maybe too expensive to call strlen all the time
    const size_t len_remaining = strlen(str) - (content_begin - str);
    char* content_end = strchr(content_begin, '"');
    if (unlikely(content_end == NULL))
    {
        dbc_set_version(dbc, PARSE_VERSION_DEFAULT);
        return PARSE_ERR_MALFORMED;
    }

    *content_end = 0;
    dbc_set_version(dbc, content_begin);
    return PARSE_ERR_SUCCESS;
}

static bool maybe_str_to_double(double* out, const char* str) {
    char* tail;
    const double conv = strtod(str, &tail);
    if (conv == 0.0 && tail == str) {
        return false;
    }
    *out = conv;
    return true;
}

/**
 * @brief Parses the nodes out of a node line.
 *
 * @note This function, even when returning PARSE_ERR_MALFORMED will attempt to
 *       parse the node list. You are, as an adult, responsible for handling
 *       the case of failure and possibly informing your user.
 *
 * @return parse_err_t PARSE_ERR_MALFORMED if one of the nodes does not tightly
 *                     adhere to the spec, ie. if it is not a "c-expr",
 *                     PARSE_ERR_SUCCESS otherwise.
 */
static parse_err_t __dbc_parse_nodes(dbc_t dbc, char* str) {
    // 'BU_:' {node_name}
    parse_err_t success = PARSE_ERR_SUCCESS;

    char* strtok_r_ptr;
    char* tok = strtok_r(str, " ", &strtok_r_ptr); // BU_, ignore.
    while ((tok = strtok_r(NULL, " ", &strtok_r_ptr)) != NULL) {
        // tok is a node name.
        const dbc_node_t node = dbc_node_new(tok);
        if (unlikely(!__dbc_valid_cexpr(tok, strlen(tok)))) {
            success = PARSE_ERR_MALFORMED;
        }
        dbc_push_node(dbc, node);
    }

    return success;
}

static parse_err_t __dbc_parse_value_table(dbc_t dbc, char* str) {
    // 'VAL_TABLE_' value_table_name {value_description} ';' ;
    // value_table_name = C_identifier ;
    // value_description = double char_string ;
    parse_err_t success = PARSE_ERR_SUCCESS;

    char* strtok_r_ptr;
    char* tok = strtok_r(str, " ", &strtok_r_ptr); // VAL_TABLE_, ignore.
    // The name comes first
    const char* const vt_name = strtok_r(NULL, " ", &strtok_r_ptr);
    if (strcmp(vt_name, ";") == 0) {
        // If we find a ';' character, then we did not find any value tables.
        // This should be ignored and we should not inject anything into the
        // dbc.
        return PARSE_ERR_MALFORMED;
    }

    // We should ensure the name is a C_identifier, but if it is not, let's
    // continue.
    success = __dbc_valid_char_str(vt_name, strlen(vt_name))
            ? PARSE_ERR_SUCCESS
            : PARSE_ERR_MALFORMED;

    // Otherwise, we may now safely create a vtable.
    dbc_value_table_t tbl = dbc_add_value_table(dbc, vt_name);
    do {
        tok = strtok_r(NULL, " ", &strtok_r_ptr);
        // If we find a null, character, we failed, because we're, eventually
        // supposed to exit when we find ';'
        if (unlikely(tok == NULL)) {
            return PARSE_ERR_CRITICAL;
        }

        if (tok[0] == ';') {
            return success;
        }

        // Alright, this should be the value, let's see if we can cast to
        // double.
        double value;
        if (unlikely(!maybe_str_to_double(&value, tok))) {
            // If we can't cast it to double, something is really bad, let's
            // fail.
            return PARSE_ERR_CRITICAL;
        }

        // If this passes, we got our value, time to look for the key.
        tok = strtok_r(NULL, " ", &strtok_r_ptr);
        // If the token is null, but we have a value, there's nothing we can
        // do. If it's a semicolon, then we found a value, but no semicolon.
        // There's no recovery from that, chief. Let's just give up.
        if (unlikely(tok == NULL || tok[0] == ';')) {
            return PARSE_ERR_CRITICAL;
        }

        // If these pass, this is a key.
        const char* key = tok;
        const size_t key_len = strlen(tok);
        if (unlikely(key[0] != '"' || key[key_len - 1] != '"')) {
            // The key must be contained within two "", if failing, mark error,
            // but we'll try our best to continue.
            success = PARSE_ERR_MALFORMED;
        }
        // If the key is not a valid char string, then this is a failure, but
        // let's continue pretending like it is all fine.
        success = __dbc_valid_char_str(key, key_len - 1)
                ? success
                : PARSE_ERR_MALFORMED;

        // Alright, key looks pretty good, let's now put both of these into our
        // value table!
        dbc_value_table_insert(tbl, value, key);
    } while (true);

    return success;
}
