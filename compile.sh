#!/bin/bash

echo "Compiling..."
echo " "
cd src/
g++ -c main.cpp Menu.cpp -I/Documents/SFML/SFML_SRC/include 
echo "Linking..."
echo " "

mv *.o ../linkers
cd ../linkers

g++ main.o Menu.o -o colors -L/Documents/SFML/SFML_SRC/lib -lsfml-graphics -lsfml-window -lsfml-system

mv colors ../
cd ../

if [ -e compile.sh~ ] ;
then 
    rm compile.sh~ 
fi

cd include/
if [ -e *.hh~ ];
then
    rm *.hh~
fi

cd ..
echo "Executing..."
echo " "
./colors

