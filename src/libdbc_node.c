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

#include "libdbc_node.h"
#include <stdlib.h>
#include "sds.h"

struct dbc_node {
    sds name;
};

dbc_node_t dbc_node_new(const char* name) {
    dbc_node_t dbc_node = (dbc_node_t)malloc(sizeof(struct dbc_node));
    dbc_node->name = sdsnew(name);

    return dbc_node;
}

void dbc_node_free(const dbc_node_t dbc_node) {
    sdsfree(dbc_node->name);
    free(dbc_node);
}

const char* dbc_node_get_name(const dbc_node_t dbc_node) {
    return dbc_node->name;
}
