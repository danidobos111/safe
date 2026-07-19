(AI used for coding!)

# Login
A login panel, which has more users and admins. The users can controll the safe lock and register a new user. The admins can controll the safe lock, add more user/admin and change passwords.

# Assembly:
CAREFULL!!! If you use an external power supply, then ALWAYS connect the GNDs!!
1. First, get a microcontroller, I prefer ESP32, but you can use Arduino as well (CAREFULL! if you use ESP32-S3-N16R8, like me, you probably have a 5V IN, NOT output, so I had to use a extra power supply for the lcd panel. On the wiring photo, the ESP32-S3 has 5V output, so I used that)
(You can switch the point 2, 3 and 4)
2. Get (a) servo(s) or (a) stepper motor(s). If you use stepper motor, then you will need (a) driver(s) (each stepper motor requires one driver!), which can controll the stepper motor(s). I used NEMA17 stepper motor and A4988 driver. (CAREFULL! you HAVE TO have a power supply (depend on the motor you use), because you won't be able to power the motor from the microcontroller) (CAREFULL! if you use the same setup as me, first, you should watch a tutorial how to connect them, because they could be different! (https://www.youtube.com/watch?v=wcLeXXATCR4&t=696s), and BE EXTRA CAREFULL with the wiring, because I already burned 2 piece A4988 driver)
After you powered the motor, test it, if you can turn it. If you can't, then congrats, you didn't messed up! But if you can, then check the wiring and everything is on.
After you tested if, use a simple code, to test it (https://www.instructables.com/Stepper-Motor-With-Arduino-and-Driver-A4988/). If it's working, then move on to the next step.
3. Get a lcd panel (I used it with I2C, but you can use a normal one), then connect the wires (the VCC have to be 5V!!). If it's on, then you can move on to the test code (https://projecthub.arduino.cc/arduino_uno_guy/i2c-liquid-crystal-displays-5eb615). If nothing shows up, or only shows up rectangles, then try to adjust the contrass with pontentionmeter(s). If still nothing, then restart the microcontroller, and make sure every wire is in the right space. If it shows the text, then move on to the next step.
4. Get a keypad panel (if you don't have it, you can use a old keyboard, but you need to mess around with it), and connect the wires. It's very easy, to be honest: the pins on the keypad: row1, row2, row3, row4, column1, column2, col3, col4, then just connect which you want to use. (like this: https://docs.wokwi.com/parts/wokwi-membrane-keypad), then copy the test code, and if it's working, then move on :D
5. Wire everything up:
Keypad: R1-4, R2-5, R3-6, R4-7, C1-15, C2-16, C3-17, C4-18
Lcd panel with I2C: GND-GND, VCC-5V!!!, SDA-1, SCL-2
A4988 with stepper motor:
step-42, direction-41, sleep - 3.3V!!, reset - 3.3V!!, enable-GND!!, GND-GND, VDD-3.3V!!, 1A,2A,1B,2B - use the youtube tutorial for that, and AI help, GND-GND(external power supply's!!!), VMOT-12V(external power supply's!!!)
6. Copy my code, then it's ready to go :D
7. Have fun!

# Wiring

<img width="866" height="701" alt="image" src="https://github.com/user-attachments/assets/c812c892-3f0a-4edf-9310-93be4f590c16" />


# Final build
<img width="2048" height="1542" alt="image" src="https://github.com/user-attachments/assets/c0d21f36-278a-4a18-a077-dd6ed138423e" />
<img width="1542" height="2047" alt="image" src="https://github.com/user-attachments/assets/18f9a26e-06fe-4a1b-8bca-8be1e8d33141" />
<img width="1542" height="2047" alt="image" src="https://github.com/user-attachments/assets/ef9ed591-51da-4ce8-8ff4-3597f8beedcb" />

# KEYCOMBINATIONS:

MENU:

A: sign in

B: sign up

LOGIN:

#: ENTER

*: delete

*:(when it's empty): go back to MENU

ADMIN PANEL:

1: go left in the list

2: execute command

3: go right in the list
