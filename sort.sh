#!/usr/bin/env bash

DIR=slus_006.01
SOURCE="$DIR/slus_006.01.c"
GEN_DIR="$DIR/generated"

mkdir -p "$GEN_DIR"

cat "$SOURCE" | grep -E '^[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]+[a-zA-Z_][a-zA-Z0-9_]*\(.*\)' > "$GEN_DIR/functions.txt"
cat "$SOURCE" | grep -E '^[[:space:]]*(typedef|struct|enum|union)' | sort  > "$GEN_DIR/types.txt"
