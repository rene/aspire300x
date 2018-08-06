/*
 * aspire300x
 * Provide backligth and other controls for Acer Aspire 3000 series laptop's
 *
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * File: ec_control.h
 * Date: Wed Feb 27 2008
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
#ifndef ASPIRE300x_EC_H_

	#define  ASPIRE300x_EC_H_

	#include <linux/preempt.h>

	#define EC_STATUS_REG		0x66	/* Status register of EC (R) */
	#define EC_CNTL_REG		    0x66	/* Controller command register of EC (W) */
	#define EC_DATA_REG		    0x62	/* EC data register (R/W) */

	#define KBD_STATUS_REG		0x64	/* Status register (R) */
	#define KBD_CNTL_REG		0x64	/* Controller command register (W) */
	#define KBD_DATA_REG		0x60	/* Keyboard data register (R/W) */

	int wait_ecwrite(void);
	int wait_ecread(void);
	unsigned char aspire300x_getreg(unsigned char reg);
	int aspire300x_setreg(unsigned char reg, unsigned char val);

#endif /* ASPIRE300x_EC_H_ */

