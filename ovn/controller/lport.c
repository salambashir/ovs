/* Copyright (c) 2015, 2016 Nicira, Inc.
 *
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

#include "lib/sset.h"
#include "lport.h"
#include "hash.h"
#include "openvswitch/vlog.h"
#include "ovn/lib/ovn-sb-idl.h"
#include "ovn/lib/ovn-nb-idl.h"
VLOG_DEFINE_THIS_MODULE(lport);

const struct nbrec_sb_port_binding *
lport_lookup_by_name(struct ovsdb_idl_index *nbrec_sb_port_binding_by_name,
                     const char *name)
{
    struct nbrec_sb_port_binding *pb = nbrec_sb_port_binding_index_init_row(
        nbrec_sb_port_binding_by_name);
    nbrec_sb_port_binding_index_set_logical_port(pb, name);

    const struct nbrec_sb_port_binding *retval = nbrec_sb_port_binding_index_find(
        nbrec_sb_port_binding_by_name, pb);

    nbrec_sb_port_binding_index_destroy_row(pb);

    return retval;
}

const struct nbrec_sb_port_binding *
lport_lookup_by_key(struct ovsdb_idl_index *nbrec_sb_datapath_binding_by_key,
                    struct ovsdb_idl_index *nbrec_sb_port_binding_by_key,
                    uint64_t dp_key, uint64_t port_key)
{
    /* Lookup datapath corresponding to dp_key. */
    const struct nbrec_sb_datapath_binding *db = datapath_lookup_by_key(
        nbrec_sb_datapath_binding_by_key, dp_key);
    if (!db) {
        return NULL;
    }

    /* Build key for an indexed lookup. */
    struct nbrec_sb_port_binding *pb = nbrec_sb_port_binding_index_init_row(
        nbrec_sb_port_binding_by_key);
    nbrec_sb_port_binding_index_set_datapath(pb, db);
    nbrec_sb_port_binding_index_set_tunnel_key(pb, port_key);

    const struct nbrec_sb_port_binding *retval = nbrec_sb_port_binding_index_find(
        nbrec_sb_port_binding_by_key, pb);

    nbrec_sb_port_binding_index_destroy_row(pb);

    return retval;
}

const struct nbrec_sb_datapath_binding *
datapath_lookup_by_key(struct ovsdb_idl_index *nbrec_sb_datapath_binding_by_key,
                       uint64_t dp_key)
{
    struct nbrec_sb_datapath_binding *db = nbrec_sb_datapath_binding_index_init_row(
        nbrec_sb_datapath_binding_by_key);
    nbrec_sb_datapath_binding_index_set_tunnel_key(db, dp_key);

    const struct nbrec_sb_datapath_binding *retval
        = nbrec_sb_datapath_binding_index_find(nbrec_sb_datapath_binding_by_key,
                                            db);

    nbrec_sb_datapath_binding_index_destroy_row(db);

    return retval;
}

const struct nbrec_sb_multicast_group *
mcgroup_lookup_by_dp_name(
    struct ovsdb_idl_index *nbrec_sb_multicast_group_by_name_datapath,
    const struct nbrec_sb_datapath_binding *db, const char *name)
{
    /* Build key for an indexed lookup. */
    struct nbrec_sb_multicast_group *mc = nbrec_sb_multicast_group_index_init_row(
        nbrec_sb_multicast_group_by_name_datapath);
    nbrec_sb_multicast_group_index_set_name(mc, name);
    nbrec_sb_multicast_group_index_set_datapath(mc, db);

    const struct nbrec_sb_multicast_group *retval
        = nbrec_sb_multicast_group_index_find(
            nbrec_sb_multicast_group_by_name_datapath, mc);

    nbrec_sb_multicast_group_index_destroy_row(mc);

    return retval;
}
