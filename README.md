# Bithereum

[![BuildStatus](https://travis-ci.org/BTHPOS/BTHPOS.svg?branch=master)](https://travis-ci.org/BTHPOS/BTHPOS)

Bithereum (codename BTH) will begin as a fork of Bitcoin and will then be mined as a separate chain shortly after.

The primary goal is to maintain a variant of Bitcoin that is centered around Proof of Stake and Ethereum's technological roadmap ultimately democratizing mining efforts, improving scalability, and increasing network consensus.

BTC which has long been touted as the most superior P2P currency, has seen it’s networks slow down significantly in lieu of the major price upswing seen this year. These points of inefficiency have not been addressed by the core development team and have resulted in a further divide in the community. The three major issues prevalent with BTC are as follows:

- Limited scalability- increased traffic and volume has hindered speed, ease, and cheapness in transacting across borders
- Community Schism- Deep ideological divide within BTC community
- Mining Cartel- Monopolization through mining pools; the 4 largest mining pools control roughly 61% of the mining hashpower

Bithereum believes in the importance of both on-chain and off-chain scaling solutions, as well as Proof of Stake being a consensus mechanism that will make the blockchain more efficient while increasing network security.

Bithereum preserves and implements Bitcoin features such as SegWit but also focuses on implementing features of Ethereum, namely PoS. Significant features of Bithereum include:

    - Initially ASIC-resistant, GPU-minable PoW algorithm (Equihash <144,5>)
    - To be later changed to Casper-like Proof of Stake FFG Consensus model via OpCodes. Additional details about our PoS model are in this blog: https://medium.com/bithereum-network/bithereums-proof-of-stake-concept-b06899fa4950
    - Increased Blocksize (4MB)
    - SegWit
    - Replay protection (SIGHASH_FORK_ID)
    - Lightning Network upcoming
    - Full Node Incentivization 

Though Proof of Stake will take some time to implement, even in the case of Ethereum, it is on the horizon of completion. Bithereum, however, will begin mining prior to its completion through the Proof of Work algorithm known as Equihash. Once PoS is complete, miners will be able to stake their holdings for dividend-like returns. Bithereum will be leveraging existing OpCodes as well as introducing several new OpCodes to create its PoS model. Furthermore, everyone who runs a Bithereum full node will be rewarded with a portion of Bithereum’s official mining pool’s fees, addressing the misalignment of incentives for full nodes present in most other chains.

Although BTH was bootstrapped to create an entirely new network, it contains the entire Bitcoin blockchain until the block from which it will be forked. As a result, Bithereum will become a full fork with all of Bitcoin's transaction history since 2009. Any Bitcoin wallet address which held Bitcoin in the BTC block before the fork held an equal number of Bithereum in the first block after the fork.

## WARNING

This is the staging tree of Bithereum. If you don’t understand what you are doing, please don’t compile and run your own client from the staging tree.

## Links

* Website: https://bithereum.network/
