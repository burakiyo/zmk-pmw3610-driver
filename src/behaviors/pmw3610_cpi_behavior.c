/*
 * Copyright (c) 2022 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <drivers/behavior.h>

#include "../pmw3610.h"

LOG_MODULE_REGISTER(pmw3610_cpi_behavior, CONFIG_ZMK_LOG_LEVEL);

#define DT_DRV_COMPAT zmk_pmw3610_cpi_behavior

struct pmw3610_cpi_behavior_config {
    bool increment;
};

static int pmw3610_cpi_behavior_pressed(struct zmk_behavior_binding *binding,
                                        struct zmk_behavior_binding_event event)
{
    const struct device *dev = device_get_binding("PMW3610");
    if (!dev) {
        LOG_ERR("PMW3610 device not found");
        return -ENODEV;
    }

    const struct device *behavior_dev = device_get_binding(binding->behavior_dev);
    if (!behavior_dev) {
        LOG_ERR("Behavior dev not found: %s", binding->behavior_dev);
        return -ENODEV;
    }
    const struct pmw3610_cpi_behavior_config *cfg = behavior_dev->config;

    if (cfg->increment) {
        increment_cpi(dev);
    } else {
        decrement_cpi(dev);
    }
    return 0;
}

static const struct behavior_driver_api pmw3610_cpi_behavior_driver_api = {
    .binding_pressed  = pmw3610_cpi_behavior_pressed,
    .binding_released = NULL,
};

static int pmw3610_cpi_behavior_init(const struct device *dev)
{
    return 0;
}

#define PMW3610_CPI_BEHAVIOR_CONFIG(inst) { \
    .increment = DT_INST_PROP(inst, increment), \
}

#define PMW3610_CPI_BEHAVIOR_DEVICE(inst)                                 \
    static const struct pmw3610_cpi_behavior_config                       \
        pmw3610_cpi_behavior_cfg_##inst = PMW3610_CPI_BEHAVIOR_CONFIG(inst); \
                                                                          \
    DEVICE_DT_INST_DEFINE(inst,                                          \
                          pmw3610_cpi_behavior_init,                     \
                          NULL,  /* PM device not needed */              \
                          NULL,  /* No runtime data needed */            \
                          &pmw3610_cpi_behavior_cfg_##inst,              \
                          APPLICATION,                                   \
                          CONFIG_APPLICATION_INIT_PRIORITY,              \
                          &pmw3610_cpi_behavior_driver_api);

DT_INST_FOREACH_STATUS_OKAY(PMW3610_CPI_BEHAVIOR_DEVICE)
