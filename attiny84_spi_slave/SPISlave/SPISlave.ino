#define SS    8
#define SCLK    4
#define MOSI    6
#define MISO    5

// SPI mode 0 or 1 - modes 2 and 3 are NOT supported
#define SPI_MODE 0

#define PIN_LED 0
#define CMD_LEDON '0'
#define CMD_LEDOFF '1'

volatile byte cmd;
volatile boolean cmdAvailable;

void setup() {
  // initialize the direction of PORTD #5 to be an output
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  cmdAvailable = false;

  // DO pin is configured for output
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCLK, INPUT);
  pinMode(SS, INPUT);

  digitalWrite(MOSI, HIGH);
  digitalWrite(SCLK, HIGH);
  // enable USI overflow interrupt, set three wire mode and set
  //  clock to external, positive edge
  USICR = 0;
  USICR = (1 << USIOIE) | (1 << USIWM0) | (1 << USICS1) | (SPI_MODE << USICS0);

  USISR = (1 << USIOIF); // clear overflow flag

  USIDR = 0;
}

volatile byte counter;

void loop() {
  if (cmdAvailable == true) {

    if (cmd == CMD_LEDOFF) {
      digitalWrite(PIN_LED, LOW);
    } else if (cmd == CMD_LEDON) {
      digitalWrite(PIN_LED, HIGH);
    }

    cmdAvailable = false;
  }

}

volatile union SendBuffer {
  int i;
  long l;
  float f;
  byte b[4];
} sendBuffer;
volatile byte sendBufferI = 0;

const int testI = 12345;
const long testL = 12345000L;
const float testF = 123.45f;

#define SPI_SEND() USIDR = sendBuffer.b[sendBufferI++]

#define SPI_SEND_INT(x) \
  sendBuffer.i = x; \
  sendBufferI = 0;  \
  SPI_SEND()

#define SPI_SEND_LONG(x) \
  sendBuffer.l = x; \
  sendBufferI = 0;  \
  SPI_SEND()

#define SPI_SEND_FLOAT(x) \
  sendBuffer.f = x; \
  sendBufferI = 0;  \
  SPI_SEND()

ISR(USI_OVF_vect) {
  cmd = USIDR;
  USISR = (1 << USIOIF); // clear overflow flag

  if (cmd == 'c') {
    USIDR = 123;
  } else if (cmd == 'i') {
    SPI_SEND_INT(testI);
  } else if (cmd == 'f') {
    SPI_SEND_FLOAT(testF);
  } else if (cmd == 'l') {
    SPI_SEND_LONG(testL);
  } else if (cmd == 0) {
    SPI_SEND();
  } else {
    USIDR = counter++;
    cmdAvailable  = true;
  }
}
