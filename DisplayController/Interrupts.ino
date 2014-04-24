ISR(TIMER1_CAPT_vect){
  if(TCCR1B&0x40){
    if(digitalRead(SIG_SELECT)){ //Horizontal rising edge
      sync[LAST_H_LOW]=sync[H_LOW];
      sync[H_LOW]=ICR1L|(int)(ICR1H<<8);}
    else{ //Vertical rising edge
      sync[LAST_V_LOW]=sync[V_LOW];
      sync[V_LOW]=ICR1L|(int)(ICR1H<<8);}}
  else{
    if(digitalRead(SIG_SELECT)){ //Horizontal falling edge
      sync[LAST_H_HIGH]=sync[H_HIGH];
      sync[H_HIGH]=ICR1L|(int)(ICR1H<<8);}
    else{ //Vertical falling edge
      sync[LAST_V_HIGH]=sync[V_HIGH];
      sync[V_HIGH]=ICR1L|(int)(ICR1H<<8);}}
  ICR1H=0x00;
  ICR1L=0x00;
  TIMSK1|=0x20;} //Reenable capture interrupt
  
ISR(TIMER1_OVF_vect){
  if(TCCR1B&0x40){
    if(digitalRead(SIG_SELECT)){ //Horizontal rising edge
      sync[LAST_H_LOW]=sync[H_LOW];
      sync[H_LOW]=0;}
    else{ //Vertical rising edge
      sync[LAST_V_LOW]=sync[V_LOW];
      sync[V_LOW]=0;}}
  else{
    if(digitalRead(SIG_SELECT)){ //Horizontal falling edge
      sync[LAST_H_HIGH]=sync[H_HIGH];
      sync[H_HIGH]=0;}
    else{ //Vertical falling edge
      sync[LAST_V_HIGH]=sync[V_HIGH];
      sync[V_HIGH]=0;}}
  TIMSK1|=0x01;} //Reenable overflow interrupt
  
ISR(TIMER2_OVF_vect){
  if(overflowCount<6){ //Looking for vertical sync
    TIMSK1|=0x21; //Enable Timer 1 interrupt capture and overflow  
    if(digitalRead(SIG_SELECT)){digitalWrite(SIG_SELECT, LOW);}
    if(!(TCCR1B&0x07)==0x02){ // Prescaler != 8
      TCCR1B&=0xf8;
      ICR1H=0x00;
      ICR1L=0x00;
      TCCR1B|=0x02;}}
  else if(overflowCount<8){ //Looking for horizontal sync
    TIMSK1|=0x21; //Enable Timer 1 interrupt capture and overflow
    if(!digitalRead(SIG_SELECT)){digitalWrite(SIG_SELECT, HIGH);}
    if(!(TCCR1B&0x07)==0x01){ // Prescaler != 1
      TCCR1B&=0xf8;
      ICR1H=0x00;
      ICR1L=0x00;
      TCCR1B|=0x01;}}
  else if(overflowCount<9){ //Figuring out what mode we are in
    TIMSK1&=~0x21; //Disable Timer 1 interrupt capture and overflow
    TCCR1B&=~0x07; //Stop Timer 1
    if(match(sync[H_LOW],sync[LAST_H_LOW],syncToler)&&match(sync[H_HIGH],sync[LAST_H_HIGH],syncToler)&&
      match(sync[V_LOW],sync[LAST_V_LOW],syncToler)&&match(sync[V_HIGH],sync[LAST_V_HIGH],syncToler)){
        int hSync;
        int vSync;
        if(sync[H_LOW]>sync[H_HIGH]){hSync=(sync[H_LOW]+sync[H_HIGH])*8.125;}
        else{hSync=(sync[H_LOW]+sync[H_HIGH])*-8.125;}
        if(sync[V_LOW]>sync[V_HIGH]){vSync=(sync[V_LOW]+sync[V_HIGH])/2;}
        else{vSync=(sync[V_LOW]+sync[V_HIGH])/-2;}
        byte newMode=0x00;
        for(byte i=0x01;i<0x20;i++){
          int modeH=0x0000|EEPROM.read((i*0x10)+0x09);
          modeH<<=0x08;
          modeH|=EEPROM.read((i*0x10)+0x0a);
          int modeV=0x0000|EEPROM.read((i*0x10)+0x0b);
          modeV<<=0x08;
          modeV|=EEPROM.read((i*0x10)+0x0c);
          if(match(modeH,hSync,syncToler)&&match(modeV,vSync,syncToler)){newMode=i;}}
          if(newMode!=currentMode){
            currentMode=newMode;
            loadConf(currentMode, confBlock);
            i2cWriteAll();}}}
  overflowCount++;
  if(overflowCount>pollInterval){overflowCount=0;}
  TIMSK2|=0x01;} //Reenable overflow interrupt
