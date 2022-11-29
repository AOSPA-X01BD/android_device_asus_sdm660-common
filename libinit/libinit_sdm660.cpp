/*
 * Copyright (C) 2022 Paranoid Android
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdlib>
#include <fstream>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"
#include "property_service.h"

using android::base::GetProperty;
using std::string;

void property_override(string prop, string value)
{
    auto pi = (prop_info*) __system_property_find(prop.c_str());

    if (pi != nullptr)
        __system_property_update(pi, value.c_str(), value.size());
    else
        __system_property_add(prop.c_str(), prop.size(), value.c_str(), value.size());
}

void NFC_check()
{
    // Check NFC
    std::ifstream infile("/proc/NFC_CHECK");
    std::string check;

    getline(infile, check);
    if (!check.compare("SUPPORTED")) {
        property_override("ro.hq.support.nfc", "1");
        property_override("ro.boot.product.hardware.sku", "NFC");
    } else
        property_override("ro.hq.support.nfc", "0");
}

void vendor_load_properties()
{
    NFC_check();

    property_override("persist.vendor.audio.fluence.voicerec", "true");
    property_override("persist.vendor.audio.fluence.speaker", "false");
    property_override("ro.vendor.audio.sdk.fluencetype", "fluence");
    property_override("vendor.audio.feature.compr_voip.enable", "true");
}
