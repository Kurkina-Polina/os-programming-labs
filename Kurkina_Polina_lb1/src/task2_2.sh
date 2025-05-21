#!/bin/bash

ans=ls -ilR / 2>/dev/null | grep "$(ls -i "$1" | cut -f1 -d' ')"
echo "$(pwd "$ans") /$ans"
