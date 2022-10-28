# Introduction To Robotics (2022-2023)

Introduction to Robotics laboratory homeworks, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code and image files.

## Homework #1
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

![](https://github.com/IoanaLivia/IntroductionToRobotics/blob/main/Assets/Homework1/Sideview.jpeg)
![](https://github.com/IoanaLivia/IntroductionToRobotics/blob/main/Assets/Homework1/Frontview.jpeg)

**!** Common anode modifications: Put the common pin to 5V instead of GND (changing the wire per logic).

### [Video](https://youtu.be/IK3PnfRw_Ss)
[![](https://img.youtube.com/vi/IK3PnfRw_Ss/0.jpg)](https://www.youtube.com/watch?v=IK3PnfRw_Ss)

## Homework #2
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
     - Duration : 8 seconds
     
     
**!** Pressing the button in any state other than state 1 should **NOT** yield any actions.
  
### Publishing Task

  * Update README with task requirements, picture of setup, link to video showcasing functionality and upload code to Github repo.

### Coding Task

  Coding style is of utmost importance. An important coding challenge present in this task is using millis() instead of delay().
  *Addendum* : using interrupts (properly coded: with debounce etc) yields a bonus.
  
### Picture of setup

### Video

