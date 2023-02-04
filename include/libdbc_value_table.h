/**
 * libdbc - The DBC File Library
 * Copyright (C) 2023 Marko Vejnovic <contact@markovejnovic.clm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __LIBDBC_VALUE_TABLE__
#define __LIBDBC_VALUE_TABLE__

#include <stdbool.h>
#include <stdlib.h>

/**
 * @typedef dbc_value_table_t
 * @brief The DBC Value Table.
 *
 * The DBC Value Table is a named bimap containing mappings from enum values
 * (typed as double), to enum names (typed as strings).
 *
 * @note This type is designed to fully encapsulate the lifetime of its
 *       children.
 */
typedef struct dbc_value_table* dbc_value_table_t;

/**
 * @brief Creates a new value table.
 * @param name The name of the table.
 */
dbc_value_table_t dbc_value_table_new(const char* name);

/**
 * @brief Frees the value table and all children members.
 * @param vt The value table to be freed.
 */
void dbc_value_table_free(const dbc_value_table_t vt);

/**
 * @brief Returns the name of the value table.
 * @param vt The value table to get the name for.
 * @return The name of the value table.
 */
const char* dbc_value_table_get_name(dbc_value_table_t vt);

/**
 * @brief Inserts a new key-value pair.
 *
 * @param vt The value table to insert to.
 * @param num The number.
 * @param desc The description for said number.
 *
 * @return true if the value has been successfully inserted
 */
bool dbc_value_table_insert(dbc_value_table_t vt,
                            double num, const char* desc);
/**
 * @brief Returns the number of key-value pairs currently stored.
 * @param vt The target value table.
 * @return The number of elements currently stored.
 */
size_t dbc_value_table_get_size(dbc_value_table_t vt);

/**
 * @brief Returns the description for the given value.
 * @param vt The target value table.
 * @param val The target value.
 * @return The description for the given value, NULL if the value is not found.
 */
const char* dbc_value_table_get_desc(dbc_value_table_t vt, const double val);

#endif
