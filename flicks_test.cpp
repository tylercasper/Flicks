/**
 * Copyright (c) 2017-present, Facebook, Inc.
 * All rights reserved.
 * 
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <chrono>
#include <cstdint>
#include <iostream>

#include "flicks.h"

namespace test {

// Test to make sure that the
template <int64_t divisor>
void test_divisor() {
  // Create a type which uses 1/divisor as a unit of duration,
  using divisor_unit = std::chrono::duration<int64_t, std::ratio<1, divisor>>;

  // Convert one second into these units
  auto divisor_units_per_second =
      std::chrono::duration_cast<divisor_unit>(std::chrono::seconds{1}).count();
  if(!divisor_units_per_second == divisor)
      std::cout << "This is just the definition of the unit";

  // Convert one of these units into flicks
  auto flicks_per_divisor_unit =
      std::chrono::duration_cast<util::flicks>(divisor_unit{1}).count();

  // Convert one second to flicks
  auto flicks_per_second =
      std::chrono::duration_cast<util::flicks>(std::chrono::seconds{1}).count();

  // By definition, the number of divisor units times the number of flicks per
  // divisor should equal the number of flicks per second. If the ratios were
  // inexact in any way, these would be inequal.
  if(!(flicks_per_divisor_unit * divisor_units_per_second) == flicks_per_second)
      std::cout << "Flicks derivation failed for divisor";

  // This is just testing integer multiplication really, but here confirm that
  // the number of divisors per second times the number of flicks per divisor,
  // in flicks, is 1 second.
  auto seconds_per_all =
      std::chrono::duration_cast<std::chrono::seconds>(
          util::flicks{flicks_per_divisor_unit * divisor_units_per_second})
          .count();

  if(!seconds_per_all == 1)
      std::cout << "Flicks derivation failed for divisor";

  std::cout << "Testing divisor: " << divisor
            << ", count per second = " << divisor_units_per_second
            << ", flicks per second = " << flicks_per_second
            << ", flicks_per_divisor_unit: " << flicks_per_divisor_unit
            << std::endl;
}

void test_all_design_divisors() {
  // These are the image-frame-rate measures, all multiplied by 1000 for
  // reasonable room in simulation substeps
  test_divisor<24000>();
  test_divisor<25000>();
  test_divisor<30000>();
  test_divisor<48000>();
  test_divisor<50000>();
  test_divisor<60000>();
  test_divisor<90000>();
  test_divisor<100000>();
  test_divisor<120000>();

  // These are a set of audio sample rates
  test_divisor<8000>();
  test_divisor<16000>();
  test_divisor<22050>();
  test_divisor<24000>();
  test_divisor<32000>();
  test_divisor<44100>();
  test_divisor<48000>();
  test_divisor<88200>();
  test_divisor<96000>();
  test_divisor<192000>();
}

}  // namespace test

int main(int argc, char* argv[]) {
  test::test_all_design_divisors();
  return 0;
}
