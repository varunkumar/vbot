#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>

AndroidAccessory acc("Google, Inc.",
		     "vbot",
		     "vbot",
		     "1.0",
		     "http://www.android.com",
		     "0000000012345678");
boolean isConnected = false;

void setup() {
  Serial.begin(115200);
  Serial.print("\r\nStart");
  acc.powerOn();
}

int ct = 0;
void loop() {
  if (ct % 2 == 0) {
     
  }
  delay(1000); 
}
