# The Docker Installation Page

WARNING: Docker is NOT the recommended method of usage, and updating the SDK when such updates become available may be more difficult.

To build a Docker image from the `Dockerfile` and make it available on your machine:

1. Install Docker, e.g. `brew cask install docker` on macOS using [Homebrew](https://brew.sh)
2. `docker build --tag n64sdkmod-docker .`

Building the image may take a while, as a GCC cross-compiler needs to be compiled. You may also need to increase the available RAM to 4 GB in Docker's advanced settings to avoid the build process being killed for using up all available container RAM.

To run the container and compile your ROM's source code within the container:

1. `cd /path/to/your/code`
2. `docker run --volume "$(pwd):/src" n64sdkmod-docker bash -c 'cd /src && make'`

This will copy your source directory into the container, invoke the cross-compiler, and output any build artifacts to your hosts's working directory.

<img src="https://github.com/CrashOveride95/n64sdkmod/raw/master/docker-fogworld-macos.png" alt="Image of fogworld under Docker on MacOS" width="1100"/>

[Back to homepage](index.html)
