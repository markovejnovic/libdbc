#include "libdbc.h"
#include <stdlib.h>
#include <string.h>
#include "__libdbc.h"

#define VECTOR_INIT_SIZE (5U)
#define MIN(x, y) ((x < y) ? (x) : (y))
#define MAX(x, y) ((x > y) ? (x) : (y))

typedef struct vector {
    void** elems;
    size_t len;
    size_t max_len;
} vector;

void vector_push(vector* vec, void* elem) {
    if (vec->len == vec->max_len) {
        const size_t new_max_len = (size_t)(vec->max_len * 1.5F);
        vec->elems = (void**)realloc(vec->elems, new_max_len * sizeof(void**));
        vec->max_len = new_max_len;
    }

    vec->elems[vec->len] = elem;
    vec->len++;
}

/**
 * @warning This function might cause the vector to not be contiguous and will
 *          cause it to require NULL checks when traversing/searching.
 */
void vector_insert(vector* vec, const size_t idx, void* elem) {
    if (idx + 1 >= vec->max_len - 1) {
        const size_t new_max_len = idx + 1;
        vec->elems = (void**)realloc(vec->elems, new_max_len * sizeof(void**));
        vec->max_len = new_max_len;
    }

    vec->len = MAX(idx + 1, vec->len);
    vec->elems[idx] = elem;
}

void vector_init(vector* vec) {
    vec->elems = (void**)malloc(VECTOR_INIT_SIZE * sizeof(void**));
    vec->len = 0;
    vec->max_len = VECTOR_INIT_SIZE;
}

void* vector_get(vector* vec, size_t idx) {
    return vec->elems[idx];
}

size_t vector_sz(vector* vec) {
    return vec->len;
}

void vector_free(vector* vec, void (*cleaner)(void*)) {
    if (cleaner != NULL) {
        for (size_t i = 0; i < vector_sz(vec); i++) {
            cleaner(vector_get(vec, i));
        }
    }
    free(vec->elems);
}

#define UTF8_MAX_CHAR_WIDTH_BYTES (4U)

// Fits "v10.10.100\0" nicely
#define DBC_DEFAULT_VERSION_INIT_SIZE (UTF8_MAX_CHAR_WIDTH_BYTES * 11U)
#define DBC_DEFAULT_NODE_NAME_INIT_SIZE (UTF8_MAX_CHAR_WIDTH_BYTES * 52U)
#define DBC_DEFAULT_MESSAGE_NAME_INIT_SIZE (UTF8_MAX_CHAR_WIDTH_BYTES * 52U)

typedef struct dbc_str {
    char* bytes;
    size_t len;
    size_t max_len;
} dbc_str;

typedef dbc_str* dbc_str_t;

dbc_str_t dbc_str_new(const size_t len) {
    const size_t max_sz = len * sizeof(char);
    dbc_str_t str = (dbc_str_t)malloc(sizeof(dbc_str) + max_sz);
    str->max_len = max_sz;
    str->len = 0;
    // Sue me
    str->bytes = (char*)str + sizeof(dbc_str);
    return str;
}

dbc_str dbc_str_from_size(const size_t len) {
    const size_t l_bytes = len * sizeof(char);
    return (dbc_str){
        .bytes = (char*)malloc(l_bytes),
        .len = 0,
        .max_len = l_bytes
    };
}

void dbc_str_set(dbc_str_t str, const char* new) {
    // TODO: Not super safe, also maybe slow
    const size_t new_len = strlen(new) + 1;
    // TODO: Should increase by a factor for possibility of future. Too lazy
    // now.
    if (new_len > str->max_len) {
        str->bytes = realloc(str->bytes, new_len);
        str->max_len = new_len;
    }
    str->len = new_len - 1;
    strncpy(str->bytes, new, new_len);
}

void dbc_str_release(dbc_str str) {
    free(str.bytes);
}

void dbc_str_free(dbc_str_t str) {
    // Don't need to clear bytes since the malloc that allocated a dbc_str_t
    // allocated it all in one chunk.
    free(str);
}

struct dbc_node {
    dbc_str name;
};

dbc_node_t dbc_node_new(const char* name) {
    dbc_node_t dbc_node = (dbc_node_t)malloc(sizeof(struct dbc_node));
    dbc_node->name = dbc_str_from_size(DBC_DEFAULT_NODE_NAME_INIT_SIZE);
    dbc_node_set_name(dbc_node, name);

    return dbc_node;
}

void dbc_node_free(const dbc_node_t dbc_node) {
    dbc_str_release(dbc_node->name);
    free(dbc_node);
}

const char* dbc_node_get_name(const dbc_node_t dbc_node) {
    return dbc_node->name.bytes;
}

void dbc_node_set_name(dbc_node_t dbc_node, const char* const name) {
    dbc_str_set(&dbc_node->name, name);
}

struct dbc_value_table {
    dbc_str name;
    size_t num_values;
    vector values;
};

dbc_value_table_t dbc_value_table_new(const char* name) {
    dbc_value_table_t tbl =
        (dbc_value_table_t)calloc(1, sizeof(dbc_value_table_t));
    tbl->name = dbc_str_from_size(strlen(name) + 1);
    dbc_str_set(&tbl->name, name);
    vector_init(&tbl->values);
    return tbl;
}

void dbc_value_table_free(const dbc_value_table_t tbl) {
    vector_free(&tbl->values, dbc_str_free);
    dbc_str_release(tbl->name);
}

const char* dbc_value_table_get_name(const dbc_value_table_t tbl) {
    return tbl->name.bytes;
}

void dbc_value_table_set_name(dbc_value_table_t tbl, const char* name) {
    dbc_str_set(&tbl->name, name);
}

void dbc_value_table_insert(dbc_value_table_t tbl, const size_t num,
                            const char* desc) {
    const dbc_str_t key = dbc_str_new(strlen(desc));
    dbc_str_set(key, desc);
    vector_insert(&tbl->values, num, key);
}

size_t dbc_value_table_get_num_values(dbc_value_table_t tbl) {
    return vector_sz(&tbl->values);
}

const char* dbc_value_table_get_value(dbc_value_table_t tbl, const double val) {
    return ((dbc_str_t)vector_get(&tbl->values, (size_t)val))->bytes;
}

struct dbc_message {
    unsigned int can_id;
    dbc_str name;
    size_t size;
    dbc_node_t transmitter;
    // TODO: Missing signal list
};

dbc_message_t dbc_message_new(dbc_node_t node) {
    dbc_message_t dbc_message = (dbc_message_t)calloc(
        1,
        sizeof(struct dbc_message)
    );
    dbc_message->name = dbc_str_from_size(DBC_DEFAULT_MESSAGE_NAME_INIT_SIZE);
    dbc_message->transmitter = node;

    return dbc_message;
}

void dbc_message_free(const dbc_message_t dbc_message) {
    dbc_str_release(dbc_message->name);
    dbc_node_free(dbc_message->transmitter);
    free(dbc_message);
}

struct dbc {
    dbc_str version;
    // TODO: Missing new symbols, useless?
    // TODO: Missing Bit Timing, useless?
    vector nodes;
    vector value_tables;
    size_t num_value_tables; // Required since vector isn't contg. TODO
    // TODO: Missing Message Defs
    // TODO: Missing Env Variables
    // TODO: Missing Signal Types
    // TODO: Missing Comments
};

dbc_t dbc_new() {
    dbc_t dbc = (dbc_t)calloc(1, sizeof(struct dbc));
    dbc->version = dbc_str_from_size(DBC_DEFAULT_VERSION_INIT_SIZE);
    vector_init(&dbc->nodes);
    vector_init(&dbc->value_tables);
    dbc->num_value_tables = 0;

    return dbc;
}

void dbc_free(const dbc_t dbc) {
    dbc_str_release(dbc->version);
    vector_free(&dbc->nodes, NULL);
    vector_free(&dbc->value_tables, dbc_value_table_free);
    free(dbc);
}

const char* dbc_get_version(const dbc_t dbc) {
    return dbc->version.bytes;
}

void dbc_set_version(dbc_t dbc, const char* const ver) {
    dbc_str_set(&dbc->version, ver);
}

void dbc_push_node(dbc_t dbc, dbc_node_t node) {
    vector_push(&dbc->nodes, node);
}

dbc_node_t dbc_get_node(const dbc_t dbc, const size_t idx) {
    return vector_get(&dbc->nodes, idx);
}

size_t dbc_get_num_nodes(const dbc_t dbc) {
    return vector_sz(&dbc->nodes);
}

dbc_value_table_t dbc_get_value_table(dbc_t dbc, const char* name) {
    // TODO: Crappy implementation, should use hashmap internally. TME
    const size_t num_tables = vector_sz(&dbc->value_tables);
    for (size_t i = 0; i < num_tables; i++) {
        const dbc_value_table_t tbl =
            (dbc_value_table_t)vector_get(&dbc->value_tables, i);
        if (strcmp(dbc_value_table_get_name(tbl), name) == 0) {
            return tbl;
        }
    }

    return NULL;
}

dbc_value_table_t dbc_add_value_table(dbc_t dbc, const char* name) {
    dbc_value_table_t tbl = dbc_value_table_new(name);
    vector_push(&dbc->value_tables, tbl);
    dbc->num_value_tables++;
    return tbl;
}

size_t dbc_get_num_value_tables(const dbc_t dbc) {
    return dbc->num_value_tables;
}
