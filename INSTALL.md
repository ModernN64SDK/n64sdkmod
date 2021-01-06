# Installation Tutorial

The following instructions have been tested on an Ubuntu 18.04 machine, a Ubuntu 20.04 machine, and WSL1 and WSL2,  but they should work fine on Debian or other distros.

If installing with Docker, please go to [The Docker Installation Page](DOCKER.md)


## Compiler setup

Before you install the SDK you must build a compiler. In our case, we're using *n64chain*.

Building the compiler requires dependencies, so you must install them:

``sudo apt install build-essential libmpfr-dev libmpc-dev libgmp-dev flex bison``

Clone my fork of n64chain: ``git clone https://github.com/CrashOveride95/n64chain.git`` .

Then run the script.

If you get a permission error on running the script, run ``chmod`` on the .sh.

``chmod +x build-posix64-toolchain.sh``

This will download and build *GCC* and the *Binutils* line of programs. This may take a while.

When it finishes, add n64chain/bin to your **PATH**, preferably in your .bashrc (or in your shell's respective profile script) so you don't have to add it every time.

In my case I cloned n64chain to my home directory, and I use **Bash**, so I added

``export PATH="$HOME/n64chain/bin:$PATH"`` to the end of my ``~/.bashrc`` script.

Next, add a definition of ``N64_LIBGCCDIR``to your .bashrc. This is to allow people with differing setups less trouble in makefile editing.

As I've cloned n64chain to my home directory, I add

``export N64_LIBGCCDIR="$HOME/n64chain/lib/gcc/mips64-elf/10.2.0"``

to the end of my ``~/.bashrc`` script.

## Allowing us to download the SDK

The SDK is packaged as an *APT repo*. APT is the software management tool used in Debian and Ubuntu.

You must add our APT repo to the list of repos APT can download from.

To add it, run ``echo "deb [trusted=yes] https://crashoveride95.github.io/apt/ ./" | sudo tee /etc/apt/sources.list.d/coneyisland.list`` in a Terminal.

Then, run ``sudo apt update`` to update the package lists.

*You can now download the SDK packages*!

## Downloading the SDK itself

You have two options for the main N64 libraries: *iquesdk*, and *n64sdk*.

**n64sdk** contains version 2.0L of the N64 SDK library, which is the latest version.

**iquesdk** contains the version used in the *iQue SDK*, which is a modified version of 2.0L that removes code for N64 acessories irrelevant to the iQue and edits code for some functions to get correct results with its slightly faster CPU and RAM.

*I highly suggest getting the n64sdk package, only get iquesdk if you are specifically targeting the iQue, otherwise things such as rumble and controller pak support will NOT WORK!*

To begin setup, install the sdk by installing the package with APT.

``sudo apt install (sdk of your choice)``

After that, unless you want to use a linkerscript, you will need to install the open source ``makerom`` replacement, spicy.

If you're compiling the demos, this is required.

``sudo apt install spicy``

Then, you need makemask, which applies the CIC data on your ROM.

``sudo apt install makemask``

Next, you need to setup the root compatibilty enviroment. This is needed because in the old nintendo SDKs, there was a "ROOT" folder which simulated a unix filesystem. 
This creates that setup for compatibility reasons.

``sudo apt install root-compatibility-enviroment``

After you install the enviroment, add a definition of ROOT to your .bashrc:

``export ROOT=/etc/n64``

If you want to install the demos, use APT install once again:

``sudo apt install n64-demos``

You are ready to compile the demos!

## Compiling demos

First, copy the ``PR`` directory from /usr/src to your home directory.

``cp -r /usr/src/PR ~/n64demos``

Then change directory to that folder:

``cd ~/n64demos``

Again, change directory to the demo of your choice, and run ``make`` to compile it.

(Note that hvqmsample requires libhvqm, a FMV library which the sample demonstrates.)

After you make the demo, you can run it in the emulator of your choice, and it should work perfectly!


## Still need help?

Feel free to contact me on Discord ``CrashOveride#2465`` or Twitter ``@crashoveride953``

![Image of fogworld](https://github.com/CrashOveride95/n64sdkmod/raw/master/fogworldshot.png)

![Image of hvqmsample](https://github.com/CrashOveride95/n64sdkmod/raw/master/hvqm.png)

