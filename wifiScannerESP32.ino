#include <WiFi.h>

//測位に使うときにWANに出るためのポケファイやテザリングのAPを除外
byte denyAPs[][6] = {{0x0A,0x0B,0x0C,0x0D,0x0E,0x0F},{0x12,0x34,0x56,0x78,0x90,0xAB}};


void setup() 
{
  Serial.begin(115200);
  Serial.println("Hello");
  WiFi.mode(WIFI_AP_STA);
}

String scanAndMakeJSON()
{
  int lenSTAs = WiFi.scanNetworks();
  int staLabel[3];
  
  if(lenSTAs >= 3)
  {
    int cntAP = 0;
    int id = 0;
    while(cntAP < 3)
    {
      for(int i = 0; i < sizeof(denyAPs) / sizeof(denyAPs[0]); i ++)
      {
        while(memcmp(denyAPs[i], WiFi.BSSID(id), 6) == 0)
        {
          id ++;            
          if(id == (lenSTAs - 1) && cntAP != 3)
          {
            return "NULL";
          }
        }
      }
      staLabel[cntAP] = id;
      id ++;
      cntAP++;
    }

    if(cntAP == 3)
    {
      for(int i = 0; i < 3; i ++)
      {
        Serial.print("ID:" + (String)i + " ");
        Serial.print("SSID:" + (String)WiFi.SSID(staLabel[i]) + " ");
        Serial.print("RSSI:" + (String)WiFi.RSSI(staLabel[i]) + " ");
        Serial.print("BSSID:");
        for(int j = 0; j < 6; j ++)
        {
          if(WiFi.BSSID(staLabel[i])[j] < 0x10)
          {
            Serial.print("0");  
          }
          Serial.print(WiFi.BSSID(staLabel[i])[j], HEX);
        }
        Serial.println();      
      }
      String json = "{";
      for(int i = 0; i < 3; i++)
      {
        json += "\"AP" + (String)i + "\":\"";      
        for(int j = 0; j < 6; j++)
        {
          char buf[4];
          sprintf(buf, "%0x", WiFi.BSSID(staLabel[0])[j]);
          json += buf;
        }
        if(i != 2)
        {
          json += "\",";
        }        
        else
        {
          json += "\"}";      
        }
      }
      return json;
    }
    else
    {
      return "NULL";  
    }
  }  
}



void loop() 
{
  Serial.println(scanAndMakeJSON());
  delay(10000);
  Serial.println();
  Serial.println();
}
