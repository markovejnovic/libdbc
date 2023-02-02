#ifndef __LIBDBC__
#define __LIBDBC__

#include <stdlib.h>

/**
 * @typedef dbc_t
 * @brief The DBC file RAM representation.
 */
typedef struct dbc* dbc_t;

/**
 * @typedef dbc_node_t
 * @brief A DBC Node Definition
 */
typedef struct dbc_node* dbc_node_t;

/**
 * @brief Creates a new, empty DBC Node structure.
 */
dbc_node_t dbc_node_new(const char* name);

/**
 * @brief Frees up the memory used by the DBC.
 */
void dbc_node_free(const dbc_node_t node);

/**
 * @return char* The name of the node
 */
const char* dbc_node_get_name(const dbc_node_t dbc_node);

/**
 * @brief Sets the DBC node name
 */
void dbc_node_set_name(dbc_node_t dbc_node, const char* const name);

/**
 * @typedef dbc_value_table_t
 * @brief The DBC Value Table Definition
 */
typedef struct dbc_value_table* dbc_value_table_t;

dbc_value_table_t dbc_value_table_new(const char*);
void dbc_value_table_free(const dbc_value_table_t);
void dbc_value_table_set_name(dbc_value_table_t, const char*);
const char* dbc_value_table_get_name(dbc_value_table_t);

/**
 * @brief Adds a new value pair.
 *
 * @param tbl The DBC Value Table
 * @param num The value number
 * @param desc The description of the value
 */
void dbc_value_table_insert(dbc_value_table_t tbl, const size_t num,
                            const char* desc);

size_t dbc_value_table_get_num_values(dbc_value_table_t val);
const char* dbc_value_table_get_value(dbc_value_table_t tbl, const double val);

/**
 * @typedef dbc_message_t
 * @brief A DBC Message Definition
 */
typedef struct dbc_message* dbc_message_t;

dbc_message_t dbc_message_new(dbc_node_t node);

void dbc_message_free(const dbc_message_t dbc_message);

/**
 * @brief Creates a new, empty DBC structure.
 */
dbc_t dbc_new();

/**
 * @brief Frees up the memory used by the DBC.
 */
void dbc_free(const dbc_t);

/**
 * @brief Returns the version of the DBC file.
 */
const char* dbc_get_version(const dbc_t);

/**
 * @brief Sets the version in the DBC.
 * @param ver The version string
 * @param len The length of the string.
 */
void dbc_set_version(dbc_t, const char* const ver);

/**
 * @brief Adds a new node to the DBC file.
 */
void dbc_push_node(dbc_t, dbc_node_t);

/**
 * @brief Returns the node at the specified index.
 */
dbc_node_t dbc_get_node(const dbc_t, const size_t);

/**
 * @brief Returns the number of registered nodes.
 */
size_t dbc_get_num_nodes(const dbc_t);

/**
 * @brief Returns the value table
 */
dbc_value_table_t dbc_get_value_table(const dbc_t, const char*);

/**
 * @brief Returns the number of value tables
 */
size_t dbc_get_num_value_tables(const dbc_t);

/**
 * @brief Creates a new, empty value table with the given name, returning it
 * @note This function has undefined behavior is the given value table already exists.
 */
dbc_value_table_t dbc_add_value_table(dbc_t, const char*);

#endif
