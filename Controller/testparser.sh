#!/bin/bash

# Vérifier que le fichier texte contenant les commandes est spécifié en argument
if [ $# -ne 1 ]
then
    echo "Usage: $0 <command_file>"
    exit 1
fi

# Vérifier que le fichier texte existe et est lisible
if [ ! -r $1 ]
then
    echo "Error: $1 is not readable"
    exit 1
fi

# Compiler le fichier C contenant la fonction de parsing de commande
rm test_parser result.txt
make clean
make test_parser

# Lire chaque ligne du fichier texte contenant les commandes et les exécuter avec la fonction de parsing de commande
while read line
do
    echo "tested command : \"" $line "\", result :">> result.txt
    ./test_parser $line >> result.txt
done < $1
