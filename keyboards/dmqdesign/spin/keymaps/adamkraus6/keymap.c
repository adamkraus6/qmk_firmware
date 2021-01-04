/* Copyright 2020 Joshua Moses Diamond
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include "version.h"
#include <stdlib.h>

#define RGB_LAYER_ACK_DURATION 500

enum layers { _NUMPAD, _MACRO, _RGB, _FN };

enum layer_base {
    LAYER_BASE     = _NUMPAD,
    LAYER_BASE_END = _FN + 1,
};

enum custom_keycodes {
    HELLO = SAFE_RANGE,
    CH_CPNL,  // AL Control Panel
    CH_ASST,  // AL Context-aware Desktop Assistant
    CH_SUSP,  // Suspend
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_NUMPAD] = LAYOUT(
        KC_7,      KC_8,      KC_9,   TO(_NUMPAD),
        KC_4,      KC_5,      KC_6,   TO(_MACRO),
        KC_1,      KC_2,      KC_3,   TO(_RGB),
        KC_0,      KC_BSPC,   KC_ENTER
        ),

    [_MACRO] = LAYOUT(
        KC_MPRV,   KC_MPLY,   KC_MNXT,   KC_TRNS, 
        KC_NO,     KC_UP,     KC_NO,     KC_TRNS,
        KC_LEFT,   KC_DOWN,   KC_RIGHT,  KC_TRNS,
        MO(_FN),   KC_NO,     KC_ENTER
        ),

    [_RGB] = LAYOUT(
        RGB_HUI,   RGB_SAI,   RGB_VAI,   KC_TRNS,
        RGB_HUD,   RGB_SAD,   RGB_VAD,   KC_TRNS,
        RGB_SPD,   RGB_SPI,   KC_NO,     KC_TRNS,
        RGB_RMOD,  RGB_TOG,   RGB_MOD
        ),

    [_FN] = LAYOUT(
        HELLO,     DEBUG,     RESET,     KC_TRNS,
        KC_NO,     KC_NO,     KC_NO,     KC_TRNS,
        G(KC_PSCR),KC_NO,     KC_NO,     KC_TRNS,
        KC_NO,     G(KC_L),   C(S(KC_ESC))
        )
};
// clang-format on 

typedef enum layer_ack {
    ACK_NO = 0,
    ACK_YES,
    ACK_MEH,
} layer_ack_t;

#define LAYER_OFFSET 0
const rgblight_segment_t PROGMEM _numpad_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 1, HSV_TEAL});
const rgblight_segment_t PROGMEM _macro_layer[]  = RGBLIGHT_LAYER_SEGMENTS({1, 1, HSV_TEAL});
const rgblight_segment_t PROGMEM _rgb_layer[]    = RGBLIGHT_LAYER_SEGMENTS({2, 1, HSV_TEAL});
const rgblight_segment_t PROGMEM _fn_layer[]     = RGBLIGHT_LAYER_SEGMENTS({0, 3, HSV_GOLDENROD});

#define ACK_OFFSET 4
const rgblight_segment_t PROGMEM _no_layer[]  = RGBLIGHT_LAYER_SEGMENTS({0, 3, HSV_RED});
const rgblight_segment_t PROGMEM _yes_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 3, HSV_GREEN});
const rgblight_segment_t PROGMEM _meh_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 3, HSV_YELLOW});

// clang-format on 
const rgblight_segment_t *const PROGMEM _rgb_layers[] = {
    [LAYER_OFFSET + 0] = _numpad_layer,
    [LAYER_OFFSET + 1] = _macro_layer,
    [LAYER_OFFSET + 2] = _rgb_layer,
    [LAYER_OFFSET + 3] = _fn_layer,
   
    [ACK_OFFSET + ACK_NO] = _no_layer,
    [ACK_OFFSET + ACK_YES] = _yes_layer,
    [ACK_OFFSET + ACK_MEH] = _meh_layer,

    [ACK_OFFSET + ACK_MEH + 1] = NULL
};
// clang-format off 

const uint8_t PROGMEM _n_rgb_layers = sizeof(_rgb_layers) / sizeof(_rgb_layers[0]) - 1;

void clear_rgb_layers(void) {
    dprint("clear_rgb_layers()\n");
    for (uint8_t i = 0; i < _n_rgb_layers; i++) {
        rgblight_set_layer_state(i, false);
    }
}

void do_rgb_layers(layer_state_t state, uint8_t start, uint8_t end) {
    dprintf("start=%u, end=%u, LAYER_OFFSET=%u\n", start, end, LAYER_OFFSET);
    for (uint8_t i = start; i < end; i++) {
        bool    is_on = layer_state_cmp(state, i);
        uint8_t rl    = LAYER_OFFSET + i;
        dprintf("layer[%u]=%u, rl=%u\n", i, is_on, rl);
        rgblight_set_layer_state(rl, is_on);
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    do_rgb_layers(state, LAYER_BASE, LAYER_BASE_END);
    return state;
}

void rgb_layer_ack(layer_ack_t n) {
    uint8_t layer = ACK_OFFSET + n;
    dprintf("rgb_layer_ack(%u) ==> %u\n", n, layer);
    rgblight_blink_layer(layer, RGB_LAYER_ACK_DURATION);
}

void rgb_layer_ack_yn(bool yn) { rgb_layer_ack(yn ? ACK_YES : ACK_NO); }

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = _rgb_layers;
    do_rgb_layers(layer_state, LAYER_BASE, LAYER_BASE_END);
}

void shutdown_user() {
    clear_rgb_layers();
    rgblight_enable();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(HSV_RED);
}

void spidey_glow(void) {
    rgblight_enable();
    rgblight_mode(RGBLIGHT_MODE_RAINBOW_MOOD);
    rgblight_sethsv(255, 230, 128);
}

void eeconfig_init_user(void) { spidey_glow(); }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            // Re-implement this here, but fix the persistence!
            case DEBUG:
                if (!debug_enable) {
                    debug_enable = 1;
                } else if (!debug_keyboard) {
                    debug_keyboard = 1;
                } else if (!debug_matrix) {
                    debug_matrix = 1;
                } else {
                    debug_enable   = 0;
                    debug_keyboard = 0;
                    debug_matrix   = 0;
                }
                uprintf("DEBUG: enable=%u, keyboard=%u, matrix=%u\n", debug_enable, debug_keyboard, debug_matrix);
                uprintln(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION ", Built on: " QMK_BUILDDATE);
                eeconfig_update_debug(debug_config.raw);
                return false;

                // clang-format off
            case CH_CPNL: host_consumer_send(AL_CONTROL_PANEL); return false;
            case CH_ASST: host_consumer_send(AL_ASSISTANT); return false;
            case CH_SUSP: tap_code16(LGUI(LSFT(KC_L))); return true;
            case HELLO:   SEND_STRING("Hello, world!"); return true;
                // clang-format on
        }
    } else {
        switch (keycode) {
            case CH_CPNL:
            case CH_ASST:
                host_consumer_send(0);
                return false;
        }
    }

    return true;
};


void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Acks follow...
        case DEBUG:
            rgb_layer_ack_yn(debug_enable);
            break;
        case RGB_TOG:
            rgb_layer_ack_yn(rgblight_is_enabled());
            break;
    }
}

void encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {
        case _RGB:
            if (index == 0) {
                if (clockwise) {
                    rgblight_increase_hue();
                } else {
                    rgblight_decrease_hue();
                }
            } else if (index == 1) {
                if (clockwise) {
                    rgblight_increase_sat();
                } else {
                    rgblight_decrease_sat();
                }
            } else if (index == 2) {
                if (clockwise) {
                    rgblight_increase_val();
                } else {
                    rgblight_decrease_val();
                }
            }
            break;

        default:
            if (index == 0) {
                //
            } else if (index == 1) {
                tap_code16(clockwise ? KC_DOWN : KC_UP);
            } else if (index == 2) {
                tap_code(clockwise ? KC_VOLU : KC_VOLD);
            }
            break;
    }
}

#ifdef OLED_DRIVER_ENABLE

static const char *ANIMATION_NAMES[] = {
	"unknown", // 0
	"static", // 1
	"breathing I", // 2
	"breathing II", // 3
	"breathing III", // 4
	"breathing IV", // 5
	"rainbow mood I", // 6
	"rainbow mood II", // 7
	"rainbow mood III", // 8
	"rainbow swirl I", // 9
	"rainbow swirl II", // 10
	"rainbow swirl III", // 11
	"rainbow swirl IV", // 12
	"rainbow swirl V", // 13
	"rainbow swirl VI", // 14
	"snake I", // 15
	"snake II", // 16
	"snake III", // 17
	"snake IV", // 18
	"snake V", // 19
	"snake VI", // 20
	"knight I", // 21
	"knight II", // 22
	"knight III", // 23
//    "christmas",
	"static gradient I", // 24
	"static gradient II", // 25
	"static gradient III", // 26
	"static gradient IV", // 27
	"static gradient V", // 28
	"static gradient VI", // 29
	"static gradient VII", // 30
	"static gradient VIII", // 31
	"static gradient IX", // 32
	"static gradient X", // 33
//	"rgb test",
	"alternating", // 34
	"twinkle I", // 35
	"twinkle II", // 36
	"twinkle III", // 37
	"twinkle IV", // 38
	"twinkle V", // 39
	"twinkle VI" // 40
};

void rgblight_get_mode_name(uint8_t mode, size_t bufsize, char *buf) {
    snprintf(buf, bufsize, "%-25s", ANIMATION_NAMES[mode]);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    #ifdef LEFT_HAND
        return OLED_ROTATION_180;
    #else
        return OLED_ROTATION_0;
    #endif
}

void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _NUMPAD:
            oled_write_P(PSTR("Numpad\n"), false);
            break;
        case _RGB:
            oled_write_P(PSTR("RGB\n"), false);
            break;
        case _MACRO:
            oled_write_P(PSTR("Macro\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    static char rgb_mode_name[30];
    rgblight_get_mode_name(rgblight_get_mode(), sizeof(rgb_mode_name), rgb_mode_name);

    oled_write_P(PSTR("Mode: "), false);
    oled_write_ln(rgb_mode_name, false);
}
#endif
