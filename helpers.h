//Helpers
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
#define D8_TO_D1_PATTERN "D8:%c D7:%c D6:%c D5:%c D4:%c D3:%c D2:%c D1:%c"
#define D8_TO_D1_PATTERN_NO_SPC "D8:%cD7:%cD6:%cD5:%cD4:%cD3:%cD2:%cD1:%c"
#define D8_TO_D1_PATTERN_PAIRS "&D8=%c&D7=%c&D6=%c&D5=%c&D4=%c&D3=%c&D2=%c&D1=%c"
#define D0_PATTERN " D0:%c"
#define D0_PATTERN_NO_SPC "D0:%c"
#define D0_PATTERN_PAIRS "&D0=%c"
#define D0(byte) \
  (byte & 0x01 ? '1' : '0')

#define D8_TO_D1(byte)\
  (byte & 0x8000 ? '1' : '0'), \
  (byte & 0x2000 ? '1' : '0'), \
  (byte & 0x1000 ? '1' : '0'),\
  (byte & 0x4000 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0')

#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80000000 ? '1' : '0'), \
  (byte & 0x40000000 ? '1' : '0'), \
  (byte & 0x20000000 ? '1' : '0'), \
  (byte & 0x10000000 ? '1' : '0'), \
  (byte & 0x08000000 ? '1' : '0'), \
  (byte & 0x04000000 ? '1' : '0'), \
  (byte & 0x02000000 ? '1' : '0'), \
  (byte & 0x01000000 ? '1' : '0'),\
  (byte & 0x800000 ? '1' : '0'), \
  (byte & 0x400000 ? '1' : '0'), \
  (byte & 0x200000 ? '1' : '0'), \
  (byte & 0x100000 ? '1' : '0'), \
  (byte & 0x080000 ? '1' : '0'), \
  (byte & 0x040000 ? '1' : '0'), \
  (byte & 0x020000 ? '1' : '0'), \
  (byte & 0x10000 ? '1' : '0'),\
  (byte & 0x8000 ? '1' : '0'), \
  (byte & 0x4000 ? '1' : '0'), \
  (byte & 0x2000 ? '1' : '0'), \
  (byte & 0x1000 ? '1' : '0'), \
  (byte & 0x0800 ? '1' : '0'), \
  (byte & 0x0400 ? '1' : '0'), \
  (byte & 0x0200 ? '1' : '0'), \
  (byte & 0x0100 ? '1' : '0'),\
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

#define GPIO_OUT ((volatile uint32_t*) 0x60000300) // WR holds the output of all the pins at once.
#define GPIO_OUT_SET ((volatile uint32_t*) 0x60000304) //that only sets a pin to high
#define GPIO_OUT_CLEAR ((volatile uint32_t*) 0x60000308) //that only sets a pin to low
#define GPIO_DIR ((volatile uint32_t*) 0x6000030C) //Any bit that is set will make that pin an output.
#define GPIO_DIR_OUTPUT ((volatile uint32_t*) 0x60000310) //writing 1's makes those pins outputs
#define GPIO_DIR_INPUT ((volatile uint32_t*) 0x60000314) //writing 1's makes those pins inputs
#define GPIO_IN ((volatile uint32_t*) 0x60000318) //R captures the state of the pins
//#define GPIO_16 ((volatile uint32_t*) 0x6000078C)
#define GPIO_16 ((volatile uint32_t*) RTC_GPIO_IN_DATA)
//  Yippy bit 0 of this is GPIO16 = #define PERIPHS_RTC_BASEADDR   0x60000700 + 8C see eagle_soc.h
// in C:\data\arduino_1.8.5_wizard\portable\packages\esp8266\hardware\esp8266\2.4.0-rc1\tools\sdk\include
//#define RTC_GPIO_IN_DATA                        (REG_RTC_BASE + 0x08C)
#define GPIO_0 ((volatile uint32_t*) 0x60000328)
#define GPIO_2 ((volatile uint32_t*) 0x60000330)
#define PIN_13 ((volatile uint32_t*) 0x6000035C)



int getPinMode(int pin) //Return 0 for Input and 1 Output
{
  int  bit = digitalPinToBitMask(pin);

  if (pin == D0)  //GPIO 16
  {
    //port = digitalPinToPort(HEATER);
    volatile uint32_t *ddr = ((volatile uint32_t*) &GP16E);
    return ((*ddr));
  }
  else
  {
    volatile uint32_t *ddr = ((volatile uint32_t*) &GPE);
    return ((bit & *ddr));

  }
}


long runningAverage(int M) {
#define LM_SIZE 10
  static int LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static long sum = 0;
  static byte count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  return sum / count;
}
