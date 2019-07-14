#!/usr/bin/env bash
echo "Building schsat-utils..."
make -f make-schsatutils > /dev/null
echo "Done!"
echo "Building schsatd..."
make -f make-schsatd > /dev/null
echo "Done!"
echo "Building schsat-test..."
make -f make-schsattest > /dev/null
echo "Done!"
echo "Building schsat-phototest..."
make -f make-photos > /dev/null
echo "Done!"
echo "Building schsat-cmdlistener..."
make -f make-schsatcmd > /dev/null
echo "Done!"
echo "Cleaning"
make -f make-schsatutils clean > /dev/null
make -f make-schsatd clean > /dev/null
make -f make-schsattest clean > /dev/null
make -f make-photos clean > /dev/null
make -f make-schsatcmd clean > /dev/null
