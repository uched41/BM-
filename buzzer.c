#include "nrf_drv_gpiote.h"
#include "nrf_drv_pwm.h"
#include "nrf_log.h"

#include "buzzer.h"
#include "display_wrapper.h"
#include "utils.h"
#include "myconfig.h"

static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
static nrf_pwm_values_individual_t m_seq_values;
static nrf_pwm_sequence_t const    m_seq =
{
    .values.p_individual = &m_seq_values,
    .length              = NRF_PWM_VALUES_LENGTH(m_seq_values),
    .repeats             = 0,
    .end_delay           = 0
};

static void pwm_init(void)
{
    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            BUZZER_PIN, // channel 0
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 1
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 2
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_500kHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 250,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    // Init PWM without error handler
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config0, NULL));

    m_seq_values.channel_0 = 50;    
}


void init_buzzer(void){
    pwm_init();
}

void buzzer_on(void){
    (void)nrf_drv_pwm_simple_playback(&m_pwm0, &m_seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

void buzzer_off(void){
    nrf_drv_pwm_stop(&m_pwm0, false);
}


