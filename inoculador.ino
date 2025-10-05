#include <Keypad.h>
#include <LiquidMenu.h>
#include <LiquidCrystal_I2C.h>
#include "Valvula.h"
#include "Caudalimetro.h"
#include "Bomba.h"
#include <Wire.h>
#include <EEPROM.h>
#include "Variables.h"
#include "Teclado.h"
#include "Menu.h"

/*conexion de pantalla
SDA a A4
SCL a A5
*/

/*conexion sensor semilla
cable negro a pin3
cable marron a 5V
cable azul a GND
*/

/*conexion caudalimetro
cable azul a pin2
*/



void setup() {
  
  attachInterrupt(digitalPinToInterrupt(2), ISRCountCaudal, RISING); //asignamos la interrupcion al caudalimetro y la funcion para contar pulsos
  attachInterrupt(digitalPinToInterrupt(3), ISRCountSemilla, RISING); //asignamos la interrupcion al caudalimetro y la funcion para contar pulsos
  
  teclado.setDebounceTime(100);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();  //Encender la luz de fondo.

  // Escribimos el Mensaje en el LCD.
  lcd.print("Bienvenidos");
  Serial.println("Bienvenidos ");
  lcd.begin(16, 2);

  cargarMenu();
  
  //cargamos datos de la eeprom
  EEPROM.get(0,dato);
  caudalimetro.setFactorK(dato.kCaudal);
  flujoSemilla.setFactorK(dato.kFlujo);

  valvula.cerrarValvula();
}

void loop() {
  char tecla = teclado.getKey();
  if (tecla) {
    if (tecla == '2') {  // mover hacia arriba
      menu.switch_focus(false);
      //menu.next_screen();
    } else if (tecla == '8') {  // mover hacia abajo
      menu.switch_focus(true);
      //menu.previous_screen();
    } else if (tecla == '#') {  // seleccionar
      menu.call_function(1);
    } else if (tecla =='*'){ //cancelar
      menu.change_screen(&pantallaPrincipal);
      menu.set_focusedLine(0);
    }
    menu.update();
  }
}


