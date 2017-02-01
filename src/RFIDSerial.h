/*
 The module is RDM630 and has two data formats. UART and Weigand. This library only works with UART data format.
 The frequency is 125Khz and compatible with EM4100 TK4100 EM/TK4102, EM4200

DATA FORMAT:
14bytes
STX(0x02) DATA(10ASCii) LRC(2ASCii) ETX(0x03)
*/

#ifndef RFIDSerial_H
#define RFIDSerial_H

#include "Arduino.h"


const int _startTag = 2;  //Value corresponding the init of the transmission
const int _endTag = 3;    // value corresponding the end of the transmission
const static int _tagLength = 12;  // number of ascii characters of the tag
const int _totalLength = _tagLength + 2; // total length of transmission.


/*
* This class represents the TAG itself.
* methods:
* - getTag() -- return TAG in ASCii format
* - getCardNumber() -- return the number of the card, according with the protocol.
* - isValid() -- check if the tag read is valid or not.
*/

class RFIDtag{
  private:
    boolean _isValid;
    long _cardNumber;
    char _tag[_tagLength + 1];
    boolean validateTag();
  public:
    RFIDtag();
    char * getTag();
    boolean isValid();
    long getCardNumber();
    boolean setTag(char tag[]);
};

/*
* This class represents the reader module
* METHODS:
*  - isAvailable() checks if any TAG was read.
*  - getTag() -- returns a pointer to a RFIDtag object.
*/

class READER{
private:
    //SoftwareSerial * _rfidSerial;
    boolean _isAvailable;  
    RFIDtag * _RFIDtag;
    
public:
    READER();
    bool isAvailable();
    bool tagFound(String);
    RFIDtag * getTag();   
    
};

#endif
