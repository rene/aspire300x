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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include "aspire3.h"
#include "bl_control.h"


int aspire300x_debug_on = 0;
static int debug_on     = 0;
static int tcontrol;
static int wcontrol;
static struct proc_dir_entry *pdir;


/**
 * read_callback
 * Callback function to read wireless and touchpad status
 */
static int read_callback(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
	int ret = 0;
    int status;
	int opt;
	char *pname;
	unsigned char on, off;
	unsigned char reg;

	if(data != NULL && offset == 0) {
		opt = *((int*)data);

		if(opt == F_TOUCHPAD) {
			pname = PROCNAME_TOUCHPAD;
			reg   = TOUCHPAD_REG;
			on    = TOUCHPAD_ENABLE;
			off   = TOUCHPAD_DISABLE;
		} else if (opt == F_WIRELESS) {
			pname = PROCNAME_WIRELESS;
			reg   = WIRELESS_REG;
			on    = WIRELESS_ENABLE;
			off   = WIRELESS_DISABLE;
		} else {
			printk(KERN_ERR "%s: Function not implemented!\n", THIS_MODULE->name);
			return(-ENOSYS);
		}

		show_debug(KERN_DEBUG "%s: procfile_read (/proc/%s/%s) called\n", THIS_MODULE->name, PROCNAME_DIR, pname);

		status = aspire300x_getreg(reg);
		if(status == on) {
			ret = sprintf(buffer, "1\n");
		} else if(status == off) {
			ret = sprintf(buffer, "0\n");
		} else {
			ret = 0;
			printk(KERN_ERR "%s: Unknown value on EC register!\n", THIS_MODULE->name);
		}
	}

	return(ret);
}


/**
 * aspire3_init
 * Init module functions
 */
static int __init aspire3_init(void)
{
	tcontrol = F_TOUCHPAD;
	wcontrol = F_WIRELESS;

	aspire300x_debug_on = debug_on;

	printk(KERN_INFO "%s: Load module\n", THIS_MODULE->name);

	/* Initialize proc filesystem */
	pdir = proc_mkdir(PROCNAME_DIR, NULL);
	show_debug(KERN_DEBUG "%s: Initializing /proc/%s\n", THIS_MODULE->name, PROCNAME_DIR);
	if(pdir == NULL) {
		printk(KERN_ERR "%s: Could not initialize /proc/%s\n", THIS_MODULE->name, PROCNAME_DIR);
		return(-ENOMEM);
	}

	show_debug(KERN_DEBUG "%s: Initializing /proc/%s/%s\n", THIS_MODULE->name, PROCNAME_DIR, PROCNAME_TOUCHPAD);
	if(create_proc_read_entry(PROCNAME_TOUCHPAD, 0644, pdir, read_callback, &tcontrol) == NULL) {
		printk(KERN_ERR "%s: Could not create /proc/%s/%s\n", THIS_MODULE->name, PROCNAME_DIR, PROCNAME_TOUCHPAD);
		goto error2;
	}

	show_debug(KERN_DEBUG "%s: Initializing /proc/%s/%s\n", THIS_MODULE->name, PROCNAME_DIR, PROCNAME_WIRELESS);
	if(create_proc_read_entry(PROCNAME_WIRELESS, 0644, pdir, read_callback, &wcontrol) == NULL) {
		printk(KERN_ERR "%s: Could not create /proc/%s/%s\n", THIS_MODULE->name, PROCNAME_DIR, PROCNAME_WIRELESS);
		goto error1;
	}

	/* Initialize backlight interface */
	if(aspire300x_blinit(THIS_MODULE->name) != 0) {
		printk(KERN_ERR "%s: Could not initialize backlight interface\n", THIS_MODULE->name);
		goto error1;
	}

	return(0);


error1:
	remove_proc_entry(PROCNAME_TOUCHPAD, pdir);
error2:
	remove_proc_entry(PROCNAME_DIR, NULL);

	return(-ENOMEM);
}


/**
 * aspire3_exit
 * cleanup module
 */
static void __exit aspire3_exit(void)
{
	printk(KERN_INFO "%s: Unload module\n", THIS_MODULE->name);

	show_debug(KERN_DEBUG "%s: Removing /proc/%s/%s\n", THIS_MODULE->name, PROCNAME_DIR, PROCNAME_WIRELESS);
	remove_proc_entry(PROCNAME_WIRELESS, pdir);

	show_debug(KERN_DEBUG "%s: Removing /proc/%s/%s\n", THIS_MODULE->name, PROCNAME_DIR, PROCNAME_TOUCHPAD);
	remove_proc_entry(PROCNAME_TOUCHPAD, pdir);

	show_debug(KERN_DEBUG "%s: Removing /proc/%s\n", THIS_MODULE->name, PROCNAME_DIR);
	remove_proc_entry(PROCNAME_DIR, NULL);

	show_debug(KERN_DEBUG "%s: Freeing memory\n", THIS_MODULE->name);

	/* Unload backlight interface */
	aspire300x_blexit();

	return;
}


module_init(aspire3_init);
module_exit(aspire3_exit);
module_param(debug_on, bool, S_IRUGO);
MODULE_AUTHOR("Renê de Souza Pinto");
MODULE_DESCRIPTION("Provide backligth and other controls for Acer Aspire 3000 series laptop's");
MODULE_LICENSE("GPL");

