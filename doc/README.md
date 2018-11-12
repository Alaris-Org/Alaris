Alaris Core
==============

Setup
---------------------
[Alaris Core](http://alaris.org/) is the official Alaris client and it builds the backbone of the network. However, it downloads and stores the entire history of Alaris transactions (which is currently several GBs); depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

Running
---------------------
The following are some helpful notes on how to run Alaris on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/alaris-qt` (GUI) or
- `bin/alarisd` (headless)

### Windows

Unpack the files into a directory, and then run alaris-qt.exe.

### OS X

Drag Alaris-Qt to your applications folder, and then run Alaris-Qt.

### Need Help?

* See the documentation at the [Alaris Site](https://alaris.org) for help and more information.
* Ask for help on [#alaris](http://webchat.freenode.net?channels=alaris) on Freenode. If you don't have an IRC client use [webchat here](http://webchat.freenode.net?channels=alaris).
* Ask for help on the [Alaris forums](https://forum.alaris.info/index.php), in the [Technical Support board](https://forum.alaris.info/viewforum.php?f=7).

Building
---------------------
The following are developer notes on how to build Bitcoin on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [OS X Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [OpenBSD Build Notes](build-openbsd.md)
- [NetBSD Build Notes](build-netbsd.md)
- [Gitian Building Guide](gitian-building.md)

Development
---------------------
The Alaris repo's [root README](https://github.com/alaris/namecore/blob/master/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://dev.visucore.com/bitcoin/doxygen/)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [Travis CI](travis-ci.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)

### Resources
* Discuss on the [Alaris forums](https://forum.alaris.info/index.php), in the [Development & Technical Discussion board](https://forum.alaris.info/viewforum.php?f=8).
* Discuss on [#alaris-dev](http://webchat.freenode.net/?channels=alaris-dev) on Freenode. If you don't have an IRC client use [webchat here](http://webchat.freenode.net/?channels=alaris-dev).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [Files](files.md)
- [Fuzz-testing](fuzzing.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)
- [ZMQ](zmq.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.
