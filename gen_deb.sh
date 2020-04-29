#!/bin/sh

echo "Warning: this resets permissions in the packages folder to the current user."

while true
do
    read -r -p 'Do you want to continue? ' choice
    case "$choice" in
      n|N) exit 1;;
      y|Y) break;;
      *) echo 'Response not valid';;
    esac
done

rm ./debs/*.deb
mkdir -p debs

sudo chown -R root:root ./packages

sudo dpkg-deb --build packages/spicy
sudo dpkg-deb --build packages/iquesdk
sudo dpkg-deb --build packages/n64sdk
sudo dpkg-deb --build packages/u64assets
sudo dpkg-deb --build packages/root-compatibility-enviroment
sudo dpkg-deb --build packages/rsp-tools
sudo dpkg-deb --build packages/vadpcm-tools
sudo dpkg-deb --build packages/libkmc
sudo dpkg-deb --build packages/libhvqm

sudo chown -R $USER:$USER ./packages

mv packages/*.deb debs
cp loose-debs/*.deb debs
