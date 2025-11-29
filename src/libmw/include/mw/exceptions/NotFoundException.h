/*
 *  Project: XThereon (XTH)
 *  Source modified by: XTH Labs
 *  Copyright © 2025
 *  Licensed under the MIT License.
 *  ---------------------------------------------------------------
 *  NOTE: This header block is automatically maintained.
 */

#pragma once

#include <mw/exceptions/LTCException.h>
#include <mw/util/StringUtil.h>

#define ThrowNotFound(msg) throw NotFoundException(msg, __FUNCTION__)
#define ThrowNotFound_F(msg, ...) throw NotFoundException(StringUtil::Format(msg, __VA_ARGS__), __FUNCTION__)

class NotFoundException : public LTCException
{
public:
    NotFoundException(const std::string& message, const std::string& function)
        : LTCException("NotFoundException", message, function)
    {

    }
};
