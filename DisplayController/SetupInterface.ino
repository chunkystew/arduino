void doSetup(){
  byte x=0x20;
  byte y=0x01;
  do{
  keyPoll();
  if(buttonOnce&BTN_UP){
    x=0x20;
    if(y==0x06){y=0x00;}}
    else{y++;}
  if(buttonOnce&BTN_DOWN){
    x=0x20;
    if(y==0x00){y=0x06;}}
    else{y--;}
  if(buttonOnce&BTN_LEFT&&(y==0x04|0x05)){
    if(x==0x00){x=0x20;}}
    else{x--;}
  if(buttonOnce&BTN_RIGHT&&(y==0x04|0x05)){
    if(x==0x20){x=0x00;}
    else{x++;}}
    if(!(match(sync[H_LOW],sync[LAST_H_LOW],syncToler)&&match(sync[H_HIGH],sync[LAST_H_HIGH],syncToler)&&
       match(sync[V_LOW],sync[LAST_V_LOW],syncToler)&&match(sync[V_HIGH],sync[LAST_V_HIGH],syncToler))&&y==0x00){
      if(buttonOnce&BTN_DOWN){y=0x06;}
      else{y=0x01;}} //Trickery to suppress mode setting with no sync
  if(buttonOnce&BTN_SAVE_OK){\
    switch(y){
      case 0x00: setMode(); break;
      case 0x01: keySetup(); break;
      case 0x02: syncSetup(); break;
      case 0x03: pollSetup(); break;
      case 0x04:
        if(x==0x20){break;}
        copyMode(x);
        break;
      case 0x05:
        if(x==0x20){break;}
        delMode(x, true);
        break;
      case 0x06: delAll(); break;}}
  lcd.setCursor(0,0);
  lcd.print("   Setup menu   ");
  lcd.setCursor(0,1);
  switch(y){
    case 0x00: lcd.print("Set current mode"); break;
    case 0x01: lcd.print("   Key repeat   "); break;
    case 0x02: lcd.print(" Sync tolerance "); break;
    case 0x03: lcd.print("Mode poll delay "); break;
    case 0x04:
      if(x<0x20){strPrint(x);}
      else{lcd.print("<- Copy mode  ->");}
      break;
    case 0x05:
      if(x<0x20){strPrint(x);}
      else{lcd.print("<- Reset mode ->");}
      break;
    case 0x06: lcd.print("   Reset all    "); break;}
  }while(!(buttonOnce&BTN_SETUP));}

void setMode(){
  byte x=0x01;
  do{
    lcd.setCursor(0,0);
    lcd.print("H");
    if(sync[H_HIGH]<sync[H_LOW]){lcd.print("+");}
    else{lcd.print("-");}
    lcd.print(1/((sync[H_HIGH]+sync[H_LOW])*cycleMs),2);
    lcd.print("  ");
    lcd.print("V");
    if(sync[V_HIGH]<sync[V_LOW]){lcd.print("+");}
    else{lcd.print("-");}
    lcd.print(1/((sync[V_HIGH]+sync[V_LOW])*8*cycle),2);
    lcd.setCursor(0,1);
    lcd.print("<- ");
    strPrint(x+0x20);
    lcd.print(" ->");
    keyPoll();
    if(buttonOnce&BTN_LEFT){
      if(x<=0x01){x=0x1f;}
      else{x--;}}
    if(buttonOnce&BTN_RIGHT){
      if(x>=0x1f){x=0x01;}
      else{x++;}}
  if(buttonOnce&BTN_SAVE_OK){
    int hSync;
    int vSync;
    if(sync[H_LOW]>sync[H_HIGH]){hSync=(sync[H_LOW]+sync[H_HIGH])*8.125;}
    else{hSync=(sync[H_LOW]+sync[H_HIGH])*-8.125;}
    if(sync[V_LOW]>sync[V_HIGH]){vSync=(sync[V_LOW]+sync[V_HIGH])/2;}
    else{vSync=(sync[V_LOW]+sync[V_HIGH])/-2;}    
    EEPROM.write((x*0x10)+0x0a,hSync);  
    EEPROM.write((x*0x10)+0x09,hSync>>8);        
    EEPROM.write((x*0x10)+0x0c,vSync);  
    EEPROM.write((x*0x10)+0x0b,vSync>>8);    
    lcd.setCursor(15,1);
    lcd.write(0x06);
    delay(500);
    return;}
  }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_DEBUG)));}

void keySetup(){
  byte newDelay=keymaticDelay;
  lcd.setCursor(0,0);
  lcd.print(" Set key repeat ");
  do{
    lcd.setCursor(0,1);
    if(newDelay<10){lcd.print("      ");}
    else if(newDelay<100){lcd.print("     ");}
    else{lcd.print("    ");}
    lcd.print(newDelay);
    lcd.print(" msec    ");
    if(buttonOnce&BTN_LEFT&&newDelay>5){newDelay-=5;}
    if(buttonOnce&BTN_RIGHT&&newDelay<255){newDelay+=5;}
    keyPoll();
    if(buttonOnce&BTN_SAVE_OK){
      keymaticDelay=newDelay;
      EEPROM.write(KEY_REPEAT, keymaticDelay);
      lcd.setCursor(13,1);
      lcd.write(0x06);
      delay(500);
      return;}
  }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_DEBUG)));}

void syncSetup(){
  byte newToler=syncToler;
  lcd.setCursor(0,0);
  lcd.print(" Set sync toler ");
  do{
    lcd.setCursor(0,1);
    if(newToler>=25){lcd.print("  (!) ");}
    else{lcd.print("      ");}
    lcd.print((float)newToler/10,1);
    lcd.print("%      ");
    if(buttonOnce&BTN_LEFT&&newToler>1){newToler--;}
    if(buttonOnce&BTN_RIGHT&&newToler<50){newToler++;}
    keyPoll();
    if(buttonOnce&BTN_SAVE_OK){
      syncToler=newToler;
      EEPROM.write(SYNC_TOLER, syncToler);
      lcd.setCursor(11,1);
      lcd.write(0x06);
      delay(500);
      return;}
  }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_DEBUG)));}

void pollSetup(){
  byte newInterval=pollInterval;
  lcd.setCursor(0,0);
  lcd.print(" Set poll delay  ");
  do{
    lcd.setCursor(0,1);
    if(newInterval<62){lcd.print("    ");}
    else{lcd.print("   ");}
    lcd.print(newInterval*16.384,0); //Doing all the math with a 16-bit float rounds to zero
    lcd.print(" msec    ");
    if(buttonOnce&BTN_LEFT&&newInterval>9){newInterval--;}
    if(buttonOnce&BTN_RIGHT&&newInterval<255){newInterval++;}
    keyPoll();
    if(buttonOnce&BTN_SAVE_OK){
      pollInterval=newInterval;
      EEPROM.write(POLL_INTERVAL, pollInterval);
      lcd.setCursor(13,1);
      lcd.write(0x06);
      delay(500);
      return;}
  }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_DEBUG)));}

void copyMode(byte source){
  lcd.setCursor(0,0);
  lcd.print("   ");
  if(source==0x00){lcd.print("(Default) ");}
  else{strPrint(source+0x20);}
  lcd.print(" ->");
  byte target=currentMode;
  do{
    lcd.setCursor(0,1);
    lcd.print("   ");
    if(target==0x00){lcd.print("(Default) ");}
    else{strPrint(target+0x20);}
    lcd.print("   ");
    if(buttonOnce&BTN_LEFT){
      if(target==0x00){target=0x1f;}
      else{target--;}}
    if(buttonOnce&BTN_RIGHT){
      if(target==0x1f){target=0x00;}
      else{target++;}}
    keyPoll();
    if(buttonOnce&BTN_SETUP||buttonOnce&BTN_LOAD_CANCEL){return;}
  }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_DEBUG)));
  byte sourceBuffer[0x10];
  byte targetBuffer[0x10];
  loadConf(source, sourceBuffer);
  loadConf(target, targetBuffer);
  for(byte i=0x00; i<=0x08; i++){targetBuffer[i]=sourceBuffer[i];} //Preserve sync info
  storeConf(target, targetBuffer);
  if(currentMode==target){loadConf(target, confBlock);}
  lcd.setCursor(14,1);
  lcd.write(0x06);
  delay(500);}

void delMode(byte target, boolean askUser){
  if(askUser){
    lcd.setCursor(0,0);
    lcd.print("Reset ");
    if(target==0){lcd.print("(Default) ");}
    else{strPrint(target+0x20);}
    lcd.setCursor(0,1);
    lcd.print(" Are you sure?  ");
    do{
      keyPoll();
      if(buttonOnce&BTN_SETUP||buttonOnce&BTN_LOAD_CANCEL){return;}
    }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_DEBUG)));}
  byte buffer[]={0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  storeConf(target, buffer);
  if(currentMode==target){loadConf(target, confBlock);}
  if(askUser){
    lcd.setCursor(15,1);
    lcd.write(0x06);
    delay(500);}}

void delAll(){
  lcd.setCursor(0,0);
  lcd.print("Reset everything");
  lcd.setCursor(0,1);
  lcd.print(" Are you sure?  ");
  do{
    keyPoll();
    if(buttonOnce&BTN_SETUP||buttonOnce&BTN_LOAD_CANCEL){return;}
  }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_DEBUG)));
  lcd.setCursor(0,0);
  lcd.print("  Really sure?  ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  do{
    keyPoll();
    if(buttonOnce&BTN_SETUP||buttonOnce&BTN_LOAD_CANCEL){
      lcd.setCursor(0,0);
      lcd.print("     Phew!      ");
      lcd.setCursor(0,1);
      lcd.print("That was close! ");
      delay(2000);
      return;}
  }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_DEBUG)));
  lcd.setCursor(0,0);
  lcd.print("  Oh No!  :-[   ");
  lcd.setCursor(0,1);
  lcd.print(" Will I dream?  ");
  for(byte i=0x00; i<0x20; i++){delMode(i, false);}
  loadConf(currentMode, confBlock);
  delay(2000);} 
