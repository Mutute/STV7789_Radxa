**OS nanopoi neo 3 :**
https://armbian.tnahosting.net/archive/nanopineo3/archive/

Notes :
 - Jammy : Ubuntu 22
 - Hirsute : Ubuntu 21
 - Focal : Ubuntu 20
 - Bionic : Ubuntu 18
 - Buster et bullseye : Debian

**Login par defaut :**
 - utilisateur : root
 - mot de passe : 1234

**Activer SPI pour RK3328**


**Etapes :**

 - Activer SPI overlay :

> nano /boot/armbianEnv.txt

    overlays=spi-spidev
    param_spidev_spi_bus=0
	

 - Adapter le fichier overlay pour le RK3328:

> nano rockchip-spi-spidev.dts

    /dts-v1/;
    
    / {
    	compatible = "rockchip,rk3328";
    
    	fragment@0 {
    		target-path = "/aliases";
    
    		__overlay__ {
    			spi0 = "/spi@ff190000";
    		};
    	};
    
    	fragment@1 {
    		target = < 0xffffffff >;
    
    		__overlay__ {
    			#address-cells = < 0x01 >;
    			#size-cells = < 0x00 >;
    
    			spidev {
    				compatible = "spidev";
    				status = "disabled";
    				reg = < 0x00 >;
    				spi-max-frequency = < 0x989680 >;
    			};
    		};
    	};
    
    	__fixups__ {
    		spi0 = "/fragment@1:target:0";
    	};
    };


> dtc -O dtb -o /boot/dtb/rockchip/overlay/rockchip-spi-spidev.dtbo rockchip-spi-spidev.dts

 - Adapter le fixup.script

> nano rockchip-fixup.script

    # overlays fixup script
    # implements (or rather substitutes) overlay arguments functionality
    # using u-boot scripting, environment variables and "fdt" command
    
    setenv decompose_pin 'setexpr tmp_pinctrl sub "GPIO(0|1|2|3|4)_\\S\\d+" "\\1";
    setexpr tmp_bank sub "GPIO\\d_(\\S)\\d+" "\\1";
    test "${tmp_bank}" = "A" && setenv tmp_bank 0;
    test "${tmp_bank}" = "B" && setenv tmp_bank 1;
    test "${tmp_bank}" = "C" && setenv tmp_bank 2;
    test "${tmp_bank}" = "D" && setenv tmp_bank 3;
    setexpr tmp_pin sub "GPIO\\d_\\S(\\d+)" "\\1";
    setexpr tmp_bank ${tmp_bank} * 8;
    setexpr tmp_pin ${tmp_bank} + ${tmp_pin}'
    
    
    if test -n "${param_spinor_spi_bus}"; then
    	test "${param_spinor_spi_bus}" = "0" && setenv tmp_spi_path "spi@ff1c0000"
    	fdt set /${tmp_spi_path} status "okay"
    	fdt set /${tmp_spi_path}/spiflash@0 status "okay"
    	if test -n "${param_spinor_max_freq}"; then
    		fdt set /${tmp_spi_path}/spiflash@0 spi-max-frequency "<${param_spinor_max_freq}>"
    	fi
    	if test "${param_spinor_spi_cs}" = "1"; then
    		fdt set /${tmp_spi_path}/spiflash@0 reg "<1>"
    	fi
    	env delete tmp_spi_path
    fi
    
    if test -n "${param_spidev_spi_bus}"; then
    	test "${param_spidev_spi_bus}" = "0" && setenv tmp_spi_path "spi@ff190000"
    	fdt set /${tmp_spi_path} status "okay"
    	fdt set /${tmp_spi_path}/spidev status "okay"
    	if test -n "${param_spidev_max_freq}"; then
    		fdt set /${tmp_spi_path}/spidev spi-max-frequency "<${param_spidev_max_freq}>"
    	fi
    	if test "${param_spidev_spi_cs}" = "1"; then
    		fdt set /${tmp_spi_path}/spidev reg "<1>";
    	fi
    fi
    
    if test -n "${param_w1_pin}"; then
    	setenv tmp_pinctrl "${param_w1_pin}"
    	setenv tmp_bank "${param_w1_pin}"
    	setenv tmp_pin "${param_w1_pin}"
    	run decompose_pin
    	#echo "${param_w1_pin} ---> pinctrl = ${tmp_pinctrl}"
    	#echo "${param_w1_pin} ---> bank = ${tmp_bank}"
    	#echo "${param_w1_pin} ---> pin = ${tmp_pin}"
    	fdt get value tmp_pinctrl /__symbols__	gpio${tmp_pinctrl}
    	#echo "${param_w1_pin} ---> tmp_pinctrl = ${tmp_pinctrl}"
    	fdt get value tmp_phandle ${tmp_pinctrl} phandle
    	#echo "${param_w1_pin} ---> tmp_phandle = ${tmp_phandle}"
    	fdt set /onewire@0 gpios "<${tmp_phandle} 0x000000${tmp_pin} 0 0>"
    	env delete tmp_pinctrl tmp_bank tmp_pin tmp_phandle
    fi

> mkimage -A arm -T script -C none -d rockchip-fixup.script /boot/dtb/rockchip/overlay/rockchip-fixup.scr

 - Redemarer:

> sudo shutdown -r now

 - Verifier que spidev0.0 apparait

> ls /dev/spi*

**! Ne surtout pas faire sudo apt upgrade, si besoin faire uniquement l'upgrade des packets désirés **

Ces étapes sont basées sur :

https://forum.armbian.com/topic/13032-rk3328-how-to-enable-spi/

> 1. Use dtc to de-compile the existing rockchip-spi-spidev.dtbo to .dts:
> 
> 	dtc -I dtb -O dts /boot/dtb/rockchip/overlay/rockchip-spi-spidev.dtbo
> -o rockchip-spi-spidev.dts
> 
> 2. Edit the resulting rockchip-spi-spidev.dts and change instances of ff1c to ff19, and 3399 to 3328
> 
> 3. (optional?) remove references to spi1, spi2, spi3
> 
> 4. Compile rockchip-spi-spidev.dts back to dtbo
> 
>     dtc -I dts -O dtb rockchip-spi-spidev.dts -o /boot/dtb/rockchip/overlay/rockchip-spi-spidev.dtbo
> 
> 5. Extract rockchip-fixup.script from cat /boot/dtb/rockchip/overlay/rockchip-fixup.scr
> 
>     (just use a text editor, and delete everything before "# overlays fixup script")
> 
> 6. Edit rockchip-fixup.script and change instances of ff1c to ff19
> 
> 7. Recompose rockchip-fixup.scr
> 
>    mkimage -A arm -T script -C none -d rockchip-fixup.script
> /boot/dtb/rockchip/overlay/rockchip-fixup.scr
> 
>  
> 
> Now, you can use armbian-config to add spidev, and reboot.
