#include <SPI.h>

// Instructions for the SRAM

const byte SRAM_READ = 0X03;
const byte SRAM_WRITE = 0X02;

// pins used for the connection with the sensor
// the other you need are controlled by the SPI library):

const int chipSelectPin = 7;
byte i = 0x01;
byte addr = 0x01;


void setup() {
  Serial.begin(9600);

  // start the SPI library:
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  // initalize the  data ready and chip select pins:
  pinMode(chipSelectPin, OUTPUT);

  // give the sensor time to set up:
  delay(100);
  
  // set the mode register to default!
  digitalWrite(chipSelectPin, LOW);
  
  SPI.transfer(0xFF);
  digitalWrite(chipSelectPin, HIGH);
  
  digitalWrite(chipSelectPin, LOW);
  SPI.transfer(0x01);
  SPI.transfer(0x00);
  digitalWrite(chipSelectPin, HIGH);
  
  
  // read mode register
  digitalWrite(chipSelectPin, LOW);
  Serial.print("Mode Register: ");
  Serial.println(SPI.transfer(0x05));
  
  digitalWrite(chipSelectPin, HIGH);
  
  // hope that everything works!
}


void loop() {
  Serial.print("address: ");
  Serial.println(addr);
  Serial.print("Load: ");
  Serial.println(loadByte(addr));
  
  delay(300);
  
  Serial.print("Savei: ");
  Serial.println(saveByte(addr, i++));
  
  addr+= 5;
}



unsigned int loadByte(byte address)
{
  unsigned int answer = 0;
  
  byte bufferMsg[5] = {SRAM_READ, 0x00, 0x00, 0x01, 0xff};
  
  // Enable SRAM
  digitalWrite(chipSelectPin, LOW);

  // Sent the message!
  SPI.transfer(SRAM_READ);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(address);
  answer = SPI.transfer(0xff);
  delay(50);
  // Disable SRAM
  digitalWrite(chipSelectPin, HIGH);
  
  return answer;
}



byte saveByte(byte address, byte data)
{
  
  byte bufferMsg[5] = {SRAM_WRITE, 0x00, 0x00, 0x01, data};

  
  // Enable SRAM
  digitalWrite(chipSelectPin, LOW);


  // Sent the message!
  SPI.transfer(SRAM_WRITE);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(address);
  SPI.transfer(data);
  delay(50);
  // Disable SRAM
  digitalWrite(chipSelectPin, HIGH);
  
  return data;

}


