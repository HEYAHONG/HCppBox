/*
rtl8169.h - Realtek RTL8169 NIC
Copyright (C) 2022  LekKit <github.com/LekKit>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

/*
 * TODO: Remove this header in favor of <rvvm/rvvm_board.h>
 * TODO: Replace "tap_api.h" with <rvvm/rvvm_net.h>
 */

#ifndef RVVM_RTL8169_H
#define RVVM_RTL8169_H

#include "tap_api.h"
#include <rvvm/rvvm_pci.h>

RVVM_PUBLIC rvvm_pci_func_t* rvvm_rtl8169_init(rvvm_machine_t* machine, tap_dev_t* tap, rvvm_pci_addr_t addr);

static inline rvvm_pci_func_t* rtl8169_init_auto(rvvm_machine_t* machine)
{
    tap_dev_t* tap = tap_open();
    if (tap) {
        return rvvm_rtl8169_init(machine, tap, RVVM_PCI_ADDR_ANY);
    }
    return NULL;
}

#endif
