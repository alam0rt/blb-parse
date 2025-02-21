// Moved the test into the "main_test" package so it can properly import "github.com/alam0rt/blb-parse/parse"
// without conflicts. This assumes your go.mod is initialized with "github.com/alam0rt/blb-parse".

package main_test

import (
	"bytes"
	"os"
	"testing"

	"github.com/alam0rt/blb-parse/parse"
)

func TestHeaderBinRoundTrip(t *testing.T) {
	data, err := os.ReadFile("header.bin")
	if err != nil {
		t.Skipf("header.bin not found, skipping test: %v", err)
	}
	if len(data) < 0x1000 {
		t.Fatalf("header.bin is too small: got %d bytes, need at least 0x1000", len(data))
	}

	hdr, err := parse.ParseHeaderBin(data)
	if err != nil {
		t.Fatalf("parse.ParseHeaderBin failed: %v", err)
	}

	got := parse.SerializeHeaderJSON(hdr)
	want := data[:0x1000] // focus on the first 0x1000 region

	if len(got) != len(want) {
		t.Fatalf("mismatch in length: got %d, want %d", len(got), len(want))
	}

	// dump the serialized bin for debugging
	if err := os.WriteFile("generated.bin", got, 0644); err != nil {
		t.Fatalf("write generated.bin: %v", err)
	}

	if !bytes.Equal(got, want) {
		for i := 0; i < len(got) && i < len(want); i++ {
			if got[i] != want[i] {
				t.Errorf("Mismatch at offset 0x%X: got 0x%02X, want 0x%02X",
					i, got[i], want[i])
				break
			}
		}
		t.Errorf("parse.ParseHeaderBin -> parse.SerializeHeaderJSON mismatch from header.bin")
	}
}

func TestHeaderBinTruncated(t *testing.T) {
	data, err := os.ReadFile("header.bin")
	if err != nil {
		t.Skip("header.bin not found, skipping test.")
	}
	if len(data) < 0x1000 {
		t.Fatalf("header.bin too small: got %d bytes, want at least 0x1000", len(data))
	}

	hdr, err := parse.ParseHeaderBin(data)
	if err != nil {
		t.Fatalf("parse.ParseHeaderBin failed: %v", err)
	}

	// Keep only the first level
	if len(hdr.Levels) > 1 {
		hdr.Levels = hdr.Levels[:1]
	}

	// Keep only the first loading screen
	var truncatedScreens []parse.LoadingScreen
	if len(hdr.LoadingScreens) > 0 {
		truncatedScreens = append(truncatedScreens, hdr.LoadingScreens[0])
	}
	hdr.LoadingScreens = truncatedScreens

	got := parse.SerializeHeaderJSON(hdr)
	if len(got) != 0x1000 {
		t.Fatalf("unexpected truncated header length: got %d, want 0x1000", len(got))
	}

	// dump the serialized bin for debugging
	if err := os.WriteFile("generated_trunc.bin", got, 0644); err != nil {
		t.Fatalf("write generated_trunc.bin: %v", err)
	}

	t.Logf("Truncated header re-serialized OK, length = %d bytes", len(got))
}
