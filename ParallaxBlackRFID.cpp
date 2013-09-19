/* ParallaxBlackRFID - https://mainfram3.ca/software/ */
/*                                                    */
/* written by Peter Rogers                            */
/* version 1.0                                        */
/* license LGPL 2.1                                   */
/* http://github.com/mainfram3ca/ParallaxBlackRFID    */


/* ParallaxBlackRFID.h */
#include "ParallaxBlackRFID.h"

#define RFID_READ            0x01
#define RFID_WRITE           0x02
#define RFID_LOGIN           0x03
#define RFID_SETPASS         0x04
#define RFID_PROTECT         0x05
#define RFID_RESET           0x06
#define RFID_READ_LEGACY     0x0f

#define READ_RESPONSE_LEN    5
#define LEGACY_RESPONSE_LEN  12

void ParallaxBlackRFID::begin(Stream *ser)
{
  serial = ser;
  ser->setTimeout(5000);
}

int ParallaxBlackRFID::scanTag(rfid_tag_t *id)
{
  /* Send the command */
  serial->print("!RW");
  serial->write(RFID_READ);
  serial->write(ADDR_SERIAL);
  
  /* Read the response */
  byte buf[READ_RESPONSE_LEN];
  int len = serial->readBytes((char*)buf, READ_RESPONSE_LEN);

  /* Make sure the response is the right length */
  if (len != READ_RESPONSE_LEN) {
    return RFID_ERR_TRUNC_DATA;
  }
  /* Stuff the buffer */
  id->buf[0] = buf[1];
  id->buf[1] = buf[2];
  id->buf[2] = buf[3];
  id->buf[3] = buf[4];
  return buf[0];
}

int ParallaxBlackRFID::scanLegacyTag(rfid_legacy_t *id)
{
  /* Send the command */
  serial->print("!RW");
  serial->write(RFID_READ_LEGACY);

  /* Read the response */
  byte buf[LEGACY_RESPONSE_LEN];
  int len = serial->readBytes((char*)buf, LEGACY_RESPONSE_LEN);

  /* Make sure we have enough bytes */
  if (len != LEGACY_RESPONSE_LEN) {
    return RFID_ERR_TRUNC_DATA;
  }
  
  /* Check the response framing */
  if (buf[0] == 0xFF && buf[LEGACY_RESPONSE_LEN-1] == 0xFF) {
    return RFID_ERR_NO_TAG;
  }

  /* Make sure the framing is what we expect */  
  if (buf[0] != 0xA || buf[LEGACY_RESPONSE_LEN-1] != 0xD) {
    return RFID_ERR_FRAME;
  }
  
  /* Copy into the output buffer */
  for (int n = 0; n < LEGACY_RESPONSE_LEN-2; n++) {
    id->buf[n] = buf[n+1];
  }
  return RFID_OK;
}

