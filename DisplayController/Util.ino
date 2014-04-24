char* byteToText(byte input){
  char output[2];
  output[0]=(input>>4);  
  output[1]=(input&0x0f);
  for(byte i=0x00; i<=0x01; i++){
    if(output[i]<=0x09){output[i]+=0x30;}
    else{output[i]+=0x57;}}
  return output;}
  
byte crc8(const byte *input, byte len){
  byte output=0x00;
  while(len--){
    byte data=*input++;
    for(byte i=0x08; i; i--){
      byte sum=(output^data)&0x01;
      output>>=1;
      if(sum){output^=0x8c;}
      data>>=1;}}
  return output;}
  
boolean match(int valOne, int valTwo, byte toler){
  if(valOne>valTwo&&valTwo*((float)toler/1000)>=valOne){return true;}
  if(valTwo>valOne&&valOne*((float)toler/1000)>=valTwo){return true;}
  return false;}
  
void strPrint(byte index){
  strcpy_P(buffer, (char*)pgm_read_word(&(strtbl[index])));
  lcd.print(buffer);}
  
