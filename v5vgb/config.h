/*
  Set any config.h overrides for your specific keymap here.
  See config.h options at https://docs.qmk.fm/#/config_options?id=the-configh-file
*/

#define PERMISSIVE_HOLD

#define USB_SUSPEND_WAKEUP_DELAY 0
#define CAPS_LOCK_STATUS
#define HOLD_ON_OTHER_KEY_PRESS
#define SERIAL_NUMBER "v5vgb/jq9NJ"
#define LAYER_STATE_8BIT

#define TAPPING_TERM_PER_KEY
#define RGB_MATRIX_STARTUP_SPD 60

#define SMTD
//#define ACHORDION


#ifdef SMTD
#define MAX_DEFERRED_EXECUTORS 10
#define SMTD_GLOBAL_SEQUENCE_TERM 200
#endif


#ifdef ACHORDION
#define ACHORDION_STREAK
#endif