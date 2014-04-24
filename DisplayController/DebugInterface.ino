void doDebug(){
  byte x=0x20;
  byte y=0x00;
  do{
  keyPoll();
  if(buttonOnce&BTN_UP){
    x=0x20;
    if(y==0x03){y=0x00;}
    else{y++;}}
  if(buttonOnce&BTN_DOWN){
    x=0x20;
    if(!y){y=0x03;}
    else{y--;}}
  if(buttonOnce&BTN_LEFT&&y==2){
    if(!x){x=0x20;}
    else{x--;}}
  if(buttonOnce&BTN_RIGHT&&y==2){
    if(x==0x20){x=0x00;}
    else{x++;}}
  if(buttonOnce&BTN_SAVE_OK){
    switch(y){
      case 0x00:
        sigInsp();
        break;
      case 0x01:
        outInsp();
        break;
      case 0x02:
        if(x==0x20){break;}
        modeData(x);
        break;
      case 0x03:
        sysInfo();
        break;}}
  lcd.setCursor(0,0);
  lcd.print("   Debug menu   ");
  lcd.setCursor(0,1);
  switch(y){
    case 0x00: lcd.print("Signal inspector"); break;
    case 0x01: lcd.print("Output inspector"); break;
    case 0x02:
      if(x<=0x1f){strPrint(x);}
      else{lcd.print("<- Mode data  ->");}
      break;
    case 0x03: lcd.print("  System info   "); break;}
  }while(!(buttonOnce&BTN_DEBUG));}
  
void sigInsp(){ //There are still some formatting glitches that could happen if we're getting sync at impossible rates
  lcd.setCursor(0,0);
  lcd.print("H             ->");
  lcd.setCursor(1,0);
  lcd.write(0x07);
  lcd.setCursor(0,1);
  lcd.print("V             <-");
  lcd.setCursor(1,1);
  lcd.write(0x07);
  byte x=0x00;
  do{
    for(byte i=0x00; i<=0x01; i++){
      lcd.setCursor(3,i);
      if(sync[H_HIGH+i]==0||sync[H_LOW+i]==0||!match(sync[H_HIGH+i],sync[H_LOW+i],syncToler)){
        lcd.print("No signal ");}
      else{
        switch(x){
          case 0x00: 
            if(!i){
              lcd.print(1/((sync[H_HIGH]+sync[H_LOW])*cycleMs),3);
              lcd.setCursor(10,i);
              lcd.print("KHz ");}
            else{
              lcd.print(1/((sync[V_HIGH]+sync[V_LOW])*8*cycle),3);
              lcd.setCursor(10,i);
              lcd.print("Hz  ");}
            break;
          case 0x01:
            if(!i){
              lcd.print((sync[H_HIGH]+sync[H_LOW])*cycleUs,3);
              lcd.setCursor(10,i);
              lcd.write(0xe4); //Lowercase mu
              lcd.print("s  ");}
            else{
              lcd.print((sync[V_HIGH]+sync[V_LOW])*8*cycleMs,3);
              lcd.setCursor(10,i);
              lcd.print("ms  ");}
            break;
          case 0x02:
            float dutyCycle;
            if(sync[H_HIGH+i]>sync[H_LOW+i]){dutyCycle=sync[H_HIGH+i]/(sync[H_HIGH+i]+sync[H_LOW+i]);}
            else{dutyCycle=-(sync[H_HIGH+i]/(sync[H_HIGH+i]+sync[H_LOW+i]));}
            lcd.print(abs(dutyCycle),2);
            if(dutyCycle<0){lcd.print("% -");}
            else{lcd.print("% +");}
            if(abs(dutyCycle)<10){lcd.print("   ");}
            else if(abs(dutyCycle)<100){lcd.print("  ");}
            else{lcd.print(" ");}
          break;}}}
    if(buttonOnce&BTN_LEFT){
      if(!x){x=0x02;}
      else{x--;}}
    if(buttonOnce&BTN_RIGHT){
      if(x==0x02){x=0x00;} 
      else{x++;}}
    keyPoll();
  }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_SETUP)));}
  
void outInsp(){
  byte x=0;
  do{
    lcd.setCursor(0,0);
    strPrint(x+0x49);
    float parameter=confBlock[x];
    float value=(parameter*potValue[x])/255000;
    lcd.setCursor(0,1);
    lcd.print(value,2);
    lcd.print("K");
    lcd.write(0xf4); //Lowercase omega
    lcd.print(" 0x");
    lcd.write(byteToText(confBlock[x])[0]);
    lcd.write(byteToText(confBlock[x])[1]);
    if(value<10){lcd.print("   <-");}
    else if(value<100){lcd.print("  <-");}
    else{lcd.print(" <-");}
    if(buttonOnce&BTN_LEFT){
      if(!x){x=0x08;}
      else{x--;}}
    if(buttonOnce&BTN_RIGHT){
      if(x==0x08){x=0x00;} 
      else{x++;}}
    keyPoll();   
    keyPoll();
  }while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_SETUP)));}

void modeData(byte mode){
  byte i=0x00;
  for(byte j=0x00; j<=0x01; j++){
    for(byte k=0x00; k<=0x0e; k=k+0x02){
      lcd.setCursor(k,j);
      lcd.print(byteToText(EEPROM.read((0x10*mode)+i)));
      i++;}}
  do{keyPoll();}while(buttonOnce==0x00);}

void sysInfo(){ //No need for a string table here
  lcd.setCursor(0,0);
  lcd.print("MCU: ");
#if defined __AVR_ATmega1280__
  lcd.print("ATmega1280 ");
#elif defined __AVR_ATmega1281__
  lcd.print("ATmega1281 ");
#elif defined __AVR_ATmega128__
  lcd.print("ATmega128  ");
#elif defined __AVR_ATmega64__
  lcd.print("ATmega64   ");
#elif defined __AVR_ATmega32__
  lcd.print("ATmega32   ");
#elif defined __AVR_ATmega16__
  lcd.print("ATmega16   ");
#elif defined __AVR_ATmega8__
  lcd.print("ATmega8    ");
#elif defined __AVR_ATmega88__
  lcd.print("ATmega88   ");
#elif defined __AVR_ATmega168__
  lcd.print("ATmega168  ");
#elif defined __AVR_ATmega328P__
  lcd.print("ATmega328P ");
#elif defined __AVR_ATmega162__
  lcd.print("ATmega162  ");
#elif defined __AVR_ATmega163__
  lcd.print("ATmega163  ");
#elif defined __AVR_ATmega169__
  lcd.print("ATmega169  ");
#elif defined __AVR_ATmega8515__
  lcd.print("ATmega8515 ");
#elif defined __AVR_ATmega8535__
  lcd.print("ATmega8535 ");
#endif 
  lcd.setCursor(0,1);
  lcd.print("Free SRAM:      ");
  lcd.setCursor(11,1);
  lcd.print(freeRam());
  lcd.print("B");
  do{keyPoll();}while(!(buttonOnce&~(BTN_RIGHT|BTN_LEFT|BTN_UP|BTN_DOWN|BTN_SETUP)));}
