#!/bin/bash

cc -fPIC -shared -o lib.so function.c
python3 function.py
