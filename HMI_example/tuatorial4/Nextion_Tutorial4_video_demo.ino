/*
This sketch shows how to show a short video on the Nextion display.

I am not going to use any library to send data to the display.


Connection with Arduino Uno/Nano:
* +5V = 5V
* TX  = none
* RX  = pin 1 (TX)
* GND = GND

If you are going to use an Arduino Mega, you have to edit everything on this sketch that says "Serial"
and replace it with "Serial1" (or whatever number you are using).


This sketch was made for my 4th video tutorial shown here: https://www.youtube.com/watch?v=eDn7LFyoEm8

Made by InterlinkKnight
Last update: 02/04/2018
*/


int TestVariable1 = 0;  // A simple variable to store the frame number








void setup() {  // Put your setup code here, to run once:
  
  Serial.begin(9600);  // Start serial comunication at baud=9600


  // I am going to change the Serial baud to a faster rate.
  delay(500);  // This dalay is just in case the nextion display didn't start yet, to be sure it will receive the following command.
  Serial.print("baud=115200");  // Set new baud rate of nextion to 115200, but it's temporal. Next time nextion is power on,
                                // it will retore to default baud of 9600.
                                // To take effect, make sure to reboot the arduino (reseting arduino is not enough).
                                // If you want to change the default baud, send the command as "bauds=115200", instead of "baud=115200".
                                // If you change the default baud, everytime the nextion is power ON is going to have that baud rate, and
                                // would not be necessery to set the baud on the setup anymore.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to nextion.
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.end();  // End the serial comunication of baud=9600

  Serial.begin(115200);  // Start serial comunication at baud=115200





}  // End of setup




void loop() {


  delay(33);


  TestVariable1++;  // Increase variable by 1
  if(TestVariable1 == 103)  // If the variable is equal to 103
  {
    TestVariable1 = 0;  // Set variable to 0
  }

  Serial.print("p0.pic=");  // Send to the display the following picture number:
  Serial.print(TestVariable1);  // Send the variable
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);



}  // End of loop


