/**
 * aspire300x
 * Provide backligth and other controls for Acer Aspire 3000 series laptop's
 *
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * File: aspire3.c
 * Date: Fri 17 Oct 2008
 * Note: Functions implemented from a reverse engineering over Aspire 3003LCi
 * Embeeded Controler (PC97551).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ASPIRE300x_H_

        #define ASPIRE300x_H_ 1

		#define show_debug 	if(aspire300x_debug_on == 1) printk

		#define PROCNAME_DIR 		"driver/aspire300x"
		#define PROCNAME_TOUCHPAD 	"touchpad"
		#define PROCNAME_WIRELESS 	"wireless"

        #define TOUCHPAD_REG 	   0xD3
        #define TOUCHPAD_ENABLE    0x00
        #define TOUCHPAD_DISABLE   0x01

        #define WIRELESS_REG 	   0x60
        #define WIRELESS_ENABLE    0x60
        #define WIRELESS_DISABLE   0x20

		#define F_TOUCHPAD		   0x01
		#define F_WIRELESS		   0x02

		static int read_callback(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data);

		extern unsigned char aspire300x_getreg(unsigned char reg);
		extern int aspire300x_setreg(unsigned char reg, unsigned char val);

#endif /* ASPIRE300x_H_ */

