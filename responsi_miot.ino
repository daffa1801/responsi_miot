// Library yang diperlukan
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Servo.h>


// Mendefinisikan pin dan tipe sensor DHT
#define DHTPIN D4
#define DHTTYPE DHT11
#define led 4
DHT dht(DHTPIN, DHTTYPE);

// Firebase Auth
#define FIREBASE_HOST "https://responsi-miot-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "gOr2SapUZxWUXw8E42hWx1Cda2kuuULSt6zA41Dm"
#define WIFI_SSID "athabi"
#define WIFI_PASSWORD "22022015"  

// mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;
// membuat servo objek untuk mengontrol servo
Servo myservo;



void setup() {
  
  Serial.begin(9600);

  myservo.attach(D5);
  dht.begin();
  
  pinMode(led, OUTPUT);
  // Koneksi ke Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() !=  WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop(){

  int minimum_temp = 25;

  // Sensor DHT11 membaca suhu dan kelembaban
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  //Memeriksa apakah sensor berhasil mambaca suhu dan kelembaban
  if (isnan(t) || isnan(h)) {
    Serial.println("Gagal membaca sensor DHT11");
    return;
  } else {
      // Menampilkan suhu dan kelembaban pada serial monitor
  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.println(" *C");
  Serial.print("Kelembaban: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.println();
  }

  if (t>minimum_temp) {
    for (int angle = 0; angle < 180; angle++) {  // go from 0 degrees to 180 degrees
    myservo.write(angle);                       // tell servo to go to position in variable 'angle'
    delay(1);                                  // waits 15ms for the servo to reach the position
    }
    for (int angle = 180; angle > 0; angle--) {  // go from 180 degrees to 0 degrees
      myservo.write(angle);                       // tell servo to go to position in variable 'angle'
      delay(1);                                  // waits 15ms for the servo to reach the position
    }

    digitalWrite(led, LOW);    
  } else {
    digitalWrite(led, HIGH);
    delay(1000);
    myservo.write(0);
  }

  // Memberikan status suhu dan kelembaban kepada firebase
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/suhu", t)){
      Serial.println("Suhu terkirim");
    } else{
      Serial.println("Suhu tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    } 
    
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/kelembaban", h)){
      Serial.println("Kelembaban terkirim");
      Serial.println();
    } else{
      Serial.println("Kelembaban tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }
    
  delay(1000);

  
 
}
