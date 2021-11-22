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
   "all") dew_it spicy makemask iquesdk n64sdk u64assets root-compatibility-environment rsp-tools vadpcm-tools n64-conv-tools n64-midi-tools n64graphics libhvq libhvqm libnusys libnustd libnaudio libmus libleo n64manual n64-demos nusys-demos kantan-demos mus-demos tutorial-demos n64sdk-common;;
   "spicy") dew_it spicy;;
   "makemask") dew_it makemask;;
   "ique") dew_it iquesdk;;
   "n64") dew_it n64sdk;;
   "leo") dew_it libleo;;
   "assets") dew_it u64assets;;
   "demos") dew_it n64-demos;;
   "root") dew_it root-compatibility-environment;;
   "rsp") dew_it rsp-tools;;
   "pcm") dew_it vadpcm-tools;;
   "conv") dew_it n64-conv-tools;;
   "midi") dew_it n64-midi-tools;;
   "n64gfx") dew_it n64graphics;;
   "hvq") dew_it libhvq;;
   "hvqm") dew_it libhvqm;;
   "naudio") dew_it libnaudio;;
   "mus") dew_it libmus;;
   "nusys") dew_it libnusys;;
   "nustd") dew_it libnustd;;
   "man") dew_it n64manual;;
   "demos") dew_it n64-demos;;
   "kantan") dew_it kantan-demos;;
   "musdem") dew_it mus-demos;;
   "tutorial") dew_it tutorial-demos;;
   "nudemos") dew_it nusys-demos;;
   "common") dew_it n64sdk-common;;
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

