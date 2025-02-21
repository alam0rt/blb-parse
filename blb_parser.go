// This version ensures names are output as strings in JSON rather than raw byte arrays.
// We define custom types Byte5, Byte21, etc. with MarshalJSON methods to convert
// the byte slices to trimmed strings.

package main

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"io"
	"os"
)

// The .bt template uses a "SECTORS" size of 0x800 (2048).
const SECTOR_SIZE = 0x800

// Custom types to handle byte arrays but serialize as strings
type Byte5 [5]byte
type Byte21 [21]byte
type Byte4 [4]byte

func (b Byte5) String() string {
	return string(bytes.TrimRight(b[:], "\x00"))
}
func (b Byte5) MarshalJSON() ([]byte, error) {
	return json.Marshal(b.String())
}

func (b Byte21) String() string {
	return string(bytes.TrimRight(b[:], "\x00"))
}
func (b Byte21) MarshalJSON() ([]byte, error) {
	return json.Marshal(b.String())
}

func (b Byte4) String() string {
	return string(bytes.TrimRight(b[:], "\x00"))
}
func (b Byte4) MarshalJSON() ([]byte, error) {
	return json.Marshal(b.String())
}

// readNullTerminatedString reads bytes from r until a 0 byte is encountered or EOF.
func readNullTerminatedString(r io.Reader) (string, error) {
	var buf []byte
	tmp := make([]byte, 1)
	for {
		n, err := r.Read(tmp)
		if err != nil {
			if err == io.EOF && len(buf) > 0 {
				return string(buf), nil
			}
			return string(buf), err
		}
		if n < 1 {
			break
		}
		if tmp[0] == 0 {
			break
		}
		buf = append(buf, tmp[0])
	}
	return string(buf), nil
}

// Level struct with custom ByteN fields for ID and Name
type Level struct {
	Offset   uint16  `json:"offset"`
	Size     uint16  `json:"size"`
	Static   [8]byte `json:"static"`
	Index    byte    `json:"index"`
	SomeFlag byte    `json:"some_flag"`

	Pad1          [14]byte `json:"pad1"`
	HiddenShort1  int16    `json:"hiddenShort1"`
	SomeOffset    uint16   `json:"someOffset"`
	SomeShort2    int16    `json:"someShort2"`
	Dynamic       [10]byte `json:"dynamic"`
	SomeSize      uint16   `json:"someSize"`
	ShortA        int16    `json:"shortA"`
	ShortB        int16    `json:"shortB"`
	ShortC        int16    `json:"shortC"`
	ShortD        int16    `json:"shortD"`
	ShortE        int16    `json:"shortE"`
	HiddenShort2  int16    `json:"hiddenShort2"`
	SomeOtherOff  uint16   `json:"someOtherOff"`
	ShortF        int16    `json:"shortF"`
	MoreDynamic   [10]byte `json:"moreDynamic"`
	SomeOtherSize uint16   `json:"someOtherSize"`
	ShortG        int16    `json:"shortG"`
	ShortH        int16    `json:"shortH"`
	ShortI        int16    `json:"shortI"`
	ShortJ        int16    `json:"shortJ"`
	ShortK        int16    `json:"shortK"`
	ShortL        int16    `json:"shortL"`
	ID            Byte5    `json:"id"`
	Name          Byte21   `json:"name"`
}

// We store each discovered "movie reference" as string data.
type Movie string

// 32 loading screens with custom ByteN for ID and Name
type LoadingScreen struct {
	Offset   uint16 `json:"offset"`
	Size     uint16 `json:"size"`
	Unknown  byte   `json:"unknown"`
	Unknown1 byte   `json:"unknown1"`
	Unknown2 byte   `json:"unknown2"`
	ID       Byte5  `json:"id"`
	Name     Byte4  `json:"name"`
}

type BLBTemplateHeader struct {
	Levels         []Level           `json:"levels"`
	Movies         []Movie           `json:"movies"`
	LoadingScreens [32]LoadingScreen `json:"loading_screens"`
}

func parseBTHeader(f *os.File) (*BLBTemplateHeader, error) {
	hdr := &BLBTemplateHeader{}

	// 1) Parse “Levels” from offset 0 until zero byte
	if err := seekTo(f, 0); err != nil {
		return nil, fmt.Errorf("seek to 0: %w", err)
	}
	for {
		var b [1]byte
		n, err := f.Read(b[:])
		if err != nil && err != io.EOF {
			return nil, fmt.Errorf("level sentinel read error: %w", err)
		}
		if n < 1 {
			break
		}
		if b[0] == 0 {
			break
		}
		if _, err := f.Seek(-1, io.SeekCurrent); err != nil {
			return nil, fmt.Errorf("rollback in levels: %w", err)
		}
		var lvl Level
		if err := binary.Read(f, binary.LittleEndian, &lvl); err != nil {
			return nil, fmt.Errorf("reading Level: %w", err)
		}
		hdr.Levels = append(hdr.Levels, lvl)
	}

	// 2) Seek to offset 0xb60 for “Movies”
	if err := seekTo(f, 0xb60); err != nil {
		// If file is smaller, skip
		return hdr, nil
	}

	// Read null-terminated strings until an empty one
	for {
		movieStr, err := readNullTerminatedString(f)
		if err != nil {
			break
		}
		if len(movieStr) == 0 {
			break
		}
		hdr.Movies = append(hdr.Movies, Movie(movieStr))
	}

	// 3) read the 32 loading screens
	for i := 0; i < 32; i++ {
		var ls LoadingScreen
		e := binary.Read(f, binary.LittleEndian, &ls)
		if e != nil {
			break
		}
		hdr.LoadingScreens[i] = ls
	}

	return hdr, nil
}

func parseBTFile(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return fmt.Errorf("open file: %w", err)
	}
	defer f.Close()

	hdr, err := parseBTHeader(f)
	if err != nil {
		return fmt.Errorf("parseBTHeader: %w", err)
	}

	// Output everything as JSON
	jsonOutput, err := json.MarshalIndent(hdr, "", "  ")
	if err != nil {
		return fmt.Errorf("json marshal: %w", err)
	}

	fmt.Println(string(jsonOutput))

	return nil
}

func seekTo(f *os.File, off int64) error {
	_, err := f.Seek(off, io.SeekStart)
	return err
}

func main() {
	if err := parseBTFile("GAME.BLB"); err != nil {
		fmt.Println("Error:", err)
	}
}
