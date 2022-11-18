# Introduction To Robotics (2022-2023)

Introduction to Robotics laboratory homework, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code, image and video files.
<details>
  <summary> 
    Homework #1 : Controlling RGB Led
  </summary>

## Controlling RGB Led
  
### Components
* RGB Led 
* Potentiometers
* Resistors and wires (per logic)

### Technical Task

* Use a separate potentiometer in controlling each of the RGB led (Red, Green and Blue). The control must be done with digital electronics.

### Publishing Task

* Update README with task requirements, picture of setup, link to video showcasing functionality and upload code to Github repo.

### [Coding Task](https://github.com/IoanaLivia/IntroductionToRobotics/blob/main/Homework%20%5BCode%5D/Homework1/Homework_1/Homework_1.ino)

* Coding style is at utmost importance. Check consistency in style and spacing.

### Picture of setup

**!** The following setup corresponds to a common cathode RGB led. 

![](https://github.com/IoanaLivia/IntroductionToRobotics/blob/main/Assets/%231/%231_Upperview.jpeg)

**!** Common anode modifications: Put the common pin to 5V instead of GND (changing the wire per logic).

### [Video](https://youtu.be/IK3PnfRw_Ss)
[![](https://img.youtube.com/vi/IK3PnfRw_Ss/0.jpg)](https://www.youtube.com/watch?v=IK3PnfRw_Ss)

  </details>
<details>
  <summary> Homework #2: Building the traffic lights for a crosswalk </summary>

## Building the traffic lights for a crosswalk

### Components
* 5 Leds
* Button
* Buzzer
* Resistors and wires (per logic)

### Technical Task

* Build the traffic lights for a crosswalk. Use 2 LEDs to represent the traffic lights for people (red and green) and 3 LEDs to represent the traffic lights for cars (red, yellow and green). There are 4 states that the traffic lights system has to go through. 

  + State 1 : Default State (will be reinstated after State 4 ends)
  
     - Cars : Green Light
     - People : Red Light
     - Sound: none
     - Duration : indefinite (changed by pressing the button)
  
  + State 2 : initiated by counting down 8 seconds after a button press
 
     - Cars : Yellow Light
     - People : Red Light
     - Sound : none
     - Duration : 3 seconds
     
  + State 3 : initiated after State 2 ends

     - Cars : Red Light
     - People : Green Light
     - Sound : Beeping Sound from the buzzer at a constant interval
     - Duration : 8 seconds
     
  + State 4 : initiated after State 3 ends
     - Cars : Red Light
     - People : Blinking Green Light
     - Sound : Beeping Sound from the buzzer at a constant interval faster than the beeping in State 3
     - Duration : 4 seconds
     
     
**!** Pressing the button in any state other than state 1 should **NOT** yield any actions.
  
### Publishing Task

  * Update README with task requirements, picture of setup, link to video showcasing functionality and upload code to Github repo.

### [Coding Task](https://github.com/IoanaLivia/IntroductionToRobotics/blob/main/Homework%20%5BCode%5D/%232/Homework_2/Homework_2.ino)

  Coding style is of utmost importance. An important coding challenge present in this task is using millis() instead of delay().
  *Addendum* : using interrupts (properly coded: with debounce etc) yields a bonus.
  
  **!** Prints of Serial Monitor Output that showcase functionality can be found [here](https://github.com/IoanaLivia/IntroductionToRobotics/tree/main/Homework%20%5BCode%5D/%232/Serial_2).
  
### Picture of setup

![](https://github.com/IoanaLivia/IntroductionToRobotics/blob/main/Assets/%232/%232_upperview.jpeg)
### [Video](https://youtu.be/M8HzgoxRh9A)
[![](https://img.youtube.com/vi/M8HzgoxRh9A/0.jpg)](https://youtu.be/M8HzgoxRh9A)
</details>


<details>
  <summary> Homework #3: Drawing on 7 segment display</summary>

## Drawing on 7 segment display
  
### Components
* 7-segment display
* Joystick
* Resistors and wires (per logic)

### Technical Task

* Use the joystick to control the position of the segment and *draw* on the display.  The movement between segments should be natural (meaning they should jump from the current positiononly to neighbors, but without passing through *walls*.

 + State 1 : Default State (also initiated after a button press in State 2)
  
     - Current segment : Blinking
     - Joystick : Can be used to move from one position to its neighbours.
     - Clicking the joystick :
        * *Short* pressing : toggles State 2.
        * *Long* pressing : resets the entired display by turning all segments OFF and moving the current position to the decimal point.
  
  + State 2 : Initiated after a button press in State 1
 
     - Current segment : Stops blinking ; Adopts the state of the segment before selection
     (ON / OFF)
     - Joystick : Toggling the X (or Y, depending on personal preference regarding implementation) axis should change the segment state to the opposite state (ON -> OFF, OFF -> ON) .
     - Clicking the joystick : save the segment state and exit back to State 1.
     
* Mentions

<ol>
  <li> Long pressing the button to reset should only be available in State 1. </li>
  
  <li> Joystick movements should be done with toggle, as presented in the laboratory. </li>
</ol>

* Mapping each segment to its neighbours

| Current Segment     | UP          | DOWN        | LEFT        | RIGHT       |
| ----------- | ----------- | ----------- | ----------- | ----------- |
| a           | N/A         | g           | f           | b           |
| b           | a           | g           | f           | N/A         |
| c           | g           | d           | e           | dp          |
| d           | g           | N/A         | e           | c           |
| e           | g           | d           | N/A         | c           |    
| f           | a           | g           | N/A         | b           |
| g           | a           | d           | N/A         | N/A         |
| dp          | N/A         | N/A         | c           | N/A         |

### Publishing Task

  * Update README with task requirements, picture of setup, link to video showcasing functionality and upload code to Github repo.

### [Coding Task](https://github.com/IoanaLivia/IntroductionToRobotics/blob/main/Homework%20%5BCode%5D/%233/Homework_3/Homework_3.ino)

  Coding style is of utmost importance. An important coding challenge present in this task is using millis() instead of delay().

### Picture of setup

![](https://github.com/IoanaLivia/IntroductionToRobotics/blob/main/Assets/%233/%233_upperview.JPG)

### [Video](https://youtu.be/CPm8NraoFVo)
[![](https://img.youtube.com/vi/CPm8NraoFVo/0.jpg)](https://www.youtube.com/watch?v=CPm8NraoFVo)


</details>

<details open>
  <summary> Homework #4: Moving through 4-digit 7 segment display with the possibility to set digits</summary>
  
## Moving through 4-digit 7 segment display with the possibility to set digits
  
### Components
* 4 digit 7-segment display
* Joystick
* 74hc595 shift registers
* Resistors and wires (per logic)

### Technical Task

* Use the joystick to move through the 4 digit 7 segment displays digits, press the button to lock in on the current digitand use the other axis to increment or decrement the number. Keep the button pressed to reset all the digit values and the current position to thefirst digit in the first state.


  + State 1 : Default State (also initiated after a button press in State 2)
  
     - Current decimal point : Blinking
     - Joystick : Can be used to cycle through digits
     - Clicking the joystick : Locks in the selected digit and enters State 2.
  
   + State 2 : Initiated after a button press in State 1
  
     - Current decimal point : ON
     - Joystick : Can no longer be used to cycle through digits. Instead, using the other axis, can be used to increment/decrement the number on the current digit in HEX (0-F).
     - Clicking the joystick : Returns to State 1.
     
    + Reset : Toggled by long pressing the button in State 1
    
      - All digits go back to 0 and the current position is set to the first (rightmost) digit, in State 1.

* Mentions

<ol>
  <li> Keep in mind that when changing the number, you must increment it for each joystick movement - it should not work continuosly increment if you keep the joystick in one position. </li>
  
</ol>


### Publishing Task

  * Update README with task requirements, picture of setup, link to video showcasing functionality and upload code to Github repo.

### Coding Task

### Picture of setup
![](https://github.com/IoanaLivia/IntroductionToRobotics/blob/main/Assets/%234/%234_upperview.jpg)
### [Video](https://www.youtube.com/watch?v=y3L7GrxTdZA&ab_channel=IoanaLiviaPopescu)
[![](https://img.youtube.com/vi/y3L7GrxTdZA/0.jpg)](https://www.youtube.com/watch?v=y3L7GrxTdZA&ab_channel=IoanaLiviaPopescu)

 </details>
