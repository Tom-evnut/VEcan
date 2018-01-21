
#include <mcp_can.h>
#include <SPI.h>

//variables for VE can
uint16_t chargevoltage = 49100; //max charge voltage in mv
uint16_t chargecurrent = 30000; //max charge current in ma
uint16_t disvoltage = 42000; // max discharge voltage in mv
uint16_t discurrent = 30000; // max discharge current in ma
uint16_t SOH = 100; // SOH place holder

unsigned char mes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char bmsname[8] = {'S', 'I', 'M', 'P', '-', 'B', 'M', 'S'};
unsigned char bmsmanu[8] = {'T', 'O', 'M', ' ', 'D', 'E', ' ', 'B'};

MCP_CAN CAN(9); //set CS pin for can controlelr

int SOC =80;

float PackVoltage = 46.7;
float AvgTemperature = 20.5;
uint16_t currentact = 0;

void setup() {

  Serial.begin(115200);
  
  // put your setup code here, to run once:
if(CAN.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN.setMode(MCP_NORMAL); 
}

void loop() {
  // put your main code here, to run repeatedly:
  VEcan();
  delay(200);
}


void VEcan() //communication with Victron system over CAN
{
  mes[0] = lowByte(chargevoltage / 100);
  mes[1] = highByte(chargevoltage / 100);
  mes[2] = lowByte(chargecurrent / 100);
  mes[3] = highByte(chargecurrent / 100);
  mes[4] = lowByte(discurrent / 100);
  mes[5] = highByte(discurrent / 100);
  mes[6] = lowByte(disvoltage / 100);
  mes[7] = highByte(disvoltage / 100);

  CAN.sendMsgBuf(0x351, 0, 8, mes);
Serial.println(" ");
for (int i = 0; i<8; i++)
{
  Serial.print(mes[i]);
  Serial.print(" , ");
}

  mes[0] = lowByte(SOC);
  mes[1] = highByte(SOC);
  mes[2] = lowByte(SOH);
  mes[3] = highByte(SOH);
  mes[4] = lowByte(SOC * 10);
  mes[5] = highByte(SOC * 10);
  mes[6] = 0;
  mes[7] = 0;

  CAN.sendMsgBuf(0x355, 0, 8, mes);
Serial.println(" ");
for (int i = 0; i<8; i++)
{
  Serial.print(mes[i]);
  Serial.print(" , ");
}

  mes[0] = lowByte(uint16_t(PackVoltage * 100));
  mes[1] = highByte(uint16_t(PackVoltage * 100));
  mes[2] = lowByte(uint16_t(currentact / 100));
  mes[3] = highByte(uint16_t(currentact / 100));
  mes[4] = lowByte(uint16_t(AvgTemperature * 10));
  mes[5] = highByte(uint16_t(AvgTemperature * 10));

  CAN.sendMsgBuf(0x356, 0, 8, mes);
Serial.println(" ");
for (int i = 0; i<8; i++)
{
  Serial.print(mes[i]);
  Serial.print(" , ");
}

  mes[0] = 0;
  mes[1] = 0;
  mes[2] = 0;
  mes[3] = 0;
  mes[4] = 0;
  mes[5] = 0;
  mes[6] = 0;
  mes[7] = 0;

  CAN.sendMsgBuf(0x35A, 0, 8, mes);

Serial.println(" ");
for (int i = 0; i<8; i++)
{
  Serial.print(mes[i]);
  Serial.print(" , ");
}

delay(5);
  CAN.sendMsgBuf(0x370, 0, 8, bmsname);

  Serial.println(" ");
for (int i = 0; i<8; i++)
{
  Serial.print(bmsname[i]);
  Serial.print(" , ");
}
delay(5);
  CAN.sendMsgBuf(0x35E, 0, 8, bmsmanu); 
Serial.println(" ");
for (int i = 0; i<8; i++)
{
  Serial.print(bmsmanu[i]);
  Serial.print(" , ");
}

}
