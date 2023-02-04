#include "libdbc.h"
#include <stdlib.h>
#include <string.h>
#include "sds.h"
#include "__libdbc.h"

#define UTF8_MAX_CHAR_WIDTH_BYTES (4U)

struct dbc {
    sds version;
    // TODO: Missing new symbols, useless?
    // TODO: Missing Bit Timing, useless?
    //vector nodes;
    //vector value_tables;
    // TODO: Missing Message Defs
    // TODO: Missing Env Variables
    // TODO: Missing Signal Types
    // TODO: Missing Comments
};

dbc_t dbc_new() {
    dbc_t dbc = (dbc_t)malloc(sizeof(struct dbc));
    dbc->version = sdsempty();

    return dbc;
}

void dbc_free(const dbc_t dbc) {
    sdsfree(dbc->version);
    free(dbc);
}

const char* dbc_get_version(const dbc_t dbc) {
    return dbc->version;
}

void dbc_set_version(dbc_t dbc, const char* const ver) {
    dbc->version = sdscpy(dbc->version, ver);
}

void dbc_push_node(dbc_t dbc, dbc_node_t node) {
    (void)dbc; (void)node; // TODO: Implement
}

dbc_node_t dbc_get_node(const dbc_t dbc, const size_t idx) {
    (void)dbc; (void)idx; // TODO: Implement
    return NULL;
}

size_t dbc_get_num_nodes(const dbc_t dbc) {
    (void)dbc; // TODO: Implemnet
    return 0;
}

dbc_value_table_t dbc_get_value_table(dbc_t dbc, const char* name) {
    (void)dbc; (void)name; // TODO
    return NULL;
}

dbc_value_table_t dbc_add_value_table(dbc_t dbc, const char* name) {
    (void)dbc; (void)name; // TODO
    return NULL;
}

size_t dbc_get_num_value_tables(const dbc_t dbc) {
    (void)dbc; // TODO
    return 0;
}
