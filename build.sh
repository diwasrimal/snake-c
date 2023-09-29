#!/bin/sh

set -xe

LIBS=$(pkg-config --libs raylib)
CFLAGS=$(pkg-config --cflags raylib)
CC=clang

$CC -o snake snake.c -g $CFLAGS $LIBS