# Arduino Primitive Oscilloscope
If you have an Arduino and a small 16x2 character LCD you can create primitive oscilloscope using this code.

## Before you begin
Make sure you are able to display ordinary text on your LCD. A good start is using Arduino's LCD Hello World example.

    File -> Examples -> LiquidCrystal -> HelloWorld

If you could run the example properly, you are already halfway through.

## Check if your LCD supprt same Characters that mine support.
* Copy Arduino code from this repo and paste it in your arduino studio.
* Delete all the code inside `void loop()` function.
* Place the following code instead

      void loop() {
          for (int i=0; i < 11; i++) {
              lcd.write(LEVELS[i]);
          }
          delay(5000);
          lcd.clear();
      }
* Make sure you define the correct LCD pins in this line `LiquidCrystal lcd(12, 11, 5, 4, 3, 2);`
* If all goes well, you should see output like this
  ![Test](https://raw.githubusercontent.com/ramast/arduino-lcd-oscilloscope/master/images/test.jpg "Test Image")

  These characters represent your arduino's analog pin reading, the first one represent the lowest reading (<110) while the full bar represent the highest reading (>1000).
  The star \* character is used to indicate high repeatition but more on that later.

* If you don't like the bars and prefer digital representation, u could replace this line
  `const byte LEVELS[] = {' ', 0, '_', 1, 2, 3, 4, 5, 6, 255, '*'};`
  with something like this
  `const byte LEVELS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*'};`
  though I'd suggest you try the bars first

## Get your primitive oscilloscope running and test it
* Bring back the original code of `void loop()`, compile and upload to Arduino.
* For the sake of this demonstration change the line `MAX_REPEATITION = 19` to `MAX_REPEATITION = 9`
* Connect a wire to analog pin A0 and the other side to ground (GND pin)
* You should see something like that
    ![Ground Image](https://raw.githubusercontent.com/ramast/arduino-lcd-oscilloscope/master/images/ground.jpg "LCD Image")
  Here the first line is completely blank, analog reading of pin A0 has been 0 (or at less than 100) 
  The 9's filling the second line indicate that each one of the top values was repeated 9 times.
* Try to connect to 5V pin instead, the first line should now be filled with bars while second line is still filled with  9's
  ![5V Image](https://raw.githubusercontent.com/ramast/arduino-lcd-oscilloscope/master/images/5v.jpg "5V LCD Image")
* If you connect to 3V pin, you would get half bar as expected
  ![3V Image](https://raw.githubusercontent.com/ramast/arduino-lcd-oscilloscope/master/images/3-3v.jpg "3V LCD Image")

* Now that we have done basic tests let's move to meaningful applications

### Measure PWM duty cycle
* Choose one of these pins (3, 5, 6, 10 or 11) and connect it to A0
* Add the following lines to your `setup()` function

      pinMode(<your_chosen_pin>, OUTPUT);
      analogWrite(<your_chosen_pin>, 128);

 * Now your LCD should show look something like this
   ![PWM Image](https://raw.githubusercontent.com/ramast/arduino-lcd-oscilloscope/master/images/pwm.jpg "PWM LCD Image")
   From that image we can tell that:
   1. It's a digital signal (5v or 0v)
   2. 5v to 0v ratio is (on average) 7:8 so duty cycle is roughly 47%

### Locating RX pin on android board
We know that RX pin is pin 0 on android board but let's imagine this is an unknown circuit and we are trying to find the RX pin.

* Make sure your arduino is connected to your computer and console is opened, in this example I used buad 9600.
* Try connecting your test pin `A0` to RX Pin, you would notice the reading being positive all the time since we are not writing anything to Arduino.
* On Linux I used the command `dd if=/dev/zero of=/dev/ttyACM0` to constantly write `0`s to Arduino's serial port.
* You would notice the LCD's reading has changed to something like this
  ![Serial Image](https://raw.githubusercontent.com/ramast/arduino-lcd-oscilloscope/master/images/serial_rx.jpg "Serial RX LCD Image")
* Your output may differ if using different bitrate, use stty to find your console's current speed

      localhost linux # stty -a -F /dev/ttyACM0 |grep speed
      speed 9600 baud; rows 0; columns 0; line = 0;

Notice that the output is consistent 1 high followed by 7 or 8 low, the output would be inconsistent if you tried writing random characters `dd if=/dev/urandom of=/dev/ttyACM0` and it will become all high if you stopped writing at all.

Based on that you can be confident this is the RX pin, also since the bar is full we know it's a 5v serial communication. If it was only partially full then it's probably 3.3v
