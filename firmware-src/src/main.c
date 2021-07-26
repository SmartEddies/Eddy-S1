#include "mgos.h"
#include "mgos_bswitch.h"
#include "mgos_bthing_gpio.h"

//$bthings/bThing-ABB28C/switch3/state
//$bthings/bThing-ABB28C/switch3/set

static int gpio_pin1 = 5;   //Wemos D1
static int gpio_pin2 = 4;   //Wemos D2
static int gpio_pin3 = 12;  //Wemos D7
static int gpio_pin4 = 13;  //Wemos D6

static void switch_state_changed_cb(int ev, void *ev_data, void *userdata) {
  mgos_bthing_t thing = MGOS_BSWITCH_THINGCAST((mgos_bswitch_t)ev_data);

  mgos_bvarc_t state = mgos_bthing_get_state(thing);
  if (mgos_bvar_is_changed(state)) {
    const char *id = mgos_bthing_get_id(thing);
    if (mgos_bvar_get_type(state) == MGOS_BVAR_TYPE_BOOL) {
      LOG(LL_INFO, ("The '%s' has been switched %d", id, mgos_bvar_get_bool(state)));
    } else {
      LOG(LL_INFO, ("The '%s' has been switched %s", id, mgos_bvar_get_str(state)));
    }
  }
}

enum mgos_app_init_result mgos_app_init(void) {
  mgos_event_add_handler(MGOS_EV_BTHING_STATE_CHANGED, switch_state_changed_cb, NULL);

  // create the switch #1
  mgos_bswitch_t sw1 = mgos_bswitch_create("switch1", MGOS_BSWITCH_NO_GROUP, MGOS_BSWITCH_DEFAULT_SWITCHING_TIME);
  mgos_bthing_gpio_attach_ex(MGOS_BSWITCH_THINGCAST(sw1), gpio_pin1, true, MGOS_GPIO_PULL_UP);

  // create the switch #2
  mgos_bswitch_t sw2 = mgos_bswitch_create("switch2", MGOS_BSWITCH_NO_GROUP, MGOS_BSWITCH_DEFAULT_SWITCHING_TIME);
  mgos_bbsensor_enable_str_state(MGOS_BSWITCH_SENSCAST(sw2), "ON", "OFF");
  mgos_bthing_gpio_attach_ex(MGOS_BSWITCH_THINGCAST(sw2), gpio_pin2, true, MGOS_GPIO_PULL_UP);
  mgos_bswitch_set_inching(sw2, 1000, true);

  // create the switch #3 and #4 (group 1)
  mgos_bswitch_t sw3 = mgos_bswitch_create("switch3", 1, MGOS_BSWITCH_DEFAULT_SWITCHING_TIME);
  mgos_bbsensor_enable_str_state(MGOS_BSWITCH_SENSCAST(sw3), "ON", "OFF");
  mgos_bthing_gpio_attach_ex(MGOS_BSWITCH_THINGCAST(sw3), gpio_pin3, true, MGOS_GPIO_PULL_UP);
  // create the switch #4 (group 1)
  mgos_bswitch_t sw4 = mgos_bswitch_create("switch4", 1, MGOS_BSWITCH_DEFAULT_SWITCHING_TIME);
  mgos_bbsensor_enable_str_state(MGOS_BSWITCH_SENSCAST(sw4), "ON", "OFF");
  mgos_bthing_gpio_attach_ex(MGOS_BSWITCH_THINGCAST(sw4), gpio_pin4, true, MGOS_GPIO_PULL_UP);

  return MGOS_APP_INIT_SUCCESS;
}