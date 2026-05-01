#! /bin/sh

mkdir -pv build/data
mkdir -v build/bin

cd src/backend
# cc -o murl main.c -Bstatic $1
make murl
cd ../..

cp -rv contrib build
cp -rv src/frontend build
mv -v src/backend/murl build/bin