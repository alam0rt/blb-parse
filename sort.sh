#!/usr/bin/env bash

set -eou pipefail

DIR=slus_006.01
SOURCE="$DIR/slus_006.01.c"
GEN_DIR="$DIR/generated"

mkdir -p "$GEN_DIR"

cat "$SOURCE" | grep -E '^[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]+[a-zA-Z_][a-zA-Z0-9_]*\(.*\)' > "$GEN_DIR/functions.txt"
cat "$SOURCE" | grep -E '^[[:space:]]*(typedef|struct|enum|union)' | sort  > "$GEN_DIR/types.txt"

# parse the SOURCE and split (preserving the SOURCE file) into functions and types under the GEN_DIR/by-type directory
# There should be a file for each type and function which contains the full definition of the type or function
cat "$SOURCE" | awk -v gen_dir="$GEN_DIR" '
    /^[[:space:]]*(typedef|struct|enum|union)/ {
        type = $0
        type_file = gen_dir "/by-type/" $2 ".txt"
        print type > type_file
    }
    /^[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]+[a-zA-Z_][a-zA-Z0-9_]*\(.*\)/ {
        fn = $0
        function_file = gen_dir "/by-function/" $2 ".txt"
        print fn > function_file
    }
'