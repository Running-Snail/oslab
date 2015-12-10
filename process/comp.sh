#!/bin/bash

gcc common.c main.c -o main
gcc common.c get.c -o get
gcc common.c copy.c -o copy
gcc common.c put.c -o put

