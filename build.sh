#! /bin/sh

mkdir -pv build/data
mkdir -v build/bin

cd src/backend
cc -o murl main.c -DWE_ARE_ON_NIX
cd ../..

cp -rv contrib build
cp -rv src/frontend build
mv -v src/backend/murl build/bin