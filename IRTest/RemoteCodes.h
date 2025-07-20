/*
 * RemoteCodes.h
 * Remote control codes for the Car MP3 remote control.
 */

const unsigned long IRR_CHANNEL_DOWN = 0x00FFA25D;
const unsigned long IRR_CHANNEL = 0x00FF629D;
const unsigned long IRR_CHANNEL_UP = 0x00FFE21D;
const unsigned long IRR_BACK = 0x00FF22DD;
const unsigned long IRR_FORWARD = 0x00FF02FD;
const unsigned long IRR_PLAY = 0x00FFC23D;
const unsigned long IRR_MINUS = 0x00FFE01F;
const unsigned long IRR_PLUS = 0x00FFA857;
const unsigned long IRR_EQ = 0x00FF906F;
const unsigned long IRR_0 = 0x00FF6897;
const unsigned long IRR_PLUS_100 = 0x00FF9867;
const unsigned long IRR_PLUS_200 = 0x00FFB04F;
const unsigned long IRR_1 = 0x00FF30CF;
const unsigned long IRR_2 = 0x00FF18E7;
const unsigned long IRR_3 = 0x00FF7A85;
const unsigned long IRR_4 = 0x00FF10EF;
const unsigned long IRR_5 = 0x00FF38C7;
const unsigned long IRR_6 = 0x00FF5AA5;
const unsigned long IRR_7 = 0x00FF42BD;
const unsigned long IRR_8 = 0x00FF4AB5;
const unsigned long IRR_9 = 0x00FF52AD;

const unsigned long REMOTE_CODES[] = {
  IRR_CHANNEL_DOWN, IRR_CHANNEL, IRR_CHANNEL_UP, 
  IRR_BACK, IRR_FORWARD, IRR_PLAY, 
  IRR_MINUS, IRR_PLUS, IRR_EQ, 
  IRR_0, IRR_PLUS_100, IRR_PLUS_200, 
  IRR_1, IRR_2, IRR_3, 
  IRR_4, IRR_5, IRR_6, 
  IRR_7, IRR_8, IRR_9
};

const char *REMOTE_KEY_NAMES[] = {
  "CH-", "CH", "CH+",
  "|<<", ">>|", ">||",
  "-", "+", "EQ",
  "0", "100+", "200+",
  "1", "2", "3",
  "4", "5", "6",
  "7", "8", "9"
};

int numCodes = sizeof(REMOTE_CODES) / sizeof(REMOTE_CODES[0]);


const char *remoteKeyFromCode(unsigned long code)
{
  int index = 0;
  while (index < numCodes) {
    if (code == REMOTE_CODES[index]) {
      break;
    }
    ++index;
  }

  return (index < numCodes ? REMOTE_KEY_NAMES[index] : "");
}
