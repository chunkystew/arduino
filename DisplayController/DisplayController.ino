#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Wire.h>
//PROGMEM stuff; better living through string tables.
#include <avr/pgmspace.h>
prog_char str00[] PROGMEM="0x00: Default   ";
prog_char str01[] PROGMEM="0x01: 240x240x60";
prog_char str02[] PROGMEM="0x02: 256x240x60";
prog_char str03[] PROGMEM="0x03: 256x256x57";
prog_char str04[] PROGMEM="0x04: 256x264x58";
prog_char str05[] PROGMEM="0x05: 288x240x60";
prog_char str06[] PROGMEM="0x06: 296x240x60";
prog_char str07[] PROGMEM="0x07: 304x240x60";
prog_char str08[] PROGMEM="0x08: 320x200x60";
prog_char str09[] PROGMEM="0x09: 320x240x60";
prog_char str0a[] PROGMEM="0x0a: 320x256x58";
prog_char str0b[] PROGMEM="0x0b: 336x240x60";
prog_char str0c[] PROGMEM="0x0c: 352x256x57";
prog_char str0d[] PROGMEM="0x0d: 352x264x58";
prog_char str0e[] PROGMEM="0x0e: 352x288x51";
prog_char str0f[] PROGMEM="0x0f: 368x240x60";
prog_char str10[] PROGMEM="0x10: 384x288x51";
prog_char str11[] PROGMEM="0x11: 392x240x60";
prog_char str12[] PROGMEM="0x12: 400x256x53";
prog_char str13[] PROGMEM="0x13: 448x240x60";
prog_char str14[] PROGMEM="0x14: 512x240x60";
prog_char str15[] PROGMEM="0x15: 512x288x51";
prog_char str16[] PROGMEM="0x16: 512x384x60";
prog_char str17[] PROGMEM="0x17: 512x448x60";
prog_char str18[] PROGMEM="0x18: 512x512x58";
prog_char str19[] PROGMEM="0x19: 632x264x57";
prog_char str1a[] PROGMEM="0x1a: 640x240x60";
prog_char str1b[] PROGMEM="0x1b: 640x288x50";
prog_char str1c[] PROGMEM="0x1c: 640x480x60";
prog_char str1d[] PROGMEM="0x1d: 648x304x60";
prog_char str1e[] PROGMEM="0x1e: 720x480x60";
prog_char str1f[] PROGMEM="0x1f: 800x600x50";
prog_char str20[] PROGMEM="No sync   ";
prog_char str21[] PROGMEM="240x240x60";
prog_char str22[] PROGMEM="256x240x60";
prog_char str23[] PROGMEM="256x256x57";
prog_char str24[] PROGMEM="256x264x58";
prog_char str25[] PROGMEM="288x240x60";
prog_char str26[] PROGMEM="296x240x60";
prog_char str27[] PROGMEM="304x240x60";
prog_char str28[] PROGMEM="320x200x60";
prog_char str29[] PROGMEM="320x240x60";
prog_char str2a[] PROGMEM="320x256x58";
prog_char str2b[] PROGMEM="336x240x60";
prog_char str2c[] PROGMEM="352x256x57";
prog_char str2d[] PROGMEM="352x264x58";
prog_char str2e[] PROGMEM="352x288x51";
prog_char str2f[] PROGMEM="368x240x60";
prog_char str30[] PROGMEM="384x288x51";
prog_char str31[] PROGMEM="392x240x60";
prog_char str32[] PROGMEM="400x256x53";
prog_char str33[] PROGMEM="448x240x60";
prog_char str34[] PROGMEM="512x240x60";
prog_char str35[] PROGMEM="512x288x51";
prog_char str36[] PROGMEM="512x384x60";
prog_char str37[] PROGMEM="512x448x60";
prog_char str38[] PROGMEM="512x512x58";
prog_char str39[] PROGMEM="632x264x57";
prog_char str3a[] PROGMEM="640x240x60";
prog_char str3b[] PROGMEM="640x288x50";
prog_char str3c[] PROGMEM="640x480x60";
prog_char str3d[] PROGMEM="648x304x60";
prog_char str3e[] PROGMEM="720x480x60";
prog_char str3f[] PROGMEM="800x600x50";
prog_char str40[] PROGMEM=" Brite";
prog_char str41[] PROGMEM=" Contr";
prog_char str42[] PROGMEM=" VPosi";
prog_char str43[] PROGMEM=" VSize";
prog_char str44[] PROGMEM=" VHold";
prog_char str45[] PROGMEM=" HPosi";
prog_char str46[] PROGMEM=" HSize";
prog_char str47[] PROGMEM=" HHold";
prog_char str48[] PROGMEM=" Pcush";
prog_char str49[] PROGMEM="Brightness    ->";
prog_char str4a[] PROGMEM="Contrast      ->";
prog_char str4b[] PROGMEM="Vert Position ->";
prog_char str4c[] PROGMEM="Vert Size     ->";
prog_char str4d[] PROGMEM="Vert Hold     ->";
prog_char str4e[] PROGMEM="Horz Position ->";
prog_char str4f[] PROGMEM="Horz Size     ->";
prog_char str50[] PROGMEM="Horz Hold     ->";
prog_char str51[] PROGMEM="Pincushion    ->";
PROGMEM const char* strtbl[]={str00, str01, str02, str03, str04, str05, str06, str07,
                              str08, str09, str0a, str0b, str0c, str0d, str0e, str0f,
                              str10, str11, str12, str13, str14, str15, str16, str17,
                              str18, str19, str1a, str1b, str1c, str1d, str1e, str1f,
                              str20, str21, str22, str23, str24, str25, str26, str27,
                              str28, str29, str2a, str2b, str2c, str2d, str2e, str2f,
                              str30, str31, str32, str33, str34, str35, str36, str37,
                              str38, str39, str3a, str3b, str3c, str3d, str3e, str3f,
                              str40, str41, str42, str43, str44, str45, str46, str47,
                              str48, str49, str4a, str4b, str4c, str4d, str4e, str4f,
                              str50, str51};
char buffer[0x11]; //16 characters and a null, longest possible string
//Pin definitions
#define LCD_D4 3
#define LCD_D5 4
#define LCD_D6 5
#define LCD_D7 6
#define PISO_CLOCK 7
#define SIG_INT 8
#define PISO_DATA 9
#define SIG_SELECT 10
#define LCD_ENABLE 11
#define LCD_SELECT 12
#define PISO_LATCH 13
//Button definitions
#define BTN_UP 0x01
#define BTN_DOWN 0x02
#define BTN_LEFT 0x04
#define BTN_RIGHT 0x08
#define BTN_SETUP 0x10
#define BTN_DEBUG 0x20
#define BTN_LOAD_CANCEL 0x40
#define BTN_SAVE_OK 0x80
//Definitions for sync[]
#define H_HIGH 0x00
#define V_HIGH 0x01
#define H_LOW 0x02
#define V_LOW 0x03
#define LAST_H_HIGH 0x04
#define LAST_V_HIGH 0x05
#define LAST_H_LOW 0x06
#define LAST_V_LOW 0x07
//Definitions for I2C
#define BRITE_I2C 0x58
#define BRITE_CMD 0x00
#define CONTR_I2C 0x5e
#define CONTR_CMD 0x00
#define VPOSI_I2C 0x5a
#define VPOSI_CMD 0x00
#define VSIZE_I2C 0x58
#define VSIZE_CMD 0x80
#define VHOLD_I2C 0x52
#define VHOLD_CMD 0x80
#define HPOSI_I2C 0x54
#define HPOSI_CMD 0x80
#define HSIZE_I2C 0x50
#define HSIZE_CMD 0x00
#define HHOLD_I2C 0x52
#define HHOLD_CMD 0x90
#define PCUSH_I2C 0x5e
#define PCUSH_CMD 0x80
//Important EEPROM locations
#define KEY_REPEAT 0x03e0
#define POLL_INTERVAL 0x03e1
#define SYNC_TOLER 0x03e2

const float cycle=1/16000000;
const float cycleMs=1000/16000000;
const float cycleUs=1000000/16000000;
LiquidCrystal lcd(LCD_SELECT, LCD_ENABLE, LCD_D7, LCD_D6, LCD_D5, LCD_D4);
byte buttonOnce=0x00;
byte buttonHeld=0x00;
byte keymaticDelay=EEPROM.read(KEY_REPEAT);
byte pollInterval=EEPROM.read(POLL_INTERVAL);
byte syncToler=EEPROM.read(SYNC_TOLER);
byte overflowCount=0x00;
int sync[8];
byte currentMode=0x00;
byte currentParam=0x00;
byte confBlock[0x10];
const float potValue[]={200000, 10000, 20000, 200000, 5000, 10000, 5000, 5000, 10000};
const byte cgRam[]={0x10, 0x10, 0x10, 0x1f, 0x1f, 0x10, 0x10, 0x10,  //Leftmost
                    0x08, 0x08, 0x08, 0x1f, 0x1f, 0x08, 0x08, 0x08,  //Leftish
                    0x04, 0x04, 0x04, 0x1f, 0x1f, 0x04, 0x04, 0x04,  //Center
                    0x02, 0x02, 0x02, 0x1f, 0x1f, 0x02, 0x02, 0x02,  //Rightish
                    0x01, 0x01, 0x01, 0x1f, 0x1f, 0x01, 0x01, 0x01,  //Rightmost
                    0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00,  //None
                    0x00, 0x01, 0x02, 0x02, 0x14, 0x14, 0x08, 0x08,  //Tick
                    0x00, 0x08, 0x14, 0x14, 0x05, 0x05, 0x02, 0x00}; //Sine

void setup(){
  for(byte i=0x00; i<=0x07; i++){sync[i]=0;}
  pinMode(LCD_D4, OUTPUT);
  pinMode(LCD_D5, OUTPUT);
  pinMode(LCD_D6, OUTPUT);
  pinMode(LCD_D7, OUTPUT);
  pinMode(PISO_CLOCK, OUTPUT);
  pinMode(SIG_INT, INPUT);
  pinMode(PISO_DATA, INPUT);
  pinMode(SIG_SELECT, OUTPUT);
  pinMode(LCD_ENABLE, OUTPUT);
  pinMode(LCD_SELECT, OUTPUT);
  pinMode(PISO_LATCH, OUTPUT);
  loadCgRam(cgRam);
  lcd.begin(16,2);
  loadConf(currentMode, confBlock);
  i2cWriteAll();
  Wire.begin();
  TCCR1A=0x00; //Timer/Counter 1 Control Register A
  TCCR1B=0x00; //Timer/Counter 1 Control Register B
  TIMSK1=0x00; //Timer/Counter 1 Interrupt Mask Register
  TCCR2A=0x00; //Timer/Counter 2 Control Register A
  TCCR2B=0x07; //Timer/Counter 2 Control Register B
  TIMSK2=0x01;} //Timer/Counter 2 Interrupt Mask Register
  
void loop() {
  writeGui();
  doInput();}
