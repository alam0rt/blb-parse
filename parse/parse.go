// Package parse provides the functionality to read, parse, and serialize the BLB header data.

package parse

import (
	"bytes"
	"encoding/binary"
	"errors"
)

// BLBTemplateHeader is the top-level structure describing the BLB header.
type BLBTemplateHeader struct {
	Levels         []Level         `json:"levels"`
	Movies         Movies          `json:"movies"`
	LoadingScreens []LoadingScreen `json:"loading_screens"`
	LeftoverFinal  []byte          `json:"-"` // leftover after loading screens
}

// Movies is the 0x26-byte region at offset 0xB60
type Movies struct {
	Data [0x26]byte
}

// The size of each "sector"
const SectorSize = 0x800

// Byte-based types for ID fields
type Byte5 [5]byte
type Byte4 [4]byte
type Byte21 [21]byte

// Base64Data can hold embedded data for code that needs it
type Base64Data string

// Level describes each "level" entry
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

// rawLevel is the 0x70 byte structure used for reading/writing each Level.
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

// LoadingScreen is used to parse the array of 32 loading screens in the header.
type LoadingScreen struct {
	Offset   uint16 `json:"offset"`
	Size     uint16 `json:"size"`
	Unknown  byte   `json:"unknown"`
	Unknown1 byte   `json:"unknown1"`
	Unknown2 byte   `json:"unknown2"`
	ID       Byte5  `json:"id"`
	Name     Byte4  `json:"name"`
}

/*************************************************************************
 *                         PARSING
 *************************************************************************/

// ParseHeaderBin reads only levels, movies (0x26 bytes), & loading screens.
func ParseHeaderBin(data []byte) (*BLBTemplateHeader, error) {
	if len(data) < 0x1000 {
		return nil, errors.New("data must be at least 0x1000 bytes")
	}
	data = data[:0x1000]

	hdr := &BLBTemplateHeader{}
	offset := 0

	// parse all possible levels
	for {
		if offset+0x70 > 0x1000 {
			break
		}
		if data[offset] == 0 {
			break
		}
		var raw rawLevel
		if err := bytesToStruct(data[offset:offset+0x70], &raw); err != nil {
			return nil, err
		}
		offset += 0x70
		hdr.Levels = append(hdr.Levels, rawToLevel(raw))
	}

	// skip up to 0xb60
	if offset < 0xb60 {
		offset = 0xb60
	}

	// parse movies if we have enough space for 0x26 bytes
	if offset+0x26 <= 0xb86 {
		copy(hdr.Movies.Data[:], data[offset:offset+0x26])
		offset += 0x26
	}

	// skip up to 0xb86 for loading screens
	if offset < 0xb86 {
		offset = 0xb86
	}

	// parse loading screens
	screenOffset := offset
	for {
		if screenOffset+16 > 0x1000 {
			break
		}
		if data[screenOffset] == 0 {
			break
		}
		buf := data[screenOffset : screenOffset+16]
		ls := LoadingScreen{
			Offset:   readU16(buf[0:2]),
			Size:     readU16(buf[2:4]),
			Unknown:  buf[4],
			Unknown1: buf[5],
			Unknown2: buf[6],
		}
		copy(ls.ID[:], buf[7:12])
		copy(ls.Name[:], buf[12:16])
		hdr.LoadingScreens = append(hdr.LoadingScreens, ls)
		screenOffset += 16
	}

	// leftover data
	if screenOffset < 0x1000 {
		hdr.LeftoverFinal = data[screenOffset:0x1000]
	}

	return hdr, nil
}

/*************************************************************************
 *                        SERIALIZATION
 *************************************************************************/

// SerializeHeaderJSON writes levels, the 0x26-byte movies region, and loading screens.
func SerializeHeaderJSON(hdr *BLBTemplateHeader) []byte {
	buf := make([]byte, 0x1000)

	offset := 0

	// 1) Write levels
	for _, lvl := range hdr.Levels {
		if offset+0x70 >= 0x1000 {
			break
		}
		raw := levelToRaw(lvl)
		copy(buf[offset:], structToBytes(raw))
		offset += 0x70
	}

	// skip to 0xb60
	if offset < 0xb60 {
		for i := offset; i < 0xb60 && i < 0x1000; i++ {
			buf[i] = 0
		}
		offset = 0xb60
	}

	// 2) write movies if we have space for 0x26 bytes
	if offset+0x26 <= 0xb86 {
		copy(buf[offset:], hdr.Movies.Data[:])
		offset += 0x26
	}

	// skip to 0xb86
	if offset < 0xb86 {
		for i := offset; i < 0xb86 && i < 0x1000; i++ {
			buf[i] = 0
		}
		offset = 0xb86
	}

	// 3) write loading screens
	for i := 0; i < len(hdr.LoadingScreens) && i < 32; i++ {
		if offset+16 > 0x1000 {
			break
		}
		temp := loadScreenToBytes(hdr.LoadingScreens[i])
		copy(buf[offset:], temp)
		offset += 16
	}

	// leftover
	if offset < 0x1000 && len(hdr.LeftoverFinal) > 0 {
		end := offset + len(hdr.LeftoverFinal)
		if end > 0x1000 {
			end = 0x1000
		}
		copy(buf[offset:end], hdr.LeftoverFinal[:end-offset])
	}

	return buf
}

/*************************************************************************
 *                      HELPER FUNCTIONS
 *************************************************************************/

func rawToLevel(r rawLevel) Level {
	return Level{
		Offset:        r.Offset,
		Size:          r.Size,
		Static:        r.Static,
		Index:         r.Index,
		SomeFlag:      r.SomeFlag,
		Pad1:          r.Pad1,
		HiddenShort1:  r.HiddenShort1,
		SomeOffset:    r.SomeOffset,
		SomeShort2:    r.SomeShort2,
		Dynamic:       r.Dynamic,
		SomeSize:      r.SomeSize,
		ShortA:        r.ShortA,
		ShortB:        r.ShortB,
		ShortC:        r.ShortC,
		ShortD:        r.ShortD,
		ShortE:        r.ShortE,
		HiddenShort2:  r.HiddenShort2,
		SomeOtherOff:  r.SomeOtherOff,
		ShortF:        r.ShortF,
		MoreDynamic:   r.MoreDynamic,
		SomeOtherSize: r.SomeOtherSize,
		ShortG:        r.ShortG,
		ShortH:        r.ShortH,
		ShortI:        r.ShortI,
		ShortJ:        r.ShortJ,
		ShortK:        r.ShortK,
		ShortL:        r.ShortL,
		ID:            Byte5(r.ID),
		Name:          Byte21(r.Name),
	}
}

func levelToRaw(l Level) rawLevel {
	return rawLevel{
		Offset:        l.Offset,
		Size:          l.Size,
		Static:        l.Static,
		Index:         l.Index,
		SomeFlag:      l.SomeFlag,
		Pad1:          l.Pad1,
		HiddenShort1:  l.HiddenShort1,
		SomeOffset:    l.SomeOffset,
		SomeShort2:    l.SomeShort2,
		Dynamic:       l.Dynamic,
		SomeSize:      l.SomeSize,
		ShortA:        l.ShortA,
		ShortB:        l.ShortB,
		ShortC:        l.ShortC,
		ShortD:        l.ShortD,
		ShortE:        l.ShortE,
		HiddenShort2:  l.HiddenShort2,
		SomeOtherOff:  l.SomeOtherOff,
		ShortF:        l.ShortF,
		MoreDynamic:   l.MoreDynamic,
		SomeOtherSize: l.SomeOtherSize,
		ShortG:        l.ShortG,
		ShortH:        l.ShortH,
		ShortI:        l.ShortI,
		ShortJ:        l.ShortJ,
		ShortK:        l.ShortK,
		ShortL:        l.ShortL,
		ID:            [5]byte(l.ID),
		Name:          [21]byte(l.Name),
	}
}

func bytesToStruct(src []byte, dst interface{}) error {
	return binary.Read(bytes.NewBuffer(src), binary.LittleEndian, dst)
}

func structToBytes(src interface{}) []byte {
	var buf bytes.Buffer
	_ = binary.Write(&buf, binary.LittleEndian, src)
	return buf.Bytes()
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

// readU16 reads a 16-bit little-endian value
func readU16(b []byte) uint16 {
	return binary.LittleEndian.Uint16(b)
}
