// Auto-generated file. Do not edit!
//   Template: src/f32-qs8-vcvt/neon.c.in
//   Generator: tools/xngen
//
// Copyright 2021 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/common.h>
#include <xnnpack/intrinsics-polyfill.h>
#include <xnnpack/vcvt.h>


void xnn_f32_qu8_vcvt_ukernel__neon_x8(
    size_t n,
    const float* x,
    uint8_t* y,
    const union xnn_f32_qu8_cvt_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_DISABLE_TSAN XNN_DISABLE_MSAN
{
  assert(n != 0);
  assert(x != NULL);
  assert(y != NULL);

  const float32x4_t vscale = vld1q_dup_f32(&params->neon.scale);
  const float32x4_t vmagic_bias = vld1q_dup_f32(&params->neon.magic_bias);
  const int32x4_t vmagic_bias_less_zero_point = vld1q_dup_s32(&params->neon.magic_bias_less_zero_point);
  const uint8x8_t voutput_min = vld1_dup_u8(&params->neon.output_min);
  const uint8x8_t voutput_max = vld1_dup_u8(&params->neon.output_max);
  for (; n >= 8 * sizeof(uint8_t); n -= 8 * sizeof(uint8_t)) {
    float32x4_t vx_lo = vld1q_f32(x); x += 4;
    float32x4_t vx_hi = vld1q_f32(x); x += 4;

    vx_lo = vmulq_f32(vx_lo, vscale);
    vx_hi = vmulq_f32(vx_hi, vscale);

    vx_lo = vaddq_f32(vx_lo, vmagic_bias);
    vx_hi = vaddq_f32(vx_hi, vmagic_bias);

    const int32x4_t vacc_lo = vqsubq_s32(vreinterpretq_s32_f32(vx_lo), vmagic_bias_less_zero_point);
    const int32x4_t vacc_hi = vqsubq_s32(vreinterpretq_s32_f32(vx_hi), vmagic_bias_less_zero_point);

    const int16x8_t vacc = vcombine_s16(vqmovn_s32(vacc_lo), vqmovn_s32(vacc_hi));

    uint8x8_t vy = vqmovun_s16(vacc);
    vy = vmax_u8(vy, voutput_min);
    vy = vmin_u8(vy, voutput_max);
    vst1_u8(y, vy); y += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    assert(n >= 1 * sizeof(uint8_t));
    assert(n <= 7 * sizeof(uint8_t));
    float32x4_t vx_lo = vld1q_f32(x);
    const float* x_hi = x + 4;
    if XNN_UNPREDICTABLE((n & (4 * sizeof(uint8_t))) == 0) {
      x_hi = x;
    }
    float32x4_t vx_hi = vld1q_f32(x_hi);

    vx_lo = vmulq_f32(vx_lo, vscale);
    vx_hi = vmulq_f32(vx_hi, vscale);

    vx_lo = vaddq_f32(vx_lo, vmagic_bias);
    vx_hi = vaddq_f32(vx_hi, vmagic_bias);

    const int32x4_t vacc_lo = vqsubq_s32(vreinterpretq_s32_f32(vx_lo), vmagic_bias_less_zero_point);
    const int32x4_t vacc_hi = vqsubq_s32(vreinterpretq_s32_f32(vx_hi), vmagic_bias_less_zero_point);

    const int16x8_t vacc = vcombine_s16(vqmovn_s32(vacc_lo), vqmovn_s32(vacc_hi));

    uint8x8_t vy = vqmovun_s16(vacc);
    vy = vmax_u8(vy, voutput_min);
    vy = vmin_u8(vy, voutput_max);

    if (n & (4 * sizeof(uint8_t))) {
      vst1_lane_u32((void*) y, vreinterpret_u32_u8(vy), 0); y += 4;
      vy = vext_u8(vy, vy, 4);
    }
    if (n & (2 * sizeof(uint8_t))) {
      vst1_lane_u16((void*) y, vreinterpret_u16_u8(vy), 0); y += 2;
      vy = vext_u8(vy, vy, 2);
    }
    if (n & (1 * sizeof(uint8_t))) {
      vst1_lane_u8(y, vy, 0);
    }
  }
}
