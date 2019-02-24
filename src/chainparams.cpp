// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2016-2017 The Zcash developers
// Copyright (c) 2018 The Bitcoin Private developers
// Copyright (c) 2017-2018 The Bithereum developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "uint256.h"
#include "arith_uint256.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

// For equihash_parameters_acceptable.
#include "crypto/equihash.h"
#include "net.h"
#include "validation.h"
#define equihash_parameters_acceptable(N, K) \
    ((CBlockHeader::HEADER_SIZE + equihash_solution_size(N, K))*MAX_HEADERS_RESULTS < \
     MAX_PROTOCOL_MESSAGE_LENGTH-1000)

#include "base58.h"
#include <assert.h>
#include <boost/assign/list_of.hpp>
#include <limits>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = ArithToUint256(arith_uint256(nNonce));
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.nHeight  = 0;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
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

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP34Height = 227931;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.BIP65Height = 388381; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 363725; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.BTHHeight = 555555; // At or around 12/26/2018 12:00 UTC
        consensus.BTHDifficultyReductionWindow = 20000;
        consensus.BTHZawyLWMAHeight = consensus.BTHHeight;
        consensus.BTHApprovalWindow = 20;
        consensus.BTHEquihashForkHeight = consensus.BTHHeight;
        consensus.BTHApprovalEnforceWhitelist = true;
        consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitStart = uint256S("0000000fffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitLegacy = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nDigishieldAveragingWindow = 30;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nDigishieldAveragingWindow);
        consensus.nDigishieldMaxAdjustDown = 32;
        consensus.nDigishieldMaxAdjustUp = 16;

        consensus.nZawyLwmaAveragingWindow = 45;
        consensus.nZawyLwmaAdjustedWeightLegacy = 13772;
        consensus.nZawyLwmaAdjustedWeight = 13772;
        consensus.nZawyLwmaMinDenominatorLegacy = 10;
        consensus.nZawyLwmaMinDenominator = 10;
        consensus.bZawyLwmaSolvetimeLimitation = true;
        consensus.BTHMaxFutureBlockTime = 12 * 10 * 60; // 120 mins

        consensus.nPowTargetTimespanLegacy = 14 * 24 * 60 * 60; // 10 minutes
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespanLegacy / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1510704000; // November 15th, 2017.

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000047e6d2d75a348b3f02a2905"); // work for 555,554
        
        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000000000000003b9ce759c2a087d52abc4266f8f4ebd6d768b89defa50a"); //477890


        // Allocations
        consensus.BTHTxFeeAlloc = 2000000 * COIN;
        consensus.BTHEthereumSupplyAlloc= 2730450 * COIN;
        consensus.BTHProjectAllocation = 1600000 * COIN;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xe3;
        pchMessageStart[1] = 0x48;
        pchMessageStart[2] = 0x6a;
        pchMessageStart[3] = 0x45;
        nDefaultPort = 18553;

        nPruneAfterHeight = 100000;
        const size_t N = 200, K = 9;
        const size_t N2 = 144, K2 = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N2, K2));
        nEquihashN = N;
        nEquihashK = K;
        nEquihashNnew = N2;
        nEquihashKnew = K2;

        genesis = CreateGenesisBlock(1231006505, 2083236893, 0x1d00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash(consensus);
        assert(consensus.hashGenesisBlock == uint256S("0x000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"));
        assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top

        vSeeds.emplace_back("us-dnsseed.bithereum.network", true);
        vSeeds.emplace_back("eu-dnsseed.bithereum.network", true);
        vSeeds.emplace_back("sg-dnsseed.bithereum.network", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,25);  // prefix: B
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,40);  // prefix: H
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                { 0, uint256S("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f")},
                { 11111, uint256S("0x0000000069e244f73d78e8fd29ba2fd2ed618bd6fa2ee92559f542fdb26e7c1d")},
                { 33333, uint256S("0x000000002dd5588a74784eaa7ab0507a18ad16a236e7b1ce69f00d7ddfb5d0a6")},
                { 74000, uint256S("0x0000000000573993a3c9e41ce34471c079dcf5f52a0e824a81e7f953b8661a20")},
                {105000, uint256S("0x00000000000291ce28027faea320c8d2b054b2e0fe44a773f3eefb151d6bdc97")},
                {134444, uint256S("0x00000000000005b12ffd4cd315cd34ffd4a594f430ac814c91184a0d42d2b0fe")},
                {168000, uint256S("0x000000000000099e61ea72015e79632f216fe6cb33d7899acb35b75c8303b763")},
                {193000, uint256S("0x000000000000059f452a5f7340de6682a977387c17010ff6e6c3bd83ca8b1317")},
                {210000, uint256S("0x000000000000048b95347e83192f69cf0366076336c639f9b7228e9ba171342e")},
                {216116, uint256S("0x00000000000001b4f4b433e81ee46494af945cf96014816a4e2370f11b23df4e")},
                {225430, uint256S("0x00000000000001c108384350f74090433e7fcf79a606b8e797f065b130575932")},
                {250000, uint256S("0x000000000000003887df1f29024b06fc2200b55f8af8f35453d7be294df2d214")},
                {279000, uint256S("0x0000000000000001ae8c72a0b0c301f67e3afca10e819efa9041e458e9bd7e40")},
                {295000, uint256S("0x00000000000000004d9b4ef50f0f9d686fd69db2e03af35a100370c64632a983")},
            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 0000000000000000002e63058c023a9a1de233554f28c7b21380b6c9003f36a8
            /* nTime    */ 1532884444,
            /* nTxCount */ 331282217,
            /* dTxRate  */ 2.4
        };
        
        vApprovedPubkeys = {
            { "02e6db859bd48db0f22e3e4c28039b719990dc68"}
        };
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
        consensus.BIP34Height = -1;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = -1;
        consensus.BIP66Height = -1;
        consensus.BTHHeight = 1;
        consensus.BTHZawyLWMAHeight = -1; // Activated on testnet
        consensus.BTHEquihashForkHeight = -1;

        consensus.BTHApprovalWindow = 3;
        consensus.BTHDifficultyReductionWindow = 1000;
        consensus.BTHApprovalEnforceWhitelist = true;

        consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitStart = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitLegacy = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nDigishieldAveragingWindow = 30;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nDigishieldAveragingWindow);
        consensus.nDigishieldMaxAdjustDown = 32;
        consensus.nDigishieldMaxAdjustUp = 16;

        consensus.nZawyLwmaAveragingWindow = 45;
        consensus.nZawyLwmaAdjustedWeightLegacy = 13632;
        consensus.nZawyLwmaAdjustedWeight = 13772;
        consensus.nZawyLwmaMinDenominatorLegacy = 3;
        consensus.nZawyLwmaMinDenominator = 10;
        consensus.bZawyLwmaSolvetimeLimitation = false;
        consensus.BTHMaxFutureBlockTime = 7 * 10 * 60; // 70 mins

        consensus.nPowTargetTimespanLegacy = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespanLegacy / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1514764800; // January 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1546300800; // January 1st, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1514764800; // January 1st 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1546300800; // January 1st 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // Allocations
        consensus.BTHTxFeeAlloc = 2000000 * COIN;
        consensus.BTHEthereumSupplyAlloc= 2730450 * COIN;
        consensus.BTHProjectAllocation = 1600000 * COIN;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xe4;
        pchMessageStart[1] = 0x48;
        pchMessageStart[2] = 0x6a;
        pchMessageStart[3] = 0x45;
        nDefaultPort = 19553;

        nPruneAfterHeight = 1000;
        const size_t N = 200, K = 9;
        const size_t N2 = 144, K2 = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N2, K2));
        nEquihashN = N;
        nEquihashK = K;
        nEquihashNnew = N2;
        nEquihashKnew = K2;

        genesis = CreateGenesisBlock(1516123516, 0x56bd5142, 0x1d00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash(consensus);
        assert(consensus.hashGenesisBlock == uint256S("0x00000000e0781ebe24b91eedc293adfea2f557b53ec379e78959de3853e6f9f6"));
        assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));
        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top

        vSeeds.emplace_back("us-testnet-dnsseed.bithereum.network", true);
        vSeeds.emplace_back("eu-testnet-dnsseed.bithereum.network", true);
        vSeeds.emplace_back("sg-testnet-dnsseed.bithereum.network", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,65); // prefix: T
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,63); // prefix: S
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x00000000e0781ebe24b91eedc293adfea2f557b53ec379e78959de3853e6f9f6")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        vApprovedPubkeys = {
            { "02e6db859bd48db0f22e3e4c28039b719990dc68"}
        };
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
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.BTHHeight = 375;
        consensus.BTHZawyLWMAHeight = -1; // Activated on regtest
        consensus.BTHEquihashForkHeight = 2001;

        consensus.BTHApprovalWindow = 3;
        consensus.BTHDifficultyReductionWindow = 1000;
        consensus.BTHApprovalEnforceWhitelist = false;

        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitStart = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitLegacy = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nDigishieldAveragingWindow = 30;
        consensus.nDigishieldMaxAdjustDown = 32;
        consensus.nDigishieldMaxAdjustUp = 16;

        consensus.nZawyLwmaAveragingWindow = 45;
        consensus.nZawyLwmaAdjustedWeightLegacy = 13772;
        consensus.nZawyLwmaAdjustedWeight = 13772;
        consensus.nZawyLwmaMinDenominatorLegacy = 10;
        consensus.nZawyLwmaMinDenominator = 10;
        consensus.bZawyLwmaSolvetimeLimitation = true;
        consensus.BTHMaxFutureBlockTime = 7 * 10 * 60; // 70 mins

        consensus.nPowTargetTimespanLegacy = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // Allocations
        consensus.BTHTxFeeAlloc = 2000000 * COIN;
        consensus.BTHEthereumSupplyAlloc= 2730450 * COIN;
        consensus.BTHProjectAllocation = 1600000 * COIN;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 20553;

        nPruneAfterHeight = 1000;
        const size_t N = 48, K = 5;
        const size_t N2 = 96, K2 = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N2, K2));
        nEquihashN = N;
        nEquihashK = K;
        nEquihashNnew = N2;
        nEquihashKnew = K2;

        genesis = CreateGenesisBlock(1296688602, 2, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash(consensus);
        assert(consensus.hashGenesisBlock == uint256S("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));
        assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }

};

class BitcoinAddressChainParam : public CMainParams
{
public:
    BitcoinAddressChainParam()
    {
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
    }
};

static std::unique_ptr<CChainParams> globalChainParams;
static BitcoinAddressChainParam chainParamsForAddressConversion;

const CChainParams &Params()
{
    assert(globalChainParams);
    return *globalChainParams;
}

const CChainParams &BitcoinAddressFormatParams()
{
    return chainParamsForAddressConversion;
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

static CScript CltvSigScript(const std::vector<std::string>& pubkeys, uint32_t lock_time) {
    CScript redeem_script;
    redeem_script << OP_DUP << OP_HASH160;
    for (const std::string& pubkey : pubkeys) {
         redeem_script << ToByteVector(ParseHex(pubkey));
    }
    redeem_script << OP_EQUALVERIFY << OP_CHECKSIG;
    return redeem_script;
}

static CScript MltvSigScript(const std::vector<std::string>& pubkeys, uint32_t lock_time) {
    CScript redeem_script;
    redeem_script << OP_HASH160;
    for (const std::string& pubkey : pubkeys) {
         redeem_script << ToByteVector(ParseHex(pubkey));
    }
    redeem_script << OP_EQUAL;
    return redeem_script;
}

unsigned int CChainParams::EquihashSolutionWidth(int height) const
{
    return EhSolutionWidth(EquihashN(height), EquihashK(height));
}

bool CChainParams::IsApprovedAddressScript(const CScript& scriptPubKey, uint32_t height) const {
  assert((uint32_t)consensus.BTHHeight <= height &&
         height < (uint32_t)(consensus.BTHHeight + consensus.BTHApprovalWindow));
  int block = height - consensus.BTHHeight;
  const std::vector<std::string> pubkeys = vApprovedPubkeys[0];
  CScript redeem_script;
  redeem_script = MltvSigScript(pubkeys, 0);
  LogPrintf("redeem_script=%s\n", HexStr(redeem_script));
  LogPrintf("scriptPubKey=%s\n", HexStr(scriptPubKey));
  return scriptPubKey == redeem_script;
}
