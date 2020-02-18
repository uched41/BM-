#include "utils.h"
#include "nrf_log.h"
#include "response.h"
#include "nrf_drv_systick.h"

void my_init(){
	NRF_LOG_INFO("Custom initialization.");
	nrfx_systick_init();
        //rtc_config();
	//tag_init();
	
}

