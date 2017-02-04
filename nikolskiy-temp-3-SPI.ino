                                                                                                                                                                                                      #include <Wire.h>
#include "Sodaq_DS3231.h"

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include "Adafruit_ILI9341.h"
//#include <Fonts/FreeMonoBold24pt7b.h> // Загрузка шрифт

// Dallas Temperature ----------------
#include <OneWire.h>
#include <DallasTemperature.h>

// DS3231 ------ Time Clock дни недели масив ----------
char weekDayR[][5] = {"0", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс", "Пн" };
char weekDayI[][4] = {"0", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", "Mon" };

//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
DateTime dt(2017, 01, 24, 1, 34, 0, 1); // Выставить начальное время

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_RST 8
#define TFT_MISO 12

// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// Pin of VoltMetr
#define PINVLT A0

// Pin of Alert
#define PINALERT 6

// Pin tft Led light
#define TFTLIGHT 5

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 7
#define TEMPERATURE_PRECISION 12 // Точность измерения ds18b20

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to . 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

// arrays to hold device address
//DeviceAddress tindrive, toutdrive, tinpecka, toutpecka, tkorobka, ttoplivo;
DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address


// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

/*  Задаем адреса сенсоров Nikolskiy
Device 0 Addr: 28 FF BE 04 75 16 03 D5 bit 12 Temp 32.75
Device 1 Addr: 28 FF 99 91 74 16 04 34 bit 12 Temp 32.56
Device 2 Addr: 28 FF B5 84 74 16 04 54 bit 12 Temp 32.50
Device 3 Addr: 28 FF 73 A6 73 16 05 B9 bit 12 Temp 23.12
Device 4 Addr: 28 FF B7 83 74 16 04 51 bit 12 Temp 32.69
Device 5 Addr: 28 FF 0F 84 73 16 05 82 bit 12 Temp 23.44
 */
DeviceAddress tindrive  = {0x28, 0xFF, 0xBE, 0x04, 0x75, 0x16, 0x03, 0xD5};
DeviceAddress toutdrive = {0x28, 0xFF, 0x99, 0x91, 0x74, 0x16, 0x04, 0x34};
DeviceAddress tinpecka  = {0x28, 0xFF, 0xB5, 0x84, 0x74, 0x16, 0x04, 0x54};
DeviceAddress toutpecka = {0x28, 0xFF, 0x73, 0xA6, 0x73, 0x16, 0x05, 0xB9};
DeviceAddress tkorobka  = {0x28, 0xFF, 0xB7, 0x83, 0x74, 0x16, 0x04, 0x51};
DeviceAddress ttoplivo  = {0x28, 0xFF, 0x0F, 0x84, 0x73, 0x16, 0x05, 0x82};

/*  // Задаем адреса сенсоров
DeviceAddress tindrive  = {0x28, 0xFF, 0x65, 0x90, 0x54, 0x14, 0x00, 0x58};
DeviceAddress toutdrive = {0x28, 0xFF, 0x7F, 0x79, 0x54, 0x14, 0x00, 0x43};
DeviceAddress tinpecka  = {0x28, 0xFF, 0x65, 0x90, 0x54, 0x14, 0x00, 0x58};
DeviceAddress toutpecka = {0x28, 0xFF, 0x7F, 0x79, 0x54, 0x14, 0x00, 0x43};
DeviceAddress tkorobka  = {0x28, 0xFF, 0x65, 0x90, 0x54, 0x14, 0x00, 0x58};
DeviceAddress ttoplivo  = {0x28, 0xFF, 0x7F, 0x79, 0x54, 0x14, 0x00, 0x43};
*/

void setup(void) {
  
  // initialize digital pin 13 as an output.
  pinMode(PINALERT, INPUT);

// TFT ------- Light ----------
  pinMode(TFTLIGHT, OUTPUT);
  analogWrite(TFTLIGHT, 150); // Яркость подсветки tft
  
// serial port -------- init ----------
  Serial.begin(9600); // init

// DS3231 ------- Time Clock ----------
    Wire.begin();
    rtc.begin();
    //rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above Установить начально время

// tft ---------- init ----------------
     tft.begin();                 // init tft
    
     tft.fillScreen(BLACK);  // Очистка экрана
     tft.setRotation(3);     // Поворот экрана
     //tft.setTextWrap(false); // Allow text to run off right edge

  // --------------------------------------------------

    Serial.print("Debug mode = ");
    tft.print("Debug mode = ");
    if (digitalRead(PINALERT)) {
        Serial.println("True");
        tft.println("True");
      } else {
        Serial.println("False");
        tft.println("False");
      }

  // --------------------------------------------------

  // Start up the library dallas
  sensors.begin();
  
  Serial.println("Dallas Temperature IC Control"); // 232
  tft.println("Dallas Temperature IC Control"); // tft

  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();  
  
  Serial.print("Locating devices...");
  tft.print("Locating devices...");
  Serial.print("Found ");
  tft.print("Found ");
  Serial.print(numberOfDevices, DEC);
  tft.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  tft.println(" devices.");
  
  

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  tft.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  if (sensors.isParasitePowerMode()) tft.println("ON");
  else tft.println("OFF");
    Serial.println("");
    tft.println("");

  // --------------------------------------------------
     
      do   
  {
        tft.setTextColor(WHITE, BLACK);
        tft.setCursor(0, 50);
        sensors.requestTemperatures(); // Send the command to get temperatures
        
        // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
  {
    
    Serial.print("Device ");
    tft.print("Device ");
    Serial.print(i, DEC);
    tft.print(i, DEC);
    Serial.print(" Addr: ");
    tft.print(" Addr: ");
    printAddress(tempDeviceAddress);
    Serial.print(" bit ");
    tft.print(" bit ");
    
    // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
    sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);

    Serial.print(sensors.getResolution(tempDeviceAddress), DEC);
    tft.print(sensors.getResolution(tempDeviceAddress), DEC);

    Serial.print(" Temp ");
    tft.print(" Temp ");
    Serial.print(sensors.getTempC(tempDeviceAddress)); // Use a simple function to print out the data
    tft.print(sensors.getTempC(tempDeviceAddress)); // Use a simple function to print out the data
    
    Serial.println();
    tft.println();
  }else{
    Serial.print("Found ghost device at ");
    tft.print("Found ghost device at ");
    Serial.print(i, DEC);
    tft.print(i, DEC);
    Serial.print(" but could not detect address. Check power and cabling");
    tft.print(" but could not detect address. Check power and cabling");
  }
  }


          //delay(1000); // Пауза в режиме отладки.
      
      } while (digitalRead(PINALERT));    // Выполнять цикл пока включена отладка

  // --------------------------------------------------  

  tft.fillScreen(BLACK);  // Очистка экрана

  // initialize digital pin 13 as an output.
  pinMode(PINALERT, OUTPUT);
}


// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.println(tempC);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (deviceAddress[i] < 16) tft.print("0");
    tft.print(deviceAddress[i], HEX);
  }
}

String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length(); i = 0;
  while (i < k) {
    n = source[i]; i++;

   if (n >= 0xBF){
     switch (n) {
     case 0xD0: {
    n = source[i]; i++;
  if (n == 0x81) { n = 0xA8; break; }
  if (n >= 0x90 && n <= 0xBF) n = n + 0x2F;
    break;
  }
case 0xD1: {
n = source[i]; i++;
if (n == 0x91) { n = 0xB7; break; }
if (n >= 0x80 && n <= 0x8F) n = n + 0x6F;
break;
}
}
}
m[0] = n; target = target + String(m);
}
return target;
}

void alarm(void) {
  digitalWrite(PINALERT, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(PINALERT, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  digitalWrite(PINALERT, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(PINALERT, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second

  }

// Измерение напряжения питания
int readVcc()
{
  int result;
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(75); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
  result = ADCL;
  result |= ADCH<<8;
  result =  1125300L / result; // (Kvcc * 1023.0 * 1000) (in mV);
  return result;
}

// Измерение напряжения питания
float readVacc()
{
  float result;
  
  //analogReference(INTERNAL); // 1023=1.1В
  analogReference(DEFAULT); // 1023=1.1В
  int currentVoltage = analogRead(PINVLT); //Смотрим напругу на батарейках
  
  // 1.1 * 1023 / АЦП-измерение
  // result = (1.1 * 1023) / currentVoltage ;
  //result = (0.6321 * 1023.0 * 1000) / currentVoltage; // Результат Vcc в милливольтах
  
  float corconst = 15.8300; // Константа для коррекции Реальный VCC / АЦП-измерение = corconst
  currentVoltage = currentVoltage * corconst;
  result = currentVoltage;
    
  return result;
}

void loop(void) {

            tft.setTextSize(2);     // Размер шрифта
            tft.setTextColor(CYAN);  // Цвет шрифта
            tft.setCursor(0, 0);    // Позиция курсора в пикселях
            tft.println("  Nikolskiy Temp ver.3.2");
            tft.println("");
            
    tft.setTextColor(WHITE);  // Цвет шрифта
    tft.setCursor(0, 30);    // Позиция курсора в пикселях
    tft.println(utf8rus("ВЫХОД ДВИГАТЕЛЬ"));// 102*С
    tft.setCursor(0, 50);    // Позиция курсора в пикселях
    tft.println(utf8rus("ВХОД ДВИГАТЕЛЬ"));// 88*С
    tft.setCursor(0, 70);    // Позиция курсора в пикселях
    tft.println(utf8rus("ВЫХОД ПЕЧКА"));// 101*С
    tft.setCursor(0, 90);    // Позиция курсора в пикселях
    tft.println(utf8rus("ВХОД ПЕЧКА"));// 85*С
    tft.setCursor(0, 110);    // Позиция курсора в пикселях
    tft.println(utf8rus("КОРОБКА"));//79*С
    tft.setCursor(0, 130);    // Позиция курсора в пикселях
    tft.println(utf8rus("ТОПЛИВО"));// 38*С


  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
  {
    // Output the device ID

    Serial.print(" ");
    Serial.print("DS");Serial.print(i,DEC);Serial.print(" ");
    printTemperature(tempDeviceAddress); // Use a simple function to print out the data
  } 
  //else ghost device! Check your power requirements and cabling
  }

  float tempC;
  
  tft.setTextColor(GREEN, BLACK);
  tft.setCursor(230, 30);
  tempC = sensors.getTempC(toutdrive);
  if (tempC > 27) { tft.setTextColor(RED, BLACK); alarm(); }
  if (tempC < 26) { tft.setTextColor(BLUE, BLACK); }
  tft.print(tempC);
  
  tft.setTextColor(GREEN, BLACK);
  tft.setCursor(230, 50);
  tempC = sensors.getTempC(tindrive);
  if (tempC > 27) { tft.setTextColor(RED, BLACK); }
  if (tempC < 26) { tft.setTextColor(BLUE, BLACK); }
  tft.print(tempC);

  tft.setTextColor(GREEN, BLACK);
  tft.setCursor(230, 70);
  tempC = sensors.getTempC(toutpecka);
  if (tempC > 27) { tft.setTextColor(RED, BLACK); }
  if (tempC < 26) { tft.setTextColor(BLUE, BLACK); }
  tft.print(tempC);

  tft.setTextColor(GREEN, BLACK);
  tft.setCursor(230, 90);
  tempC = sensors.getTempC(tinpecka);
  if (tempC > 27) { tft.setTextColor(RED, BLACK); }
  if (tempC < 26) { tft.setTextColor(BLUE, BLACK); }
  tft.print(tempC);

  tft.setTextColor(GREEN, BLACK);
  tft.setCursor(230,110);
  tempC = sensors.getTempC(tkorobka);
  if (tempC > 27) { tft.setTextColor(RED, BLACK); }
  if (tempC < 26) { tft.setTextColor(BLUE, BLACK); }
  tft.print(tempC);

  tft.setTextColor(GREEN, BLACK);
  tft.setCursor(230,130);
  tempC = sensors.getTempC(ttoplivo);
  if (tempC > 27) { tft.setTextColor(RED, BLACK); }
  if (tempC < 26) { tft.setTextColor(BLUE, BLACK); }
  tft.print(tempC);

// DS3231 --------- View Time ---------------

    DateTime now = rtc.now(); //get the current date-time
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.date(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(' ');
    Serial.print(weekDayI[now.dayOfWeek()]);
    Serial.print(' ');
    Serial.print(now.dayOfWeek());
    Serial.println();

  
  tft.setTextColor(GREEN, BLACK); // Цвет шрифта
      
    // ------ time ---------
    tft.setTextSize(7);               // Размер шрифта
    tft.setCursor(50,160);            // Позиция курсора в пикселях
    //tft.setFont(&FreeMonoBold24pt7b); // Замена шрифта
    
    if(now.hour()<10)tft.print('0');
    tft.print(now.hour(), DEC);
    
    tft.print(':');
    
    if(now.minute()<10)tft.print('0');
    tft.print(now.minute(), DEC);
    //tft.print(':');
    //tft.print(now.second(), DEC);
    tft.println();
    //tft.setFont();                     // Возврат шрифта
    
    // ------ date ----------
    tft.setTextSize(2);             // Размер шрифта
    tft.setCursor(0,220);           // Позиция курсора в пикселях
    tft.print(now.year(), DEC);
    tft.print('/');
    if(now.month()<10)tft.print('0');
    tft.print(now.month(), DEC);
    tft.print('/');
    if(now.date()<10)tft.print('0');
    tft.print(now.date(), DEC);
    tft.print(' ');
    tft.print(utf8rus(weekDayR[now.dayOfWeek()]));
    //tft.print(' ');


    // ------- readVcc() -----
    int vltVcc = readVcc();
    float vltVacc = readVacc()/1000;
    int adc = analogRead(PINVLT);
    Serial.print(adc);
    Serial.println(" ADC");
    Serial.print(vltVcc);
    Serial.println(" mV-cpu");
    Serial.print(vltVacc);
    Serial.println(" V-akb");


    tft.setCursor(165,220);               // Позиция курсора в пикселях
    //tft.println("      ");
    //tft.setCursor(165,220);               // Позиция курсора в пикселях
    //float vlt = readVcc()/1000;
    if(vltVacc<10)tft.print('0');         // Добавить незначищей ноль
    tft.print(vltVacc);
    tft.println("V");
    
    // ------- temp ----------
    rtc.convertTemperature();             //convert current temperature into registers
    Serial.print(rtc.getTemperature());   //read registers and display the temperature
    Serial.println(" C");

    tft.setCursor(240,220);               // Позиция курсора в пикселях
    tft.print(rtc.getTemperature());      //read registers and display the temperature
    tft.println("C");
    
    delay(500);
}
