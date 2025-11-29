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

#define ThrowFile(msg) throw FileException(msg, __FUNCTION__)
#define ThrowFile_F(msg, ...) throw FileException(StringUtil::Format(msg, __VA_ARGS__), __FUNCTION__)

class FileException : public LTCException
{
public:
    FileException(const std::string& message, const std::string& function)
        : LTCException("FileException", message, function)
    {

    }
};
