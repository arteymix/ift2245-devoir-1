#!/usr/bin/env sh

useradd $1 $2
mkdir -p /home/$1/tp1/{src,bin,result}
cp ptree.cpp /home/$1/tp1/src
chown -r $1 /home/$1/* 

pushd /home/$1/tp1

g++ -std=c++11 src/ptree.cpp bin -o ptree
chmod 777 bin/ptree
export PATH=$PATH:/bin/ptree

# Executer ptree et mettre dans result/zero
# Executer ptree 1 2 et mettre dans result/one_two
#ajouter txt auteur avec nos noms et da dans home/$1

popd

