#include "display_wrapper.h"
#include "cfaf.h"
#include "nrf_log.h"
#include "myconfig.h"
/*
// Positions of each line
const uint8_t y_positions[5] = {45, 65, 85, 105, 125};
color_t bg_color = {56, 149, 211};
color_t border_color = {211, 72, 56};

cfaf_t my_display = {
	._cs = DISPLAY_SS_PIN,
	._mosi = DISPLAY_MOSI_PIN,
	._miso = DISPLAY_MISO_PIN,
	._sck = DISPLAY_SCK_PIN,
	._reset = DISPLAY_RESET_PIN,
	._rs = DISPLAY_RS_PIN,
	.width = 240,
	.height = 320,
	.cursor_x = 3,
	.cursor_y = 5,
	.text_color = {0, 0, 0},
	.text_size = 2,
	.text_bgcolor = {0xff, 0x00, 0x00},
	.wrap = true,
};

void display_wrapper(void){
	Initialize_Cfaf(&my_display);
	clear_screen();
}

void clear_screen(void){
	Fill_LCD(&my_display, bg_color.r, bg_color.g, bg_color.b);
	
	// Draw border
	color_t col = {0, 0, 0};
	Draw_Line(&my_display, border_x_1, border_y_1, border_x_2, border_y_1, border_color);
	Draw_Line(&my_display, border_x_1, border_y_1, border_x_1, border_y_2, border_color);
	Draw_Line(&my_display, border_x_2, border_y_1, border_x_2, border_y_2, border_color);
	Draw_Line(&my_display, border_x_1, border_y_2, border_x_2, border_y_2, border_color);
	
	Set_Cursor(&my_display, (240 / 2) -  (sizeof("[ Mind Assist ]")*6) , 20);
	Write_Text(&my_display, "[ Mind Assist ]", sizeof("[ Mind Assist ]"));
	
	Set_Cursor(&my_display, 8, 60);
	Write_Text(&my_display, "- Get Groceries.", sizeof("- Get Groceries."));
	
	Set_Cursor(&my_display, 8, 100);
	Write_Text(&my_display, "- Bath pet dog.", sizeof("- Bath pet dog."));
	
	Set_Cursor(&my_display, 8, 140);
	Write_Text(&my_display, "- Pay utility bill.", sizeof("- Pay utility bill."));
	
	Set_Cursor(&my_display, 8, 180);
	Write_Text(&my_display, "- Charge scooter.", sizeof("- Charge scooter."));
	
	Set_Cursor(&my_display, 8, 220);
	Write_Text(&my_display, "- Go to the bar.", sizeof("- Go to the bar."));
}

void write_reminders(cfaf_t* cfaf, user_data_t* user){
  //Fill display with a given RGB value
	On_Display(cfaf);
  clear_screen();
	Set_Cursor(cfaf, 3, 25);
	uint8_t count = 0;
	
	for(uint8_t i=0; i<4; i++){
		if(user->reminder[i].is_set){
			Write_Text(cfaf, user->reminder[i].data, user->reminder[i].len);
			Set_Cursor(cfaf, 3, y_positions[count]);
			count++;
		}
	}
} 

void off_display(cfaf_t* cfaf){
	Fill_LCD(cfaf, 0x00,0x00,0x00);
	Off_Display(cfaf);
}*/