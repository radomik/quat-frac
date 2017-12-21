#!/bin/bash
geany -i -s $(find . -type f -a \( -name \*.cpp -o -name \*.hpp -o -name \*.h -o -name \*.ispc -o -name \*.txt -o -name \*.conf -o -name \*.sh -o -name \*.json -o -name common.mk -o -name Makefile -o -name Makefile.* \))
