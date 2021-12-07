// Auto-generated file. Do not edit!
//   Template: src/f32-qs8-vcvt/sse.c.in
//   Generator: tools/xngen
//
// Copyright 2021 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <emmintrin.h>

#include <xnnpack/common.h>
#include <xnnpack/vcvt.h>


void xnn_f32_qs8_vcvt_ukernel__sse2_x8(
    size_t n,
    const float* x,
    int8_t* y,
    const union xnn_f32_qs8_cvt_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_DISABLE_TSAN XNN_DISABLE_MSAN
{
  assert(n != 0);

  const __m128 vscale = _mm_load_ps(params->sse2.scale);
  const __m128 voutput_max_less_zero_point = _mm_load_ps(params->sse2.output_max_less_zero_point);
  const __m128i voutput_zero_point = _mm_load_si128((const __m128i*) params->sse2.output_zero_point);
  const __m128i voutput_min = _mm_load_si128((const __m128i*) params->sse2.output_min);

  for (; n >= 8 * sizeof(int8_t); n -= 8 * sizeof(int8_t)) {
    __m128 vx_lo = _mm_loadu_ps(x);
    __m128 vx_hi = _mm_loadu_ps(x + 4);
    x += 8;

    vx_lo = _mm_mul_ps(vx_lo, vscale);
    vx_hi = _mm_mul_ps(vx_hi, vscale);

    vx_lo = _mm_min_ps(vx_lo, voutput_max_less_zero_point);
    vx_hi = _mm_min_ps(vx_hi, voutput_max_less_zero_point);

    const __m128i vy_lo = _mm_cvtps_epi32(vx_lo);
    const __m128i vy_hi = _mm_cvtps_epi32(vx_hi);

    __m128i vy = _mm_packs_epi32(vy_lo, vy_hi);
    vy = _mm_adds_epi16(vy, voutput_zero_point);
    vy = _mm_max_epi16(vy, voutput_min);
    vy = _mm_packs_epi16(vy, vy);

    _mm_storel_epi64((__m128i*) y, vy);
    y += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    __m128 vx_lo = _mm_loadu_ps(x);
    const float* x_hi = x + 4;
    if XNN_UNPREDICTABLE((n & (4 * sizeof(int8_t))) == 0) {
      x_hi = x;
    }
    __m128 vx_hi = _mm_loadu_ps(x_hi);

    vx_lo = _mm_mul_ps(vx_lo, vscale);
    vx_hi = _mm_mul_ps(vx_hi, vscale);

    vx_lo = _mm_min_ps(vx_lo, voutput_max_less_zero_point);
    vx_hi = _mm_min_ps(vx_hi, voutput_max_less_zero_point);

    const __m128i vy_lo = _mm_cvtps_epi32(vx_lo);
    const __m128i vy_hi = _mm_cvtps_epi32(vx_hi);

    __m128i vy = _mm_packs_epi32(vy_lo, vy_hi);
    vy = _mm_adds_epi16(vy, voutput_zero_point);
    vy = _mm_max_epi16(vy, voutput_min);
    vy = _mm_packs_epi16(vy, vy);

    if (n & (4 * sizeof(int8_t))) {
      *((uint32_t*) y) = (uint32_t) _mm_cvtsi128_si32(vy);
      y += 4;
      vy = _mm_srli_epi64(vy, 32);
    }
    {
      uint32_t vy_lo = (uint32_t) _mm_cvtsi128_si32(vy);
      if (n & (2 * sizeof(int8_t))) {
        *((uint16_t*) y) = (uint16_t) vy_lo;
        y += 2;
        vy_lo >>= 16;
      }
      if (n & (1 * sizeof(int8_t))) {
        *y = (int8_t) vy_lo;
      }
    }
  }
}
