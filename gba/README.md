# InfoNES Advance

## Introduction

InfoNES Advance is a Family Computer(TM) emulator that runs on GameBoy Advance(TM). 
This release is intended for developers to publish the source list, to make comments by volunteers, and to fix bugs. 
At the moment, it is quite slow, so it is difficult to play on a real machine.
If your purpose is to play, we recommend using other emulators.

![](https://github.com/jay-kumogata/InfoNES/blob/master/screenshots/infones_gba.gif)

## How to run

1. Copy the ROM file (e.g. sample.nes) to the gba\bin folder
1. Open a DOS window, and move to the gba\bin folder

	> cd gba\bin

1. Write the ROM file to the emulator ROM file (InfoNES.mb.gba) 

	> ./kakipee.exe sample.nes

1. Execute the emulator ROM file, in which the ROM file is written, from the GBA emulator (e.g. VisualBoyAdvance)

## Specification
### CPU
- MOS 6520 opcodes
- IRQ/NMI
### PPU 
- 8x16, 8x8 Sprites
- Sprite Priority
- Horizontal/Vertical Scroll 
- VROM/VRAM 
- Horizontal/Vertical mirroring
### Mapper
- iNES Mapper: #0,  #1,  #2,  #3,  #4
- ROM size upper limit: 160KB

## How to play

### Controller #1
- [START\]: Start
- [SELECT\]: Select
- [A\]: A
- [B\]: B
- [Up-arrow\]: Up-arrow
- [Down-arrow\]: Down-arrow
- [Right-arrow\]: Right-arrow
- [Left-arrow\]: Left-arrow

## How to compile

1. Install [DevKit Advance](https://devkitadv.sourceforge.net/)
1. Open a DOS window and move to the gba\src\InfoNES_Advance directory

	> cd gba\src\InfoNES_Advance

1. Generate the emulator ROM file

	> make

## Disclaimer
InfoNES Advance is freeware. There is no guarantee. The author is not responsible for any damages caused by this software.  
Family Computer is a trademarks of Nintendo Co., Ltd.
