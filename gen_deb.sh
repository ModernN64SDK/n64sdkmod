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

sudo chown -R root:root ./packages

sudo dpkg-deb --build packages/spicy
sudo dpkg-deb --build packages/iquesdk
sudo dpkg-deb --build packages/iquesdk-bin

sudo chown -R $USER:$USER ./packages
