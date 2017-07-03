# Arduino Primitive Oscillator
If you have an Arduino and a small 16x2 character LCD you can create primitive oscillator using this code.

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
  <image goes here>

  These characters represent your arduino's analog pin reading, the first one represent the lowest reading (<110) while the full bar represent the highest reading (>1000).
  The star \* character is used to indicate high repeatition but more on that later.

* If you don't like the bars and prefer digital representation, u could replace this line
  `const byte LEVELS[] = {' ', 0, '_', 1, 2, 3, 4, 5, 6, 255, '*'};`
  with something like this
  `const byte LEVELS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*'};`
  though I'd suggest you try the bars first

## Get your primitive oscillator running and test it
* Bring back the original code of `void loop()`, compile and upload to Arduino.
* For the sake of this demonstration change the line `MAX_REPEATITION = 19` to `MAX_REPEATITION = 9`
* Connect a wire to analog pin A0 and the other side to ground (GND pin)
* You should see something like that
    <image goes here>
  Here the first line is completely blank, means Arduino didn't detect any signal.
  The 9's filling the second line indicate that each one of the top values was repeated 9 times.
* Try to connect to Vin pin instead, the first line should now be filled with bars while second line is still filled with  9's
* Now that we have done basic tests let's move to meaningful applications

### Measure Frequency
* Choose one of these pins (3, 5, 6, 10 or 11) and connect it to A0
* Add the following lines to your `setup()` function

      pinMode(<your_chosen_pin>, OUTPUT);
      analogWrite(<your_chosen_pin>, 128);

 * Now your LCD should show look something like this
   <image goes here>
