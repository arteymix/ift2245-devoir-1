#!/usr/bin/env sh

set -e

user="$1"
password="$2"

if [ -z "$user" -o -z "$password" ]; then
    echo 'usage: setup.sh <username> <password>'
    exit 1
fi

useradd -m $user
echo $user:$password | chpasswd 

# copie des fichiers dans le dossier utilisateur
sudo -u $user cp ptree.cpp setup.sh /home/$user

cd /home/$user

# hiéarchie de dossiers
sudo -u $user mkdir -p tp1/src tp1/bin tp1/result

# copie des sources
sudo -u $user mv ptree.cpp setup.sh tp1/src

# ajoute tp1/bin au $PATH
sudo -u $user echo 'export PATH=$PATH:$HOME/tp1/bin' >> .profile

cd tp1

sudo -u $user g++ src/ptree.cpp -o bin/ptree

sudo -u $user -i ptree > result/zero
sudo -u $user -i ptree 1 2 > result/one_two

# ajout des noms au fichiers d'auteurs
sudo -u $user touch Auteur.txt
echo 'Vincent Antaki (p1038646)' >> Auteur.txt
echo 'Guillaume Poirier-Morency (p1053380)' >> Auteur.txt
