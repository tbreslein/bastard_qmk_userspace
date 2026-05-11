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
    LAYER_GAMING,
    LAYER_SYMBOLS,
    LAYER_POINTER_FUNCS,
    /* LAYER_KEYPAD_POINTER, */
    /* LAYER_FUNCS, */
};

#define CHARYBDIS_MINIMUM_DEFAULT_DPI 800
#define CHARYBDIS_DEFAULT_DPI_CONFIG_STEP 200

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

#define TG_BASE DF(LAYER_BASE)
#define TG_GAMING DF(LAYER_GAMING)

#define KP_PT_ESC LT(LAYER_POINTER_FUNCS, KC_ESC)
#define KP_FUNCS_ENT LT(LAYER_FUNCS, KC_ENT)
#define FUNCS MO(LAYER_FUNCS)

#define HM_A GUI_T(KC_A)
#define HM_S ALT_T(KC_S)
#define HM_D SFT_T(KC_D)
#define HM_F CTL_T(KC_F)

#define HM_J CTL_T(KC_J)
#define HM_K SFT_T(KC_K)
#define HM_L ALT_T(KC_L)
#define HM_SCLN GUI_T(KC_SCLN)

#define HM_MINS ALT_T(KC_MINS)
#define HM_LBRC CTL_T(KC_LBRC)
#define HM_QUOT ALT_T(KC_QUOT)
#define HM_RBRC CTL_T(KC_RBRC)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// >>> TAP DANCE
// I use comma double tap to output just a single comma, and to and to activate
// as my leader key otherwise
enum { TD_LEAD_COMM };
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for LEADER, twice for Comma
    [TD_LEAD_COMM] = ACTION_TAP_DANCE_DOUBLE(QK_LEAD, KC_COMM),
};
#define TD_COMM

void leader_end_user(void) {
    if (leader_sequence_one_key(KC_SPC)) {
        tap_code16(KC_COMM);
        tap_code16(KC_SPC);
    } else if (leader_sequence_one_key(KC_ENT)) {
        tap_code16(KC_COMM);
        tap_code16(KC_ENT);

        // [K]ukebox:
        //   leader k g -> gaming
        //   leader k b -> base
    } else if (leader_sequence_two_keys(KC_K, KC_G)) {
        set_single_default_layer(LAYER_GAMING);
    } else if (leader_sequence_two_keys(KC_K, KC_B)) {
        set_single_default_layer(LAYER_BASE);

        // [J]ukebox:
        //   j/k/l : volume controls
        //   u/i/o : media controls
    } else if (leader_sequence_two_keys(KC_J, KC_I)) {
        tap_code16(KC_MPLY);
    } else if (leader_sequence_two_keys(KC_J, KC_O)) {
        tap_code16(KC_MNXT);
    } else if (leader_sequence_two_keys(KC_J, KC_U)) {
        tap_code16(KC_MPRV);
    } else if (leader_sequence_two_keys(KC_J, KC_K)) {
        tap_code16(KC_MUTE);
    } else if (leader_sequence_two_keys(KC_J, KC_L)) {
        tap_code16(KC_VOLU);
    } else if (leader_sequence_two_keys(KC_J, KC_J)) {
        tap_code16(KC_VOLD);

        // [Y]ank screen
    } else if (leader_sequence_one_key(KC_Y)) {
        tap_code16(KC_PSCR);

        // [U] caps word... up?
    } else if (leader_sequence_one_key(KC_U)) {
        tap_code16(CW_TOGG);

        // [I] caps ... and there's no mnemonic lol
    } else if (leader_sequence_one_key(KC_I)) {
        tap_code16(KC_CAPS);

        // Equal sequences(these map similarly to my regular leader symbols):
        //  leader m -> = (because it's easy to input)
        //  leader f m -> ==
        //  leader f d -> +=
        //  leader f s -> -=
        //  leader f a -> *=
        //  leader f c -> !=
        //  leader f x -> %=
        //  leader f z -> ^=
        //  leader f w -> <=
        //  leader f e -> >=
        //  leader f g -> &=
        //  leader f b -> |=
        //  leader f f g -> &&=
        //  leader f f b -> ||=
    } else if (leader_sequence_one_key(KC_M)) {
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_M)) {
        tap_code16(KC_EQL);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_D)) {
        tap_code16(KC_PLUS);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_S)) {
        tap_code16(KC_MINS);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_A)) {
        tap_code16(KC_ASTR);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_C)) {
        tap_code16(KC_EXLM);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_X)) {
        tap_code16(KC_PERC);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_Z)) {
        tap_code16(KC_CIRC);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_E)) {
        tap_code16(KC_GT);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_W)) {
        tap_code16(KC_LT);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_G)) {
        tap_code16(KC_AMPR);
        tap_code16(KC_EQL);
    } else if (leader_sequence_two_keys(KC_F, KC_B)) {
        tap_code16(KC_PIPE);
        tap_code16(KC_EQL);
    } else if (leader_sequence_three_keys(KC_F, KC_F, KC_G)) {
        tap_code16(KC_AMPR);
        tap_code16(KC_AMPR);
        tap_code16(KC_EQL);
    } else if (leader_sequence_three_keys(KC_F, KC_F, KC_B)) {
        tap_code16(KC_PIPE);
        tap_code16(KC_PIPE);
        tap_code16(KC_EQL);

        // arrow sequences:
        // leader s f -> ->
        // leader s d -> <-
        // leader s r -> =>
        // leader s e -> <=
    } else if (leader_sequence_two_keys(KC_S, KC_F)) {
        tap_code16(KC_MINS);
        tap_code16(KC_GT);
    } else if (leader_sequence_two_keys(KC_S, KC_D)) {
        tap_code16(KC_LT);
        tap_code16(KC_MINS);
    } else if (leader_sequence_two_keys(KC_S, KC_R)) {
        tap_code16(KC_EQL);
        tap_code16(KC_GT);
    } else if (leader_sequence_two_keys(KC_S, KC_E)) {
        tap_code16(KC_LT);
        tap_code16(KC_EQL);

        // symbols
        // leader q -> @
        // leader w -> #
        // leader e -> {
        // leader r -> (
        // leader 3 -> }
        // leader 4 -> )
        // leader a -> *
        // leader d -> +
        // leader g -> &
        // leader z -> ^
        // leader x -> %
        // leader c -> !
        // leader v -> $
        // leader b -> |
    } else if (leader_sequence_one_key(KC_Q)) {
        tap_code16(KC_AT);
    } else if (leader_sequence_one_key(KC_W)) {
        tap_code16(KC_HASH);
    } else if (leader_sequence_one_key(KC_E)) {
        tap_code16(KC_LCBR);
    } else if (leader_sequence_one_key(KC_R)) {
        tap_code16(KC_LPRN);
    } else if (leader_sequence_one_key(KC_3)) {
        tap_code16(KC_RCBR);
    } else if (leader_sequence_one_key(KC_4)) {
        tap_code16(KC_RPRN);
    } else if (leader_sequence_one_key(KC_A)) {
        tap_code16(KC_ASTR);
    } else if (leader_sequence_one_key(KC_D)) {
        tap_code16(KC_PLUS);
    } else if (leader_sequence_one_key(KC_G)) {
        tap_code16(KC_AMPR);
    } else if (leader_sequence_one_key(KC_Z)) {
        tap_code16(KC_CIRC);
    } else if (leader_sequence_one_key(KC_X)) {
        tap_code16(KC_PERC);
    } else if (leader_sequence_one_key(KC_C)) {
        tap_code16(KC_EXLM);
    } else if (leader_sequence_one_key(KC_V)) {
        tap_code16(KC_DLR);
    } else if (leader_sequence_one_key(KC_B)) {
        tap_code16(KC_PIPE);

        // misc
        // leader / -> string("tommy.breslein@protonmail.com")
    } else if (leader_sequence_one_key(KC_SLSH)) {
        SEND_STRING("tommy.breslein@protonmail.com");
    }
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
     S(KC_TAB),    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_GRV,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       HM_MINS,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, HM_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       HM_LBRC,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, TD_COMM,  KC_DOT, KC_SLSH, HM_RBRC,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                             KC_SPC, KC_LSFT, RAG_T(KC_DEL),     KP_FUNCS_ENT, KC_LSFT,
                              RCG_T(KC_BSPC),     KP_PT_ESC,     GUI_T(KC_EQL)
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_GAMING] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_GRV,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BTN2,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, HM_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LALT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, TD_COMM,  KC_DOT, KC_SLSH, KC_BTN1,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                             KC_SPC, KC_LSFT, RAG_T(KC_DEL),     KP_FUNCS_ENT, KC_LSFT,
                              RCG_T(KC_BSPC),     KP_PT_ESC,     GUI_T(KC_EQL)
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER_FUNCS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, _______, _______, _______, _______, _______,    _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, KC_BTN4, KC_BTN3, KC_BTN2, KC_BTN1, KC_BTN5,    _______,   KC_F5,   KC_F6,   KC_F7,   KC_F8, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, MS_WHLU,    KC_HOME,   KC_F9,  KC_F10,  KC_F11,  KC_F12, KC_PGUP,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, _______, KC_BTN4, DRGSCRL, KC_BTN5, MS_WHLD,    KC_END,   KC_F13,  KC_F14,  KC_F15,  KC_F16, KC_PGDN,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, KC_LSFT,    KC_DEL,  _______,
                                           _______, _______,    _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  /* [LAYER_GAMING_POINTER] = LAYOUT( */
  /* // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */
  /*      QK_GESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, DRGSCRL, */
  /* // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */
  /*      KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS, */
  /* // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */
  /*      KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT, */
  /* // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */
  /*      KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_LALT, */
  /* // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
  /*                                  KC_SPC, KC_BTN2, KC_BTN1,     LGUI_T(KC_EQL), KP_FUNCS_ENT, */
  /*                                          KC_BSPC, KC_BTN3,     KC_DEL */
  /* //                            ╰───────────────────────────╯ ╰──────────────────╯ */
  /* ), */

  /*
  [LAYER_KEYPAD_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, KC_BTN4, KC_BTN3, KC_BTN2, KC_BTN1, KC_BTN5,    KC_PLUS,    KC_7,    KC_8,    KC_9, KC_PERC, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, MS_WHLU,    KC_EQL,     KC_4,    KC_5,    KC_6, KC_ASTR, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, _______, KC_BTN4, DRGSCRL, KC_BTN5, MS_WHLD,    KC_MINS,    KC_1,    KC_2,    KC_3, KC_SLSH, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, KC_LSFT,    KC_COMM,    KC_0,
                                           _______, _______,    KC_DOT
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_FUNCS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
         KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,      KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
  TG_GAMING_PT, KC_CAPS, CW_TOGG, KC_HOME, KC_PGUP, _______,    KC_BRIU, KC_MPRV, KC_MPLY, KC_MNXT, KC_PSCR, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
     TG_GAMING, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, MS_WHLU,    KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       TG_BASE, _______, _______, KC_END,  KC_PGDN, MS_WHLD,    KC_BRID, KC_VOLD, KC_MUTE, KC_VOLU, _______, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, _______,    _______, _______,
                                            KC_DEL, _______,    _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
                         ),
  */
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
