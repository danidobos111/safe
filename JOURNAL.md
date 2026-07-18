07.17.
I started my second projekt on HackClub, which started as a simple safe, whose lock can controlled by a keypad, if the safe combination is right. For display, I use lcd screen with I2C.
I did start the wiring, but I didn't know which pin should I use with the A4988 and stepper motor, so I looked it up. After that, I tested it, it worked, so I moved on to the lcd panel. I searched up a code example (https://projecthub.arduino.cc/arduino_uno_guy/i2c-liquid-crystal-displays-5eb615), so I could start somewhere, but it didn't worked. After many trying, and a conversation with Claude, it still didn't worked, so I gave up and went to sleep, so I can continue next day.
07.18.
I asked ChatGPT, why the lcd panel not working, and it highlighted the problem: I missed a line of code (Wire.begin(1, 2);), after that, it worked, so I moved on. I started testing the keypad, everything worked really well, so I started adding stuffs, like the safecode, which can open the lock........
