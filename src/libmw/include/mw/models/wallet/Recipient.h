/*
 *  Project: XThereon (XTH)
 *  Source modified by: XTH Labs
 *  Copyright © 2025
 *  Licensed under the MIT License.
 *  ---------------------------------------------------------------
 *  NOTE: This header block is automatically maintained.
 */

#pragma once

#include <mw/common/Macros.h>
#include <mw/models/wallet/StealthAddress.h>
#include <amount.h>

MW_NAMESPACE

struct Recipient {
    CAmount amount;
    StealthAddress address;
};

END_NAMESPACE
