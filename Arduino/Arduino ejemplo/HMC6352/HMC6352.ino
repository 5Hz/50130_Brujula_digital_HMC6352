/******************************************************************
** Código creado en Electrónica 5Hz                              **
** www.5hz-electronica.com                                       **
** Por: Baruc                                                    **
** Basado en el código de: http://playground.arduino.cc/Learning/Hmc6352                                **
** Descripción del código:                                       **
**                                                               **
** Ejemplo para el sensor HMC6352 brújula                        **
** Programa sencillo que lee la presion atmosférica              **
** y la imprime en el puerto serial                              **
*******************************************************************

Conexiones para la comunicacion i2c usando el breakout de 5Hz con un Arduino Uno / Duemilanove:
Arduino <-> HMC6352
Gnd      -  GND
5v       -  VCC
A4       -  SDA
A5       -  SCL

*/


#include <Wire.h>
int HMC6352Address = 0x42;
// This is calculated in the setup() function
int slaveAddress;
int ledPin = 13;
boolean ledState = false;
byte headingData[2];
int i, headingValue;
void setup()
{
// Shift the device's documented slave address (0x42) 1 bit right
// This compensates for how the TWI library only wants the
// 7 most significant bits (with the high bit padded with 0)
slaveAddress = HMC6352Address >> 1;   // This results in 0x21 as the address to pass to TWI
Serial.begin(9600);
pinMode(ledPin, OUTPUT);      // Set the LED pin as output
Wire.begin();
}
void loop()
{
  // Flash the LED on pin 13 just to show that something is happening
  // Also serves as an indication that we're not "stuck" waiting for TWI data
  ledState = !ledState;
  if (ledState) {
    digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
  // Send a "A" command to the HMC6352
  // This requests the current heading data
  Wire.beginTransmission(slaveAddress);
  Wire.write("A");              // The "Get Data" command
  Wire.endTransmission();
  delay(10);                   // The HMC6352 needs at least a 70us (microsecond) delay
  // after this command.  Using 10ms just makes it safe
  // Read the 2 heading bytes, MSB first
  // The resulting 16bit word is the compass heading in 10th's of a degree
  // For example: a heading of 1345 would be 134.5 degrees
  Wire.requestFrom(slaveAddress, 2);        // Request the 2 byte heading (MSB comes first)
  i = 0;
  while(Wire.available() && i < 2)
  { 
    headingData[i] = Wire.read();
    i++;
  }
  headingValue = headingData[0]*256 + headingData[1];  // Put the MSB and LSB together
  Serial.print("Current heading: ");
  Serial.print(int (headingValue / 10));     // The whole number part of the heading
  Serial.print(".");
  Serial.print(int (headingValue % 10));     // The fractional part of the heading
  Serial.println(" degrees");
  delay(500);
} 
