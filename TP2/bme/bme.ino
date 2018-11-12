#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

#define _SEEED_H_

#define ADDRESS   0x76

#define REG_DIG_T1    0x88
#define REG_DIG_T2    0x8A
#define REG_DIG_T3    0x8C

#define REG_DIG_P1    0x8E
#define REG_DIG_P2    0x90
#define REG_DIG_P3    0x92
#define REG_DIG_P4    0x94
#define REG_DIG_P5    0x96
#define REG_DIG_P6    0x98
#define REG_DIG_P7    0x9A
#define REG_DIG_P8    0x9C
#define REG_DIG_P9    0x9E

#define REG_DIG_H1    0xA1
#define REG_DIG_H2    0xE1
#define REG_DIG_H3    0xE3
#define REG_DIG_H4    0xE4
#define REG_DIG_H5    0xE5
#define REG_DIG_H6    0xE7

#define REG_CHIPID          0xD0
#define REG_VERSION         0xD1
#define REG_SOFTRESET       0xE0

#define REG_CAL26           0xE1

#define REG_CONTROLHUMID    0xF2
#define REG_CONTROL         0xF4
#define REG_CONFIG          0xF5
#define REG_PRESSUREDATA    0xF7
#define REG_TEMPDATA        0xFA
#define REG_HUMIDITYDATA    0xFD

#define _INVALID_DATA 0


#define _address 0x76
#define SEALEVELPRESSURE_HPA (1013.25)

bool isTransport_OK;
uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;

uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;

uint8_t dig_H1;
int16_t dig_H2;
uint8_t dig_H3;
int16_t dig_H4;
int16_t dig_H5;
int8_t  dig_H6;

int32_t t_fine;

void init2()
{
  Wire.begin();

  if(Read8(REG_CHIPID) != 0x60)
    return ;

  dig_T1 = Read16LE(REG_DIG_T1);
  dig_T2 = ReadS16LE(REG_DIG_T2);
  dig_T3 = ReadS16LE(REG_DIG_T3);

  dig_P1 = Read16LE(REG_DIG_P1);
  dig_P2 = ReadS16LE(REG_DIG_P2);
  dig_P3 = ReadS16LE(REG_DIG_P3);
  dig_P4 = ReadS16LE(REG_DIG_P4);
  dig_P5 = ReadS16LE(REG_DIG_P5);
  dig_P6 = ReadS16LE(REG_DIG_P6);
  dig_P7 = ReadS16LE(REG_DIG_P7);
  dig_P8 = ReadS16LE(REG_DIG_P8);
  dig_P9 = ReadS16LE(REG_DIG_P9);

  dig_H1 = Read8(REG_DIG_H1);
  dig_H2 = Read16LE(REG_DIG_H2);
  dig_H3 = Read8(REG_DIG_H3);
  dig_H4 = (Read8(REG_DIG_H4) << 4) | (0x0F & Read8(REG_DIG_H4 + 1));
  dig_H5 = (Read8(REG_DIG_H5 + 1) << 4) | (0x0F & Read8(REG_DIG_H5) >> 4);
  dig_H6 = (int8_t)Read8(REG_DIG_H6);

  writeRegister(REG_CONTROLHUMID, 0x05);  //Choose 16X oversampling
  writeRegister(REG_CONTROL, 0xB7);  //Choose 16X oversampling

}

float getTemperature(void)
{
  int32_t var1, var2;

  int32_t adc_T = Read24(REG_TEMPDATA);

  if(!isTransport_OK) {
    return _INVALID_DATA;
  }
  adc_T >>= 4;
  var1 = (((adc_T >> 3) - ((int32_t)(dig_T1 << 1))) *
    ((int32_t)dig_T2)) >> 11;

  var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
    ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
    ((int32_t)dig_T3)) >> 14;

  t_fine = var1 + var2;
  float T = (t_fine * 5 + 128) >> 8;
  return T/100;
}

uint32_t getPressure(void)
{
  int64_t var1, var2, p;


  getTemperature();

  if(!isTransport_OK) {
    return _INVALID_DATA;
  }

  int32_t adc_P = Read24(REG_PRESSUREDATA);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)dig_P6;
  var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
  var2 = var2 + (((int64_t)dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;
  if (var1 == 0) return 0;
  p = 1048576-adc_P;
  p = (((p<<31)-var2)*3125)/var1;
  var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
  return (uint32_t)p/256;
}

uint32_t getHumidity(void)
{
  int32_t v_x1_u32r, adc_H;

  getTemperature();
  if(!isTransport_OK) {
    return _INVALID_DATA;
  }

  adc_H = Read16(REG_HUMIDITYDATA);

  v_x1_u32r = (t_fine - ((int32_t)76800));
  v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
  v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
  return (uint32_t)(v_x1_u32r>>12)/1024.0;
}

float calcAltitude(float pressure)
{
  float A = pressure/SEALEVELPRESSURE_HPA;
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 1.0 - C;
  C = C /0.0000225577;
  return C;
}

uint8_t Read8(uint8_t reg)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(_address, 1);

  if(Wire.available() < 1) {
    isTransport_OK = false;
    return 0;
  } else {
    isTransport_OK = true;
  }

  return Wire.read();
}

uint16_t Read16(uint8_t reg)
{
  uint8_t msb, lsb;

  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(_address, 2);
  // return 0 if slave didn't response
  if(Wire.available() < 2) {
    isTransport_OK = false;
    return 0;
  } else {
    isTransport_OK = true;
  }
  msb = Wire.read();
  lsb = Wire.read();

  return (uint16_t) msb<<8 | lsb;
}

uint16_t Read16LE(uint8_t reg)
{
  uint16_t data = Read16(reg);
  return (data >> 8) | (data << 8);
}

int16_t ReadS16(uint8_t reg)
{
  return (int16_t)Read16(reg);
}

int16_t ReadS16LE(uint8_t reg)
{
  return (int16_t)Read16LE(reg);
}

uint32_t Read24(uint8_t reg)
{
  uint32_t data;

  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(_address, 3);
  // return 0 if slave didn't response
  if(Wire.available() < 3) {
    isTransport_OK = false;
    return 0;
  } else {
    isTransport_OK = true;
  }
  data = Wire.read();
  data <<= 8;
  data |= Wire.read();
  data <<= 8;
  data |= Wire.read();

  return data;
}

void writeRegister(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

void setup() {
    Serial.begin(9600);

    init2(); 
       
    lcd.begin(16, 2);
    lcd.setRGB(255, 255, 255);
}


void loop() { 
    float t = getTemperature();
    Serial.print("Temperature = ");
    Serial.print(t);
    Serial.println(" *C");

    float h = getHumidity();
    Serial.print("Humidity = ");
    Serial.print(h);
    Serial.println(" %");

    float p = getPressure() / 100.0;
    Serial.print("Pressure = ");
    Serial.print(p);
    Serial.println(" hPa");

    float a = calcAltitude(p);
    Serial.print("Approx. Altitude = ");
    Serial.print(a);
    Serial.println(" m");

    Serial.println();
    
    float f = 255*(t-20.0)/(30.0);
    lcd.setRGB(255-f, f, h / 100.0 * 255);

    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.setCursor(2, 0);
    lcd.print(t);
    
    lcd.setCursor(8, 0);
    lcd.print("P:");
    lcd.setCursor(10, 0);
    lcd.print(p);

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.setCursor(2, 1);
    lcd.print(h);
    
    lcd.setCursor(8, 1);
    lcd.print("A:");
    lcd.setCursor(10, 1);
    lcd.print(a);

    delay(1000);
}

