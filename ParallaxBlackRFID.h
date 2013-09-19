/* ParallaxBlackRFID - https://mainfram3.ca/software/ */
/*                                                    */
/* written by Peter Rogers                            */
/* version 1.0                                        */
/* license LGPL 2.1                                   */
/* http://github.com/mainfram3ca/ParallaxBlackRFID    */

#ifndef __PARALLAX_BLACK_RFID__
#define __PARALLAL_BLACK_RFID__

#include "Arduino.h"
#include "HardwareSerial.h"

// Addresses of noteworthy memory locations on the RFID tags. Each memory location contains 4 bytes of data.
#define ADDR_PASSWD          0
#define ADDR_PROTECT         1
#define ADDR_CONTROL         2
#define ADDR_USER            3
#define ADDR_SERIAL          32
#define ADDR_DEVICEID        33

// Errors defined by the RFID module API
#define RFID_OK              1
#define RFID_ERR_NO_TAG      2
#define RFID_ERR_NAK         3
#define RFID_ERR_NAK_OLDPW   4
#define RFID_ERR_NAK_NEWPW   5
#define RFID_ERR_LIW_NEWPW   6
#define RFID_ERR_PARITY      7

// Additional error codes returned by the scan functions below
#define RFID_ERR_TRUNC_DATA  (-1)
#define RFID_ERR_FRAME       (-2)

// Struct for holding an RFID tag ID 
typedef struct {
  byte buf[4];
} rfid_tag_t;

// Struct for holding a legacy tag ID
typedef struct {
  byte buf[10];
} rfid_legacy_t;

class ParallaxBlackRFID
{
  public:
    Stream *serial;

    void begin(Stream *ser);

    /* Scan a tag from the reader, and fills the ID into the given tag. 
     * This function returns RFID_OK on success, otherwise one of the
     * RFID_ERR_* codes from above */
    int scanTag(rfid_tag_t*);
    
    /* Same as "scanTag" except this function works on legacy cards */
    int scanLegacyTag(rfid_legacy_t*);

};


#endif

