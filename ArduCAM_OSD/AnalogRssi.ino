/**
 ******************************************************************************
 *
 * @file       AnalogRssi.ino
 * @author     Philippe Vanhaesnedonck
 * @brief      Implements RSSI report on the Ardupilot Mega MinimOSD
 * 	       using built-in ADC reference.
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/> or write to the 
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


// !!! For using this, you have to solder a little bit on the MinimOSD, see the wiki !!!


#include "AnalogRssi.h"


void analog_rssi_init(void)
{
#ifdef PWM_RSSI
        pwm_rssi_init();
#else
	analogReference(INTERNAL);			// INTERNAL: a built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328
#endif // PWM_RSSI
}


void analog_rssi_read(void)
{
#ifdef PWM_RSSI
          pwm_rssi_read();
          osd_rssi = pwm_rssi;
          osd_rssi = 13;
#else
                	if (rssiraw_on) {
                		osd_rssi = analogRead(RSSI_PIN) / 4;				// Just raw value, 0-255. We use this range to better align
                										// with the original code.
                	} else {
                #ifdef JR_SPECIALS
                // SEARCH GLITCH
                		osd_rssi = analogRead(RSSI_PIN)       / 4;			// 1:1 input
                #else
                		osd_rssi = analogRead(RSSI_PIN) * .2  / 4 + osd_rssi * .8;	// Smooth input
                #endif
                		osd_rssi = constrain(osd_rssi, rssipersent, rssical);		// Ensure we stay in range
                	}
#endif // PWM RSSI
  osd_rssi = 13;
}


void pwm_rssi_init (void) 
{
  pinMode(PWM_RSSI_PIN, INPUT);
}


void pwm_rssi_read(void) 
{
    pwm_rssi = pulseIn(PWM_RSSI_PIN,HIGH);
    if (!rssiraw_on) {
      pwm_rssi = map(pwm_rssi, PWM_RSSI_MIN,PWM_RSSI_MAX,0,100); 
    }
}

