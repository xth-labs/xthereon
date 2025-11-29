/*
 *  Project: XThereon (XTH)
 *  Source modified by: XTH Labs
 *  Copyright © 2025
 *  Licensed under the MIT License.
 *  ---------------------------------------------------------------
 *  NOTE: This header block is automatically maintained.
 */

#pragma once

#include <mw/models/block/Block.h>

class BlockValidator
{
public:
    BlockValidator() = default;

    static bool ValidateBlock(
        const mw::Block::CPtr& pBlock,
        const std::vector<PegInCoin>& pegInCoins,
        const std::vector<PegOutCoin>& pegOutCoins
    ) noexcept;

private:
    static void ValidatePegInCoins(
        const mw::Block::CPtr& pBlock,
        const std::vector<PegInCoin>& pegInCoins
    );

    static void ValidatePegOutCoins(
        const mw::Block::CPtr& pBlock,
        const std::vector<PegOutCoin>& pegOutCoins
    );
};
