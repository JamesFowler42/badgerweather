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

#define INTER_DISPLAY_MS 3000

enum WeatherKey {
  WEATHER_ICON_KEY = 0x0,         // TUPLE_CSTRING
  WEATHER_TEMPERATURE_KEY = 0x1   // TUPLE_CSTRING
};

typedef struct {
  char icon[10];
  uint32_t resource_id;
  int16_t tm_x;
  int16_t tm_y;
  int16_t tm_w;
  int16_t tm_h;
  int tm_font;
  int16_t tp_x;
  int16_t tp_y;
  int16_t tp_w;
  int16_t tp_h;
  int tp_font;
} IconEntry;



#endif /* COMMON_H_ */
