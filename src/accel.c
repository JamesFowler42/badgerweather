/*
 * Badger Weather
 *
 * Copyright (c) 2013 James Fowler/Indigo Roth
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "pebble.h"
#include "common.h"

static int level = 0;



static AppTimer *level_timer;

static void level_down(void *data) {
  // Down a level
  level--;
  new_timewarp(level);

  // If we are not back to zero then we have another level to go down, start another timer
  if (level > 0) {
    level_timer = app_timer_register(LEVEL_WAIT_MS, level_down, NULL);
  }
}

static void level_up() {

  // Up a level

  // Hit the top and had to stop and that's what's bothering me
  if (level == MAX_LEVEL) {
    return;
  }

  // First level - we need a timer to bring us back down
  if (level == 0) {
    level_timer = app_timer_register(LEVEL_WAIT_MS, level_down, NULL);
  } else {
    // If we are already up a level then the timer should be lengthened
    app_timer_reschedule (level_timer, LEVEL_WAIT_MS);
  }

  // Level up
  level++;
  new_timewarp(level);
}

/*
 * Can't be bothered to play with negative numbers
 */
static uint16_t scale_accel(int16_t val) {
  int16_t retval = 4000 + val;
  if (retval < 0)
    retval = 0;
  return retval;
}

/*
 * Process accelerometer data
 */
static void accel_data_handler(AccelData *data, uint32_t num_samples) {

  // Average the data
  uint32_t avg_x = 0;
  uint32_t avg_y = 0;
  uint32_t avg_z = 0;
  AccelData *dx = data;
  for (uint32_t i = 0; i < num_samples; i++, dx++) {
    avg_x += scale_accel(dx->x);
    avg_y += scale_accel(dx->y);
    avg_z += scale_accel(dx->z);
  }
  avg_x /= num_samples;
  avg_y /= num_samples;
  avg_z /= num_samples;

  // Work out deviations
  uint16_t bx = 0;
  uint16_t by = 0;
  uint16_t bz = 0;
  AccelData *d = data;
  for (uint32_t i = 0; i < num_samples; i++, d++) {
    uint16_t x = scale_accel(d->x) ;
    uint16_t y = scale_accel(d->y) ;
    uint16_t z = scale_accel(d->z) ;

    if (x < avg_x)
      x = avg_x - x;
    else
      x -= avg_x;

    if (y < avg_y)
      y = avg_y - y;
    else
      y -= avg_y;

    if (z < avg_z)
      z = avg_z - z;
    else
      z -= avg_z;

    // Store the worst case for that period
    if (!(d->did_vibrate)) {
      if (x > bx) bx = x;
      if (y > by) by = y;
      if (z > bz) bz = z;
    }
  }

  if (bz > 250)
    level_up();

}


void init_motion_control() {
  accel_data_service_subscribe(10, accel_data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}

void deinit_motion_control() {
  accel_data_service_unsubscribe();
}

