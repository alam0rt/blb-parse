# BLB File Format

This document describes the likely structure and purpose of the “.blb” files, based on decompiled references in “slus_006.01.c”.

## Overview

BLB files appear to store collection(s) of related assets used by the game engine, including levels, movies/cutscenes, and possibly additional data or metadata required to load them. The code uses functions like parse_blb_header, ptr_end_of_blb_header, and get_movie_count to parse these files and retrieve chunks of data.

## Header Layout

Although the exact structure remains partially inferred, the overall pattern suggests:
1. A header region with fixed-size records identifying each asset or entry.
2. A count of total entries for levels (or similar) at a known offset (found by calling ptr_end_of_blb_header).
3. Possibly a count of movie/cutscene entries (managed by get_movie_count).
4. An offset table or one or more listings that point to assets, which the engine iterates over to read or set up memory pointers (level_name, (&levels)[level_id], sub_level_list, etc.).

## Byte-Level Breakdown (Hypothesized)

● Offset 0 (magic/type/version): Some initial bytes or identifier indicating the file is a BLB, or zero if no explicit magic is found.  
● Offset 2-3 (entry count): Possibly holds the number of level entries, read in a loop that populates a structure.  
● Offset 4... at intervals: Each entry’s metadata, including pointers within the file pointing to location(s) of the actual data.  
● Terminating region or size field: The engine calls ptr_end_of_blb_header to identify the boundary or total size.

## Asset Types

1. Levels: Each entry references level data, loaded by load_level_ or related functions.  
2. Movies/Cutscenes: get_movie_count indicates the file also stores references to “movie” data or cutscenes.  
3. Possibly Sprites/Text Tiles: Other code references loading assets like sprites or tiles for a given level, although it’s unclear if these are contained directly in the BLB or only referenced.

## Usage in Code

• parse_blb_header(astruct_5 *something, int ptr_to_level_data) – Potentially reads the BLB’s main header and populates an internal structure with offset pointers, counts, etc.  
• ptr_end_of_blb_header(astruct_10 *buffer) – Likely obtains how many entries or the file size.  
• level_name(int buffer, uint size) – Grabs a text string or name for each level entry.  
• get_movie_count(int blb) – Counts how many movie references exist.  
• movie_file_name(int blb, uint index) – Retrieves the string pointer for a movie file name.

## Concluding Notes

Because the format is partially inferred, further reverse engineering may be necessary to confirm exact offsets, field sizes, and the presence of additional unknown data. However, from the game code, a .blb file appears to be a multi-entry container that includes level definitions and movie/cutscene references, along with a header region that enumerates them.
