#include "nrf_drv_gpiote.h"
#include "nrf_log.h"
#include "buttons.h"
#include "myconfig.h"

void button1_cb(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    switch(pin){
      case SWITCH1_PIN:
        break;
      case SWITCH2_PIN:
        break;
      case SWITCH3_PIN:
        break;
      case SWITCH4_PIN:
        break;
      default:
        break;
    }NRF_LOG_INFO("Initializing buttons ...");
}

void init_buttons(void){
    ret_code_t err_code;
    NRF_LOG_INFO("Initializing buttons ...");
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    in_config.pull = NRF_GPIO_PIN_PULLDOWN;

    err_code = nrf_drv_gpiote_in_init(SWITCH1_PIN, &in_config, button1_cb);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_gpiote_in_init(SWITCH2_PIN, &in_config, button1_cb);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_gpiote_in_init(SWITCH3_PIN, &in_config, button1_cb);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_gpiote_in_init(SWITCH4_PIN, &in_config, button1_cb);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(SWITCH1_PIN, true);
    nrf_drv_gpiote_in_event_enable(SWITCH2_PIN, true);
    nrf_drv_gpiote_in_event_enable(SWITCH3_PIN, true);
    nrf_drv_gpiote_in_event_enable(SWITCH4_PIN, true);

    NRF_LOG_INFO("Button initialization complete.");

}