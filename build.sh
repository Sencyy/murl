#! /usr/bin/env bash

mkdir -pv build/data

cd src/backend
cc -o murl main.c
cd ../..

cp -rv contrib build
cp -rv src/frontend build
mv -v src/backend/murl build