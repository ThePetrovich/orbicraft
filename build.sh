#!/usr/bin/env bash
echo "Building schsat-utils..."
make -f make-schsatutils > /dev/null
echo "Done!"
echo "Building schsatd..."
make -f make-schsatd > /dev/null
echo "Done!"
echo "Cleaning"
make -f make-schsatutils clean > /dev/null
make -f make-schsatd clean > /dev/null
