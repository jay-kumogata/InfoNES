# InfoNES

## Introduction
InfoNES is a Family Computer emulator that runs on SDL/Linux.

![](https://github.com/jay-kumogata/InfoNES/blob/master/screenshots/infones_gba.png)

This screenshot shows the demo running on InfoNES Advance, which runs on VirtualBoyAdvance.

## How to run

	> ./InfoNES sample1.nes &

## How to play
### Controller #1
- [S\]: Start
- [A\]: Select
- [X]: A button
- [Z]: B button 
- [Up-arrow]: Up button 
- [Down-arrow]: Down button 
- [Right-arrow]: Right button 
- [Left-arrow]: Left button 
### System
- [ESC]: Terminate emulation
- [ALT] + [Enter]: Toggle full screen mode and window mode 
- [C]: Clip 8 pixels at the top and bottom of the screen
- [M]: Sound mute

## Specification
### CPU
- MOS 6520 opcodes
- IRQ/NMI/Frame IRQ
- SRAM load/save function
- Trainer function
- MOS 6502 bugs
- MOS 6502 undisclosed instructions (partial)
### PPU 
- 8x16, 8x8 Sprites
- Sprite Priority
- Horizontal/Vertical Scroll 
- VROM/VRAM 
- Horizontal/Vertical/1 screen/4 screen mirroring
### APU
- Square wave #0, #1
- Triangle wave
- Noise
- DPCM
### Mapper
- iNES Mapper: #0,  #1,  #2,  #3,  #4,  #5,  #6,  #7,  #8,  #9, #10, #11, #13, #15, #16, #17, #18, #19, #21, #22,
- iNES Mapper: #23, #24, #25, #26, #32, #33, #34, #40, #41, #42, #43, #44, #45, #46, #47, #48, #49, 
- iNES Mapper: #50, #51, #57, #58, #60, #61, #62, #64, #65, #66, #67, #68, #69, #70, #71, #72, #73, 
- iNES Mapper: #74, #75, #76, #77, #78, #79, #80, #82, #83, #85, #86, #87, #88, #89, #90, #91, #92, 
- iNES Mapper: #93, #94, #95, #96, #97, #99,#100,#101,#105,#107,#108,#109,#110,#112,#113,#114,
- iNES Mapper: #115,#116,#117,#118,#119,#120,#122,#133,#134,#135,#140,#151,#160,#180,#181,
- iNES Mapper: #182,#183,#185,#187,#188,#189,#191,#193,#194,#200,#201,#202,#222,#225,#226,
- iNES Mapper: #227,#228,#229,#230,#231,#232,#233,#234,#235,#236,#240,#241,#242,#243,
- iNES Mapper: #244,#245,#246,#248,#249,#251,#252,#255
- Nintendo MMC: MMC1(#1), MMC2(#9), MMC3(#4), MMC4(#10), MMC5(#5)

## Known source ports

InfoNES has been ported to various platforms.

|Platform|Author|URL|
|--------|------|---|
|PlayStation Portable|TMK|https://github.com/PSP-Archive/InfoNES<br>Internet Archive: https://archive.org/details/info-nes-r-03.7z|
|iPhone|stepwhite<br>nervegas|http://code.google.com/p/iphonenes/<br>YouTube: https://www.youtube.com/watch?v=LVh429Dp7r0|
|Android|Scott Glover|https://sourceforge.net/projects/infones-droid/|
|Rockbox|Nathaniel|https://github.com/pcwizzy37/infones-rockbox|
|STM32F429I-DISCO|Woody|https://github.com/Woody00h/InfoNES|
|Embedded linux<br>based on ZLG easyARM imax283 board|yongzhena|https://github.com/yongzhena/infoNES|
|Raspberry Pi<br>Arm Linux|nejidev|https://github.com/nejidev/arm-NES-linux|
|Android|Tonyfield|https://github.com/Tonyfield/Nester|
|Raspberry Pi Pico|Shuichi Takano|https://github.com/shuichitakano/pico-infones<br>Twitter: https://twitter.com/shuichi_takano/status/1477702448907419649|
|Raspberry Pi Pico with SD Card|Frank Hoedemakers|https://github.com/fhoedemakers/pico-infonesPlus<br>YouTube: https://www.youtube.com/watch?v=OEcpNMNzZCQ|
|[PicoSystem](https://shop.pimoroni.com/products/picosystem)|Frank Hoedemakers|https://github.com/fhoedemakers/PicoSystem_InfoNes<br>Twitter: https://twitter.com/frenskefrens/status/1637439574375800832|
|RP2040 with ILI9341 LCD|pondahai|https://github.com/pondahai/rp2040-ili9341-infones<br>YouTube: https://www.youtube.com/shorts/_yGHtf-_yRU|
|[Murmulator](https://murmulator.ru/)|Ilya Maslennikov|https://github.com/xrip/pico-nes|
|[Picopad](https://picopad.eu/en/index.html)|Miroslav Nemecek|https://www.breatharian.eu/hw/picolibsdk/index_en.html#nes<br>Twitter: https://x.com/TororoLab/status/1989999786641232124|

## Disclaimer
InfoNES is freeware. There is no guarantee. 
The author is not responsible for any damages caused by this software. 
Family Computer is a trademarks of Nintendo Co., Ltd.
