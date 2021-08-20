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

#define EDDY_RELAY_DOMAIN_NAME      "relays"
#define EDDY_SW_DOMAIN_NAME         "switches"

#define EDDY_RELAY1_PIN              5 // D1 on WEMOS D1 Mini shield
#define EDDY_RELAY1_PIN_ACTIVE_HIGH  true
#define EDDY_RELAY1_GPIO_PULL_TYPE   MGOS_GPIO_PULL_UP

#define EDDY_SW1_PIN                 0 // D3 on WEMOS D1 Mini shield
#define EDDY_SW1_PIN_ACTIVE_HIGH     false
#define EDDY_SW1_GPIO_PULL_TYPE      MGOS_GPIO_PULL_UP

enum mg_eddy_sw_mode {
  MG_EDDY_SW_MODE_DASH_BUTTON = 0,
  MG_EDDY_SW_MODE_DETACHED = 1,
  MG_EDDY_SW_MODE_TOGGLE_ON_PUSH = 2,
  MG_EDDY_SW_MODE_TOGGLE_ON_EDGE = 3
};

enum mg_eddy_sw_mode mg_eddy_get_sw_mode(mgos_bbsensor_t sw) {
  if (sw == s_sw1) {
    return  mgos_sys_config_get_eddy_sw1_mode();
  }
  return MG_EDDY_SW_MODE_DETACHED;
}

static void mg_eddy_sw_state_changed(int ev, void *ev_data, void *userdata) {
  struct mgos_bthing_state *args = (struct mgos_bthing_state *)ev_data;
  mgos_bbsensor_t sw = (mgos_bbsensor_t)args->thing;
  enum mg_eddy_sw_mode mode = mg_eddy_get_sw_mode(sw);

  bool bool_state;
  bool state_init = ((args->state_flags & MGOS_BTHING_STATE_FLAG_INITIALIZING) == MGOS_BTHING_STATE_FLAG_INITIALIZING);
  if (mgos_bbsensor_state_parse(sw, args->state, &bool_state)) {
    switch (mode) {
      case MG_EDDY_SW_MODE_TOGGLE_ON_PUSH:
        if (bool_state) {
          mgos_bbactuator_toggle_state(s_relay1);
        }
        break;
      case MG_EDDY_SW_MODE_TOGGLE_ON_EDGE: 
        if (!state_init || (state_init && bool_state)) {
          mgos_bbactuator_toggle_state(s_relay1);
        }
        break;
      default:
        break;
    }
  }
  (void) ev;
  (void) userdata;
}

mgos_bswitch_t mg_eddy_init_bswitch(const char *id, int pin, bool active_high,
                                    enum mgos_gpio_pull_type pull_type,
                                    int grp_id, double inching_timeout, bool inching_lock) {
  mgos_bswitch_t relay = mgos_bswitch_create(id, grp_id,
    MGOS_BSWITCH_DEFAULT_SWITCHING_TIME, EDDY_RELAY_DOMAIN_NAME);
  if (relay) {
    mgos_bthing_t thing = MGOS_BSWITCH_THINGCAST(relay);
    mgos_bbsensor_set_verbose_state(MGOS_BSWITCH_SENSCAST(relay), EDDY_RELAY_PAYLOAD_ON, EDDY_RELAY_PAYLOAD_OFF);
    if (mgos_bthing_gpio_attach(thing, pin, active_high, pull_type)) {
      if (inching_timeout == 0) return relay; // success
      if (mgos_bswitch_set_inching(relay, (inching_timeout * 1000), true)) return relay; // success
    }
  }
  return NULL; // something went wrong
}

mgos_bbsensor_t mg_eddy_init_bbsensor(const char *id, int pin, enum mg_eddy_sw_mode mode,
                                      bool active_high, enum mgos_gpio_pull_type pull_type) {
  if (mode == MG_EDDY_SW_MODE_DASH_BUTTON) {
    mgos_bbutton_t btn = mgos_bbutton_create(id, EDDY_SW_DOMAIN_NAME);
    if (btn && mgos_bthing_gpio_attach(MGOS_BBUTTON_THINGCAST(btn), pin, active_high, pull_type)) {
      return btn;
    }

  } else {
    mgos_bbsensor_t sw = mgos_bbsensor_create(id, EDDY_SW_DOMAIN_NAME);
    if (sw) {
      mgos_bthing_t thing = MGOS_BBSENSOR_THINGCAST(sw);
      mgos_bbsensor_set_verbose_state(sw, EDDY_SW_PAYLOAD_ON, EDDY_SW_PAYLOAD_OFF);
      if (mgos_bsensor_update_on_int(MGOS_BBSENSOR_DOWNCAST(sw), pin, pull_type, MGOS_GPIO_INT_EDGE_ANY, 50) &&
          mgos_bthing_gpio_attach(thing, pin, active_high, pull_type)) {
        if (mode != MG_EDDY_SW_MODE_DETACHED) {
          mgos_bthing_on_event(thing, MGOS_EV_BTHING_STATE_CHANGED, mg_eddy_sw_state_changed, NULL); 
        }
        return sw; // success
      }
    }
  }
  return NULL; // something went wrong
}

bool mg_eddy_init_actuators() {
  // create and initialize the relay #1
  s_relay1 = mg_eddy_init_bswitch(mgos_sys_config_get_eddy_relay1_id(),
     EDDY_RELAY1_PIN, EDDY_RELAY1_PIN_ACTIVE_HIGH, EDDY_SW1_GPIO_PULL_TYPE,
     mgos_sys_config_get_eddy_relay1_grp_id(),
     mgos_sys_config_get_eddy_relay1_inching_timeout(),
     mgos_sys_config_get_eddy_relay1_inching_lock());
  if (!s_relay1) return false;

  return true; //success
}

bool mg_eddy_init_sensors() {
  // create and initialize the switch #1
  s_sw1 = mg_eddy_init_bbsensor(mgos_sys_config_get_eddy_sw1_id(),
    EDDY_SW1_PIN, mgos_sys_config_get_eddy_sw1_mode(),
    EDDY_SW1_PIN_ACTIVE_HIGH, EDDY_SW1_GPIO_PULL_TYPE);
  if (!s_sw1) return false;

  return true; //success
}

enum mgos_app_init_result mgos_app_init(void) {
  if (!mg_eddy_init_actuators())
    return MGOS_APP_INIT_ERROR;

  if (!mg_eddy_init_sensors())
    return MGOS_APP_INIT_ERROR;

  return MGOS_APP_INIT_SUCCESS;
}