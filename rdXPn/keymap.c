#include QMK_KEYBOARD_H
#include "version.h"
#include "i18n.h"
#define MOON_LED_LEVEL LED_LEVEL
#ifndef ZSA_SAFE_RANGE
#define ZSA_SAFE_RANGE SAFE_RANGE
#endif

enum custom_keycodes {
  RGB_SLD = ZSA_SAFE_RANGE,
  HSV_0_255_255,
  HSV_74_255_255,
  HSV_169_255_255,
};

enum thumb_keycodes {
  KC_LTHUMB = SAFE_RANGE,
  KC_RTHUMB = SAFE_RANGE+1
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_voyager(
    DK_DIAE,        KC_1,           KC_2,           KC_3,           KC_4,           KC_5,                                           KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           DK_ACUT,        
    KC_TAB,         KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,                                           KC_Y,           KC_U,           KC_I,           KC_O,           KC_P,           DK_ARNG,        
    DK_PLUS,        MT(MOD_LGUI, KC_A),MT(MOD_LALT, KC_S),MT(MOD_LCTL, KC_D),MT(MOD_LSFT, KC_F),KC_G,                                           KC_H,           MT(MOD_RSFT, KC_J),MT(MOD_RCTL, KC_K),MT(MOD_RALT, KC_L),MT(MOD_RGUI, DK_AE),DK_OSTR,        
    DK_LABK,        KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,                                           KC_N,           KC_M,           KC_COMMA,       KC_DOT,         DK_MINS,        DK_QUOT,        
                                                    KC_ENTER,       KC_LTHUMB,                                      KC_RTHUMB,      KC_SPACE
  ),
  [1] = LAYOUT_voyager(
    KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,          KC_F6,                                          KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_F11,         KC_F12,         
    KC_CAPS,        DK_LABK,        LALT(LSFT(KC_8)),LALT(LSFT(KC_9)),DK_RABK,        KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_KP_7,        KC_KP_8,        KC_KP_9,        KC_TRANSPARENT, KC_TRANSPARENT, 
    DK_DIAE,        DK_LPRN,        DK_LBRC,        DK_RBRC,        DK_RPRN,        KC_RPRN,                                        KC_TRANSPARENT, KC_KP_4,        KC_KP_5,        KC_KP_6,        KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_KP_1,        KC_KP_2,        KC_KP_3,        KC_TRANSPARENT, KC_TRANSPARENT, 
                                                    KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_DELETE,      CW_TOGG
  ),
  [2] = LAYOUT_voyager(
    RGB_TOG,        TOGGLE_LAYER_COLOR,RGB_MODE_FORWARD,RGB_SLD,        RGB_VAD,        RGB_VAI,                                        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, QK_BOOT,        
    KC_CAPS,        KC_TRANSPARENT, KC_AUDIO_VOL_DOWN,KC_AUDIO_VOL_UP,KC_AUDIO_MUTE,  KC_TRANSPARENT,                                 KC_PAGE_UP,     KC_HOME,        KC_UP,          KC_END,         KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_PGDN,        KC_LEFT,        KC_DOWN,        KC_RIGHT,       KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, HSV_0_255_255,  HSV_74_255_255, HSV_169_255_255,                                KC_TRANSPARENT, LCTL(LSFT(KC_TAB)),LCTL(KC_TAB),   KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, 
                                                    KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT
  ),
};

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 
  '*', '*', '*', '*'
);

#ifdef VOYAGER_USER_LEDS
// Number of LEDs on the keyboard.
#define NUM_LEDS  4
// Period for LED_BLINK_FAST blinking. Smaller value implies faster.
#define LED_BLINK_FAST_PERIOD_MS  300

// Possible LED states.
enum { LED_OFF = 0, LED_ON = 1, LED_BLINK_SLOW = 2, LED_BLINK_FAST = 3 };
static uint8_t led_blink_state[NUM_LEDS] = {0};

// Track caps lock
static bool is_caps_lock_on = false;

static void update_caps_indicator(void) {
    if (is_caps_lock_on) {
        led_blink_state[3] = LED_ON;
    } else if (is_caps_word_on()) {
        led_blink_state[3] = LED_BLINK_FAST;
    } else {
        led_blink_state[3] = LED_OFF;
    }
}

bool led_update_user(led_t led_state) {
    is_caps_lock_on = led_state.caps_lock;
    update_caps_indicator();
    return true;
}
#endif

extern rgb_config_t rgb_matrix_config;

void keyboard_post_init_user(void) {
  rgb_matrix_enable();

#ifdef VOYAGER_USER_LEDS
  uint32_t led_blink_callback(uint32_t trigger_time, void* cb_arg) {
    static const uint8_t pattern[4] = {0x00, 0xff, 0x0f, 0xaa};
    static uint8_t phase = 0;
    phase = (phase + 1) % 8;

    uint8_t bit = 1 << phase;

    STATUS_LED_1((pattern[led_blink_state[0]] & bit) != 0);
    STATUS_LED_2((pattern[led_blink_state[1]] & bit) != 0);
    STATUS_LED_3((pattern[led_blink_state[2]] & bit) != 0);
    STATUS_LED_4((pattern[led_blink_state[3]] & bit) != 0);
    
    return LED_BLINK_FAST_PERIOD_MS / 2;
  }

  defer_exec(1, led_blink_callback, NULL);
#endif
}

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [1] = { {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255}, {188,255,255} },

    [2] = { {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255}, {74,255,255} },

};

void set_layer_color(int layer) {
  for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb( hsv );
        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
    }
  }
}

bool rgb_matrix_indicators_user(void) {
  if (rawhid_state.rgb_control) {
      return false;
  }
  if (keyboard_config.disable_layer_led) { return false; }
  switch (biton32(layer_state)) {
    case 1:
      set_layer_color(1);
      break;
    case 2:
      set_layer_color(2);
      break;
   default:
    if (rgb_matrix_get_flags() == LED_FLAG_NONE)
      rgb_matrix_set_color_all(0, 0, 0);
    break;
  }
  return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_LTHUMB:
        LT(1, KC_BSPC);
        return true;
    case KC_RTHUMB:
        LT(2, KC_ESCAPE);
        return true;

#ifdef USER_VOYAGER_LEDS
    case LED_LEVEL:
        if (record->event.pressed) {
            keyboard_config.led_level ^= 1;
            eeconfig_update_kb(keyboard_config.raw);
            if (keyboard_config.led_level) {
                layer_state_set_kb(layer_state);
            } else {
                STATUS_LED_1(false);
                STATUS_LED_2(false);
                STATUS_LED_3(false);
                STATUS_LED_4(false);
            }
        }
        return true;
#endif
    case RGB_SLD:
      if (record->event.pressed) {
        rgblight_mode(1);
      }
      return false;
    case HSV_0_255_255:
      if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(0,255,255);
      }
      return false;
    case HSV_74_255_255:
      if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(74,255,255);
      }
      return false;
    case HSV_169_255_255:
      if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(169,255,255);
      }
      return false;
  }
  return true;
}

// Key Overrides
const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, LT(1,KC_BSPC), KC_DELETE);

const key_override_t **key_overrides = (const key_override_t *[]) {
  &delete_key_override,
  NULL
};


bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
    case DK_AE: // æ
    case DK_OSTR: // ø
    case DK_ARNG: // å
      add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to the next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case DK_MINS: // This is dash
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}

#ifdef VOYAGER_USER_LEDS
void caps_word_set_user(bool active) {
    update_caps_indicator();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

//    STATUS_LED_1(layer & (1<<0));
//    STATUS_LED_2(layer & (1<<1));
//    STATUS_LED_3(layer & (1<<2));
    led_blink_state[0] = (layer & (1<<0)) ? LED_ON : LED_OFF;
    led_blink_state[1] = (layer & (1<<1)) ? LED_ON : LED_OFF;
    led_blink_state[2] = (layer & (1<<2)) ? LED_ON : LED_OFF;
#   if !defined(CAPS_LOCK_STATUS)
//    STATUS_LED_4(layer & (1<<3));
    update_caps_indicator();
#   endif
    return state;
}
#endif

