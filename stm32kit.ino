#include <SPI.h>
#include "lcdlib.h"
#include "lcdfont.h"

/*
   stm32f103RCT6 board
   PC1 = key0
   PC13 = key1
   PA8 = LED0
   PD2 = LED1
   PA0 = keywakeup
   W25016 16Mbit eeprom
   PA2 = EEPROM_#Chipselect
   PA6 = MISO EEPROM data out
   VCC = EEPROM #WP write protect
   VCC = EERROM #HOLD
   PA5 = EERROM SCLK
   PA7 = EEPROM Data in

*/
/*
   Waveshare SPI LCD22 TFT module
   BD663474 hitachi 320x240pixel 256K colour
   pin 1 GND 0V
   pin 3 3.3V
   pin 35 SPISCLK -> PA5 SCLK
   pin 33 SPIDATAIN -> PA7 MOSI
   pin 31 RES -> RESET PC13
   pin 37 CS -> PA4
   pin 39 RS -> PA3
*/
/* SPT2046 touch panel controller
   pin 19 TIRQ
   pin 21 TBUSY
   pin 23 TCS
   pin 25 TCLK
   pin 27 DIN
   pin 29 TOUT

*/
// constants won't change. Used here to set a pin number :
const int ledPin =  PA8;// LED D0the number of the LED pin
const int ledPin2 = PD2;// LED D1

void LCD_WRITE_REG(unsigned int index)
{
  unsigned int value_index;
  digitalWrite (PA3, LOW);
  digitalWrite (PA4, LOW);
  value_index = index;
  value_index = value_index >> 8;
  SPI.transfer (value_index);    //00000000 000000000
  value_index = index;
  value_index &= 0x00ff;
  SPI.transfer (index);
  digitalWrite (PA3, HIGH);
  digitalWrite (PA4, HIGH);
}

void LCD_SEND_COMMAND(unsigned int index, unsigned int data)
{
  //select command register
  digitalWrite (PA3, LOW);
  digitalWrite (PA4, LOW);
  SPI.transfer (index >> 8);  //00000000 000000000
  SPI.transfer (index);
  digitalWrite (PA4, HIGH);
  //send data
  digitalWrite (PA3, HIGH);
  digitalWrite (PA4, LOW);
  SPI.transfer(data >> 8);  //00000000 000000000
  SPI.transfer(data);
  digitalWrite (PA4, HIGH);
}

void LCD_WRITE_COMMAND(unsigned int index, unsigned int data)
{
  //select command register
  digitalWrite (PA3, LOW);
  digitalWrite (PA4, LOW);
  SPI.transfer (index >> 8);  //00000000 000000000
  SPI.transfer (index);
  digitalWrite (PA4, HIGH);
  //send data
  digitalWrite (PA3, HIGH);
  digitalWrite (PA4, LOW);
  SPI.transfer (data >> 8);  //00000000 000000000
  SPI.transfer (data);
  digitalWrite (PA4, HIGH);
}

void LCD_WRITE_DATA(unsigned int data)
{
  SPI.transfer(data >> 8);  //00000000 000000000
  SPI.transfer(data);
}

void lcd_init(void)
{
  LCD_WRITE_COMMAND( 0x000, 0x0001 ); /* oschilliation start */
  delay (1);
  /* Power settings */
  LCD_WRITE_COMMAND( 0x100, 0x0000 ); /*power supply setup*/
  LCD_WRITE_COMMAND( 0x101, 0x0000 );
  LCD_WRITE_COMMAND( 0x102, 0x3110 );
  LCD_WRITE_COMMAND( 0x103, 0xe200 );
  LCD_WRITE_COMMAND( 0x110, 0x009d );
  LCD_WRITE_COMMAND( 0x111, 0x0022 );
  LCD_WRITE_COMMAND( 0x100, 0x0120 );
  delay( 2 );
  LCD_WRITE_COMMAND( 0x100, 0x3120 );
  delay( 8 );
  /* Display control */
  LCD_WRITE_COMMAND( 0x001, 0x0100 );
  LCD_WRITE_COMMAND( 0x002, 0x0000 );
  LCD_WRITE_COMMAND( 0x003, 0x1230 );
  LCD_WRITE_COMMAND( 0x006, 0x0000 );
  LCD_WRITE_COMMAND( 0x007, 0x0101 );
  LCD_WRITE_COMMAND( 0x008, 0x0808 );
  LCD_WRITE_COMMAND( 0x009, 0x0000 );
  LCD_WRITE_COMMAND( 0x00b, 0x0000 );
  LCD_WRITE_COMMAND( 0x00c, 0x0000 );
  LCD_WRITE_COMMAND( 0x00d, 0x0018 );
  /* LTPS control settings */
  LCD_WRITE_COMMAND( 0x012, 0x0000 );
  LCD_WRITE_COMMAND( 0x013, 0x0000 );
  LCD_WRITE_COMMAND( 0x018, 0x0000 );
  LCD_WRITE_COMMAND( 0x019, 0x0000 );

  LCD_WRITE_COMMAND( 0x203, 0x0000 );
  LCD_WRITE_COMMAND( 0x204, 0x0000 );

  LCD_WRITE_COMMAND( 0x210, 0x0000 );
  LCD_WRITE_COMMAND( 0x211, 0x00ef );
  LCD_WRITE_COMMAND( 0x212, 0x0000 );
  LCD_WRITE_COMMAND( 0x213, 0x013f );
  LCD_WRITE_COMMAND( 0x214, 0x0000 );
  LCD_WRITE_COMMAND( 0x215, 0x0000 );
  LCD_WRITE_COMMAND( 0x216, 0x0000 );
  LCD_WRITE_COMMAND( 0x217, 0x0000 );

  // Gray scale settings
  LCD_WRITE_COMMAND( 0x300, 0x5343);
  LCD_WRITE_COMMAND( 0x301, 0x1021);
  LCD_WRITE_COMMAND( 0x302, 0x0003);
  LCD_WRITE_COMMAND( 0x303, 0x0011);
  LCD_WRITE_COMMAND( 0x304, 0x050a);
  LCD_WRITE_COMMAND( 0x305, 0x4342);
  LCD_WRITE_COMMAND( 0x306, 0x1100);
  LCD_WRITE_COMMAND( 0x307, 0x0003);
  LCD_WRITE_COMMAND( 0x308, 0x1201);
  LCD_WRITE_COMMAND( 0x309, 0x050a);

  /* RAM access settings */
  LCD_WRITE_COMMAND( 0x400, 0x4027 );
  LCD_WRITE_COMMAND( 0x401, 0x0000 );
  LCD_WRITE_COMMAND( 0x402, 0x0000 ); /* First screen drive position (1) */
  LCD_WRITE_COMMAND( 0x403, 0x013f ); /* First screen drive position (2) */
  LCD_WRITE_COMMAND( 0x404, 0x0000 );

  LCD_WRITE_COMMAND( 0x200, 0x0000 );
  LCD_WRITE_COMMAND( 0x201, 0x0000 );

  LCD_WRITE_COMMAND( 0x100, 0x7120 );
  LCD_WRITE_COMMAND( 0x007, 0x0103 );
  delay(1);
  LCD_WRITE_COMMAND( 0x007, 0x0113 );
  delay(1);

}

void lcd_clear_screen(unsigned int color_background)
{
  unsigned int i, j;
  LCD_WRITE_COMMAND(0x210, 0x00);
  LCD_WRITE_COMMAND(0x212, 0x0000);
  LCD_WRITE_COMMAND(0x211, 0xEF);
  LCD_WRITE_COMMAND(0x213, 0x013F);
  LCD_WRITE_COMMAND(0x200, 0x0000);
  LCD_WRITE_COMMAND(0x201, 0x0000);
  digitalWrite (PA3, LOW);
  LCD_WRITE_REG(0x202); //RAM Write index
  digitalWrite (PA4, LOW);
  for (i = 0; i < 320; i++)
  {
    for (j = 0; j < 240; j++)
    {
      LCD_WRITE_DATA( color_background );
    }
  }
  digitalWrite (PA3, LOW);
  digitalWrite (PA4, HIGH);
}

void lcd_set_cursor(unsigned char x, unsigned int y)
{
  if ( (x > 320) || (y > 240) )
  {
    return;
  }
  LCD_WRITE_COMMAND( 0x200, x );
  LCD_WRITE_COMMAND( 0x201, y );
}

void lcd_display_char(  char ch_asc,
                        unsigned int color_front,
                        unsigned int color_background,
                        unsigned char postion_x,
                        unsigned char postion_y)
{
  unsigned char i, j, b;
  const unsigned char *p = 0;
  LCD_WRITE_COMMAND(0x210, postion_x * 8); //x start point
  LCD_WRITE_COMMAND(0x212, postion_y * 16); //y start point
  LCD_WRITE_COMMAND(0x211, postion_x * 8 + 7); //x end point
  LCD_WRITE_COMMAND(0x213, postion_y * 16 + 15); //y end point
  LCD_WRITE_COMMAND(0x200, postion_x * 8);
  LCD_WRITE_COMMAND(0x201, postion_y * 16);
  digitalWrite (PA3, LOW);
  LCD_WRITE_REG(0x202);
  digitalWrite (PA4, LOW);
  p = ascii;
  p += ch_asc * 16;
  for (j = 0; j < 16; j++)
  {
    b = *(p + j);
    for (i = 0; i < 8; i++)
    {
      if ( b & 0x80 )
      {
        LCD_WRITE_DATA(color_front);
      }
      else
      {
        LCD_WRITE_DATA(color_background);
      }
      b = b << 1;
    }
  }
  digitalWrite (PA4, HIGH);
}

void lcd_display_string(char *str, unsigned int color_front,
                        unsigned int color_background, unsigned char x, unsigned char y )
{
  while (*str)
  {
    lcd_display_char( *str, color_front, color_background, x, y);
    if (++x >= 30)
    {
      x = 0;
      if (++y >= 20)
      {
        y = 0;
      }
    }
    str ++;
  }
}
//
// test colours
const unsigned int color[] =
{
  // 0 ~ 262143, 0x00000 ~ 0x3FFFF
  0xf800, //red
  0x07e0, //green
  0x001f, //blue
  0xffe0, //yellow
  0x0000, //black
  0xffff, //white
  0x07ff, //light blue
  0xf81f  //purple
};

void lcd_display_test(void)
{
  unsigned int temp, num;
  unsigned int i;
  unsigned char n;

  lcd_clear_screen(RED);
  delay(10);
  LCD_WRITE_COMMAND(0x210, 0x00);
  LCD_WRITE_COMMAND(0x212, 0x0000);
  LCD_WRITE_COMMAND(0x211, 0xEF);
  LCD_WRITE_COMMAND(0x213, 0x013F);

  LCD_WRITE_COMMAND(0x200, 0x0000);
  LCD_WRITE_COMMAND(0x201, 0x0000);

  digitalWrite (PA3, LOW);
  LCD_WRITE_REG(0x202); //RAM Write index
  digitalWrite (PA4, LOW);
  //dispaly color bar
  for (n = 0; n < 8; n++)
  {
    digitalWrite (PA4, LOW);
    digitalWrite (PA3, HIGH);
    temp = color[n];
    for (num = 40 * 240; num > 0; num--)
    {
      LCD_WRITE_DATA(temp);
    }
  }
  //display pure color
  for (n = 0; n < 8; n++)
  {
    temp = color[n];
    for (i = 0; i < 240; i++)
    {
      for (num = 0; num < 320; num++)
      {
        LCD_WRITE_DATA(temp);
      }
    }
    // delay(6);
  }
  lcd_clear_screen(BLACK);
  digitalWrite (PA4, HIGH);
  digitalWrite (PA3, HIGH);

}

// test text
char txt[80] = "A quick brown fox ABCDEFG12345";
char txt1[80] = "******************************";
char txt2[80] = "!@#$%^&*()ABCDEFGHIJKLMNOP<>_+";

void testtext () {
  int c;
  //char utmp[80];
  //memset (utmp, 0, 80);
  //ee_writelong2 (0x1000, txt , 80);
  //ee_writelong2 (0x1100, txt , 80);
  //ee_writelong2 (0x1200, txt , 80);
  for (c = 0; c < 18; c++) {
    //ee_readlong2 (0x10000, utmp, 80);
    lcd_display_string(txt, WHITE, color [c / 4 ], 0, c );
  }
  lcd_display_string(txt1, WHITE, BLUE, 0, 18 );
  lcd_display_string(txt2, WHITE, VIOLET, 0, 19 );
}

/* **************************************************
    eeprom library
*/
//opcodes
// byte 1 ,2 ,3, 4, ,5 ,6
#define WREN  0x06  // write enable
#define WRDI  0x04  // write disable
#define RDSR  0x05  // s7-s0 read status register, the Status Register contents will repeat continuously until /CS terminates the instruction.
#define WRSR  0x01  // s7-s0, s15-s8 write status register
#define READ  0x03  // a23-16, a15-8, a7-0, d7-0, read data
#define WRITE 0x02  // a23-16, a15-8, a7-0, d7-0, page program
// winbond w25Q16DV option
#define WRITEVSR 0x50 // write volatile status register
#define RDSR2 0x35  // s7-s0 read status register 2, the Status Register contents will repeat continuously until /CS terminates the instruction.
#define QUADPGM 0x32 // a23-16, a15-8, a7-0, d7-0_io0-3,quad page program
#define SECTERASE 0x20 // a23-16, a15-8, a7-0, sector erase
#define BLKERASE 0x52  // a23-16, a15-8, a7-0, block erase
#define BLKERASE2 0xd8  // a23-16, a15-8, a7-0, block erase
#define CHIPERASE 0x60  // chip erase
#define CHIPERASEX 0xc7 // chip erase
#define ERASEPGMSUS 0x75 // erase, programm suspend
#define ERASEPGMRES 0x7a // erase, program resume
#define PWRDOWN 0xb9     // power down
#define CREAD  0xff // 0xff coutinous read mode reset

#define FASTREAD  0x03  // a23-16, a15-8, a7-0, dummy, d7-0, read data

long address = 0;
void ee_send_address(long addr) {
  SPI.transfer((byte)(addr >> 16));
  SPI.transfer((byte)(addr >> 8));
  SPI.transfer((byte)(addr));
}

void ee_start_write() {
  digitalWrite(PA2, LOW);
  SPI.transfer(WREN); //send WREN command
  digitalWrite(PA2, HIGH);
  digitalWrite(PA2, LOW);
  SPI.transfer(WRITE); //send WRITE command
}

bool ee_isWIP() {
  byte data;
  digitalWrite(PA2, LOW);
  SPI.transfer(RDSR); // send RDSR command
  data = SPI.transfer(0xFF); //get data byte
  digitalWrite(PA2, HIGH);
  return (data & (1 << 0));
}

byte ee_readstatus() {
  byte data;
  digitalWrite(PA2, LOW);
  SPI.transfer(RDSR); // send RDSR command
  data = SPI.transfer(0xFF); //get data byte
  digitalWrite(PA2, HIGH);
  return (data);
}

byte ee_readstatus2() {
  byte data;
  digitalWrite(PA2, LOW);
  SPI.transfer(RDSR2); // send RDSR command
  data = SPI.transfer(0xFF); //get data byte
  digitalWrite(PA2, HIGH);
  return (data);
}

void ee_writestatus(byte lowdata, byte highdata) {
  byte data;
  digitalWrite(PA2, LOW);
  SPI.transfer(WRSR); // send WRSR command
  SPI.transfer(lowdata); //get data byte
  SPI.transfer(highdata);
  digitalWrite(PA2, HIGH);
  while (ee_isWIP()) {
    delay(10);
  }
}

void ee_chiperase() {
  digitalWrite(PA2, LOW);
  SPI.transfer(CHIPERASE); // send CHIPERASE command
  digitalWrite(PA2, HIGH);
  while (ee_isWIP()) {
    delay(10);
  }
}

void ee_chiperasex() {
  digitalWrite(PA2, LOW);
  SPI.transfer(CHIPERASEX); // send CHIPERASE command
  digitalWrite(PA2, HIGH);
  while (ee_isWIP()) {
    delay(10);
  }
}
void ee_write(long addr, byte data) {
  ee_start_write();
  ee_send_address(addr); // send address
  SPI.transfer(data); // transfer data
  digitalWrite(PA2, HIGH);
  while (ee_isWIP()) {
    delay(1);
  }
}

void ee_writelong (long addr, byte data[], int arrLength) {
  ee_start_write();
  ee_send_address(addr); // send address
  for (int i = 0; i < arrLength; i++) {
    SPI.transfer(data[i]); // transfer data
  }
  digitalWrite(PA2, HIGH);
  while (ee_isWIP()) {
    delay(1);
  }
}

void ee_writelong2 (long addr, char data[], int arrLength) {
  ee_start_write();
  ee_send_address(addr); // send address
  for (int i = 0; i < arrLength; i++) {
    SPI.transfer(data[i]); // transfer data
  }
  digitalWrite(PA2, HIGH);
  while (ee_isWIP()) {
    delay(1);
  }
}

byte ee_read_byte(long addr) {
  byte data;
  digitalWrite(PA2, LOW);
  SPI.transfer(READ); // send READ command
  ee_send_address(addr); // send address
  data = SPI.transfer(0xFF); //get data byte
  digitalWrite(PA2, HIGH); //release chip, signal end transfer
  return data;
}

char ee_read_char(long addr) {
  char data;
  digitalWrite(PA2, LOW);
  SPI.transfer(READ); // send READ command
  ee_send_address(addr); // send address
  data = SPI.transfer(0xFF); //get data byte
  digitalWrite(PA2, HIGH); //release chip, signal end transfer
  return data;
}

void ee_readlong (long addr, byte data[], int arrLength) {
  digitalWrite(PA2, LOW);
  SPI.transfer(READ); // send READ command
  for (int i = 0; i < arrLength; i++) {
    data[i] = SPI.transfer(0xFF); //get data byte
  }
  digitalWrite(PA2, HIGH);
}

void ee_readlong2 (long addr, char data[], int arrLength) {
  digitalWrite(PA2, LOW);
  SPI.transfer(READ); // send READ command
  for (int i = 0; i < arrLength; i++) {
    data[i] = SPI.transfer(0xFF); //get data byte
  }
  digitalWrite(PA2, HIGH);
}



/**** debug

*/

void testreadstatus () {
  char tmp[32];
  int c = 0;
  sprintf (tmp, "eeprom status S7-S0  0X%02X:", ee_readstatus());
  lcd_display_string( tmp, WHITE, BLUE, 0, c++ );
  sprintf (tmp, "eeprom status S15-S8 0X%02X:", ee_readstatus2());
  lcd_display_string( tmp, WHITE, BLUE, 0, c++ );
}

void testeefill() {
  //*  test eeprom
  char tmp[32];
  int c = 3;
  int da;
  memset (tmp, 0, 32);
  long adr = 0;
  for (adr = 0; adr < 256; adr ++) {
    da = adr & 0xff;
    ee_write(adr, da);
    sprintf (tmp, "write eeprom addr:%04X <%02X>", adr, da);
    lcd_display_string( tmp, WHITE, BLUE, 0, c );
  }

}

void dumphex () {
  int c, y, o, ad ;
  char utmp [32];
  memset (utmp , 0, 32);
  ad = 0;
  for (y = 0; y < 16; y++) {
    o = 0;
    o += sprintf(utmp , "%04X=", ad & 0xffff);
    for (c = 0; c < 8; c++) {
      o +=  sprintf (utmp + o , "%02X:" , ee_read_byte (ad++));
    }
    lcd_display_string(utmp, WHITE, BLUE, 0, y );
  }
}

/* **************************************************
    startup
*/
void setup() {
  pinMode(ledPin, OUTPUT); // LED 0
  pinMode(ledPin2, OUTPUT); // LED 1
  pinMode (PA4, OUTPUT); // LCD #CS
  pinMode (PA3, OUTPUT); // LCD RS= command
  pinMode (PC13, OUTPUT); // LCD #RESET
  digitalWrite (PA4, HIGH);
  digitalWrite (PA3, HIGH);
  digitalWrite (PC13, LOW);
  // eeprom #cs
  pinMode (PA2, OUTPUT);
  digitalWrite (PA2, HIGH);
  // LED
  digitalWrite (ledPin, HIGH);
  digitalWrite (ledPin2, HIGH);
  // serial init
  SerialUSB.end();
  SerialUSB.begin(115200);
  delay (500);
  digitalWrite (PC13, HIGH);
  // spi init
  SPI.begin();
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  //SPI.setClockDivider (2);
  //SPI.endTransaction();
  // init eeprom
  lcd_init();
  lcd_clear_screen(BLUE);
  SerialUSB.println ("*****lcd test*********");
  delay (1000);
  ee_chiperase ();
  ee_writestatus (0x00, 0x00);
  testreadstatus();
  testeefill();
  dumphex ();
  delay (10000);
}

uint count = 0;
void loop() {
  lcd_init();
  delay (10);
  testtext();
  delay (10000);
  lcd_display_test();
  delay (1000);
  digitalWrite(ledPin, (count + 1) % 2 );
  digitalWrite(ledPin2, (count ++) % 2 );

}
