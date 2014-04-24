void doInput(){
  keyPoll();
  if(buttonOnce&BTN_UP){
    if(currentParam>0x00){currentParam--;}
    else{currentParam=0x08;} //Underflow roll
    return;}
  if(buttonOnce&BTN_DOWN){
    if(currentParam<0x08){currentParam++;}
    else{currentParam=0x00;} //Overflow roll
    return;}
  if(buttonHeld&BTN_LEFT&&confBlock[currentParam]>0x00){
    confBlock[currentParam]--;
    i2cWrite(currentParam, confBlock[currentParam]);
    delay(keymaticDelay);
    return;}
  if(buttonHeld&BTN_RIGHT&&confBlock[currentParam]<0xff){
    confBlock[currentParam]++;
    i2cWrite(currentParam, confBlock[currentParam]);
    delay(keymaticDelay);
    return;}
  if(buttonOnce&BTN_SETUP){doSetup(); return;}
  if(buttonOnce&BTN_DEBUG){doDebug(); return;}
  if(buttonOnce&BTN_LOAD_CANCEL){
    if(!loadConf(currentMode, confBlock)){ //CRC failed
      loadConf(0x00, confBlock);
      storeConf(currentMode, confBlock);}
    i2cWriteAll();
    return;}
  if(buttonOnce&BTN_SAVE_OK){
    if(!currentMode==0x00){
      storeConf(currentMode, confBlock);
      lcd.setCursor(0,0);
      lcd.print(" Settings saved ");
      delay(1000);} //Don't overwrite defaults
    return;}}

void writeGui(){
  lcd.setCursor(0,0);
  strPrint(currentMode+0x20);
  strPrint(currentParam+0x40);
  lcd.setCursor(0,1);
  for(byte i=0x00; i<0x50; i=i+0x05){ //Here be dragons
    if((confBlock[currentParam]/3.2)-i<0x05&&(confBlock[currentParam]/3.2)-i>=0)
      {lcd.write((byte)(int(confBlock[currentParam]/3.2))%0x05);}
    else{lcd.write((byte)0x05);}}}
