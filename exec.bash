#!/bin/bash

# Ejecutar makefile
make cleanall && make && make clean

# Ejecutar programa
# $1: argumento del menu principal
./main.bin $1
