/**
 * aspire300x
 * Provide backligth and other controls for Acer Aspire 3000 series laptop's
 *
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * File: bl_control.c
 * Date: Mon 20 Oct 2008
 * Note: This driver is based on Carillo Ranch video subsystem and
 * HP Jornada 680 drivers.
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
#include "bl_control.h"


static int old_brightness = 0;
static int suspend;
static char *mname;

/** Power management interface */
#ifdef CONFIG_PM
static int bl_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct backlight_device *bd = platform_get_drvdata(pdev);
	old_brightness				= bd->props.brightness;
 	bd->props.brightness 	    = MIN_BRIGHTNESS;
	bl_set_intensity(bd);

	suspend = 1;

	show_debug(KERN_DEBUG "%s: Go to suspend mode\n", mname);
    return 0;
}

static int bl_resume(struct platform_device *pdev)
{
	struct backlight_device *bd = platform_get_drvdata(pdev);
	bd->props.brightness 	    = old_brightness;
	bl_set_intensity(bd);

	suspend = 0;

	show_debug(KERN_DEBUG "%s: Resume from suspend mode\n", mname);
	return 0;
}
#else
	#define bl_suspend NULL
	#define bl_resume  NULL
#endif
/** -------------- */

static struct platform_device *bl_device;

static struct backlight_ops bl_ops = {
	.get_brightness = bl_get_intensity,
	.update_status  = bl_set_intensity,
};


static int bl_probe(struct platform_device *pdev)
{
	struct backlight_properties props;
	struct backlight_device *bd;

	memset(&props, 0, sizeof(struct backlight_properties));
	props.max_brightness = MAX_BRIGHTNESS;
    bd = backlight_device_register ("acpi_video0", &pdev->dev, NULL, &bl_ops, &props);
	if (IS_ERR(bd)) {
		return PTR_ERR(bd);
	} else {
		bd->props.max_brightness = MAX_BRIGHTNESS;
		bd->props.brightness     = DEFAULT_BRIGHTNESS; // Default brightness
	}

    platform_set_drvdata(pdev, bd);
	return(0);
}


static int bl_remove(struct platform_device *pdev)
{
	struct backlight_device *bd = platform_get_drvdata(pdev);
	backlight_device_unregister(bd);
	return(0);
}

static struct platform_driver bl_driver = {
	.probe   = bl_probe,
	.remove  = bl_remove,
	.suspend = bl_suspend,
	.resume  = bl_resume,
	.driver  = {
		.name = "aspire300x",
	},
};



/** Backlight interface */
int bl_set_intensity(struct backlight_device *bd)
{
	int intensity = bd->props.brightness;

	show_debug(KERN_DEBUG "%s: Set brightness to 0x%x\n", mname, bd->props.brightness);
	
	if (bd->props.power != FB_BLANK_UNBLANK)
		intensity = 0;
	if (bd->props.fb_blank != FB_BLANK_UNBLANK)
		intensity = 0;
	if (suspend)
		intensity = 0;

	if (intensity != 0) {
		return(aspire300x_setreg(LCD_REG, 0xF0 | bd->props.brightness));
	} else {
		return(0);
	}
}

int bl_get_intensity(struct backlight_device *bd)
{
	show_debug(KERN_DEBUG "%s: Get brightness value\n", mname);
	return(aspire300x_getreg(LCD_REG));
}
/** -------------- */


/**
 * Initialize backlight interface
 */
int __init aspire300x_blinit(char *modname)
{
	int ret;
	old_brightness = MIN_BRIGHTNESS;
	mname          = modname;

	show_debug(KERN_DEBUG "%s: Load backlight interface\n", modname);
	ret = platform_driver_register(&bl_driver);
	if (!ret) {
		bl_device = platform_device_alloc("aspire300x", -1);
		if (!bl_device)
			return(-ENOMEM);

		ret = platform_device_add(bl_device);
		if (ret) {
			platform_device_put(bl_device);
			platform_driver_unregister(&bl_driver);
		}
	}

	return(ret);
}


/**
 * Unload backlight interface
 */
void __exit aspire300x_blexit(void)
{
	show_debug(KERN_DEBUG "%s: Unload backlight interface\n", mname);
	platform_device_unregister(bl_device);
	platform_driver_unregister(&bl_driver);
	return;
}

