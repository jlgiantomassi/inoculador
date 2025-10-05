#ifndef Variables_h
#define Variables_h
//Variables, constantes, objetos y funciones de interrupciones del entorno

struct Datos{  //estructura de datos para guardar en eeprom
  float kCaudal;
  float kFlujo;
  float kgs;
  float lts;
};
Datos dato; //creamos la variable que vamos a utilizar para almacenar los datos 

const byte pinCaudalimetro = 2; //pin que maneja el caudalimetro
const byte pinFlujo = 3; //pin que maneja el flujo de semilla
const byte pinValvula = 13; //pin que maneja la valvula
const byte pinBomba = 11; //salida al modulo de control ENA regula la tension de la bomba
const byte pinBombaEncendida = 12; //salida al modulo de control IN1 para encender o apagar la bomba



Caudalimetro caudalimetro(pinCaudalimetro, 70.10, 1); //pin,factorK (70.10), caudal inicial lts por min 
                                        //cable negro --> GND -- rojo --> 5V -- amarillo -->Señal pin 2

Caudalimetro flujoSemilla(pinFlujo, 0.068, 85);  //pin sensor semilla, factor k (0.068), flujo inicial de semilla por min k medida 
                                          //sensor inductivo cable marron vcc,cable celeste gnd, cable negro señal de datos pin 3
Valvula valvula(pinValvula);
Bomba bomba(pinBomba,pinBombaEncendida,200); //se inicializa con un valor intermedio del rango utilizado (rango 147[6V]-255[12V])
LiquidCrystal_I2C lcd(0x3F, 16, 2); //Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas SCL-A5  , SDA-A4
                                    //violeta --> SCL-A5 -- azul --> SDA-A4 --  verde -- VCC-5V -- amarillo --> GND


float cantidadSemilla=3000;
float cantidadLiquido=15;

//temporizadores
unsigned long tiempoFinal = 0;
unsigned long t1 = 0;
unsigned long tiempo;
unsigned long t;
bool inicio = true;

#define COUNT(x) sizeof(x)/sizeof(*x)                   // Macro para contar el numero de elementos de un array

const byte rowsLCD    = 2;                              // Filas del LCD
const byte columnsLCD = 16;                             // Columnas del LCD

enum Button { Unknown, Ok, Up, Down, Left, Right, Cancel } btnPressed;  // Enumerador con los diferentes botones disponibles


//interrupciones del caudalimetro
void ISRCountCaudal()
{
  caudalimetro.setPulse();
}

//interrupciones del sensor de semilla
void ISRCountSemilla()
{
  flujoSemilla.setPulse();
}
#endif
