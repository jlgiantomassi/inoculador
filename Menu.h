#ifndef Menu_h
#define Menu_h

#include "Funciones.h"

//void inocular1();
//void limpieza1();
void irACalibraciones();
void irAConstantes();
void volverAlMenu();
void editarSemilla();
void editarLiquido();

//se declaran las lineas de la pantalla principal
LiquidLine linea1(1, 0, "Comenzar");
LiquidLine linea2(1, 1, "Limpieza");
LiquidLine linea3(1, 0, "Calibraciones");
LiquidLine linea4(1, 1, "Constantes");
LiquidScreen pantallaPrincipal(linea1, linea2, linea3, linea4);

// --- Submenu Calibraciones ---
LiquidLine cal1(1, 0, "Cal. Semilla");
LiquidLine cal2(1, 1, "Cal. Liquido");
LiquidLine cal3(1, 0, "Sem x Liq");
LiquidLine calBack(1, 1, "Volver");
LiquidScreen pantallaCal(cal1, cal2, cal3, calBack);

// --- Submenu Constantes ---
LiquidLine const1(1, 0, "Semillas:");
LiquidLine const2(1, 1, "Liquido:");
LiquidLine constBack(1, 0, "Volver");
LiquidLine constEmpty(1, 1, "");
LiquidScreen pantallaConst(const1, const2, constBack,constEmpty);

// --- Menu general ---
LiquidMenu menu(lcd, pantallaPrincipal, pantallaCal, pantallaConst);

void cargarMenu(){
  //inicializamos el menu
  menu.init();
  /*asignamos en que lugar va el indicador de las lineas del menu*/
  linea1.set_focusPosition(Position::LEFT);
  linea2.set_focusPosition(Position::LEFT);
  linea3.set_focusPosition(Position::LEFT);
  linea4.set_focusPosition(Position::LEFT);
  cal1.set_focusPosition(Position::LEFT);
  cal2.set_focusPosition(Position::LEFT);
  cal3.set_focusPosition(Position::LEFT);
  calBack.set_focusPosition(Position::LEFT);
  const1.set_focusPosition(Position::LEFT);
  const2.set_focusPosition(Position::LEFT);
  constBack.set_focusPosition(Position::LEFT);

  /**asignamos las funciones a cada linea***/
  linea1.attach_function(1, inciarInoculado);
  linea2.attach_function(1, limpieza);
  linea3.attach_function(1, irACalibraciones);
  linea4.attach_function(1, irAConstantes);

  // Calibraciones
  cal1.attach_function(1, calSemilla);
  cal2.attach_function(1, calLiquido);
  cal3.attach_function(1, calPesoLiquido);
  calBack.attach_function(1, volverAlMenu);

  // Constantes → ahora editables
  const1.attach_function(1, editarSemilla);
  const2.attach_function(1, editarLiquido);
  constBack.attach_function(1, volverAlMenu);

  //definir cantidad de lineas por pantalla
  pantallaPrincipal.set_displayLineCount(2);
  pantallaCal.set_displayLineCount(2);
  pantallaConst.set_displayLineCount(2);

  //foco del menu en la pasicion 0
  menu.set_focusedLine(0);
  //actualizamos el menu
  menu.update();
}


// --- Funciones principales ---
void inocular1() {
  lcd.clear();
  lcd.print("Inoculando...");
  delay(2000);
  menu.change_screen(&pantallaPrincipal);
}

void limpieza1() {
  lcd.clear();
  lcd.print("Limpieza...");
  delay(2000);
  menu.change_screen(&pantallaPrincipal);
}

// --- Submenus ---
void irACalibraciones() {
  menu.change_screen(&pantallaCal);
  menu.set_focusedLine(0);
}

void irAConstantes() {
  menu.change_screen(&pantallaConst);
  menu.set_focusedLine(0);
}

void volverAlMenu() {
  menu.change_screen(&pantallaPrincipal);
}


void editarSemilla(){
  lcd.clear();
  lcd.print("editando semilla");
  delay(2000);
  menu.change_screen(&pantallaConst);
}

void editarLiquido(){
  lcd.clear();
  lcd.print("editando liquido");
  abrirYCerrarValvula();
  menu.change_screen(&pantallaConst);
}
#endif
