// Copyright 2014 Olivier Gillet.
//
// Author: Olivier Gillet (pichenettes@mutable-instruments.net)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Resonator.

#ifndef ELEMENTS_DSP_RESONATOR_H_
#define ELEMENTS_DSP_RESONATOR_H_

#include "stmlib/stmlib.h"

#include <cmath>
#include <algorithm>

#include "elements/dsp/dsp.h"
#include "stmlib/dsp/filter.h"
#include "stmlib/dsp/delay_line.h"

namespace elements {

#if defined(ELEMENTS_RESONATOR_MODES)
const size_t kMaxModes = ELEMENTS_RESONATOR_MODES;
#else
const size_t kMaxModes = 24;
#endif
const size_t kMaxBowedModes = 2;
const size_t kMaxDelayLineSize = 1024;

//const size_t kMaxModes = 24;
//const size_t kMaxBowedModes = 2;
//const size_t kMaxDelayLineSize = 512;

class Resonator {
 public:
  Resonator() { }
  ~Resonator() { }
  
  void Init();
  void Process(
      const float* bow_strength,
      const float* in,
      float* center,
      float* sides,
      size_t size);
  
  inline void set_frequency(float frequency) {
    frequency_ = frequency;
  }
  
  inline void set_geometry(float geometry) {
    geometry_ = geometry;
  }
  
  inline void set_brightness(float brightness) {
    brightness_ = brightness;
  }
  
  inline void set_damping(float damping) {
    damping_ = damping;
  }
  
  inline void set_position(float position) {
    position_ = position;
  }
  
  inline void set_resolution(size_t resolution) {
    resolution_ = previous_num_modes_ = std::min(resolution, kMaxModes);
  }
  
  inline void set_modulation_frequency(float modulation_frequency) {
    modulation_frequency_ = modulation_frequency;
  }

  inline void set_modulation_offset(float modulation_offset) {
    modulation_offset_ = modulation_offset;
  }
  
  inline float BowTable(float x, float velocity) const {
    x = 0.13f * velocity - x;
    float bow = x;
    bow *= 6.0f;
    bow = fabs(bow) + 0.75;
    bow *= bow;
    bow *= bow;
    bow = 0.25f / bow;
    if (bow < 0.0025f) bow = 0.0025f;
    if (bow > 0.245f) bow = 0.245f;
    return x * bow;
  }
  
 private:
  size_t ComputeFilters();
  
  float frequency_;
  float geometry_;
  float brightness_;
  float position_;
  float previous_position_;
  float damping_;
  
  float modulation_frequency_;
  float modulation_offset_;
  float lfo_phase_;

  float bow_signal_;
  
  size_t resolution_;
  size_t num_modes_;
  size_t previous_num_modes_;

  float harmonic_;
  float stretch_factor_;
  float stiffness_;
  float q_;
  float q_loss_;
  float q_loss_damping_rate_;
  
  stmlib::Svf f_[kMaxModes];
  stmlib::Svf f_bow_[kMaxBowedModes];
  stmlib::DelayLine<float, kMaxDelayLineSize> d_bow_[kMaxBowedModes];
  
  size_t clock_divider_;
  
  DISALLOW_COPY_AND_ASSIGN(Resonator);
};

}  // namespace elements

#endif  // ELEMENTS_DSP_RESONATOR_H_
