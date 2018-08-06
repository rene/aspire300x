/**
 * aspire300x
 * Provide backligth and other controls for Acer Aspire 3000 series laptop's
 *
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * File: bl_control.h
 * Date: Mon 20 Oct 2008
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
#ifndef BLCONTROL_H_

	#include <linux/module.h>
	#include <linux/kernel.h>
	#include <linux/init.h>
	#include <linux/backlight.h>
	#include <linux/platform_device.h>
	#include <linux/fb.h>

	#define BLCONTROL_H_ 1

	#define show_debug 	if(aspire300x_debug_on == 1) printk

	#define MIN_BRIGHTNESS   	  0x00
	#define MAX_BRIGHTNESS   	  0x0F
	#define DEFAULT_BRIGHTNESS    0x08
	#define LCD_REG        		  0x51


	int __init aspire300x_blinit(char *modname);
	void __exit aspire300x_blexit(void);
	int bl_set_intensity(struct backlight_device *bd);
	int bl_get_intensity(struct backlight_device *bd);

	extern int aspire300x_debug_on;
	extern unsigned char aspire300x_getreg(unsigned char reg);
	extern int aspire300x_setreg(unsigned char reg, unsigned char val);

#endif /* BLCONTROL_H_ */

