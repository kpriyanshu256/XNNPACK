// Auto-generated file. Do not edit!
//   Template: src/x24-transposec/scalar.c.in
//   Generator: tools/xngen
//
// Copyright 2022 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.


#include <assert.h>

#include <xnnpack/common.h>
#include <xnnpack/math.h>
#include <xnnpack/transpose.h>

void xnn_x24_transposec_ukernel__4x1_scalar(
    const void *input,
    void * output,
    size_t input_stride,
    size_t output_stride,
    size_t block_width,
    size_t block_height)
{
  assert(output_stride >= block_height * 3);
  assert(input_stride >= block_width * 3);

  const size_t tile_height = 4;
  const size_t tile_width = 1;
  const size_t tile_wbytes = tile_width * 3;
  const size_t input_reset = tile_wbytes - round_down_po2(block_height, tile_height) * input_stride;
  const size_t output_reset = (tile_width - 1) * output_stride;
  const size_t input_offset = tile_height * input_stride;

  const uint8_t* i0 = (const uint8_t*) input;
  const uint8_t* i1 = (const uint8_t*) ((uintptr_t) i0 + input_stride);
  const uint8_t* i2 = (const uint8_t*) ((uintptr_t) i1 + input_stride);
  const uint8_t* i3 = (const uint8_t*) ((uintptr_t) i2 + input_stride);

  uint8_t* o0 = (uint8_t*) output;

  do {
    size_t bh = block_height;
    for (; bh >= 4; bh -= 4) {
      o0[0] = i0[0];
      o0[1] = i0[1];
      o0[2] = i0[2];
      o0 += 3;
      o0[0] = i1[0];
      o0[1] = i1[1];
      o0[2] = i1[2];
      o0 += 3;
      o0[0] = i2[0];
      o0[1] = i2[1];
      o0[2] = i2[2];
      o0 += 3;
      o0[0] = i3[0];
      o0[1] = i3[1];
      o0[2] = i3[2];
      o0 += 3;
      i0 = (const uint8_t*) ((uintptr_t) i0 + input_offset);
      i1 = (const uint8_t*) ((uintptr_t) i1 + input_offset);
      i2 = (const uint8_t*) ((uintptr_t) i2 + input_offset);
      i3 = (const uint8_t*) ((uintptr_t) i3 + input_offset);
    }
    const uint8_t* i = i0;
    if (bh & 2) {
      o0[0] = i0[0];
      o0[1] = i0[1];
      o0[2] = i0[2];
      o0 += 3;
      o0[0] = i1[0];
      o0[1] = i1[1];
      o0[2] = i1[2];
      o0 += 3;
      i = i2;
    }
    if (bh & 1) {
      o0[0] = i[0];
      o0[1] = i[1];
      o0[2] = i[2];
      o0 += 3;
    }

    i0 = (const uint8_t*) ((uintptr_t) i0 + input_reset);
    i1 = (const uint8_t*) ((uintptr_t) i0 + input_stride);
    i2 = (const uint8_t*) ((uintptr_t) i1 + input_stride);
    i3 = (const uint8_t*) ((uintptr_t) i2 + input_stride);
    o0 = (uint8_t*) ((uintptr_t) o0 + output_reset);
    block_width = doz(block_width, tile_width);
  } while (block_width != 0);
}
