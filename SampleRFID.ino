/* ParallaxBlackRFID - https://mainfram3.ca/software/ */
/*                                                    */
/* written by Peter Rogers                            */
/* version 1.0                                        */
/* license LGPL 2.1                                   */
/* http://github.com/mainfram3ca/ParallaxBlackRFID    */



/* SampleRFID */
/* Read RFID tags and print the IDs to the serial port. Designed to run on an Arduino Mega (code uses
 * two serial ports) and a Parallax RFID read/write module (black coloured PCB). There some sample
 * code here:
 * 
 * http://playground.arduino.cc/Learning/ParallaxRFIDreadwritemodule
 * 
 * You can find more info and a programming guide on the parallax website:
 *
 * http://www.parallax.com/Store/Accessories/CommunicationRF/tabid/161/ProductID/688/List/0/Default.aspx?SortField=ProductName,ProductName
 *
 */
#include "ParallaxBlackRFID.h"

ParallaxBlackRFID reader;

void setup()
{
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial.println("Reading RFID tags...");

  reader.begin(&Serial1);
}

void loop()
{
  char output[64];
  rfid_tag_t id;
  int err = reader.scanTag(&id);

  /* Attempt to read in an RFID tag */
  if (err == RFID_OK) {
    /* Found the tag */
    sprintf(output, "New tag: %02x %02x %02x %02x\n", id.buf[0], id.buf[1], id.buf[2], id.buf[3]);
    Serial.print(output);
  } else if (err != RFID_ERR_NO_TAG) {
    Serial.print("error reading new tag: ");
    Serial.println(err);
  }
  
  /* Attempt to read a legacy tag */
  rfid_legacy_t id2;
  err = reader.scanLegacyTag(&id2);
  if (err == RFID_OK) {
    /* Got the data - print it */
    Serial.print("Legacy: DATA=");
    for (int n = 0; n < 10; n++) {
      sprintf(output, "%02x", id2.buf[n]);
      Serial.print(output);
    }
    Serial.println("");
  } else if (err != RFID_ERR_NO_TAG) {
    Serial.print("error reading legacy tag: ");
    Serial.println(err);
  }

  /* Consume any stray characters coming from the reader. This should never happen,
   * but maybe if we get out of sync with the reader this will help gets us back
   * again. */
  while(Serial1.available()) {
    Serial.println("clearing extra data");
    Serial1.read();
    delay(10);
  }
}

