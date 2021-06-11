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
	"sync/atomic"
	"unsafe"
)

type safeMat interface {
	// setPtr sets the Mat's underlying object pointer.
	// Be careful to use this function. Use it unless you known what you are doing.
	// If you need to acquire a pointer, use `AcquirePtr`.
	setPtr(p C.Mat)

	// Return data slice
	getD() []byte

	// Set data slice
	setD(d []byte)

	// Release the ownership of the pointer p
	releasePtr() C.Mat

	// Acquire the ownership of the pointer p
	acquirePtr(p C.Mat)
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
	p := atomic.SwapPointer((*unsafe.Pointer)(&m.p), nil)
	if p != nil {
		C.Mat_Close(C.Mat(p))
	}

	return nil
}

// setPtr sets the Mat's underlying object pointer.
// Be careful to use this function. Use it unless you known what you are doing.
// If you need to acquire a pointer, use `AcquirePtr`.
func (m *mat) setPtr(p C.Mat) {
	m.p = p
}

// Return data slice
func (m *mat) getD() []byte {
	return m.d
}

// Set data slice
func (m *mat) setD(d []byte) {
	m.d = d
}

// Release the ownership of the pointer p
func (m *mat) releasePtr() C.Mat {
	p := m.p
	m.p = nil
	return p
}

// Acquire the ownership of the pointer p
func (m *mat) acquirePtr(p C.Mat) {
	if m.p != nil {
		C.Mat_Close(m.p)
	}
	m.p = p
}
