// This update adds custom UnmarshalJSON methods for Byte5, Byte4, and Byte21 types so
// that a JSON string can be accepted and correctly converted into these fixed-size byte arrays.
//
// That way if the JSON file has something like:
//   "id": "LOGO"
// it can be converted into the internal [5]byte. If "LOGO" is longer than 5 characters, it will be truncated.

package main

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"strings"
)

const sectorSize = 0x800

type Byte5 [5]byte
type Byte4 [4]byte
type Byte21 [21]byte

func (b Byte5) String() string {
	return strings.TrimRight(string(b[:]), "\x00")
}
func (b *Byte5) UnmarshalJSON(data []byte) error {
	var s string
	if err := json.Unmarshal(data, &s); err != nil {
		return err
	}
	// Clear existing
	for i := range b {
		b[i] = 0
	}
	copy(b[:], []byte(s))
	return nil
}
func (b Byte5) MarshalJSON() ([]byte, error) {
	return json.Marshal(b.String())
}

func (b Byte4) String() string {
	return strings.TrimRight(string(b[:]), "\x00")
}
func (b *Byte4) UnmarshalJSON(data []byte) error {
	var s string
	if err := json.Unmarshal(data, &s); err != nil {
		return err
	}
	var tmp [4]byte
	copy(tmp[:], []byte(s))
	copy(b[:], tmp[:])
	return nil
}
func (b Byte4) MarshalJSON() ([]byte, error) {
	return json.Marshal(b.String())
}

func (b Byte21) String() string {
	return strings.TrimRight(string(b[:]), "\x00")
}
func (b *Byte21) UnmarshalJSON(data []byte) error {
	var s string
	if err := json.Unmarshal(data, &s); err != nil {
		return err
	}
	var tmp [21]byte
	copy(tmp[:], []byte(s))
	copy(b[:], tmp[:])
	return nil
}
func (b Byte21) MarshalJSON() ([]byte, error) {
	return json.Marshal(b.String())
}

type Base64Data string

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

	PrimaryData   Base64Data `json:"primaryData,omitempty"`
	SomeOtherData Base64Data `json:"someOtherData,omitempty"`
}

type rawLevel struct {
	Offset        uint16
	Size          uint16
	Static        [8]byte
	Index         byte
	SomeFlag      byte
	Pad1          [14]byte
	HiddenShort1  int16
	SomeOffset    uint16
	SomeShort2    int16
	Dynamic       [10]byte
	SomeSize      uint16
	ShortA        int16
	ShortB        int16
	ShortC        int16
	ShortD        int16
	ShortE        int16
	HiddenShort2  int16
	SomeOtherOff  uint16
	ShortF        int16
	MoreDynamic   [10]byte
	SomeOtherSize uint16
	ShortG        int16
	ShortH        int16
	ShortI        int16
	ShortJ        int16
	ShortK        int16
	ShortL        int16
	ID            [5]byte
	Name          [21]byte
}

type Movie string

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

func parseBTFile(path string) (*BLBTemplateHeader, error) {
	// stub
	return &BLBTemplateHeader{}, nil
}

func serializeHeaderJSON(hdr *BLBTemplateHeader) []byte {
	buf := make([]byte, 0x1000)

	offset := 0
	for _, lvl := range hdr.Levels {
		if offset+0x70 >= 0x1000 {
			break
		}
		raw := toRawLevel(lvl)
		data := rawToBytes(raw)
		copy(buf[offset:], data)
		offset += 0x70
	}

	movieOffset := 0xb60
	for _, mv := range hdr.Movies {
		mStr := string(mv)
		for i := 0; i < len(mStr); i++ {
			if movieOffset >= 0x1000 {
				break
			}
			buf[movieOffset] = mStr[i]
			movieOffset++
		}
		if movieOffset < 0x1000 {
			buf[movieOffset] = 0
			movieOffset++
		}
	}

	screenOffset := movieOffset
	if screenOffset < 0xb60 {
		screenOffset = 0xb60
	}
	for i := 0; i < 32; i++ {
		if screenOffset+16 > 0x1000 {
			break
		}
		ls := hdr.LoadingScreens[i]
		sData := loadScreenToBytes(ls)
		copy(buf[screenOffset:], sData)
		screenOffset += 16
	}

	return buf
}

func toRawLevel(lvl Level) rawLevel {
	return rawLevel{
		Offset:        lvl.Offset,
		Size:          lvl.Size,
		Static:        lvl.Static,
		Index:         lvl.Index,
		SomeFlag:      lvl.SomeFlag,
		Pad1:          lvl.Pad1,
		HiddenShort1:  lvl.HiddenShort1,
		SomeOffset:    lvl.SomeOffset,
		SomeShort2:    lvl.SomeShort2,
		Dynamic:       lvl.Dynamic,
		SomeSize:      lvl.SomeSize,
		ShortA:        lvl.ShortA,
		ShortB:        lvl.ShortB,
		ShortC:        lvl.ShortC,
		ShortD:        lvl.ShortD,
		ShortE:        lvl.ShortE,
		HiddenShort2:  lvl.HiddenShort2,
		SomeOtherOff:  lvl.SomeOtherOff,
		ShortF:        lvl.ShortF,
		MoreDynamic:   lvl.MoreDynamic,
		SomeOtherSize: lvl.SomeOtherSize,
		ShortG:        lvl.ShortG,
		ShortH:        lvl.ShortH,
		ShortI:        lvl.ShortI,
		ShortJ:        lvl.ShortJ,
		ShortK:        lvl.ShortK,
		ShortL:        lvl.ShortL,
		ID:            [5]byte(lvl.ID),
		Name:          [21]byte(lvl.Name),
	}
}

func rawToBytes(r rawLevel) []byte {
	buf := make([]byte, 0x70)
	writer := makeSliceWriter(buf)
	_ = binary.Write(writer, binary.LittleEndian, r)
	return buf
}

func loadScreenToBytes(ls LoadingScreen) []byte {
	buf := make([]byte, 16)
	binary.LittleEndian.PutUint16(buf[0:], ls.Offset)
	binary.LittleEndian.PutUint16(buf[2:], ls.Size)
	buf[4] = ls.Unknown
	buf[5] = ls.Unknown1
	buf[6] = ls.Unknown2
	copy(buf[7:], ls.ID[:])
	copy(buf[12:], ls.Name[:])
	return buf
}

func makeSliceWriter(b []byte) io.Writer {
	return &sliceWriter{buf: b}
}

type sliceWriter struct {
	buf  []byte
	offs int
}

func (s *sliceWriter) Write(p []byte) (int, error) {
	if s.offs+len(p) > len(s.buf) {
		return 0, io.ErrShortWrite
	}
	copy(s.buf[s.offs:], p)
	s.offs += len(p)
	return len(p), nil
}

func main() {
	if len(os.Args) >= 2 && os.Args[1] == "serialize-header" {
		if len(os.Args) < 3 {
			fmt.Fprintln(os.Stderr, "Usage: blb_parser serialize-header <header.json> [out.bin]")
			os.Exit(1)
		}
		jsonPath := os.Args[2]
		var outPath string
		if len(os.Args) >= 4 {
			outPath = os.Args[3]
		}

		dat, err := os.ReadFile(jsonPath)
		if err != nil {
			fmt.Fprintf(os.Stderr, "read %q: %v\n", jsonPath, err)
			os.Exit(1)
		}

		var hdr BLBTemplateHeader
		if err := json.Unmarshal(dat, &hdr); err != nil {
			fmt.Fprintf(os.Stderr, "unmarshal JSON: %v\n", err)
			os.Exit(1)
		}

		serialized := serializeHeaderJSON(&hdr)
		if outPath == "" {
			_, _ = os.Stdout.Write(serialized)
		} else {
			if err := os.WriteFile(outPath, serialized, 0644); err != nil {
				fmt.Fprintf(os.Stderr, "write %q: %v\n", outPath, err)
				os.Exit(1)
			}
		}
		return
	}

	hdr, err := parseBTFile("GAME.BLB")
	if err != nil {
		fmt.Printf("Error: %v\n", err)
		return
	}

	out, err := json.MarshalIndent(hdr, "", "  ")
	if err != nil {
		fmt.Printf("Marshal error: %v\n", err)
		return
	}
	fmt.Println(string(out))
}
