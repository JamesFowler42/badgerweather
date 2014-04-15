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

static uint32_t outbound_size;
static uint32_t inbound_size;

extern char icon1[4];
extern char icon2[4];
extern char icon3[4];
extern char temp1[20];
extern char temp2[20];
extern char temp3[20];
extern char dt1[21];
extern char dt2[21];
extern char dt3[21];

/*
 * Incoming message dropped handler
 */
static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "App Msg Drop %d", reason);
}

/*
 * Outgoing message failed handler
 */
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "App Msg Send Fail %d", reason);
}

/*
 * Outgoing message success handler
 */
static void out_sent_handler(DictionaryIterator *iterator, void *context) {
  // Nothing at the moment
}


static void in_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *icon1_tuple = dict_find(iter, WEATHER_ICON1_KEY);
  Tuple *icon2_tuple = dict_find(iter, WEATHER_ICON2_KEY);
  Tuple *icon3_tuple = dict_find(iter, WEATHER_ICON3_KEY);
  Tuple *temp1_tuple = dict_find(iter, WEATHER_TEMP1_KEY);
  Tuple *temp2_tuple = dict_find(iter, WEATHER_TEMP2_KEY);
  Tuple *temp3_tuple = dict_find(iter, WEATHER_TEMP3_KEY);
  Tuple *dt1_tuple = dict_find(iter, WEATHER_DT1_KEY);
  Tuple *dt2_tuple = dict_find(iter, WEATHER_DT2_KEY);
  Tuple *dt3_tuple = dict_find(iter, WEATHER_DT3_KEY);

  if (icon1_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "WEATHER_ICON1_KEY %s", icon1_tuple->value->cstring);
    strncpy(icon1, icon1_tuple->value->cstring, sizeof(icon1));
    trigger_fire_when_loaded();
  }

  if (icon2_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "WEATHER_ICON2_KEY %s", icon2_tuple->value->cstring);
    strncpy(icon2, icon2_tuple->value->cstring, sizeof(icon2));
    trigger_fire_when_loaded();
  }

    if (icon3_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "WEATHER_ICON3_KEY %s", icon3_tuple->value->cstring);
    strncpy(icon3, icon3_tuple->value->cstring, sizeof(icon3));
    trigger_fire_when_loaded();
    }

    if (temp1_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "WEATHER_TEMP1_KEY");
    strncpy(temp1, temp1_tuple->value->cstring, sizeof(temp1));
    trigger_fire_when_loaded();
    }

    if (temp2_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "WEATHER_TEMP2_KEY");
    strncpy(temp2, temp2_tuple->value->cstring, sizeof(temp2));
    trigger_fire_when_loaded();
    }

    if (temp3_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "WEATHER_TEMP3_KEY");
    strncpy(temp3, temp3_tuple->value->cstring, sizeof(temp3));
    trigger_fire_when_loaded();
    }

    if (dt1_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "WEATHER_DT1_KEY %s", dt1_tuple->value->cstring);
    strncpy(dt1, dt1_tuple->value->cstring, sizeof(dt1));
    trigger_fire_when_loaded();
    }

    if (dt2_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "WEATHER_DT2_KEY %s", dt2_tuple->value->cstring);
    strncpy(dt2, dt2_tuple->value->cstring, sizeof(dt2));
    trigger_fire_when_loaded();
    }

    if (dt3_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "WEATHER_DT3_KEY %s", dt3_tuple->value->cstring);
    strncpy(dt3, dt3_tuple->value->cstring, sizeof(dt3));
    trigger_fire_when_loaded();
    }


}

void send_cmd(void) {
  Tuplet value = TupletInteger(WEATHER_CTRL_KEY, 1);

  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (iter == NULL) {
    return;
  }

  dict_write_tuplet(iter, &value);
  dict_write_end(iter);

  app_message_outbox_send();
}

/*
 * Accurate buffer size count
 */
static void in_out_size_calc() {

  Tuplet out_values[] = {
      TupletInteger(WEATHER_CTRL_KEY, 2)   // Only send one value at a time so this is right
  };

  outbound_size = dict_calc_buffer_size_from_tuplets(out_values, ARRAY_LENGTH(out_values)) + FUDGE;

  Tuplet in_values[] = {
      TupletCString(WEATHER_ICON1_KEY, "01d"),
      TupletCString(WEATHER_TEMP1_KEY, "1234\u00B0C/1234\u00B0F"),
      TupletCString(WEATHER_DT1_KEY, "2000-01-01 00:00:00"),
      TupletCString(WEATHER_ICON2_KEY, "01d"),
      TupletCString(WEATHER_TEMP2_KEY, "1234\u00B0C/1234\u00B0F"),
      TupletCString(WEATHER_DT2_KEY, "2000-01-01 00:00:00"),
      TupletCString(WEATHER_ICON3_KEY, "01d"),
      TupletCString(WEATHER_TEMP3_KEY, "1234\u00B0C/1234\u00B0F"),
      TupletCString(WEATHER_DT3_KEY, "2000-01-01 00:00:00")
  };

  inbound_size = dict_calc_buffer_size_from_tuplets(in_values, ARRAY_LENGTH(in_values)) + FUDGE;

  APP_LOG(APP_LOG_LEVEL_INFO, "In buff %ld, Out buff %ld", inbound_size, outbound_size);
}

void init_comms() {
  // Register message handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  app_message_register_outbox_sent(out_sent_handler);

  // Size calc
  in_out_size_calc();

  // Open buffers
  app_message_open(inbound_size, outbound_size);

  send_cmd();
}

void deinit_comms() {
  accel_data_service_unsubscribe();
}
