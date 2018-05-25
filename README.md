# OpenWRT_Atheros_CSI_Tool_for_Netgear_WNDR3700v4
We modified the openWRT source code that embeds Atheros CSI tool to let it work for Netgear WNDR3700v4, which is not supported by [the release](https://github.com/xieyaxiongfly/Atheros_CSI_tool_OpenWRT_src) of Dr. Yaxiong Xie.

If you are interested in Atheros CSI tool, please refer to [the maintenance page](http://pdcc.ntu.edu.sg/wands/Atheros/) for more information.

## Install OpenWRT
### Install from binary firmware
You need the system firmware to install OpenWRT on your device. Please obtain the firmware for Netgear WNDR3700v4 from our [GitHub](). There are only two firmware in this Github folder:
* openwrt-ar71xx-nand-wndr3700v4-ubi-factory.img --- it is used when you want to upgrade from the origin system to OpenWRT for the first time.
* openwrt-ar71xx-nand-wndr3700v4-squashfs-sysupgrade.tar --- it is used when your device has already been flashed with OpenWRT and you want to update the system running on it.

The device runs the system provided by the vendors, please use the web browser. First of all, connect your PC with the router LAN port via ethernet cable. After buiding connection with the router, open your web browser and visit IP "192.168.1.1". Now, you are actually visitng the WebUI provided by the vendor. Find the "Firmware Upgrade" option and use it to upgrade the firmware (openwrt-ar71xx-nand-wndr3700v4-ubi-factory.img).

If your device has already been flashed with OpenWRT, you can use the WebUI provided by OpenWRT called [Luci](https://wiki.openwrt.org/doc/techref/luci) to upgrade the firmware.



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Below is the README from official OpenWRT github
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

This is the buildsystem for the OpenWrt Linux distribution.

Please use "make menuconfig" to choose your preferred
configuration for the toolchain and firmware.

You need to have installed gcc, binutils, bzip2, flex, python, perl, make,
find, grep, diff, unzip, gawk, getopt, subversion, libz-dev and libc headers.

Run "./scripts/feeds update -a" to get all the latest package definitions
defined in feeds.conf / feeds.conf.default respectively
and "./scripts/feeds install -a" to install symlinks of all of them into
package/feeds/.

Use "make menuconfig" to configure your image.

Simply running "make" will build your firmware.
It will download all sources, build the cross-compile toolchain,
the kernel and all choosen applications.

To build your own firmware you need to have access to a Linux, BSD or MacOSX system
(case-sensitive filesystem required). Cygwin will not be supported because of
the lack of case sensitiveness in the file system.
