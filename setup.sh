#!/bin/bash
rm linux-build/*
rm -rf linux-build
mkdir linux-build
cd linux-build
cmake .. -DTESTING=1
start cmake --build . -j --config Release
start cmake --build . -j --config Debug
cd ..

