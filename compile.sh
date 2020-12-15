#!/bin/bash
VERSION=$(cat VERSION)
make clean
make
cp build/additive_voice_$VERSION.bin build/additive_voice_latest.bin
git add build/additive_voice_latest.bin

