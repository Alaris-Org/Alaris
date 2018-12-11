// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>
#include <memory>

#include <chainparamsseeds.h>

bool CChainParams::IsHistoricBug(const uint256& txid, unsigned nHeight, BugType& type) const
{
    const std::pair<unsigned, uint256> key(nHeight, txid);
    std::map<std::pair<unsigned, uint256>, BugType>::const_iterator mi;

    mi = mapHistoricBugs.find (key);
    if (mi != mapHistoricBugs.end ())
    {
        type = mi->second;
        return true;
    }

    return false;
}

static CBlock CreateGenesisBlock(const CScript& genesisInputScript, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = genesisInputScript;
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, const char* pszTimestamp="On 10/10/2018 the Alaris Genesis was created")
{
    const CScript genesisInputScript = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(genesisInputScript, genesisOutputScript, nTime, nNonce, nBits, 1, 0);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        /* Note that these are not the actual activation heights, but blocks
           after them.  They are too deep in the chain to be ever reorged,
           and thus this is also fine.  */
        // FIXME: Activate BIP16 with a softfork.
        consensus.BIP16Height = 100;
        consensus.BIP34Height = 100;
        consensus.BIP34Hash = uint256S("0x2ac21fb2a21b1e26e6124dce26d6677f38fb5da561f89d0fd7d961dbdea3e759");
        consensus.BIP65Height = 100;
        consensus.BIP66Height = 100;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 6 * 60 * 60; // 6 hours
        consensus.nPowTargetSpacing = 45;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 456; // 95% of 480
        consensus.nMinerConfirmationWindow = 480; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000006f006f0");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x2ac21fb2a21b1e26e6124dce26d6677f38fb5da561f89d0fd7d961dbdea3e759");

        consensus.nAuxpowChainId = 0x0001;
        consensus.nAuxpowStartHeight = 0;
        consensus.fStrictChainId = true;
        consensus.nLegacyBlocksBefore = -1;

        consensus.rules.reset(new Consensus::MainNetConsensus());

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0xcc;
        pchMessageStart[3] = 0xd4;
        nDefaultPort = 9012;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1542051081, 770702, 0x1e0ffff0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xaf42de56d1f053411aef797a1be732299be260a2df6629c7f298d1ca28da8a70"));
        assert(genesis.hashMerkleRoot == uint256S("0x8301dc42f9464447e2c560d6acdebf205e5f346636680682f7b60de1f6d43838"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("mainnet.alaris.io");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,30);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,25);
        base58Prefixes[SECRET_KEY_EBST] = std::vector<unsigned char>(1, 220);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0xC6, 0x0B, 0xC8};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0xC6, 0x0B, 0xC9};

        bech32_hrp = "al";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {100, uint256S("0x2ac21fb2a21b1e26e6124dce26d6677f38fb5da561f89d0fd7d961dbdea3e759")},
            }
        };

        chainTxData = ChainTxData{
            1542051081, // * UNIX timestamp of last known number of transactions
            0,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0           // * estimated number of transactions per second after checkpoint
        };

        assert(mapHistoricBugs.empty());
    }

    int DefaultCheckNameDB () const
    {
        return -1;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000;
        /* As before, these are not the actual activation heights but some
           blocks after them.  */
        // FIXME: Activate BIP16 with a softfork.
        consensus.BIP16Height = 100;
        consensus.BIP34Height = 100;
        consensus.BIP34Hash = uint256S("0xe0a05455d89a54bb7c1b5bb785d6b1b7c5bda42ed4ce8dc19d68652ba8835954");
        consensus.BIP65Height = 100;
        consensus.BIP66Height = 100;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 6 * 60 * 60; // 6 hours
        consensus.nPowTargetSpacing = 45;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.nMinDifficultySince = 1394838000; // 15 Mar 2014
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 456; // 95% of 480
        consensus.nMinerConfirmationWindow = 480; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000100010");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x4c6965779894e4bd876a972a34a8365e3c46d4fb9eb3fa3947d83709ec94883e");

        consensus.nAuxpowStartHeight = 0;
        consensus.nAuxpowChainId = 0x0001;
        consensus.fStrictChainId = false;
        consensus.nLegacyBlocksBefore = -1;

        consensus.rules.reset(new Consensus::TestNetConsensus());

        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0xcc;
        pchMessageStart[3] = 0xd3;
        nDefaultPort = 19012;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1542051081, 1094517, 0x1e0ffff0, "On 10/10/2018 the Alaris Testnet Genesis was created");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x4c6965779894e4bd876a972a34a8365e3c46d4fb9eb3fa3947d83709ec94883e"));
        assert(genesis.hashMerkleRoot == uint256S("0x6e1b1e196f3c33b37cb3435a303f5243df76ab7a9975cd13af5d06785b52cfb7"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet.alaris.io");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,33);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,55);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY_EBST] = std::vector<unsigned char>(1, 220);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x05, 0x5E, 0xA2, 0x49};
        base58Prefixes[EXT_SECRET_KEY] = {0x05, 0x5E, 0xA6, 0x31};

        bech32_hrp = "at";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
            }
        };

        chainTxData = ChainTxData{
            1541909473,
            1,
            0
        };

        assert(mapHistoricBugs.empty());
    }

    int DefaultCheckNameDB () const
    {
        return -1;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.nMinDifficultySince = 0;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        consensus.nAuxpowStartHeight = 0;
        consensus.nAuxpowChainId = 0x0001;
        consensus.fStrictChainId = true;
        consensus.nLegacyBlocksBefore = 0;

        consensus.rules.reset(new Consensus::RegTestConsensus());

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18445;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1541917234, 0, 0x207fffff, "On 10/10/2018 the Alaris Testnet Genesis was created");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x13697ab74440361a18f2613e9576b829d0a02523bea1aeaa5ab9a68bf73e2993"));
        assert(genesis.hashMerkleRoot == uint256S("0x6e1b1e196f3c33b37cb3435a303f5243df76ab7a9975cd13af5d06785b52cfb7"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("0x13697ab74440361a18f2613e9576b829d0a02523bea1aeaa5ab9a68bf73e2993")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,34);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,56);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,51);
        base58Prefixes[SECRET_KEY_EBST] = std::vector<unsigned char>(1, 220);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x05, 0x5E, 0xA2, 0x50};
        base58Prefixes[EXT_SECRET_KEY] = {0x05, 0x5E, 0xA6, 0x32};

        bech32_hrp = "atrt";

        assert(mapHistoricBugs.empty());
    }

    int DefaultCheckNameDB () const
    {
        return 0;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
