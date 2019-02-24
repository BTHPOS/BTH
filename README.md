# Bithereum

[![Build Status](https://travis-ci.org/dondreytaylor/BTH.svg?branch=master)](https://travis-ci.org/dondreytaylor/BTH)

Bithereum (codename BTH) began as a fork of Bitcoin after block height 491406 on Tue, 24 Oct 2017 01:17:35 GMT and began being mined as a separate chain at block height 491407 on Sun, 12 Nov 2017 13:34:01 GMT.

The primary goal is to maintain a variant of Bitcoin that is more decentralized by using an ASIC-resistant Proof of Work algorithm, removing barriers to entry for new miners around the world and reducing the concentration of power in the hands of massive-scale mining operations.

Bithereum preserves and implements Bitcoin features such as SegWit, which enables advanced scaling technologies like Lightning Network. Significant differences at launch time included:

- ASIC-resistant GPU-minable PoW algorithm (Equihash)
- Per-block difficulty adjustment algorithm
- Replay protection (SIGHASH_FORK_ID)
- Unique wallet addresses (prefixes of G and A)

Although BTH was bootstrapped on 12 Nov to create an entirely new network, it contains the entire Bitcoin blockchain until block 491406. As a result, Bithereum became a full fork with all Bitcoin transaction history since 2009. Any Bitcoin wallet address which held Bitcoin in BTC block 491406 before the fork held an equal number of Bithereum in BTH block 491407 after the fork.

## Releases

This is the staging tree of Bithereum. For release version, please switch to [0.15 branch](https://github.com/dondreytaylor/BTH/tree/0.15) or [release page](https://github.com/dondreytaylor/BTH/releases).

To compile from source, please check the build documentations under [doc](https://github.com/dondreytaylor/BTH/tree/master/doc).

## Responsible Disclosure

At the Bithereum, we consider the security of our systems a top priority. But no matter how much effort we put into system security, there can still be vulnerabilities present.

If you discover a vulnerability, we would like to know about it so we can take steps to address it as quickly as possible. We would like to ask you to help us better protect our users and our systems.

Please check our [Responsible Disclosure](https://bithereum.org/responsible-disclosure/) page for more details.

## Links

* Website: https://bithereum.org
* Tech Spec: https://github.com/dondreytaylor/BTH/wiki/Technical-Spec
* Twitter: https://twitter.com/bithereum
* Facebook: https://www.facebook.com/bithereumofficial/
* Forum: https://forum.bithereum.org
* Discord: [invitation](https://discord.gg/HmVUU6S)
* Telegram (Global): https://t.me/BithereumNetworkHQ
