#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 7, 8, 9, 10);

int TRIG_PIN = 13;
int ECHO_PIN = 12;
int LED_PIN = 6;
int LED_PIN2 = 5;
int LED_PIN3 = 4;
int ALERT = 11;
int SENSOR_LLUVIA = A0;  // Sensor de lluvia en entrada analógica

long duration;
int distance;
String estadoLluvia = "SIN LLUVIA";
String mensajeLCD = "";

void setup() {
    lcd.begin(16, 2); // Configura el LCD de 16x2
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(LED_PIN3, OUTPUT);
    pinMode(ALERT, OUTPUT);
  
    Serial.begin(9600);
  
    lcd.setCursor(0, 0);
    lcd.print("Distancia: ");
}

void loop() {
    // Lectura del sensor ultrasónico
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(10);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;

    // Lectura del sensor de lluvia (A0)
    int valorLluvia = analogRead(SENSOR_LLUVIA);
    
    // Determinar el nivel de lluvia
    if (valorLluvia < 400) {
        estadoLluvia = "TORMENTA";
    } else if (valorLluvia >= 400 && valorLluvia < 700) {
        estadoLluvia = "LLUVIOSO";
    } else if (valorLluvia >= 700 && valorLluvia < 900) {
        estadoLluvia = "LLOVIZNA";
    } else {
        estadoLluvia = "SIN LLUVIA";
    }

    // Mostrar distancia en el LCD
    lcd.setCursor(10, 0);
    lcd.print("    ");    
    lcd.setCursor(10, 0);
    lcd.print(distance);
    lcd.print("cm");

    // Determinar mensaje de la segunda fila
    if (distance < 5) {
        digitalWrite(LED_PIN2, HIGH);
        digitalWrite(LED_PIN, LOW);
        digitalWrite(LED_PIN3, LOW);
        digitalWrite(ALERT, HIGH);
        mensajeLCD = "PELIGRO - " + estadoLluvia;
    } else if (distance >= 5 && distance <= 6) {
        digitalWrite(LED_PIN, LOW);
        digitalWrite(LED_PIN3, HIGH);
        digitalWrite(LED_PIN2, LOW);
        digitalWrite(ALERT, LOW);
        mensajeLCD = "PRECAUCION - " + estadoLluvia;
    } else {
        digitalWrite(LED_PIN2, LOW);
        digitalWrite(LED_PIN3, LOW);
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(ALERT, LOW);
        mensajeLCD = "SEGURO - " + estadoLluvia;
    }
    if (valorLluvia < 500) {
        digitalWrite(ALERT, HIGH);;
    }

    // Mostrar estado en el Monitor Serie
    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.print(" cm | Lluvia: ");
    Serial.println(estadoLluvia);

    // Desplazamiento del mensaje en la segunda fila
    for (int i = 0; i < mensajeLCD.length() - 15; i++) {
        lcd.setCursor(0, 1);
        lcd.print(mensajeLCD.substring(i, i + 16)); // Muestra 16 caracteres por vez
        delay(400); // Controla la velocidad del desplazamiento
    }

    delay(2000);  // Espera antes de la próxima lectura
}