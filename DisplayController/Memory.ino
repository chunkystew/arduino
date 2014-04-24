int freeRam(){
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v-(__brkval==0?(int)&__heap_start:(int)__brkval);}

void loadCgRam(const byte *input){       
  for(byte i=0x00; i<0x08; i++){
  byte thisChar[8];
    for(byte j=0x00; j<0x08; j++){
      thisChar[j]=input[(i*8)+j];}
    lcd.createChar(i, thisChar);}}
  
boolean loadConf(const byte reqMode, byte* output){
  for(int i=0x10*reqMode; i<(0x10*reqMode)+0x10; i++){
    output[i-(0x10*reqMode)]=EEPROM.read(i);}
  if(output[0x0f]==crc8(output,0x0f)){return true;}
  return false;}
    
void storeConf(const byte reqMode, byte* input){
  input[0x0f]=crc8(input, 0x0f);
  for(int i=0x10*reqMode; i<(0x10*reqMode)+0x10; i++){
    EEPROM.write(i, input[i-(0x10*reqMode)]);}}
