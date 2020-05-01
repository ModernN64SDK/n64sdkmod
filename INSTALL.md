# Installation Tutorial

The following instructions have been tested on an Ubuntu 18.04 machine, but they should work fine on Debian or other distros.


## Compiler setup

Before you install the SDK you must build a compiler. In our case, we're using n64chain.

Clone my fork of n64chain: ``https://github.com/CrashOveride95/n64chain.git`` and run ``build-posix64-toolchain.sh``.

If you get a permission error on running the .sh, run chmod +x on the .sh.

``chmod +x build-posix64-toolchain.sh``

This will download and build gcc and binutils. This may take a while.

When it finishes, add n64chain/bin to your PATH, preferably in your .bashrc so you don't have to add it every time.

In my case I cloned n64chain to my home directory, so I added

``export PATH="$HOME/n64chain/bin:$PATH"`` to the end of my .bashrc.

## Allowing us to download the SDK

Then you must add the apt repo to the list of repos apt can download from.

To add it, run ``sudo nano /etc/apt/sources.list.d/coneyisland.list`` in a Terminal.

Then when nano opens, paste ``deb [trusted=yes] https://coneyislanddiscopalace.xyz/apt ./`` into the file, then save and exit with Ctrl+X.

~~yes the repo url is seriously coneyislanddiscopalace.xyz, if you have a problem with that, feel free to buy a better url domain and server~~

Then run ``sudo apt update`` to update the package lists.

You can now download the SDK packages!

## Downloading the SDK itself

You have two options for the main N64 libraries: ``iquesdk``, and ``n64sdk``.

``n64sdk`` contains version 2.0L of the library, which was the last known version.

``iquesdk`` contains the version used in the iQue SDK, which is a modified version of 2.0L that adds logic for detecting an iQue and getting correct results with it's slightly faster hardware.

It will still detect a regular N64 and work completely fine.

To begin setup, install the sdk by installing the package with apt.

``sudo apt install n64sdk/iquesdk``

After that, unless you want to use a linkerscript, you will need to install the open source ``makerom`` replacement, spicy.

If you're compiling the demos, this is required.

``sudo apt install spicy``

Then, you need makemask, which applies the CIC data on your ROM.

``sudo apt install makemask``
