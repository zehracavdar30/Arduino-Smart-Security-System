#include <IRremote.h>

// --- PİNLER ---
const int irReceiverPin = 2;      
const int trigPin = 4;            
const int echoPin = 5;            
const int buzzerPin = 6;                
const int leftHeadlightPin = 8;   
const int redPin = 9;             
const int bluePin = 10;           
const int rightHeadlightPin = 12; 
const int ldrPin = A0;            

// --- KUMANDA KODLARI (KENDİ SAYILARINI YAZ) ---
const int tus1 = 69;    // 1 Tuşu 
const int tus2 = 70;    // 2 Tuşu 
const int tus3 = 71;    // 3 Tuşu 
const int tusSol = 8;  // Sol Ok Tuşu
const int tusOK = 28;   // OK Tuşu 

// --- SİSTEM HAFIZASI ---
bool farlarAcik = false;
bool alarmAcik = false;
bool otonomFarModu = false; 
bool nobetciMod = false;    

// --- ZAMANLAYICI VE YARDIMCI DEĞİŞKENLER ---
unsigned long oncekiZaman = 0;
unsigned long sonTusZamani = 0; 
unsigned long sonBuzzerZamani = 0; 
unsigned long sonRadarZamani = 0;
bool polisModu = false;
bool buzzerDurumu = false;         

void setup() {
  pinMode(leftHeadlightPin, OUTPUT);
  pinMode(rightHeadlightPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  IrReceiver.begin(irReceiverPin, ENABLE_LED_FEEDBACK);
  Serial.begin(9600);
}

void loop() {
  // --- 1. KUMANDA DİNLEME VE ÖNCELİK MANTIĞI ---
  if (IrReceiver.decode()) {
    unsigned long suAnkiZaman = millis();
    
    if (suAnkiZaman - sonTusZamani > 500) { 
      sonTusZamani = suAnkiZaman; 
      int gelenKod = IrReceiver.decodedIRData.command;
      
      if (gelenKod == tus1) {
        otonomFarModu = false; 
        farlarAcik = !farlarAcik; 
      } 
      else if (gelenKod == tus2) {
        alarmAcik = !alarmAcik; 
        nobetciMod = false; // YENİ: Manuel müdahale gelirse radarı devre dışı bırak (Çakışmayı önler)
      }
      else if (gelenKod == tus3) {
        nobetciMod = !nobetciMod; 
        alarmAcik = false; // YENİ: Nöbetçi moda girerken veya çıkarken alarmı temizle
      }
      else if (gelenKod == tusSol) {
        otonomFarModu = !otonomFarModu; 
      }
      else if (gelenKod == tusOK) {
        farlarAcik = false;
        alarmAcik = false;
        otonomFarModu = false;
        nobetciMod = false;
      }
    }
    IrReceiver.resume(); 
  }

  // --- 2. OTONOM FAR SİSTEMİ ---
  if (otonomFarModu) {
    int isikSeviyesi = analogRead(ldrPin);
    if (isikSeviyesi < 300) { 
      farlarAcik = true;
    } else {
      farlarAcik = false;
    }
  }

  if (farlarAcik) {
    digitalWrite(leftHeadlightPin, HIGH);
    digitalWrite(rightHeadlightPin, HIGH);
  } else {
    digitalWrite(leftHeadlightPin, LOW);
    digitalWrite(rightHeadlightPin, LOW);
  }

  // --- 3. OTONOM NÖBETÇİ SİSTEMİ ---
  if (nobetciMod && !alarmAcik) {
    unsigned long suAnkiRadarZamani = millis();
    
    if (suAnkiRadarZamani - sonRadarZamani > 150) {
      sonRadarZamani = suAnkiRadarZamani;
      
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      long duration = pulseIn(echoPin, HIGH, 10000); 
      
      if (duration > 0) {
        int distance = duration * 0.034 / 2;
        if (distance > 0 && distance < 15) {
          alarmAcik = true; 
        }
      }
    }
  }

  // --- 4. ALARM VE SİREN KONTROLÜ ---
  if (alarmAcik) {
    unsigned long suAnkiZamanAlarm = millis(); 
    
    if (suAnkiZamanAlarm - oncekiZaman >= 100) {
      oncekiZaman = suAnkiZamanAlarm; 
      polisModu = !polisModu;    
      
      if (polisModu) {
        digitalWrite(redPin, HIGH);
        digitalWrite(bluePin, LOW);
      } else {
        digitalWrite(redPin, LOW);
        digitalWrite(bluePin, HIGH);
      }
    }

    unsigned long suAnkiMikro = micros(); 
    int sesHizi = polisModu ? 400 : 600;  

    if (suAnkiMikro - sonBuzzerZamani > sesHizi) {
      sonBuzzerZamani = suAnkiMikro;
      buzzerDurumu = !buzzerDurumu; 
      digitalWrite(buzzerPin, buzzerDurumu);
    }
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(buzzerPin, LOW); 
  }
}
