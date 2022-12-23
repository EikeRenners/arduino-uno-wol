#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet3.h>
#include <EthernetUdp3.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <utility/socket.h>


// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;


int UDP_RawSendto(byte* pDataPacket, int nPacketLength, int nLocalPort, byte* pRemoteIP, int nRemotePort)
{
  int nResult;
  int nSocketId; // Socket ID for Wiz5100
 
  // Find a free socket id.
  nSocketId = MAX_SOCK_NUM;
  for (int i = 0; i < MAX_SOCK_NUM; i++) 
  {
    uint8_t s = w5500.readSnSR(i);
    if (s == SnSR::CLOSED || s == SnSR::FIN_WAIT) 
	  {
      nSocketId = i;
      break;
    }
  }
 
  if (nSocketId == MAX_SOCK_NUM)
    return 0; // couldn't find one. 
 
  if (socket(nSocketId, SnMR::UDP, nLocalPort, 0))
  {
    nResult = sendto(nSocketId,(unsigned char*)pDataPacket,nPacketLength,(unsigned char*)pRemoteIP,nRemotePort);
    close(nSocketId);
  } else
    nResult = 0;
 
  return nResult;
}


 
void SendWOLMagicPacket(byte * pMacAddress)
{
  // The magic packet data sent to wake the remote machine. Target machine's
  // MAC address will be composited in here.
  const int nMagicPacketLength = 102;
  byte abyMagicPacket[nMagicPacketLength] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  byte abyTargetIPAddress[] = { 255, 255, 255, 255 }; // don't seem to need a real ip address.
  const int nWOLPort = 7;
  const int nLocalPort = 8888; // to "listen" on (only needed to initialize udp)
 
  
  // Compose magic packet to wake remote machine. 
  for (int ix=6; ix<102; ix++)
    abyMagicPacket[ix]=pMacAddress[ix%6];
  
  if (UDP_RawSendto(abyMagicPacket, nMagicPacketLength, nLocalPort, 
  abyTargetIPAddress, nWOLPort) != nMagicPacketLength)
    Serial.println("Error sending WOL packet");
}
 


static byte g_abyMyMacAddress[] = {0x00,0x1A,0x4B,0x38,0x0C,0x5C};
static IPAddress g_MyIPAddress(192,168,178,250);
 
// The machine to wake up. WOL should already be configured for the target machine. 
// The free windows program "Wake On LAN Ex 2" by Joseph Cox can be useful for testing the remote
// machine is properly configured. Download it here: http://software.bootblock.co.uk/?id=wakeonlanex2
// d4:93:90:12:c8:56
static byte g_TargetMacAddress[] = {0xd4,0x93,0x90,0x12,0xc8,0x56};
static byte g_maxSend = 5;
 
void setup () 
{
  Ethernet.setCsPin(6);
  Ethernet.begin(g_abyMyMacAddress, g_MyIPAddress);
 
  Serial.begin(9600);
  Serial.println("Wake on Lan (WOL) Demo");
}
 
void loop() 
{
  byte cnt = 0; 
  while(true){
    if( cnt < g_maxSend ) {
      Serial.println("Sending WOL package"); 
      SendWOLMagicPacket(g_TargetMacAddress);
      Serial.println("Magic packet sent");
      ++cnt; 
    }
    delay(10000); 
  }
}
