#!/bin/bash

# Ejecutar makefile
make && make clean

# Ejecutar programa
# $1: argumento del menu principal
./main.bin $1
