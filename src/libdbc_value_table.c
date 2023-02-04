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

#include "libdbc_value_table.h"
#include "hashtable.h"
#include "hashtable_itr.h"
#include "sds.h"

typedef struct hashtable* hashtable_t;
typedef struct hashtable_itr* hashtable_itr_t;

static unsigned int key_hash(void* key) {
    return (unsigned int)(*(double*)key);
}

static int keys_eq(void* k1, void* k2) { return *(double*)k1 == *(double*)k2; }

struct dbc_value_table {
    sds name;
    hashtable_t val_to_desc;
};

dbc_value_table_t dbc_value_table_new(const char* name) {
    dbc_value_table_t vt =
        (dbc_value_table_t)malloc(sizeof(dbc_value_table_t));
    vt->name = sdsnew(name);

    vt->val_to_desc = create_hashtable(4, key_hash, keys_eq);

    return vt;
}

void dbc_value_table_free(const dbc_value_table_t vt) {
    sdsfree(vt->name);

    if (hashtable_count(vt->val_to_desc) != 0) {
        // Need to go and clear all those values manually. Let's use the
        // iterator, for this.
        hashtable_itr_t iter = hashtable_iterator(vt->val_to_desc);
        sds val;
        while ((val = hashtable_iterator_value(iter)) != NULL) {
            sdsfree(val);
            if (hashtable_iterator_advance(iter) == 0) {
                break;
            }
        }
    }

    hashtable_destroy(vt->val_to_desc, false);
    free(vt);
}

const char* dbc_value_table_get_name(const dbc_value_table_t vt) {
    return vt->name;
}

bool dbc_value_table_insert(dbc_value_table_t vt, double num,
                            const char* desc) {
    double* m_num = (double*)malloc(sizeof(num));
    *m_num = num;
    sds m_desc = sdsnew(desc);
    return hashtable_insert(vt->val_to_desc, m_num, m_desc);
}

size_t dbc_value_table_get_size(dbc_value_table_t vt) {
    return hashtable_count(vt->val_to_desc);
}

const char* dbc_value_table_get_desc(dbc_value_table_t vt, const double val) {
    return hashtable_search(vt->val_to_desc, (void*)&val);
}
