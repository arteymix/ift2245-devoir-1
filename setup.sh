#!/usr/bin/env sh

set -e

user="$1"
password="$2"

if [ -z "$user" -o -z "$password" ]; then
    echo 'usage: setup.sh <username> <password>'
    exit 1
fi

useradd -m $user
chpasswd $user:$password

# copie des fichiers dans le dossier utilisateur
cp ptree.cpp setup.sh /home/$user

# La commande suivante est inspirée d'une réponse sur StackOverflow
# http://stackoverflow.com/questions/23929235/bash-multi-line-string-with-extra-space
su $user << EOM

cd $HOME

# hiéarchie de dossiers
mkdir -p tp1/{src,bin,result}

cd tp1

# copie des sources
cp ptree.cpp setup.sh tp1/src

g++ src/ptree.cpp bin -o bin/ptree

export PATH=$PATH:$HOME/bin

ptree > result/zero
ptree 1 2 > result/one_two

echo Vincent Antaki (p1038646) > Auteur.txt
echo Guillaume Poirier-Morency (p1053380) >> Auteur.txt
EOM
