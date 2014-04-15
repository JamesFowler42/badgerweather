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

static Window *window;

static TextLayer *temperature_layer;
static BitmapLayer *icon_layer;
static GBitmap *icon_bitmap = NULL;
static TextLayer *text_time_layer;
static char time_text[6];

static IconEntry icon_lookup[] = {
                           { .icon="???", .resource_id=RESOURCE_ID_CLUELESS, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="01d", .resource_id=RESOURCE_ID_DAY_SUN, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="01n", .resource_id=RESOURCE_ID_NIGHT_CLEAR, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="02d", .resource_id=RESOURCE_ID_DAY_SUN_CLOUD, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="02n", .resource_id=RESOURCE_ID_NIGHT_CLEAR_CLOUD, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="03d", .resource_id=RESOURCE_ID_CLOUD, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="03n", .resource_id=RESOURCE_ID_CLOUD, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="04d", .resource_id=RESOURCE_ID_BROKEN_CLOUD, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="04n", .resource_id=RESOURCE_ID_BROKEN_CLOUD, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="09d", .resource_id=RESOURCE_ID_SHOWERS, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="09n", .resource_id=RESOURCE_ID_SHOWERS, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="10d", .resource_id=RESOURCE_ID_DAY_RAIN, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="10n", .resource_id=RESOURCE_ID_NIGHT_RAIN, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="11d", .resource_id=RESOURCE_ID_THUNDERSTORM, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="11n", .resource_id=RESOURCE_ID_THUNDERSTORM, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="13d", .resource_id=RESOURCE_ID_SNOW, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="13n", .resource_id=RESOURCE_ID_SNOW, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="50d", .resource_id=RESOURCE_ID_FOG, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18},
                           { .icon="50n", .resource_id=RESOURCE_ID_FOG, .tm_x=0, .tm_y=112, .tm_w=143, .tm_h=168-112, .tp_x=0, .tp_y=94, .tp_w=143, .tp_h=168-94, .tm_font=FONT_KEY_BITHAM_30_BLACK, .tp_font=FONT_KEY_GOTHIC_18}
};

static bool inter_showing = false;
static Window *inter_window;
static BitmapLayer *inter_layer;
static GBitmap *inter_bitmap;
static AppTimer *inter_timer;

char icon1[4] = "---";
char icon2[4] = "---";
char icon3[4] = "---";
char temp1[20] = "";
char temp2[20] = "";
char temp3[20] = "";
char dt1[21] = "";
char dt2[21] = "";
char dt3[21] = "";

static bool fire_when_loaded_triggered = false;
static AppTimer *fire_when_loaded_timer;
static int level = 0;

/*
 * Remove the intermission window
 */
static void hide_inter_layer(void *data) {
  if (inter_showing) {
    window_stack_remove(inter_window, true);
    bitmap_layer_destroy(inter_layer);
    gbitmap_destroy(inter_bitmap);
    window_destroy(inter_window);
    inter_showing = false;
  }
}



/*
 * Show the intermission window
 */
static void show_inter(int type) {

  // Already showing - extend time
  if (inter_showing) {
    app_timer_reschedule(inter_timer, INTER_DISPLAY_MS);
    return;
  }

  // Bring up notice
  inter_showing = true;
  inter_window = window_create();
  window_set_fullscreen(inter_window, true);
  window_stack_push(inter_window, true /* Animated */);
  window_set_background_color(inter_window, GColorBlack);

  Layer *window_layer = window_get_root_layer(inter_window);

  inter_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  layer_add_child(window_layer, bitmap_layer_get_layer(inter_layer));

  if (type == REFRESH)
  inter_bitmap = gbitmap_create_with_resource(RESOURCE_ID_INTERMISSION);
  else if (type == TIMEWARP)
  inter_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TIMEWARP);
  else if (type == TITLE)
  inter_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TITLE);
  bitmap_layer_set_bitmap(inter_layer, inter_bitmap);

  inter_timer = app_timer_register(INTER_DISPLAY_MS, hide_inter_layer, NULL);
}



static IconEntry* findInfo(const char *icon) {
  for (uint16_t i=0; i < ARRAY_LENGTH(icon_lookup); i++) {
    if (strncmp(icon_lookup[i].icon, icon, sizeof(icon_lookup[i].icon)) == 0) {
      return &icon_lookup[i];
    }
  }
  return &icon_lookup[0];
}

static GFont get_font(char *font_id) {
  // Can intercept here for custom fonts
  return fonts_get_system_font(font_id);
}

static void set_weather_info(const char *icon) {
  IconEntry *new_info = findInfo(icon);
  if (icon_bitmap) {
    gbitmap_destroy(icon_bitmap);
  }
  icon_bitmap = gbitmap_create_with_resource(new_info->resource_id);
  bitmap_layer_set_bitmap(icon_layer, icon_bitmap);
  layer_set_frame(text_layer_get_layer(text_time_layer), GRect(new_info->tm_x, new_info->tm_y, new_info->tm_w, new_info->tm_h));
  layer_set_frame(text_layer_get_layer(temperature_layer), GRect(new_info->tp_x, new_info->tp_y, new_info->tp_w, new_info->tp_h));
  text_layer_set_font(text_time_layer, get_font(new_info->tm_font));
  text_layer_set_font(temperature_layer, get_font(new_info->tp_font));

}

void new_timewarp(int level_cng) {
  level = level_cng;
  if (level == 0) {
    show_inter(TIMEWARP);
    set_weather_info(icon1);
    text_layer_set_text(temperature_layer, temp1);
    clock_copy_time_string(time_text, sizeof(time_text));
    if (time_text[4] == ' ')
      time_text[4] = '\0';
    text_layer_set_text(text_time_layer, time_text);
  }
  if (level == 1) {
    show_inter(TIMEWARP);
    set_weather_info(icon2);
    text_layer_set_text(temperature_layer, temp2);
    text_layer_set_text(text_time_layer, dt2);
  }
  if (level == 2) {
    show_inter(TIMEWARP);
    set_weather_info(icon3);
    text_layer_set_text(temperature_layer, temp3);
    text_layer_set_text(text_time_layer, dt3);
  }
}


static void fire_when_loaded(void *data) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "1: icon: %s, date: %s", icon1, dt1);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "2: icon: %s, date: %s", icon2, dt2);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "3: icon: %s, date: %s", icon3, dt3);
  fire_when_loaded_triggered = false;
  show_inter(REFRESH);
  set_weather_info(icon1);
  text_layer_set_text(temperature_layer, temp1);
}

void trigger_fire_when_loaded() {
  if (fire_when_loaded_triggered) {
    app_timer_reschedule (fire_when_loaded_timer, 200);
  } else {
    fire_when_loaded_triggered = true;
    fire_when_loaded_timer = app_timer_register(200, fire_when_loaded, NULL);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  icon_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  layer_add_child(window_layer, bitmap_layer_get_layer(icon_layer));

  temperature_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_text_color(temperature_layer, GColorWhite);
  text_layer_set_background_color(temperature_layer, GColorClear);
  text_layer_set_text_alignment(temperature_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(temperature_layer));

  text_time_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_time_layer));

  set_weather_info("???");



}

static void window_unload(Window *window) {
  if (icon_bitmap) {
    gbitmap_destroy(icon_bitmap);
  }

  text_layer_destroy(text_time_layer);
  text_layer_destroy(temperature_layer);
  bitmap_layer_destroy(icon_layer);

}

/*
 * Clock tick
 */
static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {

  if (level == 0) {
    clock_copy_time_string(time_text, sizeof(time_text));
    if (time_text[4] == ' ')
      time_text[4] = '\0';
    text_layer_set_text(text_time_layer, time_text);
  }

  if (tick_time->tm_min % 10 == 0) {
    send_cmd();
  }

}

static void init(void) {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_fullscreen(window, true);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });

  window_stack_push(window, false);

  show_inter(TITLE);

  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);

  init_comms();

  init_motion_control();
}

static void deinit(void) {
  deinit_motion_control();
  deinit_comms();
  tick_timer_service_unsubscribe();
  window_destroy(window);

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
