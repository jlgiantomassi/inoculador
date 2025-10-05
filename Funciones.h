#ifndef Funciones_h
#define Funciones_h

// Prototipos de las funciones de menú
void iniciarInoculado();
void limpieza();
void calLiquido();
void calSemilla();
void calKgsLts();
void KLiquido();
void kSemilla();
void abrirYCerrarValvula();


Button readButtons();
//void openMenu();

unsigned long tiempoInicio;
unsigned long intervaloMedicion = 500;

void inciarInoculado() {
  float Lmin;
  float Pmin = 0;
  float porc;
  int giro;
  float caudal;
  unsigned long tInicio;
  int tDisplay = 4000;  //tiempo en que tarda en refrescar la informacion en el display
  bool mostrarFlujo = true;

  lcd.clear();
  lcd.print("Aguarde...");

  valvula.abrirValvula();  //se abre la valvula por 3seg para largar un poco de producto y esperar a que pase la semilla
  bomba.encender();
  delay(3000);
  bomba.apagar();
  valvula.cerrarValvula();

  lcd.clear();
  lcd.print("Inicie el Proceso");

  tiempoInicio = millis();
  caudalimetro.setPulse(0);
  while (Pmin == 0)  //espera a que empiece a pasar semilla
  {
    if (millis() - tiempoInicio > intervaloMedicion) {
      Pmin = flujoSemilla.calcularCaudal(intervaloMedicion);  //Peso por minutos de semilla
      flujoSemilla.setPulse(0);
      tiempoInicio = millis();
    }
  }
  lcd.clear();
  lcd.print("Proceso");
  lcd.setCursor(0, 1);
  lcd.print("Iniciado");

  tiempoInicio = millis();
  tInicio = millis();  //tiempo inicial para visualizar en pantalla
  caudalimetro.setPulse(0);
  flujoSemilla.setPulse(0);
  do {

    if (!caudalimetro.getSinAgua()) {


      if (millis() - tiempoInicio > intervaloMedicion) {
        Pmin = flujoSemilla.calcularCaudal(intervaloMedicion);  //Peso por minutos de semilla
        //Pmin=random(80,90);
        Lmin = caudalimetro.calcularCaudal(intervaloMedicion);
        caudal = Pmin * (cantidadLiquido / cantidadSemilla);  //nuevo caudal a regular

        caudalimetro.setCaudal(caudal);
        if (Pmin > 2) {
          flujoSemilla.sumarVolumen(Pmin, intervaloMedicion);
        }
        if (Lmin > 0.07) {
          caudalimetro.sumarVolumen(Lmin, intervaloMedicion);
        }
        tiempoInicio = millis();
        caudalimetro.setPulse(0);
        flujoSemilla.setPulse(0);

        porc = caudalimetro.errorCaudal();  //calcula el error del caudal en porcentaje. Si es positivo el cuadual es mayor al deseado.
        /*
          if (Pmin > 0)
          {
          Serial.print("El caudal medido es: ");
          Serial.println(Lmin);
          Serial.print("El flujo medido es: ");
          Serial.println(Pmin);
          Serial.print("El caudal calculado es: ");
          Serial.println(caudal);
          Serial.print("El error es: ");
          Serial.println(porc);
          Serial.println("");
          }
        */
        if (porc > 0) {
          giro = -1;  //cierra la valvula
        } else {
          giro = 1;  //abre la valvula
        }
        /* PID para calcular el estado de la bomba */
        /****
        ****
        hacer codigo con libreria PID
        ****/
        bomba.setEstado(55);
        delay(intervaloMedicion);
      }

      if (millis() - tInicio > tDisplay) {
        if (mostrarFlujo)  //muestra el flujo, sino muestra la cantidad consumida. Ira cambiando cada (tDisplay) milisegundos
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(Lmin);
          lcd.setCursor(6, 0);
          lcd.print("Lts/min");

          lcd.setCursor(0, 1);
          lcd.print(round(Pmin));
          lcd.setCursor(6, 1);
          lcd.print("Kgs/min");

          mostrarFlujo = false;
          tInicio = millis();
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Total Lts:");
          lcd.setCursor(11, 0);
          lcd.print(round(caudalimetro.getVolumen()));

          lcd.setCursor(0, 1);
          lcd.print("Total Kgs:");
          lcd.setCursor(11, 1);
          lcd.print(round(flujoSemilla.getVolumen()));

          mostrarFlujo = true;
          tInicio = millis();
        }
      }

      if ((Lmin < 0.1) && (valvula.estadoValvula())) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cerrando Valvula");
        delay(1500);
        bomba.apagar();
        valvula.cerrarValvula();
        caudalimetro.setSinAgua(true);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sin Agua en");
        lcd.setCursor(0, 1);
        lcd.print("el sistema");
        delay(2000);
      }
    }


    btnPressed = readButtons();
  } while (btnPressed != Button::Cancel);

  valvula.cerrarValvula();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Proceso");
  lcd.setCursor(0, 1);
  lcd.print("Finalizado");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kgs:");
  lcd.setCursor(4, 0);
  lcd.print(round(flujoSemilla.getVolumen()));

  lcd.setCursor(0, 1);
  lcd.print("Lts:");
  lcd.setCursor(4, 1);
  lcd.print(round(caudalimetro.getVolumen()));

  delay(4000);

  flujoSemilla.resetVolumen();
  caudalimetro.resetVolumen();
  //openMenu();
}

void calLiquido() {
  char tec;
  int num;
  int col = 0;
  unsigned long tProceso;
  float C1, C2, k1, kNew, Lmin;

  if (caudalimetro.getFactorK() <= 0)  //si se guardo el valor de k en 0 o menor lo modificamos y ponemos 1
    caudalimetro.setFactorK(1);
  lcd.clear();
  lcd.print("Junte Liquido");
  lcd.setCursor(0, 1);
  lcd.print("espere...");
  delay(2000);

  lcd.clear();
  lcd.print("Presione # para");
  lcd.setCursor(0, 1);
  lcd.print("empezar. espere...");
  do  //espera hasta presionar ok
  {
    btnPressed = readButtons();
    delay(100);
  } while (!((btnPressed == Button::Ok) || (btnPressed == Button::Cancel)));
  if (btnPressed != Button::Cancel) {

    lcd.clear();
    lcd.print("Proceso Iniciado");
    lcd.setCursor(0, 1);
    lcd.print("Aguarde...");
    
    valvula.abrirValvula();
    bomba.setEstado(200);
    bomba.encender();
    tiempoInicio = millis();
    tProceso = millis();
    caudalimetro.resetVolumen();
    caudalimetro.setPulse(0);
    while (millis() - tProceso < 60000) {
      if (millis() - tiempoInicio > intervaloMedicion) {
        Lmin = caudalimetro.calcularCaudal(intervaloMedicion);
        caudalimetro.setPulse(0);
        //Serial.println(Lmin);
        if (Lmin > 0.01) {
          caudalimetro.sumarVolumen(Lmin, intervaloMedicion);
        }
        tiempoInicio = millis();
      }
    }

    valvula.cerrarValvula();
    delay(1000);
    C1 = caudalimetro.getVolumen() * 1000;
    
    //Serial.print("el volumen total es: ");
    //Serial.println(round(C1));
    lcd.clear();
    lcd.print("se midieron cm3:");
    lcd.setCursor(0, 1);
    lcd.print(round(C1));
    delay(3000);
    lcd.clear();
    lcd.print("Ingrese los cm3");
    lcd.setCursor(0, 1);
    C2 = 0;
    do  //leemos numeros del teclado
    {
      tec = teclado.getKey(); //leemos el teclado en ascii
      if (tec) {
        num=tec-'0'; //convertimos el ascii en entero '0' es 48. '1' es 49, etc
        if ((num >= 0) && (num <= 9)) {
          lcd.print(num);
          col++;
          C2 = (C2 * 10) + num;
          tec='\0'; //vaciamos tec
        }
        if (tec == '*')  //con la tecla * borramos todo
        {
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          col = 0;
          C2 = 0;
          tec='\0'; //vaciamos tec
        }
      }
    } while (tec != '#'); //confirmamos con #
    
    kNew = C1 * caudalimetro.getFactorK() / C2;
    /*
      Serial.print("El volumen ingresado es: ");
      Serial.println(C2);
      Serial.print("La nueva constante k es: ");
      Serial.println(kNew);
    */
    if (kNew > 0) {
      caudalimetro.setFactorK(kNew);
      lcd.clear();
      lcd.print("la nueva K es");
      lcd.setCursor(0, 1);
      lcd.print(kNew);
      dato.kCaudal = kNew;
      EEPROM.put(0, dato);  //guardamos en la memoria

    } else {
      lcd.clear();
      lcd.print("Hubo un error en");
      lcd.setCursor(0, 1);
      lcd.print("la medicion");
    }
    
    delay(3000);
  }

  //openMenu();
}

void calSemilla() {
  char tec;
  int num;
  int col = 0;
  unsigned long tProceso;
  float P1, P2, k1, kNew, Pmin;

  if (flujoSemilla.getFactorK() <= 0)  //si se guardo el valor de k en 0 o menor lo modificamos y ponemos 1
    flujoSemilla.setFactorK(1);
  lcd.clear();
  lcd.print("Pase kg conocido");
  lcd.setCursor(0, 1);
  lcd.print("espere...");
  delay(2000);

  lcd.clear();
  lcd.print("presione # para");
  lcd.setCursor(0, 1);
  lcd.print("empezar. Espere...");

  do  //espera hasta presionar ok
  {
    btnPressed = readButtons();
    delay(100);
  } while (!((btnPressed == Button::Ok) || (btnPressed == Button::Cancel)));
  if (btnPressed != Button::Cancel) {

    lcd.clear();
    lcd.print("Empiece a pasar");
    lcd.setCursor(0, 1);
    lcd.print("la semilla.");

    tProceso = millis();
    flujoSemilla.resetVolumen();
    flujoSemilla.setPulse(0);
    do {
      if (millis() - tiempoInicio > intervaloMedicion) {
        Pmin = flujoSemilla.calcularCaudal(intervaloMedicion);
        flujoSemilla.setPulse(0);
        Serial.println(Pmin);
        if (Pmin > 2) {
          flujoSemilla.sumarVolumen(Pmin, intervaloMedicion);
        }
        tiempoInicio = millis();
      }
      btnPressed = readButtons();
    } while (btnPressed != Button::Ok);

    P1 = flujoSemilla.getVolumen();

    lcd.clear();
    lcd.print("Los Kgs medidos");
    lcd.setCursor(0, 1);
    lcd.print(round(P1));


    delay(3000);

    lcd.clear();
    lcd.print("Ingrese los Kgs");
    lcd.setCursor(0, 1);
    P2 = 0;
    do  //leemos numeros del teclado
    {
      tec = teclado.getKey();  //leemos teclado en ascii
      if (tec) {
        num = tec - '0';  // convertimos ascii en entero
        if ((num >= 0) && (num <= 9)) {
          lcd.print(num);
          col++;
          P2 = (P2 * 10) + num;
          tec = '\0';  //vaciamos tec
        }
        if (tec == '*')  //con la tecla * borramos todo
        {
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          col = 0;
          P2 = 0;
          tec = '\0';
        }
      }
    } while (tec != '#');  //confirmamos con #

    kNew = P1 * flujoSemilla.getFactorK() / P2;
    /*
      Serial.print("Los kgs ingresados son: ");
      Serial.println(P2);
      Serial.print("La nueva constante k es: ");
      Serial.println(kNew);
    */
    if (kNew > 0) {
      flujoSemilla.setFactorK(kNew);
      lcd.clear();
      lcd.print("la nueva K es");
      lcd.setCursor(0, 1);
      lcd.print(kNew);
      dato.kFlujo = kNew;
      EEPROM.put(0, dato);  //guardamos en la memoria
    } else {
      lcd.clear();
      lcd.print("Hubo un error en");
      lcd.setCursor(0, 1);
      lcd.print("la medicion");
    }
    delay(3000);
  }
  //openMenu();
}

void calPesoLiquido() {
  char tec;
  int kgs = 0;
  int lts = 0;
  int num;
  int col = 0;
  lcd.clear();
  lcd.print("Kgs totales x");
  lcd.setCursor(0, 1);
  lcd.print("Lts totales");
  delay(1000);

  lcd.clear();
  lcd.print("Ingrese los Kgs");
  lcd.setCursor(0, 1);
  do  //leemos numeros del teclado
  {
    tec = teclado.getKey();  //leemos que tecla se presiono
    if (tec) {               //si se presiono una tecla
      num = tec - '0';       //como lee el ascii del char lo transformamos a entero. en ascii el '0' es 48, el '1' es 49, etc
      if ((num >= 0) && (num <= 9)) {
        lcd.print(num);
        col++;
        kgs = (kgs * 10) + num;
        tec = '\0';  //colocamos la tec en vacio para que no vuelva a entrar
      }
      if (tec == '*')  //con la tecla * borramos todo
      {
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        col = 0;
        kgs = 0;
        tec = '\0';
      }
    }
  } while (tec != '#');


  lcd.clear();
  lcd.print("Ingrese los Lts");
  lcd.setCursor(0, 1);
  do  //leemos numeros del teclado
  {
    tec = teclado.getKey();
    if (tec) {
      num = tec - '0';
      if ((num >= 0) && (num <= 9)) {
        lcd.print(num);
        col++;
        lts = (lts * 10) + num;
        tec = '\0';
      }
      if (tec == '*')  //con la tecla # borramos todo
      {
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        col = 0;
        lts = 0;
        tec = '\0';
      }
    }
  } while (tec != '#');
  cantidadSemilla = kgs;
  cantidadLiquido = lts;
  lcd.clear();
  lcd.print("Kgs");
  lcd.setCursor(5, 0);
  lcd.print((int)kgs);
  lcd.setCursor(0, 1);
  lcd.print("Lts");
  lcd.setCursor(5, 1);
  lcd.print((int)lts);
  Serial.println(cantidadSemilla);
  Serial.println(cantidadLiquido);
  dato.kgs = kgs;
  dato.lts = lts;
  EEPROM.put(0, dato);  //guardamos en la memoria
  delay(3000);
  //openMenu();
}


void limpieza() {
  lcd.clear();
  lcd.print("Limpieza sistema");
  lcd.setCursor(0, 1);
  lcd.print("Ponga aguaLimpia");
  delay(3000);
  lcd.clear();
  lcd.print("Presione # ");
  lcd.setCursor(0, 1);
  lcd.print("para empezar");
  do  //espera hasta presionar ok
  {
    btnPressed = readButtons();
    delay(100);
  } while (!((btnPressed == Button::Ok) || (btnPressed == Button::Cancel)));
  lcd.clear();
  lcd.print("Limpiando!!!");
  lcd.setCursor(0, 1);
  lcd.print("Aguarde 1 min");

  bomba.setEstado(255);  //bomba a pleno
  bomba.encender();
  valvula.cerrarValvula();  //hacer retorno por 15seg para limpiar el retorno
  delay(15000);
  valvula.abrirValvula();  //abrimos valvula para limpiar el sistema por 30seg
  delay(30000);
  valvula.cerrarValvula();
  //openMenu();
}

void abrirYCerrarValvula() //prueba despues borrar
{
  valvula.abrirValvula();
  delay(5000);
  valvula.cerrarValvula();
}
#endif