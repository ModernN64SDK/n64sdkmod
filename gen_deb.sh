#!/bin/bash

dew_it()
{
    for var in "$@"
    do
        sudo dpkg-deb --build packages/$var
    done
}

echo "Warning: this resets permissions in the packages folder to the current user."

if [ -z $1 ]
    then
        choice="all"
elif [ -n $1 ]
    then
        choice=$1
fi

mkdir -p debs

sudo chown -R root:root ./packages

case $choice in
   "all") dew_it root-compatibility-environment n64sdk-common n64sdk n64-irix-env n64-midi-tools vadpcm-tools ;;
   *) echo "Sorry nothing";;
esac

sudo chown -R $USER:$USER ./packages

mv packages/*.deb debs
cp loose-debs/*.deb debs

if [[ $choice == "all" ]]
    then
        echo "Creating Packages.gz for APT archive"

        cd debs
        dpkg-scanpackages . /dev/null | gzip -9c > Packages.gz
        cd ..
fi

