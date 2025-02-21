# BLB Format Documentation

This document describes the structure of the "BLB" format as inferred from the decompiled code in the file "blb.c". This information is derived from analysis of functions that parse or manipulate "BLB" data, including clues about offsets, header fields, and how different sections are read and interpreted.

--------------------------------------------------------------------------------
## Overview

The "BLB" format appears to be some form of container or archive used by the game to load assets (such as movies, streams, textures, levels, etc.). Each BLB file includes:

1. A header region containing:
   - Identifiers/flags for how to interpret the data
   - Potential type codes at offset 0xF36
   - References to further offsets in the BLB (often at offsets 0xF92, 0xF1C, 0xB64, etc.)

2. Possibly multiple entries or sections that the game calls “assets,” each with a separate descriptor.  

Much of the code in “blb.c” references constants and offsets in the BLB data structure at or after offset 0xF36. Different values at 0xF36 (and the byte at 0xF92) can change how the BLB is parsed. Many function parameters revolve around reading from base addresses (like “blb_header_ptr” plus some offset).

--------------------------------------------------------------------------------
## High-Level Parsing Logic

The code repeatedly references:
- (blb_base_ptr + 0xF36) : This indicates a type or ID for the BLB data. Examples encountered:
  - Value = 1 → Possibly indicates a “DREAM/PIRA/SCI” block or a special “movie” block.
  - Value = 2 → A certain block type that includes sub-checks.
  - Value = 3 → Another block type that triggers a different parse path.  
  - Values 4 or 5 → Additional parse paths in “FUN_8007ae5c” or “more_parsing_of_blb” dealing with advanced data sections.

- (blb_base_ptr + 0xF92) : Often used as an index to compute further offsets. The code multiplies the byte at 0xF92 by some constant (like 0x1C, 0x0C, 0x70, 0x10) to jump to a new address that references additional data (e.g. 0xB60, 0xB64, 0xB6C, 0xCD3, 0xF1C, etc.).

- (blb_base_ptr + 0xF30) : Holds a byte used to check “blb_level_” in certain segments (the maximum number of “levels” or segments?).

- (blb_base_ptr + 0xF32) : May indicate movie count or related data.

Queries in the code revolve around these values:
- A “type” stored at offset “+ 0xF36” 
- A “sub-id” stored at “+0xF92”
- Another possible “ID” at “+ 0xF31” or “+ 0xF32”
- Arrays or tables living at 0xB60, 0xB64, 0xF1C, 0x1C-based data, 0x70-based data, etc.

From these initial references, the code branches differently and loads or interprets different sections. For instance:
- Value = 2 often calls “get_blb_offset” & “get_blb_sector_count” to load a chunk, possibly a streaming video or texture block.
- Value = 3 often triggers a path that references a 0x70-sized structure array.
- Value = 1 references a 0x1C-sized structure array at offset 0xB64, apparently used for “movies” or naming references (“DREA,” “PIRA,” or “SCI”).

--------------------------------------------------------------------------------
## Magic Values / Key Offsets

The BLB data uses many “magic offsets” or “tags”:
1. **Offset 0xF36**  
   The primary “Type/ID” byte. Checking this determines how to parse.  
   Observed Values:
   • 0x01: Possibly movie references like “DREA” or “PIRA.”  
   • 0x02, 0x03, 0x04, 0x05, 0x06: Trigger other parse routines.  

2. **Offset 0xF92**  
   Byte used as an index multiplied by 0xC, 0x1C, 0x10, or 0x70 to locate sub-block entries.  
   In the code, we see:
   • If (type = 1), index * 0x1C + 0xB64  
   • If (type = 2), index * 0x0C + 0xF1C  
   • If (type = 3), index * 0x70 + 0x?? (various fields)  
   • If (type in 4 or 5), index * 0x10 + 0xCD3, etc.

3. **Offsets 0xF30, 0xF31, 0xF32**  
   - 0xF31 or 0xF32 might be counters for how many “movies” or segments exist.  
   - 0xF30 is used in logic comparing “blb_level_” with *(base + 0xF30).

4. **Reference Offsets in the Data**  
   The code references addresses like 0xB60, 0xB64, 0xB6C, 0xCD3, 0xF1C, 0xECC, 0xF36, 0xF92, 0xCCC, etc. Many of these are sub-structures or sub-headers.  
   - 0xB60 → Possibly a 2-byte field for “Another asset pointer” if type=1.  
   - 0xB64 → Possibly the “movie file name pointer” if type=1.  
   - 0xB6C → Another pointer for certain movie references.  
   - 0xF1C → Typically used if type=2.  
   - 0x70 blocks → If type=3, references chunked data of size 0x70.  
   - 0x1C blocks → If type=1, references chunked data of size 0x1C.  
   - 0xC blocks → If type=2, references chunked data of size 0xC.  
   - 0x10 blocks → If type=4 or 5, references chunked data of size 0x10.

5. **IDs / “Tags”**  
   In function “parse_blb_header” and “more_parsing_of_blb,” there are references to IDs:  
   - 0x65, 0x66, 100 (0x64), 200 (0xC8), 300 (0x12C), 400 (0x190), 500 (0x1F4), 600 (0x258), 700 (0x2BC), 0x259, 0x25A, etc.  
   - These appear to be sub-chunk or “asset” IDs within the BLB, each pointing to some memory offset for that data.  
   - For instance, if “uVar7 == 600,” store a pointer in “something->field16_0x40,” etc.  
   - Another example: if “uVar7 == 0x25A,” store in “field20_0x50.”  

These IDs likely represent distinct data blocks:
- 0x191 (“⟳???”)  
- 0x1F5, 0x1F6, 0x1F7, 0x1F8 (“cutscene” blocks or advanced data)  
- 0x259, 0x25A, 0x258 (some special resources, e.g. a sound or a texture block).
  
--------------------------------------------------------------------------------
## Parsing Flow

From the code, a typical parse sequence involves:
1. **Locate BLB base pointer**  
   Usually stored in something like “blb_header_ptr” or “blb_base_ptr.”

2. **Calculate an offset**  
   offset = blb_header_ptr + (byte)(someField) + 0xF36 or 0xF92, etc.

3. **Check the “type/ID”**  
   The byte at offset (0xF36) drives logic (switch statements like “case 1, 2, 3, 4, 5”).

4. **Use the byte at offset (0xF92) as an index**  
   Multiply by a block size (0x1C, 0x0C, 0x70, 0x10, etc.) to locate a sub-array. Then read sub-fields or parameters from that sub-array (like offsets, sector counts, file references, memory pointers, etc.).

5. **Extract more data**  
   Code reading various “IDs” or chunk markers (0x65, 0x66, 300, 400, 500, 600, etc.) in a loop. Each ID leads to storing a pointer or length in the final structure.

6. **Load or interpret**  
   The loaded pointers or offsets might be used to decompress data, load a “.STR” (movie), or parse further sub-chunks.  

--------------------------------------------------------------------------------
## Potential Magic or Signature Values

No obvious plain-text signatures (like “BLB\0”) appear in the code. Instead, the “magic” references revolve around these numeric offsets (0xF36, 0xF92, etc.). The only textual references are for strings like “DREA,” “PIRA,” “SCI,” or “INT1_BLB,” “CRD1_BLB,” “MVDWI.STR” which may be actual asset names. Also references “END2_BLB,” “INT1_BLB,” “PIRA2_BLB,” etc. These presumably identify big blocks inside the BLB.

--------------------------------------------------------------------------------
## Typical Examples in the Code

### Example of type=1 (0xF36 = 1)  

• The code calls “(index * 0x1C) + 0xB64,” possibly retrieving a pointer to “MVDWI.STR” or some string that references a movie file.  
• Another pointer is loaded from “(index * 0x1C) + 0xB60,” and so forth.  

### Example of type=2 (0xF36 = 2)  

• Uses “(index * 0x0C) + 0xF1C.”  
• Data might detail sector offsets for streaming a chunk from disc.  

### Example of type=3 (0xF36 = 3)  

• Uses “(index * 0x70).”  
• The code sets up some “uVar6, uVar7 = *(some16BitData).”  
• Possibly more complex chunk referencing.

### Example of type=4/5 (0xF36 = 4 or 5)  

• Uses “(index * 0x10) + 0xCD3.”  
• Possibly advanced or compressed data blocks.

--------------------------------------------------------------------------------
## Data Structures

Multiple structures are used in the code, like “astruct_8,” “astruct_5,” “astruct_14,” etc. Key fields:

• “game_blb_ptr” (int)  
  - Usually points to the start of the BLB data in memory.  

• “blb_header_ptr” (int)  
  - Another pointer referencing the early offset in the BLB.  

• “blb_level_” (byte) or “set_to_a_on_start” (byte)  
  - Some level or iteration index.  

• Offsets or pointers to sub-chunks  
  - The code sets fields in the struct to pointers after reading certain IDs. For example, “field30_0x6c,” “field18_0x48,” etc.  

--------------------------------------------------------------------------------
## Summary of Observations

1. The BLB format is not trivially labeled by a 4-byte ASCII signature, but rather controlled by values stored at offsets 0xF36 and 0xF92 within the data.  
2. 0xF36 is the main “Type/ID” controlling how data is parsed (1 through 6 are the main values).  
3. 0xF92 is a sub-index that, when multiplied by a block size (0x1C, 0x0C, 0x70, or 0x10), references additional sub-chunks.  
4. Many sub-chunk IDs exist (up to hundreds, e.g., 300, 400, 500, 600, 700, 0x259, 0x25A, etc.), each referencing different data or instructions.  
5. The code calls functions that load or decompress these sub-chunks for use in the game.  
6. “Movie” or streaming logic references addresses like “0xB64,” “MVDWI.STR,” or “DREA/PIRA/SCI,” possibly referencing cinematics or audio streams.  

--------------------------------------------------------------------------------
## Notes on Using the Format

• To parse a BLB file manually:  
  1) Read offset 0xF36 to get the “type.”  
  2) Read offset 0xF92 for the “sub-index.”  
  3) Depending on the type, compute the base using “sub-index * (block size) + some offset.”  
  4) Within that sub-block, read 2-byte or 4-byte values that represent addresses, sizes, or asset IDs.  
  5) If needed, reference other structures for chunk IDs like 300, 0x12C, 0xF4, 0x1F4, etc.

• The presence of multiple “sub-chunk IDs” within the same BLB header suggests a flexible container format. The game engine uses these IDs to find and map the correct data segment.  

--------------------------------------------------------------------------------

This document is based on reverse-engineered logic from “blb.c.” Due to the nature of decompilation, some naming or interpretations may be speculative. However, the consistent references to type codes at 0xF36, sub-index usage at 0xF92, and sub-chunk IDs appear to be the core of how the BLB format is parsed in this Playstation game.
