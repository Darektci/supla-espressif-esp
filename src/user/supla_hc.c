/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// Code based on http://tech.scargill.net/esp8266-and-the-dallas-ds18b20-and-ds18b20p/

#include <os_type.h>
#include <osapi.h>
#include <eagle_soc.h>
#include <ets_sys.h>
#include <gpio.h>

#include "supla-dev/log.h"
#include "supla_hc.h"
#include "supla_esp_devconn.h"
#include "supla_esp_cfg.h"

#ifdef HC

static double distance = 0.0;
static double mindist = 3;
static double maxdist = 0;

int supla_hc_trig_pin = GPIO_ID_PIN(4);
int supla_hc_echo_pin = GPIO_ID_PIN(5);

ETSTimer supla_hc_timer1;
ETSTimer supla_hc_timer2;

void supla_hc_init(void) {
	
 	
 	 	
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	//PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U);
   	
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	//PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO5_U);

}


void
supla_hc_read_distanceB(void *timer_arg) {

	os_timer_disarm(&supla_hc_timer2);

//	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	//PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U);	   	
   // PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	//PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO5_U);
	//distance= 8;
    
   
    GPIO_DIS_OUTPUT(supla_hc_echo_pin);
    
	
	int readStatus=0;
	long EchoTime=0;

	
	supla_log(LOG_DEBUG, "distance %d",distance);
	
	
    GPIO_OUTPUT_SET( supla_hc_trig_pin, 0 );
    os_delay_us(2);
    
      
    GPIO_OUTPUT_SET( supla_hc_trig_pin, 1 );
    os_delay_us(10);
       
    GPIO_OUTPUT_SET( supla_hc_trig_pin, 0 );
                
     
      while(readStatus == 0)
       	{
       		while(GPIO_INPUT_GET(supla_hc_echo_pin) == 1)
       		{
       			EchoTime++;
       			readStatus = 1;
       		}
       	}	
    
      //supla_log(LOG_DEBUG, "Echo %i",EchoTime);
    distance = (EchoTime * 0.00862) ;  //return data in cm	
       	
    supla_log(LOG_DEBUG, "distance %i",distance);
    supla_log(LOG_DEBUG, "maxdist %i",maxdist);
    supla_log(LOG_DEBUG, "mindist %i",mindist);
     
    //todo jezeli max jest wiekszy niz 200 to = 200 -> wartosc max z konfiguracji
    if (distance >= maxdist) {
    	distance = maxdist;
    }

    if (distance <= mindist) {
    	distance = mindist;
    }
    
    //jeeli opcja odrotnego pomiaru to
    if (supla_esp_cfg.DepthDirection == 1) {
      distance = (maxdist) - distance;
  
      if (distance == (maxdist - mindist)) {
    	distance = maxdist;
      }
    }
    
    
    distance = distance / 100;
    
    supla_esp_devconn_on_temp_humidity_changed(distance);
    
        
      
   

}




void
supla_hc_read_distnaceA(void *timer_arg) {

	os_delay_us(480);
	os_delay_us(480);
	
	
	os_timer_disarm(&supla_hc_timer2);
	os_timer_setfn(&supla_hc_timer2, supla_hc_read_distanceB, NULL);
	os_timer_arm (&supla_hc_timer2, 1000, 0);

}

void supla_hc_start(void)
{
	//distance = supla_esp_cfg.DepthCoal;
	maxdist = supla_esp_cfg.DepthCoal ;

	os_timer_disarm(&supla_hc_timer1);
	os_timer_setfn(&supla_hc_timer1, supla_hc_read_distnaceA, NULL);
	os_timer_arm (&supla_hc_timer1, 2000, 1);
}

void supla_get_hc(char value[SUPLA_CHANNELVALUE_SIZE]) {
	
	memcpy(value, &distance, sizeof(double));
}
#endif

 
