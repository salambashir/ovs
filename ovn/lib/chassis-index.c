/* Copyright (c) 2016, 2017 Red Hat, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <config.h>
#include "ovn/lib/chassis-index.h"
#include "ovn/lib/ovn-sb-idl.h"
#include "ovn/lib/ovn-nb-idl.h"

struct ovsdb_idl_index *
chassis_index_create(struct ovsdb_idl *idl)
{
    return ovsdb_idl_index_create1(idl, &nbrec_sb_chassis_col_name);
}

/* Finds and returns the chassis with the given 'name', or NULL if no such
 * chassis exists. */
const struct nbrec_sb_chassis *
chassis_lookup_by_name(struct ovsdb_idl_index *nbrec_sb_chassis_by_name,
                       const char *name)
{
    struct nbrec_sb_chassis *target = nbrec_sb_chassis_index_init_row(
        nbrec_sb_chassis_by_name);
    nbrec_sb_chassis_set_name(target, name);

    struct nbrec_sb_chassis *retval = nbrec_sb_chassis_index_find(
        nbrec_sb_chassis_by_name, target);

    nbrec_sb_chassis_index_destroy_row(target);

    return retval;
}

//Salam - all function
const struct nbrec_sb_chassis *
nb_chassis_lookup_by_name(struct ovsdb_idl_index *nbrec_sb_chassis_by_name,
                       const char *name)
{
    struct nbrec_sb_chassis *target = nbrec_sb_chassis_index_init_row(
        nbrec_sb_chassis_by_name);
    nbrec_sb_chassis_set_name(target, name);

    struct nbrec_sb_chassis *retval = nbrec_sb_chassis_index_find(
        nbrec_sb_chassis_by_name, target);

    nbrec_sb_chassis_index_destroy_row(target);

    return retval;
}