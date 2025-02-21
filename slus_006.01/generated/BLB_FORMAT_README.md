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

Because the format is partially inferred, further reverse engineering may be necessary to confirm exact offsets, field sizes, and the presence of additional unknown data. However, from the code in “slus_006.01/blb.c” and references throughout the code base, we can identify how the BLB is parsed, which data structures get populated, etc. In particular:

• The “initialise_blb_struct(astruct_8 *blb_loading_struct,int buffer,void *load_asset_function)” function sets up a “blb_loading_struct”—a container that holds a pointer to the BLB data (game_blb_ptr) plus a function pointer for loading or referencing assets.

• The “parse_blb_header(astruct_5 *something,int ptr_to_level_data)” function reads the BLB’s header section to locate specific entries. It checks values at offsets like 0xf36, 0xf92, etc. to determine how many sub-entries exist and what each one contains.

• The “ptr_end_of_blb_header(astruct_10 *buffer)” function returns a byte from the BLB header region (0xf31 offset) used to determine how many entries are in the file, or a boundary in the header.

• Calls like “load_game_blb_asset(offset,sector_count,u_long *buffer_pointer)” indicate that data from the BLB may be loaded from disc, using offset and sector counts in the BLB’s header.

• Specific values of (byte)0x01, (byte)0x02, (byte)0x03, etc. can identify different resource types (levels, cutscenes, etc.), with code branching depending on these values. At run time, the engine checks these bytes to decide whether to load a movie reference or a level resource.

Thus, from the game code, a .blb file is effectively a multi-entry container of level definitions, references to movie/cutscene data, and offset tables for each resource chunk. Further reverse engineering or debugging can confirm how many total entries exist, what each block stores, and any unknown fields remaining in the BLB format.
