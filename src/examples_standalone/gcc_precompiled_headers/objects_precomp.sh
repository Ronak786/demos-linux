#!/bin/bash
let "x=0"; while [[ $x -lt 10 ]]; do echo -n "main$x.precompiled.o "; let "x=x+1"; done
