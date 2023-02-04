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

#ifndef __DBC_NODE__
#define __DBC_NODE__

/**
 * @typedef dbc_node_t
 * @brief A DBC Node Definition
 *
 * This data structure encapsulates all the information available regarding a specific node.
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

#endif
