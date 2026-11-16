
const int PIN_PIR      = 27;
const int PIN_REED     = 26;
const int PIN_LED_PIR  = 25;
const int PIN_LED_REED = 33;
const int PIN_LED_VIVO = 2;  

const unsigned long CALENTAMIENTO_PIR_MS = 2000;  
const unsigned long ANTIREBOTE_REED_MS   = 50;
const unsigned long PARPADEO_VIVO_MS     = 1000;

int  estadoPir  = LOW;
int  estadoReed = -1;  
int  lecturaReedAnterior = -1;
unsigned long ultimoCambioReed = 0;
unsigned long ultimoParpadeo   = 0;
bool pirListo = false;
unsigned long inicio = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_REED, INPUT_PULLUP);
  pinMode(PIN_LED_PIR, OUTPUT);
  pinMode(PIN_LED_REED, OUTPUT);
  pinMode(PIN_LED_VIVO, OUTPUT);

  digitalWrite(PIN_LED_PIR, LOW);
  digitalWrite(PIN_LED_REED, LOW);

  inicio = millis();

  Serial.println();
  Serial.println("=====================================");
  Serial.println("Presencia de zona");
  Serial.println(" ESP32 iniciado correctamente");
  Serial.println("=====================================");
  Serial.println("PIR: calentando (30 s), espera antes de probar...");
}

void loop() {
  unsigned long ahora = millis();

  if (ahora - ultimoParpadeo >= PARPADEO_VIVO_MS) {
    ultimoParpadeo = ahora;
    digitalWrite(PIN_LED_VIVO, !digitalRead(PIN_LED_VIVO));
  }

  if (!pirListo) {
    if (ahora - inicio >= CALENTAMIENTO_PIR_MS) {
      pirListo = true;
      estadoPir = digitalRead(PIN_PIR);
      digitalWrite(PIN_LED_PIR, estadoPir);
      Serial.println("PIR: listo");
      Serial.println(estadoPir == HIGH ? "PIR: detected" : "PIR: clear");
    }
  } else {
    int lecturaPir = digitalRead(PIN_PIR);
    if (lecturaPir != estadoPir) {
      estadoPir = lecturaPir;
      digitalWrite(PIN_LED_PIR, estadoPir);
      Serial.println(estadoPir == HIGH ? "PIR: detected" : "PIR: clear");
    }
  }

  int lecturaReed = digitalRead(PIN_REED);
  if (lecturaReed != lecturaReedAnterior) {
    ultimoCambioReed = ahora;
    lecturaReedAnterior = lecturaReed;
  }
  if ((ahora - ultimoCambioReed) >= ANTIREBOTE_REED_MS && lecturaReed != estadoReed) {
    estadoReed = lecturaReed;
    bool abierto = (estadoReed == HIGH);
    digitalWrite(PIN_LED_REED, abierto ? HIGH : LOW); 
    Serial.println(abierto ? "REED: encendido" : "REED: apagado");
  }
}
