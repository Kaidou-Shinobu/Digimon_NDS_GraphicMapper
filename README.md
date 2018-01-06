# Digimon_NDS_GraphicMapper
Automatic mapping of palette, tile, and map files found in the Digimon NDS games.

#Usage

./DigiGraphics [/path/to/arm9.bin] [/dir/to/extracted/graphics/]



#Optional NARC archiving

If you also want each graphic to be archived in a NARC, please use the following command:

./DigiGraphics -n [/path/to/arm9.bin] [/path/to/narctool.exe] [/dir/to/extracted/graphics/]

Third-Party programs required for Narc archiving
Executable of narctool needs to be in the same directory as this program's executable as well.
Executable can be easily found here:
https://github.com/delguoqing/LMDumper/tree/master/tools/narctool-0.1-p
