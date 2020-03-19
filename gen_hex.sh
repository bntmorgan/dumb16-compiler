#!/bin/bash
if test $# -ne 2
then
  echo lol $0 "<source> <dest>"
  exit 1
fi
~/documents/laas/eric/eric_tools/bin2hex "$1" "$2" 4096 32MPU
