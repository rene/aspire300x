# aspire300x
Linux kernel module that provides backlight and other controls for Acer Aspire 3000 series laptop's

Please, note that **this device driver is outdated**, I will keep this source code available here just
for legacy systems or other purposes.

## About

Aspire300x is a Linux kernel module that provides backlight and other controls for Acer Aspire 3000 series laptop's.

On Windows®, functions such as get the touchpad and wireless status can be
accessed from Acer e-manager tool, that unfortunately, is not avaliable at
Linux in this model. Current features of aspire300x are:

* Control the LCD brightness from software
* Get the touchpad status (enable/disable)
* Get the wireless card status (enable/disable)

This device driver was written after a reverse engineering proccess of PC97551 embedded controller (EC), present in an old Acer Aspire 3003LCi, but its suppose to work on other models that have this EC.

## Install

make

make install

## Uninstall

make uninstall

## License

License under GPLv2. See file COPYING for more details.

## Bugs

Bugs reports to:
	Renê S. Pinto <rene@renesp.com.br>

