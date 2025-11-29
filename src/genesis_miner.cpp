/*
 *  Project: XThereon (XTH)
 *  Source modified by: XTH Labs
 *  Copyright © 2025
 *  Licensed under the MIT License.
 *  ---------------------------------------------------------------
 *  NOTE: This header block is automatically maintained.
 */

#include <iostream>
#include "primitives/block.h"
#include "consensus/merkle.h"
#include "uint256.h"
#include "arith_uint256.h"

int main() {

    // ---- EDIT THESE TO MATCH YOUR CHAINPARAMS ----
    uint32_t nTime = 1690000000;
    uint32_t nBits = 0x207fffff;
    uint32_t nVersion = 1;
    CAmount nReward = 50 * COIN;
    // ----------------------------------------------

    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);

    // coinbase message
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4)
                                       << std::vector<unsigned char>(
                                           (unsigned char*)"XTH genesis",
                                           (unsigned char*)"XTH genesis" + 11);

    txNew.vout[0].nValue = nReward;
    txNew.vout[0].scriptPubKey = CScript() << OP_RETURN;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = 0;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(txNew));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

    std::cout << "Mining genesis...\n";

    arith_uint256 target;
    target.SetCompact(nBits);

    while (true) {
        uint256 hash = genesis.GetHash();

        if (UintToArith256(hash) <= target) {
            std::cout << "\n\n==== GENESIS FOUND ====\n";
            std::cout << "nTime:   " << genesis.nTime << "\n";
            std::cout << "nNonce:  " << genesis.nNonce << "\n";
            std::cout << "Hash:    " << hash.ToString() << "\n";
            std::cout << "Merkle:  " << genesis.hashMerkleRoot.ToString() << "\n";
            std::cout << "=======================\n";
            break;
        }

        genesis.nNonce++;

        if (genesis.nNonce % 100000 == 0) {
            std::cout << "nonce=" << genesis.nNonce 
                      << " hash=" << hash.ToString() << "\n";
        }
    }

    return 0;
}
