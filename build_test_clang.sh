#!/bin/sh
clang++ -std=gnu++14 -stdlib=libc++ -I . -o test tests/*.cpp
