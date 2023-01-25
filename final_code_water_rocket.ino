#include <SD.h> //carga la libreria SD.h
#include<SPI.h> //carga la libreria SPI.h
#include "Adafruit_BMP085.h"  // importa la libreria de para emplear bmp085-bmp180
#include <Servo.h>

Adafruit_BMP085 bmp180;  // crea un objeto sensor llamado bmp180
 
 
float tempC;  //Variable de temperatura en *C
float pressure; //Variable para mantener la lectura de presión
float altitud;  //Variable para mantener la altitud
float altitud_init;//Lee altitud en metros

float altura_max = 0;
float altura;

int led_rojo = 7;
int led_azul = 8;
int buzzer = 10;

int chipSelect = 4; //chipSelect pin para la tarjeta SD Reader
File bmp180_Data; //Objeto de datos en el que se escribirán los datos de sensor
Servo servomotor;
void setup(){
Serial.begin(9600); //turn on serial monitor


//bmp180.begin();   //tambien se puede inciar el sensor de esta manera
 if ( !bmp180.begin() ) {       // si falla la comunicacion con el sensor mostrar
    Serial.println("BMP180 no encontrado !"); // texto y detener flujo del programa
    error();
    while (1);// mediante bucle infinito
  }
  
pinMode(buzzer, OUTPUT);
pinMode(led_rojo, OUTPUT);
pinMode(led_azul, OUTPUT);


//SD.begin(4); //Initialize the SD card reader
if (!SD.begin(4)) {      // inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    error(); 
    return;         // se sale del setup() para finalizar el programa
  }

servomotor.attach(3);
servomotor.write(0);

altitud_init=bmp180.readAltitude();//Lee altitud en metros

delay (1000);
}
 
void loop() {
tempC = bmp180.readTemperature(); //  Lee temperatura en *C
pressure=bmp180.readPressure(); //Le presion en pascal
altitud=bmp180.readAltitude();//Lee altitud en metros
altura = altitud - altitud_init;
 
bmp180_Data = SD.open("PTData.txt", FILE_WRITE);
conectado();

if (bmp180_Data) {
Serial.print(tempC);
Serial.print("degrees C,");
Serial.print(pressure);
Serial.print("Pa, ");
Serial.print(altitud);
Serial.print("m, ");
Serial.print(altura);
Serial.println("m, ");
delay(150); //Pause between readings.
 
 
bmp180_Data.print(tempC);                             //write temperature data to card
bmp180_Data.print(",");  
bmp180_Data.print(pressure);                             //write temperature data to card
bmp180_Data.print(","); //write a commma
bmp180_Data.print(altitud);  
bmp180_Data.print(","); //write a commma
bmp180_Data.print(altura);
bmp180_Data.close();                                  //close the file
}


         
        if (altura_max < altura)
        {
          altura_max = altura;
        }
        if (altura_max > altura+2.1)
        {
           servomotor.write(180);  
               
        }


      
}


void conectado() 
{

  digitalWrite(led_azul, HIGH);
  tone(buzzer, 4100, 180);

}

void error()
{
  digitalWrite(led_rojo, HIGH);
  tone(buzzer, 1000, 500);
  delay(600);
  tone(buzzer, 1000, 500);
  delay(600);
}
