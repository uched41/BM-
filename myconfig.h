#ifndef MYCONFIG_H
#define MYCONFIG_H

#define PROXIMITY_TIMEOUT	10 * 8  	// (10 secs) Time after which we assume that the tag is no longer in range														
												
/*
 *  Pin definitions
 */
#define SWITCH1_PIN             19
#define SWITCH2_PIN             6
#define SWITCH3_PIN             26
#define SWITCH4_PIN             10

#define BUZZER_PIN              31

#define LED_RING_DATA_PIN       25

#define ACCEL_INTERRUPT_PIN 	9
#define ACCEL_SCL_PIN 		12		
#define ACCEL_SDA_PIN 		11

#define DISPLAY_MISO_PIN 	255 // NOT USED
#define DISPLAY_MOSI_PIN 	16
#define DISPLAY_SCK_PIN 	22
#define DISPLAY_SS_PIN 		23
#define DISPLAY_RESET_PIN	24
#define DISPLAY_RS_PIN		15
#define DISPLAY_BACKLIGHT	14

#define RTC_CE_PIN              28
#define RTC_IO_PIN              29
#define RTC_SCLK_PIN            30


/*
 * LED Ring
 */
#define LED_LENGTH      30

#endif