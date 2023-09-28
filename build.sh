#!/bin/sh

set -xe

LIBS=$(pkg-config --libs raylib)
CFLAGS=$(pkg-config --cflags raylib)

cc -o snake snake.c $CFLAGS $LIBS