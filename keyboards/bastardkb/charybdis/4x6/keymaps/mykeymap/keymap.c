/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_KEYPAD_POINTER,
    LAYER_FUNCS,
    LAYER_GAMING,
    LAYER_GAMING_POINTER,
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_KEYPAD_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define DF_BASE DF(LAYER_BASE)
#define DF_GAMING DF(LAYER_GAMING)
#define DF_GAMING_PT DF(LAYER_GAMING_POINTER)

#define KP_PT MO(LAYER_KEYPAD_POINTER)
#define FUNCS MO(LAYER_FUNCS)

#define HM_A GUI_T(KC_A)
#define HM_S ALT_T(KC_S)
#define HM_D SFT_T(KC_D)
#define HM_F CTL_T(KC_F)

#define HM_J CTL_T(KC_J)
#define HM_K SFT_T(KC_K)
#define HM_L ALT_T(KC_L)
#define HM_SCLN GUI_T(KC_SCLN)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_DEL,  KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_EQL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TILD,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MINS,    HM_A,    HM_S,    HM_D,    HM_F,    KC_G,       KC_H,    HM_J,    HM_K,    HM_L, HM_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LBRC,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RBRC,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                     KC_SPC, KC_ESC, KC_TAB,     KC_ENT, KC_BSPC,
                                             KP_PT, CW_TOGG,     FUNCS
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_KEYPAD_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, _______, _______, _______, _______, KC_END,     _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       S_D_MOD, DPI_MOD, KC_BTN3, KC_BTN1, KC_BTN2, KC_HOME,    KC_PLUS,    KC_7,    KC_8,    KC_9, KC_PERC, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       SNP_TOG, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_PGUP,    KC_EQL,     KC_4,    KC_5,    KC_6, KC_ASTR, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     S_D_RMOD, DPI_RMOD, _______, DRGSCRL, SNIPING, KC_PGDN,    KC_MINS,    KC_1,    KC_2,    KC_3, KC_SLSH, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, _______,    _______, _______,
                                           _______, _______,    KC_0
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_FUNCS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
         KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,      KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
  DF_GAMING_PT, _______, _______, _______, _______, _______,    _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     DF_GAMING, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, _______,    KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       DF_BASE, _______, _______, _______, _______, _______,    _______, KC_VOLD, KC_MUTE, KC_VOLU, _______, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, _______,    _______, _______,
                                           _______, _______,    _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_GAMING] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_EQL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BTN2,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    HM_J,    HM_K,    HM_L, HM_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_BTN1,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                   KC_SPC, KC_TILD, KC_LALT,     KC_ENT, KC_BSPC,
                                           KC_DEL,  KC_MINS,     FUNCS
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_GAMING_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_EQL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_LALT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                   KC_SPC, KC_TILD, KC_BTN3,     KC_ENT, KC_BSPC,
                                           KC_BTN1, KC_BTN2,     FUNCS
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  /* [LAYER_FUNCS] = LAYOUT( */
  /* // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */
  /*       KC_F12,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11, */
  /* // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */
  /*      KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU, */
  /* // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */
  /*      KC_MPLY, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, KC_MUTE, */
  /* // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */
  /*      KC_MPRV, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD, */
  /* // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
  /*                                 _______, _______, XXXXXXX,    _______, XXXXXXX, */
  /*                                          _______, _______,    XXXXXXX */
  /* //                            ╰───────────────────────────╯ ╰──────────────────╯ */
  /* ), */

  /* [LAYER_POINTER] = LAYOUT( */
  /* // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */
  /*      QK_BOOT,  EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,  EE_CLR, */
  /* // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */
  /*      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, */
  /* // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */
  /*      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX, */
  /* // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */
  /*      XXXXXXX, _______, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, XXXXXXX, */
  /* // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
  /*                                 KC_BTN2, KC_BTN1, KC_BTN3,    KC_BTN3, KC_BTN1, */
  /*                                          XXXXXXX, KC_BTN2,    KC_BTN2 */
  /* //                            ╰───────────────────────────╯ ╰──────────────────╯ */
  /* ), */
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif
