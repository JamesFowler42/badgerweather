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

#ifndef common_h
#define common_h

#include "pebble.h"

#define FUDGE 4
#define INTER_DISPLAY_MS 3000

// All TUPLE_CSTRING
enum WeatherKey {
  WEATHER_CTRL_KEY = 0,
  WEATHER_ICON1_KEY = 1,
  WEATHER_TEMP1_KEY = 2,
  WEATHER_DT1_KEY = 3,
  WEATHER_ICON2_KEY = 4,
  WEATHER_TEMP2_KEY = 5,
  WEATHER_DT2_KEY = 6,
  WEATHER_ICON3_KEY = 7,
  WEATHER_TEMP3_KEY = 8,
  WEATHER_DT3_KEY = 9
};

#define REFRESH 1
#define TIMEWARP 2
#define TITLE 3
#define MAX_LEVEL 2
#define LEVEL_WAIT_MS 10000

typedef struct {
  char icon[10];
  uint32_t resource_id;
  int16_t tm_x;
  int16_t tm_y;
  int16_t tm_w;
  int16_t tm_h;
  char *tm_font;
  int16_t tp_x;
  int16_t tp_y;
  int16_t tp_w;
  int16_t tp_h;
  char *tp_font;
} IconEntry;

void init_motion_control();
void deinit_motion_control();
void trigger_fire_when_loaded();
void init_comms();
void deinit_comms();
void send_cmd();
void new_timewarp(int level);

#endif /* COMMON_H_ */
