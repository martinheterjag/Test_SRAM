#include <SPI.h>

// Instructions for the SRAM

const byte SRAM_READ = 0X03;
const byte SRAM_WRITE = 0X02;

// pins used for the connection with the sensor
// the other you need are controlled by the SPI library):

int audioInput = A0;
int audioOutput = 6;  //5 and 6 pwm have higher frequency! 
int sample = 0;
int delayTime = 120;
int output;
const int chipSelectPin = 7;
byte i = 0x01;
unsigned long addr = 0x00000000;



void setup() {
  // faster pwm!
  TCCR0B = TCCR0B & 0b11111000 | 0x01;
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
  // sample audio signal
  sample = analogRead(audioInput);
  // shoft 2 bits to make it fit in a 8 bit memory slot.
  sample = (sample >> 2) * 0.9;
  // save to ram
  saveByte(addr, sample);
  output = loadByte((addr)%16777216) * 0.3 +
           loadByte((addr - 100)%16777216) * 0.3 + 
           loadByte((addr - 200)%16777216) * 0.3;
  analogWrite(audioOutput, output);
  addr++;
  delay(1);
  if(addr == 16777216) {
    addr = 0;
    Serial.println("r");
  }
  
  /*
  Serial.print("address: ");
  Serial.println(addr);
  Serial.print("Sample: ");
  Serial.println(sample);
  */
  
  
}



unsigned int loadByte(unsigned long address)
{
  unsigned int answer = 0;

  // Enable SRAM
  digitalWrite(chipSelectPin, LOW);

  // Send the message!
  SPI.transfer(SRAM_READ);
  SPI.transfer((address >> 8) & 0xff);
  SPI.transfer((address >> 4) & 0xff);
  SPI.transfer(address & 0xff);
  answer = SPI.transfer(0xff);
  //delay(50);
  // Disable SRAM
  digitalWrite(chipSelectPin, HIGH);
  
  return answer;
}



byte saveByte(unsigned long address, byte data)
{
  
  // Enable SRAM
  digitalWrite(chipSelectPin, LOW);

  // Send the message!
  SPI.transfer(SRAM_WRITE);
  SPI.transfer((address >> 8) & 0xff);
  SPI.transfer((address >> 4) & 0xff);
  SPI.transfer(address & 0xff);
  SPI.transfer(data);
  //delay(50);
  // Disable SRAM
  digitalWrite(chipSelectPin, HIGH);
  
  return data;

}


