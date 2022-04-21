#include <Arduino.h>

/*
 * Test to establish the timeout period of the ESP8266 watchdog timers 
 */

#define SOFT_WATCHDOG     // comment out to test hardware watchdog time-out

#ifdef SOFT_WATCHDOG
  #define LOOPS 15
  #define STEP 10
  unsigned long interval = 3150;
#else
  #define LOOPS 10
  #define STEP 100
  unsigned long interval = 7900;
#endif

unsigned long etime;

void setup() {
  Serial.begin(115200);
  Serial.printf("\n\nSdk version: %s\n", ESP.getSdkVersion());
  Serial.printf("Core Version: %s\n", ESP.getCoreVersion().c_str());
  Serial.printf("Boot Version: %u\n", ESP.getBootVersion());
  Serial.printf("Boot Mode: %u\n", ESP.getBootMode());
  Serial.printf("CPU Frequency: %u MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Reset reason: %s\n", ESP.getResetReason().c_str());
  Serial.println("---------------------------");
  Serial.println("-- WATCHDOG TIMING TESTS --");
  Serial.println("---------------------------\n");

  while (1) {
  
    interval += STEP;
    
    Serial.print("  Testing ");
    #ifdef SOFT_WATCHDOG
      Serial.print("software");
    #else
      Serial.print("hardware");
    #endif  
    Serial.printf(" watchdog with interval of %u ms - ", interval);
    Serial.flush();

    ESP.wdtDisable();
    #ifdef SOFT_WATCHDOG
      ESP.wdtEnable(0);
    #endif  
      
    for (int i = 0; i < LOOPS; i++) {
  
      ESP.wdtFeed();
            
      ESP.wdtDisable();
      #ifdef SOFT_WATCHDOG
        ESP.wdtEnable(0);
      #endif  
        
      etime = millis();
      while (millis() - etime < interval) { }

      ESP.wdtDisable();
      ESP.wdtEnable(0);
      ESP.wdtFeed();
      Serial.printf("%d ", i+1);
    }  
    Serial.println("done.");
  }
}

void loop() { }