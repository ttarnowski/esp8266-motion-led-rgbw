# 100DaysOfCode in IoT Challenge
## Day 30

Change color of Motion-Activated LED Strip Lights through the web interface.

## Requirements
1. Visual Studio Code IDE:
https://code.visualstudio.com/
2. C/C++ VSCode extension:
https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
3. Clang-Format VSCode extension:
https://marketplace.visualstudio.com/items?itemName=xaver.clang-format
4. PlatformIO VSCode extension:
https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide

## Building 
In Visual Studio Code go to:

`View -> Command Pallette...`

Type `PlatformIO: Build`

Press Enter

## Uploading files
In Visual Studio Code in the project main directory:

- create folder `data`
- copy or move all the files you'd like to upload to newly created `data` folder
- make sure ESP8266/ESP32 is connected to your computer
- go to PlatformIO quick access (menu on the left, grey ant icon)
- expand `PROJECT TASKS` and go to `nodemcuv2 -> Platform` and click on `Upload Filesystem Image`

## Deploying code to ESP8266
Connect ESP8266 to your computer

In Visual Studio Code go to:

`View -> Command Pallette...`

Type `PlatformIO: Upload`

Press Enter

## Link to YouTube Video (Day30):

https://www.youtube.com/watch?v=d1hdwap2gT8

## All Days List:

https://github.com/ttarnowski/100DaysOfCodeInIoT