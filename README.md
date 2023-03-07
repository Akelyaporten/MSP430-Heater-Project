# MSP430 Heater Project
![My Image](pspice_circuit.png)
<hr>
<h2>Equipment List</h2> <br>
<ul>
<li>MCU = MSP43062553<br>
<li>LCD = TC16020<br>
<li>Membrane Keypad<br>
<li>I2C Module<br>
<li>Resistors = 1k, 22<br>
<li>Capacitor = 100u<br>
<li>Jumper Cables<br>
<li>BreadBoard<br>
<li>9V Battery<br>
<li>2N2222 Transistor<br>
<li>LM324<br>
<li>LM35<br>
  </ul>
<hr>
Heating element in this circuit is named as R1. LM35 Should be placed very close to R1 resistance. V4 voltage source is placed to imitate PWM signal that is output of one of the MSP430g2553's pins.

In this project, Msp430 takes input from 4x3 membrane keypad. According to input it heats resistor and after reaching specified value taken from input, it stops for 10 second. Current temperature value is showed on LCD until specified value is reached.
