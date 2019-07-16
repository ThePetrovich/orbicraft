#!/usr/bin/env bash
unzip cubesat-src.zip
cd cubesat-src
make -f make-photos
./build/photos
