/*
 * Arm SCP/MCP Software
 * Copyright (c) 2022, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "clock_soc.h"
#include "tc2_dvfs.h"
#include "tc2_psu.h"
#include "tc2_timer.h"

#include <mod_dvfs.h>
#include <mod_scmi_perf.h>

#include <fwk_element.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

static struct mod_dvfs_opp operating_points_hayes[6] = {
    {
        .level = 768 * 1000000UL,
        .frequency = 768 * FWK_KHZ,
        .voltage = 550,
    },
    {
        .level = 1153 * 1000000UL,
        .frequency = 1153 * FWK_KHZ,
        .voltage = 650,
    },
    {
        .level = 1537 * 1000000UL,
        .frequency = 1537 * FWK_KHZ,
        .voltage = 750,
    },
    {
        .level = 1844 * 1000000UL,
        .frequency = 1844 * FWK_KHZ,
        .voltage = 850,
    },
    {
        .level = 2152 * 1000000UL,
        .frequency = 2152 * FWK_KHZ,
        .voltage = 950,
    },
    { 0 }
};

static struct mod_dvfs_opp operating_points_hunter[6] = {
    {
        .level = 946 * 1000000UL,
        .frequency = 946 * FWK_KHZ,
        .voltage = 550,
    },
    {
        .level = 1419 * 1000000UL,
        .frequency = 1419 * FWK_KHZ,
        .voltage = 650,
    },
    {
        .level = 1893 * 1000000UL,
        .frequency = 1893 * FWK_KHZ,
        .voltage = 750,
    },
    {
        .level = 2271 * 1000000UL,
        .frequency = 2271 * FWK_KHZ,
        .voltage = 850,
    },
    {
        .level = 2650 * 1000000UL,
        .frequency = 2650 * FWK_KHZ,
        .voltage = 950,
    },
    { 0 }
};

static const struct mod_dvfs_domain_config cpu_group_hayes = {
    .psu_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_PSU, PSU_ELEMENT_IDX_HAYES),
    .clock_id =
        FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_CLOCK, CLOCK_IDX_CPU_GROUP_HAYES),
    .alarm_id = FWK_ID_SUB_ELEMENT_INIT(
        FWK_MODULE_IDX_TIMER,
        0,
        TC2_CONFIG_TIMER_DVFS_CPU_HAYES),
    .retry_ms = 1,
    .latency = 1200,
    .sustained_idx = 2,
    .opps = operating_points_hayes,
};

static const struct mod_dvfs_domain_config cpu_group_hunter = {
    .psu_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_PSU, PSU_ELEMENT_IDX_HUNTER),
    .clock_id =
        FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_CLOCK, CLOCK_IDX_CPU_GROUP_HUNTER),
    .alarm_id = FWK_ID_SUB_ELEMENT_INIT(
        FWK_MODULE_IDX_TIMER,
        0,
        TC2_CONFIG_TIMER_DVFS_CPU_HUNTER),
    .retry_ms = 1,
    .latency = 1200,
    .sustained_idx = 2,
    .opps = operating_points_hunter,
};

static const struct fwk_element element_table[DVFS_ELEMENT_IDX_COUNT + 1] = {
    [DVFS_ELEMENT_IDX_HAYES] =
        {
            .name = "CPU_GROUP_HAYES",
            .data = &cpu_group_hayes,
        },
    [DVFS_ELEMENT_IDX_HUNTER] =
        {
            .name = "CPU_GROUP_HUNTER",
            .data = &cpu_group_hunter,
        },
    { 0 },
};

static const struct fwk_element *dvfs_get_element_table(fwk_id_t module_id)
{
    return element_table;
}

const struct fwk_module_config config_dvfs = {
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dvfs_get_element_table),
};
