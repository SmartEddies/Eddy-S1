#include "mgos.h"
#include "mgos_bswitch.h"
#include "mgos_bbutton.h"
#include "mgos_bthing_gpio.h"

static mgos_bswitch_t s_relay1 = NULL;
static mgos_bbsensor_t s_sw1 = NULL;

#define EDDY_RELAY_PAYLOAD_ON        "ON"
#define EDDY_RELAY_PAYLOAD_OFF       "OFF"

#define EDDY_SW_PAYLOAD_ON          "CLOSED"
#define EDDY_SW_PAYLOAD_OFF         "OPEN"

#define EDDY_RELAY1_PIN              5
#define EDDY_RELAY1_PIN_ACTIVE_HIGH  true
#define EDDY_RELAY1_GPIO_PULL_TYPE   MGOS_GPIO_PULL_UP

#define EDDY_SW1_PIN                 14
#define EDDY_SW1_PIN_ACTIVE_HIGH     false
#define EDDY_SW1_GPIO_PULL_TYPE      MGOS_GPIO_PULL_UP

enum mg_eddy_sw_mode {
  MG_EDDY_SW_MODE_DETACHED = 0,
  MG_EDDY_SW_MODE_DASHBUTTON = 1,
  MG_EDDY_SW_MODE_PUSH_TOGGLE = 2,
  MG_EDDY_SW_MODE_EDGE_TOGGLE = 4,
};

static void mg_eddy_sw_state_changed(struct mgos_bthing_state_changed_arg *args, void *userdata) {
  if (args->state_init) return;

  mgos_bbsensor_t sw = (mgos_bbsensor_t)args->thing;
  enum mg_eddy_sw_mode mode = MG_EDDY_SW_MODE_DETACHED;
  if (sw == s_sw1) {
    mode = mgos_sys_config_get_eddy_sw1_mode();
  }

  bool bool_state;
  if (mgos_bbsensor_state_parse(sw, args->state, &bool_state)) {
    switch (mode) {
      case MG_EDDY_SW_MODE_PUSH_TOGGLE:
        if (bool_state) {
          mgos_bbactuator_toggle_state(s_relay1);
        }
        break;
      case MG_EDDY_SW_MODE_EDGE_TOGGLE:
        mgos_bbactuator_toggle_state(s_relay1);
        break;
      default:
        break;
    }
  }

  (void) userdata;
}

enum mgos_app_init_result mgos_app_init(void) {
  NULL;

  // create and initialize the relay #1
  s_relay1 = mgos_bswitch_create(mgos_sys_config_get_eddy_relay1_id(),
    MGOS_BSWITCH_NO_GROUP, MGOS_BSWITCH_DEFAULT_SWITCHING_TIME);
  mgos_bthing_gpio_attach(MGOS_BSWITCH_THINGCAST(s_relay1),
    EDDY_RELAY1_PIN, EDDY_RELAY1_PIN_ACTIVE_HIGH, EDDY_RELAY1_GPIO_PULL_TYPE);
  mgos_bbsensor_set_verbose_state(MGOS_BSWITCH_SENSCAST(s_relay1),
    EDDY_RELAY_PAYLOAD_ON, EDDY_RELAY_PAYLOAD_OFF);
  if (mgos_sys_config_get_eddy_relay1_inching_timeout() > 0) {
    mgos_bswitch_set_inching(s_relay1, (mgos_sys_config_get_eddy_relay1_inching_timeout() * 1000), false);
  }

  // create and initialize the switch #1
  if (mgos_sys_config_get_eddy_sw1_mode() == MG_EDDY_SW_MODE_DASHBUTTON) {
    mgos_bbutton_t btn = mgos_bbutton_create(mgos_sys_config_get_eddy_sw1_id());
    mgos_bthing_gpio_attach(MGOS_BBUTTON_THINGCAST(btn), EDDY_SW1_PIN,
      EDDY_SW1_PIN_ACTIVE_HIGH, EDDY_SW1_GPIO_PULL_TYPE);
  } else {
    s_sw1 = mgos_bbsensor_create(mgos_sys_config_get_eddy_sw1_id());
    mgos_bbsensor_set_verbose_state(s_sw1, EDDY_SW_PAYLOAD_ON, EDDY_SW_PAYLOAD_OFF);
    mgos_bsensor_update_on_int(MGOS_BBSENSOR_DOWNCAST(s_sw1),
      EDDY_SW1_PIN, EDDY_SW1_GPIO_PULL_TYPE, MGOS_GPIO_INT_EDGE_ANY, 50);
    mgos_bthing_gpio_attach(MGOS_BBSENSOR_THINGCAST(s_sw1), EDDY_SW1_PIN,
      EDDY_SW1_PIN_ACTIVE_HIGH, EDDY_SW1_GPIO_PULL_TYPE);
    if (mgos_sys_config_get_eddy_sw1_mode() != MG_EDDY_SW_MODE_DETACHED) {
      mgos_bthing_on_state_changed(MGOS_BBSENSOR_THINGCAST(s_sw1), mg_eddy_sw_state_changed, NULL);
    }
  }

  return MGOS_APP_INIT_SUCCESS;
}