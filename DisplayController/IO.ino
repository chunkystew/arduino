void keyPoll(){
  digitalWrite(PISO_LATCH,1);
  delayMicroseconds(20);
  digitalWrite(PISO_LATCH,0);
  byte output=0;
  for(int i=7; i>=0; i--){ //Must be an int
    digitalWrite(PISO_CLOCK, 0);
    delayMicroseconds(2);
    if(digitalRead(PISO_DATA)){
      if(buttonHeld&0x01<<i){buttonOnce&=~(0x01<<i);}
      else{
        buttonHeld|=0x01<<i;
        buttonOnce|=0x01<<i;}}
    else{
      buttonOnce&=~(0x01<<i);
      buttonHeld&=~(0x01<<i);}
    digitalWrite(PISO_CLOCK, 1);}}

void i2cWriteAll(){for(byte i=0x00; i<=0x08; i++){i2cWrite(i,confBlock[i]);}}

//Dummy method, kill me!
//void i2cWrite(byte parameter, byte data){}

//Commented out to prevent deadlock - shield not constructed yet!
void i2cWrite(byte parameter, byte data){
  switch(parameter){
    case 0x00: Wire.beginTransmission(BRITE_I2C); Wire.write(BRITE_CMD); break;
    case 0x01: Wire.beginTransmission(CONTR_I2C); Wire.write(CONTR_CMD); break;
    case 0x02: Wire.beginTransmission(VPOSI_I2C); Wire.write(VPOSI_CMD); break;
    case 0x03: Wire.beginTransmission(VSIZE_I2C); Wire.write(VSIZE_CMD); break;
    case 0x04: Wire.beginTransmission(VHOLD_I2C); Wire.write(VHOLD_CMD); break;
    case 0x05: Wire.beginTransmission(HPOSI_I2C); Wire.write(HPOSI_CMD); break;
    case 0x06: Wire.beginTransmission(HSIZE_I2C); Wire.write(HSIZE_CMD); break;
    case 0x07: Wire.beginTransmission(HHOLD_I2C); Wire.write(HHOLD_CMD); break;
    case 0x08: Wire.beginTransmission(PCUSH_I2C); Wire.write(PCUSH_CMD); break;}
  Wire.write(data);
  Wire.endTransmission();}


