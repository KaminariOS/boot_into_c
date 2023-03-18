#define COM1    0x3f8
#define VGA_ADDR 0xb8000
/*const static int BUFFER_HEIGHT = 25;*/
/*const static int BUFFER_WIDTH = 80;*/

const char Yellow = 14;
const char Black = 0;

struct ScreenChar {
    char ascii;
    char colorCode;
};

/*struct Writer {*/
    /*int columnPos;*/
    /*int rowPos;*/
    /*volatile struct ScreenChar** buffer;*/
/*};*/

/*struct Writer writer;*/


/*void newLine() {*/

/*}*/

/*void writeByte(char ch, char color) {*/
    /*if (ch == '\n') {*/
        /*newLine();*/
        /*return;*/
    /*}*/
    /*int row = BUFFER_HEIGHT - 1;*/
    /*int col = writer.columnPos;*/
    /*struct ScreenChar cha = {.ascii = ch, .colorCode = color};*/
    /*writer.buffer[row][col] = cha;*/
    /*writer.columnPos += 1;*/
/*}*/



struct ScreenChar* buffer =  (struct ScreenChar*) VGA_ADDR;

char getColorCode(char foreground, char background) {
    return (background << 4) | (foreground);
}

static int uart;    // is there a uart?

void microdelay(unsigned long us) {

}

static inline unsigned char inb(unsigned short port)
{
    unsigned char data;

    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

static inline void outb(unsigned short port, unsigned char data)
{
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

void uartinit(void)
{

  // Turn off the FIFO
  outb(COM1+2, 0);

  // 9600 baud, 8 data bits, 1 stop bit, parity off.
  outb(COM1+3, 0x80);    // Unlock divisor
  outb(COM1+0, 115200/115200);
  outb(COM1+1, 0);
  outb(COM1+3, 0x03);    // Lock divisor, 8 data bits.
  outb(COM1+4, 0);
  outb(COM1+1, 0x01);    // Enable receive interrupts.

  // If status is 0xFF, no serial port.
  if(inb(COM1+5) == 0xFF)
      return;

  uart = 1;

  // Acknowledge pre-existing interrupt conditions;
  // enable interrupts.
  inb(COM1+2);
  inb(COM1+0);
}

void
uartputc(int c)
{
  int i;

  if(!uart)
      return;
  
  for(i = 0; i < 128 && !(inb(COM1+5) & 0x20); i++)
      microdelay(10);

  outb(COM1+0, c);
}


// Print to the console
void printk(char *str)
{
    int i, c;

    for(i = 0; (c = str[i]) != 0; i++){
        uartputc(c);
        struct ScreenChar ch = {.ascii = c, .colorCode = getColorCode(Yellow, Black)};
        *(buffer + i) = ch;
    }
}


