/*
 *  Project: XThereon (XTH)
 *  Source modified by: XTH Labs
 *  Copyright © 2025
 *  Licensed under the MIT License.
 *  ---------------------------------------------------------------
 *  NOTE: This header block is automatically maintained.
 */

// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <hash.h> // for signet block challenge hash
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>
#include <arith_uint256.h>
#include <assert.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits,
                                 int32_t nVersion, const CAmount& genesisReward);
 

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;

        //
        // Consensus & PoW settings
        // (keep these as your coin’s parameters)
        //
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();

        consensus.nSubsidyHalvingInterval = 840000; // keep or adjust for XTH

        // For a brand-new chain, clear all historical BIP activation heights.
        // You can later set these to 1 or some height once you know what you want.
        consensus.BIP16Height            = 0;
        consensus.BIP34Height            = 0;
        consensus.BIP34Hash              = uint256();  // will set to genesis later if you want
        consensus.BIP65Height            = 0;
        consensus.BIP66Height            = 0;
        consensus.CSVHeight              = 0;
        consensus.SegwitHeight           = 0;
        consensus.MinBIP9WarningHeight   = 0;

        // Keep your PoW parameters (you can tweak if you want a different difficulty schedule)
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing  = 2.5 * 60;           // 2.5 minutes

        consensus.fPowAllowMinDifficultyBlocks = false;
        // Allow mining on demand (for custom XTH blockchain)
	m_fMineBlocksOnDemand = true;

        consensus.fPowNoRetargeting            = false;

        consensus.nRuleChangeActivationThreshold = 6048; // 75% of 8064
        consensus.nMinerConfirmationWindow       = 8064; // nPowTargetTimespan / nPowTargetSpacing * 4

        //
        // BIP9 deployments – disable everything for a fresh chain
        //
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit         = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime  = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout    = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartHeight    = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeoutHeight  = 0;

        // Taproot disabled for now (you can configure a proper deployment later)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit           = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartHeight  = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeoutHeight= 0;

        // MWEB disabled for now
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].bit              = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nStartHeight     = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nTimeoutHeight   = 0;

        //
        // No historical chain work or assume-valid:
        //
        consensus.nMinimumChainWork = uint256S("0x0");
        consensus.defaultAssumeValid = uint256S("0x0");

        //
        // Network magic / ports (you can change these if you want your own)
        //
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xdb;

        nDefaultPort = 9333;          // change if you want a unique port
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 40;
        m_assumed_chain_state_size = 2;

        //
        // GENESIS BLOCK
        //
        // For now, use some starting values. You’ll probably want to mine a proper genesis
        // and then hard-code the nonce/time and the expected hashes.
        //
        const CAmount genesisReward = 50 * COIN;
        // === TEMP GENESIS MINING LOOP ===
	// This will run ONCE, then you remove it after you get your values.

	genesis = CreateGenesisBlock(
	    1690000000,     // nTime
	    0,              // nNonce
	    0x207fffff,     // nBits
	    1,              // nVersion
	    genesisReward   // 50 * COIN
	);

	consensus.hashGenesisBlock = genesis.GetHash();

	// Optional: you can now comment this LogPrintf out if you like
	// LogPrintf("XTH MAINNET GENESIS: hash=%s merkle=%s\n",
	//           consensus.hashGenesisBlock.ToString(),
	//           genesis.hashMerkleRoot.ToString());

	assert(consensus.hashGenesisBlock ==
	       uint256S("0x3f57d5e7416b34656e74d2b78ba867296ea0c7c104dc31942918402fc0bf8116"));

	assert(genesis.hashMerkleRoot ==
	       uint256S("0x241c3682f1bc6d54ba37618f7f59edfafbd78d8868387ccfd554d15941ecbc5f"));
			
	// Once you mine your genesis, set these and uncomment:
        // DNS seeds & fixed seeds – empty for a brand new coin
        //
	vSeeds.clear();
	vFixedSeeds.clear();

	vSeeds.emplace_back("seed.xthereon.com");  // Primary bootstrap node
	//vSeeds.emplace_back("seed2.xthereon.com");  // Secondary fallback seed

        //
        // XTH address prefixes (keep as you had them)
        //
        base58Prefixes[PUBKEY_ADDRESS]  = std::vector<unsigned char>(1, 88); // 'X' for XTH
        base58Prefixes[SCRIPT_ADDRESS]  = std::vector<unsigned char>(1, 50); // e.g. 'M'
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1, 50);
        base58Prefixes[SECRET_KEY]      = std::vector<unsigned char>(1, 176); // WIF

        base58Prefixes[EXT_PUBLIC_KEY]  = {0x04, 0x88, 0xB2, 0x1E}; // xpub
        base58Prefixes[EXT_SECRET_KEY]  = {0x04, 0x88, 0xAD, 0xE4}; // xprv

        bech32_hrp = "xth";         // bech32 HRP for mainnet
        mweb_hrp   = "xthmweb";     // if/when you use MWEB

        fDefaultConsistencyChecks = false;
        fRequireStandard          = true;
        m_is_test_chain           = false;
        m_is_mockable_chain       = false;

        //
        // Checkpoints – none for a brand new coin
        //
        checkpointData = {
            {
                // empty
            }
        };

        //
        // ChainTxData – no history yet
        //
        chainTxData = ChainTxData{
            /* nTime    */ 0,
            /* nTxCount */ 0,
            /* dTxRate  */ 0.0
        };
    }
};

/**
 * Testnet
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;

        //
        // Consensus
        //
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();

        // Halving / economic parameters
        consensus.nSubsidyHalvingInterval = 840000; // or match mainnet if you changed it

        // BIP activations – new chain, start everything at 0
        consensus.BIP16Height          = 0;
        consensus.BIP34Height          = 0;
        consensus.BIP34Hash            = uint256();
        consensus.BIP65Height          = 0;
        consensus.BIP66Height          = 0;
        consensus.CSVHeight            = 0;
        consensus.SegwitHeight         = 0;
        consensus.MinBIP9WarningHeight = 0;

        // PoW – for testnet we make it very easy and allow min-difficulty blocks
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = int64_t(3.5 * 24 * 60 * 60); // doesn't matter much on testnet
        consensus.nPowTargetSpacing  = int64_t(2.5 * 60);           // 2.5 min blocks (you can change to 5 min if you like)

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting            = false;

        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow       = 2016;

        // Disable all deployments for now (you can re-enable later)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit             = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime      = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout        = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartHeight    = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeoutHeight  = 0;

        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit               = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartHeight      = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeoutHeight    = 0;

        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].bit                  = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nStartHeight         = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nTimeoutHeight       = 0;

        // No historical work / assume-valid for a brand new testnet
        consensus.nMinimumChainWork  = uint256();
        consensus.defaultAssumeValid = uint256();

        //
        // Network
        //
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xd2;
        pchMessageStart[2] = 0xc8;
        pchMessageStart[3] = 0xf1;   // you can change these if you want, but this is fine

        nDefaultPort = 19335;        // testnet P2P port
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;
	// ----------------- FINAL XTH TESTNET GENESIS -----------------
	genesis = CreateGenesisBlock(
    1732400000,     // DIFFERENT timestamp
    0,
    0x207fffff,
    1,
    50 * COIN
);

	consensus.hashGenesisBlock = genesis.GetHash();
	assert(genesis.GetHash() ==
	       uint256S("0x124b14a9e92add6a79f81c04788287606d413f2e54db31788ca008d1f493e900"));

	assert(genesis.hashMerkleRoot ==
	       uint256S("0x241c3682f1bc6d54ba37618f7f59edfafbd78d8868387ccfd554d15941ecbc5f"));

        // After mining, we’ll replace these with asserts:
        // assert(consensus.hashGenesisBlock == uint256S("0x..."));
        // assert(genesis.hashMerkleRoot == uint256S("0x..."));

        //
        // Seeds – empty for now, you can add real testnet seeds later
        //
        vFixedSeeds.clear();
        vSeeds.clear();

        //
        // Prefixes – keep standard testnet style
        //
        base58Prefixes[PUBKEY_ADDRESS]  = std::vector<unsigned char>(1,111); // 'm' or 'n' style
        base58Prefixes[SCRIPT_ADDRESS]  = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY]      = std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY]  = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY]  = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "txth";  // changed from tltc → txth for your coin
        mweb_hrp   = "txthmweb";

        fDefaultConsistencyChecks = false;
        fRequireStandard          = false;
        m_is_test_chain           = true;
        m_is_mockable_chain       = false;

        // No checkpoints / stats yet
        checkpointData = {
            {
                // empty
            }
        };

        chainTxData = ChainTxData{
            0, // nTime
            0, // nTxCount
            0  // dTxRate
        };
    }
};


    // Correct signature for updating version bits params (replace the clipped one)
    void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout, int nStartHeight, int nTimeoutHeight)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
        consensus.vDeployments[d].nStartHeight = nStartHeight;
        consensus.vDeployments[d].nTimeoutHeight = nTimeoutHeight;
    }

void CChainParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (args.IsArgSet("-segwitheight")) {
        int64_t height = args.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }

    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() < 3 || 5 < vDeploymentParams.size()) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end[:heightstart:heightend]");
        }
        int64_t nStartTime, nTimeout, nStartHeight, nTimeoutHeight;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        if (vDeploymentParams.size() > 3 && !ParseInt64(vDeploymentParams[3], &nStartHeight)) {
            throw std::runtime_error(strprintf("Invalid nStartHeight (%s)", vDeploymentParams[3]));
        }
        if (vDeploymentParams.size() > 4 && !ParseInt64(vDeploymentParams[4], &nTimeoutHeight)) {
            throw std::runtime_error(strprintf("Invalid nTimeoutHeight (%s)", vDeploymentParams[4]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout, nStartHeight, nTimeoutHeight);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld, start_height=%d, timeout_height=%d\n", vDeploymentParams[0], nStartTime, nTimeout, nStartHeight, nTimeoutHeight);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN) {
        return std::unique_ptr<CChainParams>(new CMainParams());
    } else if (chain == CBaseChainParams::TESTNET) {
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    } else if (chain == CBaseChainParams::SIGNET) {
        return std::unique_ptr<CChainParams>(new CTestNetParams()); // TODO: Support SigNet
    } else if (chain == CBaseChainParams::REGTEST) {
        // TEMP: reuse testnet params for regtest until CRegTestParams is reintroduced
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    }
 throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(gArgs, network);
}

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript,
                                 uint32_t nTime, uint32_t nNonce, uint32_t nBits,
                                 int32_t nVersion, const CAmount& genesisReward) {
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4)
        << std::vector<unsigned char>((const unsigned char*)pszTimestamp,
                                      (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits,
                                 int32_t nVersion, const CAmount& genesisReward) {
    const char* pszTimestamp = "XThereon begins 18 Nov 2025";
    const CScript genesisOutputScript = CScript() << ParseHex("04ffff001d010445") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}
