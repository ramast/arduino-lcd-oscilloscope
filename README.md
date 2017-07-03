# Arduino LCD Oscilloscope
If you have an Arduino and a small 16x2 character LCD you can create a primitive oscilloscope using this code.

## Before you begin
Make sure you are able to display ordinary text on your LCD. A good start is using Arduino's LCD Hello World example.

    File -> Examples -> LiquidCrystal -> HelloWorld

If you could run the example properly, you are already halfway through.

## Validate/Customize voltage representation in your LCD.
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

## Get your LCD Oscilloscope running and test it
* Bring back the original code of `void loop()`, compile and upload to Arduino.
* For the sake of this demonstration change the line `MAX_REPEATITION = 19` to `MAX_REPEATITION = 9`
* Connect a wire to analog pin A0 and the other side to ground (GND pin)
* You should see something like that
    ![Ground Image](https://raw.githubusercontent.com/ramast/arduino-lcd-oscilloscope/master/images/ground.jpg "LCD Image")
  
  Here the first line is completely blank (= analog reading was < 110). 
  The 9's filling the second line indicate that each one of the top values was repeated 9 times. That means what you see on screen represent 144 (16x9) analog reading.
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
   
   From that output we can tell that:
   1. There are only fullbar or empty bar so it's a digital signal alternating between 5v and 0v
   2. 5v to 0v ratio is (on average) 7:8 so duty cycle is roughly 47%

### Locating RX pin on Arduino board
We know that RX pin is pin 0 on Arduino board but let's imagine this is an unknown circuit and we are trying to find the RX pin.

* Make sure your Arduino is connected to your computer and serial monitor is opened, in this example I used buad 9600.
* Try connecting your test pin `A0` to RX Pin, you would notice the reading being positive all the time since we are not writing anything to the serial console yet.
* On Linux I use this command `dd if=/dev/zero of=/dev/ttyACM0` to constantly write `0`'s to Arduino's serial console.
* You would notice the LCD's reading has changed to something like this
  ![Serial Image](https://raw.githubusercontent.com/ramast/arduino-lcd-oscilloscope/master/images/serial_rx.jpg "Serial RX LCD Image")
* Your output may differ if using different bitrate, on linux use `stty` to find your console's current speed

      localhost linux # stty -a -F /dev/ttyACM0 |grep speed
      speed 9600 baud; rows 0; columns 0; line = 0;

Notice that the output is consistent 1 high followed by 7 or 8 low, the output would be inconsistent if you tried writing random characters `dd if=/dev/urandom of=/dev/ttyACM0` and it will become all high if you stopped writing at all.

Based on that you can be confident this is the RX pin, also since high signal is represented by full bar we know it's a 5v serial communication. If it 3v serial communication, the bar would've only been partially full.

## Choosing maximum number of repeatition
LCD screen can only show 16 character per line and to make things worse refreshing it quickly make the values not readable at all so to save up space, we *try* to write each value once and state in the line below how many didn't was repeated. for example if we perform 10 readings to analog pin and got these values "900, 0, 0, 0, 0, 0, 50, 0, 0, 0, 900"
We represent it like this

      9 0 5 0 9
      1 5 1 3 1

There is a constant `MAX_REPEATITION` which allow you to decide how many times a value can be repeated before being represented once more. In the example above if we set `MAX_REPEATITION = 2`, we get this output.

      9 0 0 5 0 9
      1 3 2 1 3 1

set `MAX_REPEATITION = 0` disable repeatition and your LCD output will look like this

      9 0 0 0 0 0 5 5 5 0 0 0 9
      1 1 1 1 1 1 1 1 1 1 1 1 1

__Note:__ `MAX_REPEATITION = 2` means a number maybe repeated up to 2 times **after** it's first reading so 3 times in total.

### Increasing repeatition byond 9
Each character on the first line of the LCD has it's repeatition represented in the character right below it.
So what happen if we set `MAX_REPEATITION` to something beyond 9, say `19`. Since I am only able to represent repeatition with one digit, I resort to voltage's bars to represent values beyond 9.
so numbers becomes 0 to 9 then dotted line, one dash, two dashes, ...

If `MAX_REPEATITION` were to be increased beyond `19`, say for example `99` then bars start representing factors of 10.
So after repeatition number 9, you get getting the dotted bar until repeatition number 19, then you get a dash until 29 and so on.

If `MAX_REPEATITION` increased beyond that point, you get a '\*' which means that value was repeated 100 or more times.

__Note:__ In a stable/semi-stable signal the more you increase repeatition the longer it will take to refresh your LCD reading. For example say `MAX_REPEATITION = 1000` and you connect your `A0` to `5V`. In this case arduino will perform 1000 analog reading for *each* LCD character (16) so in total arduino needs to perform 16,000 reading before showing any result on your LCD.


## Disadvantages and design decisons

Current implementation does the work on two separate phases. First phase is data collection and aggregation. Once this phase is complete the data is represented on the LCD and an appropriate `delay` is added to give you a chance to read it.

My original code (not published) used to write data on LCD in *almost* realtime. It was perfect for things like testing a motion sensor or to see serial data as soon as they are written.
However, Arduino needs time to set the cursor on the LCD, write the value, then set it again to bottom row and write repeatition number. When using the original code to measure something like RX data or PWM cycle, it produced semi-random repeatition because it misses many values while being busy talking to the LCD display.

The current implementation gives relatively accurate result but it's not good for detecting a sudden events (a signal from motion sensor or a single letter you've typed to a serial port).
