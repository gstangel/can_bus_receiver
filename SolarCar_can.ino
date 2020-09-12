#include <mcp_can.h>
#include <mcp_can_dfs.h>
#define recID 0x55
#define canPin 3
#define ledOnePin 3
#define ledTwoPin 4
#define ledThreePin 5

char *drivers[] = {"Gabe", "Jakob", "Savannah", "Alex", "Bailey", "Dean", "James", "Josh"};
char *gears[] = {"F","N","R","0"};

const unsigned long checkVal = 0x55;
const unsigned long recId = 0x88;

#define SPI_CS_PIN 10
MCP_CAN CAN(SPI_CS_PIN);

void canRead(){
  int id=0;
  unsigned char len = 0;
  unsigned char buf[3];
  while (CAN_MSGAVAIL == CAN.checkReceive()){
    CAN.readMsgBuf(&len, buf);
    id=CAN.getCanId();
    char checkVal = (buf[3]);
    if (id == recID && buf[3] == checkVal){
        boolean buttonOne = (buf[0] & 0b10000000) >> 7;
        boolean buttonTwo = (buf[0] & 0b0100000) >> 6;
        boolean buttonThree = (buf[0] & 0b0010000) >> 5;
        int driver = (buf[0] & 0b00011100) >> 2;
        int gear = (buf[0] & 0b00000011);
        int airTemp = (buf[1]);
        char ASCII = (buf[2]);
        
        if(buttonOne)
        {
          digitalWrite(ledOnePin, HIGH);
        }
        if(buttonTwo)
        {
          digitalWrite(ledTwoPin, HIGH);
        }
        if(buttonThree)
        {
          digitalWrite(ledThreePin, HIGH);
        }
        Serial.print("Driver: " + String(drivers[driver]));
        Serial.print("Gear: " + String(gears[gear]));
        Serial.print("Air Temp" + String(airTemp));
        Serial.print("ASSCI" + String(ASCII));
    }
    }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledOnePin, OUTPUT);
  pinMode(ledTwoPin,OUTPUT);
  pinMode(ledThreePin, OUTPUT);
  pinMode(canPin, INPUT_PULLUP);
  while(CAN_OK != CAN.begin(CAN_500KBPS)){
    delay(100);
  }
  CAN.init_Mask(0,0,0x7ff);
  CAN.init_Filt(0,0,0x88);
}

void loop() {
  canRead();
}
