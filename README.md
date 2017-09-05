VBot - Arduino based Quadbot
============================
VBot is an Arduino based Quadbot which can walk, stretch and even dance. Watch it in action [here](http://www.youtube.com/watch?v=lD_z1iR0yf0). This repo contains the firmware and the controller code of VBot. 

Blog post: [http://blog.varunkumar.me/2013/01/vbot-arduino-based-quadbot.html](http://blog.varunkumar.me/2013/01/vbot-arduino-based-quadbot.html)

Modules
-------
### Arduino Firmware
- The firmware which needs to be flashed on to the Arduino board. 
- The bot can be controlled using an IR remote or from the Android app. IRremote library and Android Accessory library are used for receiving the commands from IR remote / Android app. The related libraries are also included in the folder named 'firmware'.

### Android App
- Android app used for controlling the bot.
- Runs in USB accessory mode and uses Android Open Accessory Protocol ([AOA](http://source.android.com/tech/accessories/index.html)) to send commands to the bot.
- Compatible with Android 2.3+

### Node.js + Johnny-Five
- JavaScript port of the firmware.
- Uses Node module 'Johnny-Five' which uses Firmata protocol for communicating with the Arduino board.
- The bot can be controlled from the REPL interface. Available methods: moveForward(), moveBackward(), moveLeft(), moveRight(), standUp(), stretch().

Requirements
------------
- [Robot kit](http://www.simplelabs.co.in/content/quadbot-4-legged-walking-robot-kit-diy-robot-kit)
- [Arduino ADK Board](http://arduino.cc/en/Main/ArduinoBoardADK) (Any board can be used)
- [Arduino Software](http://www.arduino.cc/)
- [Android SDK](http://developer.android.com/sdk/index.html)
- [Node.js](http://nodejs.org/)
- [Johnny-Five](https://github.com/rwldrn/johnny-five)

License
-------
The source code is available [here](https://github.com/varunkumar/vbot) under [MIT licence](http://varunkumar.mit-license.org/). Please send any bugs, feedback, complaints, patches to me at varunkumar[dot]n[at]gmail[dot]com.

-- [Varun](http://www.varunkumar.me)
