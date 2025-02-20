#!/usr/bin/env bash

DIR=slus_006.01
SOURCE="$DIR/slus_006.01.c"
GEN_DIR="$DIR/generated"

mkdir -p "$GEN_DIR"

cat "$SOURCE" | awk '/^[a-z]/' | sort  | grep FUN > "$GEN_DIR/unsorted_functions.txt"
cat "$SOURCE" | awk '/^[a-z]/' | sort  > "$GEN_DIR/unsorted_all.txt"
