#!/usr/bin/bash

## script to compile all project with snapcraft

function help {
    echo "compile for clean, create and compile"
    echo "clean for snapcraft clean"
    echo "create for create VM"
}

if [ "$#" -ne 1 ]; then
    echo "Wrong command: "
    help
    exit
fi

if [ $1 == 'compile' ]; then
    make clean
    rm build/writernote
    snapcraft clean
    multipass launch --name snapcraft-writernote --cpus 4 --mem 4G --disk 100G
    snapcraft

elif [ $1 == 'clean' ]; then
    snapcraft clean

elif [ $1 == 'create' ]; then
    multipass launch --name snapcraft-writernote --cpus 20 --mem 24G --disk 100G

elif [ $1 == 'normal' ]; then
    /snap/bin/snapcraft


else
    help


fi

