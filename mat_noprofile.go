// +build !matprofile
// +build matunsafe

package gocv

/*
#include <stdlib.h>
#include "core.h"
*/
import "C"

// addMatToProfile does nothing if matprofile tag is not set.
func addMatToProfile(p C.Mat) {
	return
}

// newMat returns a new Mat from a C Mat
func newMat(p C.Mat) Mat {
	return &mat{p: p}
}

// Close the Mat object.
func (m *mat) Close() error {
	C.Mat_Close(m.p)
	m.p = nil
	m.d = nil
	return nil
}
