// +build !matprofile
// +build !matunsafe

package gocv

/*
#include <stdlib.h>
#include "core.h"
*/
import "C"
import (
	"runtime"
)

type SafeMat interface {
	// SetPtr sets the Mat's underlying object pointer.
	// Be careful to use this function. Use it unless you known what you are doing.
	// If you need to acquire a pointer, use `AcquirePtr`.
	SetPtr(p C.Mat) C.Mat

	// Release the ownership of the pointer p
	ReleasePtr() C.Mat

	// Acquire the ownership of the pointer p
	AcquirePtr(p C.Mat)
}

// addMatToProfile does nothing if matprofile tag is not set.
func addMatToProfile(p C.Mat) {
}

// newMat returns a new Mat from a C Mat
func newMat(p C.Mat) Mat {
	m := mat{p: p}
	runtime.SetFinalizer(&m, func(m *mat) {
		m.Close()
	})
	return &m
}

// Close the Mat object.
func (m *mat) Close() error {
	if m.p != nil {
		C.Mat_Close(m.p)
		m.p = nil
		m.d = nil
	}

	return nil
}

// SetPtr sets the Mat's underlying object pointer.
// Be careful to use this function. Use it unless you known what you are doing.
// If you need to acquire a pointer, use `AcquirePtr`.
func (m *mat) SetPtr(p C.Mat) C.Mat {
	p, m.p = m.p, p
	return p
}

// Release the ownership of the pointer p
func (m *mat) ReleasePtr() C.Mat {
	p := m.p
	m.p = nil
	return p
}

// Acquire the ownership of the pointer p
func (m *mat) AcquirePtr(p C.Mat) {
	if m.p != nil {
		C.Mat_Close(m.p)
	}
	m.p = p
}
