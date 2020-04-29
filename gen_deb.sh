#!/bin/sh

dew_it()
{
    for var in "$@"
    do
        sudo dpkg-deb --build packages/$var
    done
}

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
   "all") dew_it spicy iquesdk n64sdk u64assets root-compatibility-enviroment rsp-tools vadpcm-tools libkmc libhvqm;;
   "spicy") dew_it spicy;;
   "ique") dew_it iquesdk;;
   "n64") dew_it n64sdk;;
   "assets") dew_it u64assets;;
   "root") dew_it root-compatibility-enviroment;;
   "rsp") dew_it rsp-tools;;
   "pcm") dew_it vadpcm-tools;;
   "kmc") dew_it libkmc;;
   "hvqm") dew_it libhvqm;;
   *) echo "Sorry nothing";;
esac

sudo chown -R $USER:$USER ./packages

mv packages/*.deb debs
cp loose-debs/*.deb debs

echo "Creating Packages.gz for APT archive"

cd debs
dpkg-scanpackages . /dev/null | gzip -9c > Packages.gz
cd ..

