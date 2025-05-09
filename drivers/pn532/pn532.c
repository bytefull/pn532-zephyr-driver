/*
 * Copyright (c) 2025 Bayrem Gharsellaoui
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT nxp_pn532

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "pn532.h"

LOG_MODULE_REGISTER(pn532, CONFIG_PN532_LOG_LEVEL);

struct pn532_config {
    const struct device *i2c_dev;
};

struct pn532_data {
    uint32_t dummy;
};

static int pn532_get_firmware_version(const struct device *dev, uint32_t *version)
{
    (void *)dev;
    *version = 0xDEADBEEF;
    return 0;
}

static DEVICE_API(pn532, pn532_api) = {
    .get_firmware_version = pn532_get_firmware_version,
};

static int pn532_init(const struct device *dev)
{
    const struct pn532_config *config = dev->config;
    struct pn532_data *data = dev->data;

    if (!device_is_ready(config->i2c_dev)) {
        LOG_ERR("I2C device %s not ready", config->i2c_dev->name);
        return -ENODEV;
    }

    LOG_INF("PN532 initialized on I2C device %s", config->i2c_dev->name);
    return 0;
}

#define PN532_DEFINE(inst)                                         \
    static struct pn532_data data##inst;                           \
                                                                   \
    static const struct pn532_config config_##inst = {             \
        .i2c_dev = DEVICE_DT_GET(DT_INST_BUS(inst)),               \
    };                                                             \
                                                                   \
    DEVICE_DT_INST_DEFINE(inst, pn532_init, NULL,                  \
                          &data_##inst, &config_##inst,            \
                          POST_KERNEL, CONFIG_PN532_INIT_PRIORITY, \
                          &pn532_api);

DT_INST_FOREACH_STATUS_OKAY(PN532_DEFINE)
