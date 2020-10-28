



/*****************************************************
 *    Start up LCD 
 ***************************************************/
void startlcd(void)
{
   lcd.setCursor (5,0);lcd.print("Smart Farm");
   lcd.setCursor (1,1);lcd.print("Megantari Suhendar");
   lcd.setCursor (3,2);lcd.print("NIM.C1B160005");
   lcd.setCursor (3,3);lcd.print("SI/FTI UNIBBA");
    
}


 /*************************************************
  * Display Var
  *************************************************/
void varlcd(void)
{
   lcd.setCursor (3,0);lcd.print("SmartFarm Var");
  // lcd.setCursor (11,0);lcd.print(String(SW_VERSION));
   lcd.setCursor (0,1);lcd.print("Dry:");
   lcd.setCursor (5,1);lcd.print(String(DRY_SOIL)+ " %");
   lcd.setCursor (10,1);lcd.print("Wet:");
   lcd.setCursor (15,1);lcd.print(String(WET_SOIL)+ " %");
   lcd.setCursor (0,2);lcd.print("Cld:");
   lcd.setCursor (5,2);lcd.print(String(COLD_TEMP)+ (char)223 +"C");
   lcd.setCursor (10,2);lcd.print("Hot:");
   lcd.setCursor (15,2);lcd.print(String(HOT_TEMP)+ (char)223 +"C");
   lcd.setCursor (0,3);lcd.print("P-O:");
   lcd.setCursor (5,3);lcd.print(String(TIME_PUMP_ON)+ " s");
   lcd.setCursor (10,3);lcd.print("L-O:");
   lcd.setCursor (15,3);lcd.print(String(TIME_LAMP_ON)+ " s");
    
}


/***************************************************
 * Display data at Serial Monitora & OLED Display
 **************************************************/
void displayData(void)
{
  String pumpStatusStr;
  String lampStatusStr;
  if (pumpStatus == 1) pumpStatusStr = "ON ";
  if (pumpStatus == 0) pumpStatusStr = "OFF";
  if (lampStatus == 1) lampStatusStr = "ON ";
  if (lampStatus == 0) lampStatusStr = "OFF";

   lcd.setCursor (4,0);lcd.print("Data Display");
   lcd.setCursor (0,1);lcd.print("AT:");
   lcd.setCursor (4,1);lcd.print(airTemp,1)+" ";
   lcd.setCursor (10,1);lcd.print("AH:");
   lcd.setCursor (14,1);lcd.print(airHum,1)+" ";
   lcd.setCursor (0,2);lcd.print("ST:");
   lcd.setCursor (4,2);lcd.print(soilTemp,1)+" ";
   lcd.setCursor (10,2);lcd.print("SH:");
   lcd.setCursor (14,2);lcd.print(soilMoister,1)+" ";
   lcd.setCursor (0,3);lcd.print("PMP:");
   lcd.setCursor (4,3);lcd.print(String(pumpStatusStr));
   lcd.setCursor (10,3);lcd.print("LMP:");
   lcd.setCursor (14,3);lcd.print(String(lampStatusStr));     
}
