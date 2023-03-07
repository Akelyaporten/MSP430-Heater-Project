# MSP430 Heater Project
![My Image](pspice_circuit.png)
<hr size=‘’1’’ width=‘’50%’’ align=‘’center’’ color=‘’black’’>
<h2>Equipment List</h2> <br>
MCU = MSP43062553<br>
LCD = TC16020<br>
Membrane Keypad<br>
I2C Module<br>
Resistors = 1k, 22<br>
Capacitor = 100u<br>
Jumper Cables<br>
BreadBoard<br>
9V Battery<br>
2N2222 Transistor<br>
LM324<br>
LM35<br>
<hr>
Heating element in this circuit is named as R1. LM35 Should be placed very close to R1 resistance. V4 voltage source is placed to imitate PWM signal that is output of one of the MSP430g2553's pins.

In this project, Msp430 takes input from 4x3 membrane keypad. According to input it heats resistor and after reaching specified value taken from input, it stops for 10 second. Current temperature value is showed on LCD until specified value is reached.
