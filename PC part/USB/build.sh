#!/bin/sh

gcc -c -fPIC zservo.c
gcc -shared -fPIC -o libzdrone.so zservo.o

