/*
 * aspire300x
 * Provide backligth and other controls for Acer Aspire 3000 series laptop's
 *
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * File: ec_control.c
 * Date: Wed Feb 27 2008
 * Note: Functions (with minimal changes) from acerhk kernel driver, written 
 *       by Olaf Tauber. Thanks to Olaf.
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
#include <linux/mc146818rtc.h>
#include <linux/delay.h>
#include <linux/i8042.h>
#include "ec_control.h"


int wait_ecwrite(void)
{
	int i = 0;
    while ((inb(EC_STATUS_REG) & 0x02) && (i < 10000)) {
    	udelay(50);
        i++;
    }
	return -(i == 10000);
}


int wait_ecread(void)
{
	int i = 0;
    while (!(inb(EC_CNTL_REG) & 0x01) && (i < 10000)) {
    	udelay(10);
        i++;
    }
	return -(i == 10000);
}


/**
 * Return a EC register value
 * @reg Register
 */
unsigned char aspire300x_getreg(unsigned char reg)
{
	unsigned char val = 0;

	preempt_disable();

	if (!wait_ecwrite())
		outb(0x80, EC_STATUS_REG);

	if (!wait_ecwrite())
		outb(reg, EC_DATA_REG);

	if (!wait_ecread())
		val = inb(EC_DATA_REG);

	preempt_enable_no_resched();
	return(val);
}


/**
 * Set EC register value
 * @reg Register
 * @val Register value
 */
int aspire300x_setreg(unsigned char reg, unsigned char val)
{
	preempt_disable();

	if(!wait_ecwrite())
	  outb(0x81, EC_CNTL_REG);

	if (!wait_ecwrite())
      outb(reg, EC_DATA_REG);

	if (!wait_ecwrite())
      outb(val, EC_DATA_REG);

	preempt_enable_no_resched();
	return(1);
}

