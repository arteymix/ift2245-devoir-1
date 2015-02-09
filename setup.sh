#!/usr/bin/env sh
user="$1"
useradd user "$2" #faut crypte $2 non?
mkdir -p /home/$user/tp1/{src,bin,result}
cp ptree.cpp /home/$user/tp1/src
chown -r $user /home/$user/* 

pushd /home/$user/tp1

g++ -std=c++11 src/ptree.cpp bin -o ptree
chmod 777 bin/ptree
export PATH=$PATH:/bin/ptree
ptree > result/zero
ptree 1 2 > result/one_two

#La commande suivante a été prise sur StackOverflow :
# 
cat > Auteur.txt <<- EOM
Vincent Antaki (p1038646),
Guillaume Poirier-Morency (p1053380)
EOM
popd

