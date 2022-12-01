#include<LiquidCrystal.h>
LiquidCrystal lcd(13,12,11,10,9,8);
#include <SoftwareSerial.h>
SoftwareSerial fingerPrint(2, 3);
#include<Servo.h>
Servo myServo;
#include <Adafruit_Fingerprint.h>
uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);
#define enroll 14
#define del 15
#define up 16
#define down 17
#define openLight 7
#define closeLight 6
#define servoPin 5

int LED_B = 4;
int micro = A4;
int photo = A5;

void setup() 
{
    delay(1000);
    myServo.attach(servoPin);
    myServo.write(180);
    pinMode(enroll, INPUT_PULLUP);
    pinMode(up, INPUT_PULLUP);
    pinMode(down, INPUT_PULLUP); 
    pinMode(del, INPUT_PULLUP); 
    pinMode(openLight, OUTPUT);
    pinMode(closeLight, OUTPUT);
    pinMode(LED_B,OUTPUT);
    pinMode(micro,INPUT);
    pinMode(photo,INPUT);
    lcd.begin(16,2);
    lcd.print("Caja Fuerte");
    lcd.setCursor(0,1);
    lcd.print("Juan J. Mejia");
    delay(2000);
    lcd.clear();
    lcd.print("Electrónica");
    lcd.setCursor(0,1);
    lcd.print("2022-2");
    delay(2000);
    finger.begin(57600);
    Serial.begin(9600);
    lcd.clear();
    lcd.print("Buscando sensor");
    lcd.setCursor(0,1);
    delay(1000);
    if (finger.verifyPassword()) 
    {
      Serial.println("Sensor en linea");
      lcd.clear();
      lcd.print("Sensor en linea");
      delay(1000);
    } 
    else 
    {
    Serial.println("");
    lcd.clear();
    lcd.print("No hay modulo");
    lcd.setCursor(0,1);
    lcd.print("Revise conexion");
    while (1);
    }
}
void loop() 
{
  int micro_signal = analogRead(micro);

 lcd.setCursor(0,0);
 lcd.print("Arriba/abajo     ");
 lcd.setCursor(0,1);
 lcd.print("para iniciar      ");
 digitalWrite(closeLight, HIGH);
 if(digitalRead(up)==0 || digitalRead(down)==0)
 {
  for(int i=0;i<5;i++)
  {
    lcd.clear();
    lcd.print("Coloque huella");
    delay(2000);
    int result=getFingerprintIDez();
    if(result>=0)
    {
        digitalWrite(openLight, HIGH);
        digitalWrite(closeLight, LOW);
        lcd.clear();
        lcd.print("Bienvenid@");
        lcd.setCursor(0,1);
        lcd.print("Puerta abierta   ");
        myServo.write(0);
        if (micro_signal == 1)
        {
          myServo.write(180);
          digitalWrite(closeLight, HIGH);
          digitalWrite(openLight, LOW);
          lcd.setCursor(0,1);
          lcd.print("Puerta cerrada   ");
          return;
        }
     }
   }
 }
 checkKeys();
 checkLight();
 delay(1000);
}
void checkLight()
{
  int luz = analogRead(photo);
  if (luz<25)
  {
    digitalWrite(LED_B,HIGH);
  }
  else
  {
    digitalWrite(LED_B,LOW);
  }
}
void checkKeys()
{
   if(digitalRead(enroll) == 0)
   {
    lcd.clear();
    lcd.print("Porfavor esperar");
    delay(1000);
    while(digitalRead(enroll) == 0);
    Enroll();
   }
   else if(digitalRead(del) == 0)
   {
    lcd.clear();
    lcd.print("Porfavor esperar");
    delay(1000);
    delet();
   }  
}
void Enroll()
{
   int count=0;
   lcd.clear();
   lcd.print("Guarde huella    ");
   lcd.setCursor(0,1);
   lcd.print("Ubicacion: ");
   while(1)
   {
     lcd.setCursor(9,1);
     lcd.print(count);
     if(digitalRead(up) == 0)

     {
       count++;
       if(count>25)
       count=0;
       delay(500);
     }
     else if(digitalRead(down) == 0)
     {
       count--;
       if(count<0)
       count=25;
       delay(500);
     }
     else if(digitalRead(del) == 0)
     {
          id=count;
          getFingerprintEnroll();
          return;
     }
       else if(digitalRead(enroll) == 0)
     {        
          return;
     }
 }
}
void delet()
{
   int count=0;
   lcd.clear();
   lcd.print("Elimine huella    ");
   lcd.setCursor(0,1);
   lcd.print("Ubicacion:");
   while(1)
   {
     lcd.setCursor(9,1);
     lcd.print(count);
     if(digitalRead(up) == 0)
     {
       count++;
       if(count>25)
       count=0;
       delay(500);
     }
     else if(digitalRead(down) == 0)
     {
       count--;
       if(count<0)
       count=25;
       delay(500);
     }
     else if(digitalRead(del) == 0)
     {
          id=count;
          deleteFingerprint(id);
          return;
     }
       else if(digitalRead(enroll) == 0)
     {        
          return;
     }
 }
}
uint8_t getFingerprintEnroll() 
{
  int p = -1;
  lcd.clear();
  lcd.print("ID dactilar:");
  lcd.print(id);
  lcd.setCursor(0,1);
  lcd.print("Coloque huella");
  delay(2000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Imagen tomada");
      lcd.clear();
      lcd.print("Imagen tomada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No hay huella");
      lcd.clear();
      lcd.print("No hay huella");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicación");
      lcd.clear();
      lcd.print("Error en comm");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Error de imagen");
      lcd.clear();
      lcd.print("Error de imagen");
      break;
    default:
      Serial.println("Error desconocido");
       lcd.clear();
      lcd.print("Error desconocido");
      break;
    }
  }
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagen convertida");
      lcd.clear();
      lcd.print("Imagen convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagen borrosa");
       lcd.clear();
       lcd.print("Imagen borrosa");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicación");
      lcd.clear();
      lcd.print("Error en comm");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("No se encontraron características de huella");
      lcd.clear();
      lcd.print("Huella invalida");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("No se encontró huella valida");
      lcd.clear();
      lcd.print("Huella invalida");
      return p;
    default:
      Serial.println("Error desconocido");
      lcd.clear();
      lcd.print("Error desconocido");
      return p;
  }
  Serial.println("Remueva huella");
  lcd.clear();
  lcd.print("Remueva huella");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
   lcd.clear();
      lcd.print("Coloque huella");
      lcd.setCursor(0,1);
      lcd.print("De nuevo");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagen tomada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      return;
    }
  }
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  Serial.print("Creating model for #");  Serial.println(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.print("Guardada!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  }
  else {
    Serial.println("Unknown error");
    return p;
  }  
}
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  
  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
   lcd.clear();
   lcd.print("Huella no encontrada");
   lcd.setCursor(0,1);
   lcd.print("Intente de nuevo");
   delay(2000);  
  return -1;
  }
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  return finger.fingerID; 
}
uint8_t deleteFingerprint(uint8_t id) 
{
  uint8_t p = -1;  
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Eliminida");
    lcd.clear();
    lcd.print("Huella eliminada");
    lcd.setCursor(0,1);
    lcd.print("Exitosamente");
    delay(1000);
  } 
  else 
  {
    Serial.print("Algo anda mal"); 
    lcd.clear();
    lcd.print("Algo anda mal");
    lcd.setCursor(0,1);
    lcd.print("Intente de nuevo");
    delay(2000);
    return p;
  }  
}