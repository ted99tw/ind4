#include "RFIDSerial.h"
#include "Arduino.h"
#include <stdio.h>

unsigned char charToHexDigit(char c)
{
  if (c >= 'A')
    return c - 'A' + 10;
  else
    return c - '0';
}

unsigned char stringToByte(char c[2])
{
  return charToHexDigit(c[0]) * 16 + charToHexDigit(c[1]);
}

READER::READER() {
    _isAvailable = false;
    _RFIDtag = new RFIDtag();
}

bool READER::isAvailable() {

    int bytesRead;
    char buff[_tagLength + 1];
    
    _isAvailable = false;
    
    if (Serial.available() >= _totalLength) {
        if (Serial.read() == _startTag) {
            bytesRead = 0;
            while (bytesRead < _tagLength) {
                int val = Serial.read();
		            //Serial.println(val);
                if ((val == _startTag) || (val == _endTag)) {
                    break;
                }
                buff[bytesRead] = val;
                bytesRead += 1;
            }

            if (Serial.read() == _endTag) {
                buff[bytesRead] = 0;
                
                _isAvailable  = true;
               _RFIDtag->setTag(buff);
            }
        }
    }
    return _isAvailable;
}

bool READER::tagFound(String tagNo){
  if (isAvailable()){
    long ltag = tagNo.toInt();
    Serial.println(_RFIDtag->getCardNumber());
    return ltag == _RFIDtag->getCardNumber();
  }
  return false;
}

RFIDtag * READER::getTag() {
    return _RFIDtag;
}

/*
 * Classe RFIDtag 
 *
 */

RFIDtag::RFIDtag(){

  *_tag = NULL;
  _isValid = true;
  _cardNumber = 0;
}



char * RFIDtag::getTag(){
  
  return _isValid? _tag:NULL;
  
}
boolean RFIDtag::validateTag(){

char checksum[2] = {'0', '0'};
char checksumValue = '\0';
char piece[2] = {'0', '0'};
char hexValue = '\0';

 strncpy(checksum,_tag+10,2);
 checksumValue = stringToByte(checksum);

 for (int i = 0; i < _tagLength-2; i+=2){
   strncpy(piece,_tag+i,2);
   hexValue ^= stringToByte(piece);   
   
   if ( i > 0 and  i < (_tagLength-2)){
    _cardNumber = _cardNumber << 8;
    _cardNumber += stringToByte(piece); 
    
   }
 }

 return _isValid = (checksumValue == hexValue);
 

}

boolean RFIDtag::isValid(){
  return _isValid;
}

boolean RFIDtag::setTag(char tag[]){
  strcpy(_tag,tag);
  validateTag();

  if(_isValid){
    return true;
  }
  else{
    *_tag  = NULL;
    _cardNumber = 0;
    return false;
  }
}

long RFIDtag::getCardNumber(){

  return _isValid? _cardNumber:NULL;
  
}

 
