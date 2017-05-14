#include <Servo.h> //NO BORRAR
#include <stdio.h> //NO BORRAR
#include <stdlib.h> //NO BORRAR

//Este es un ejemplo de programacion.
//Todo lo que veas escrito despues de // es un comentario
//Esto quiere decir que no será cargado al Arduino.

/* Asi se inicia un comentario de varias lineas
  Las conexiones son las siguientes:
  Sensor de Color --------> Pin A0
  Motor Pinza ------------> Pin 9
  Motor Codo -------------> Pin 10
  Motor Hombro -----------> Pin 
  Asi se termina un comentario de varias lineas */
/*
En los lenguajes de programacion no se admiten las tildes.
Es por esto que algunas palabras las veras sin esta.
*/


void MoverLento(Servo *mservo,int pi,int pf,unsigned long t); //No borres esto, es una función.

//-------- Las siguientes variables NO SE DEBEN MODIFICAR-----------
char BUFF[100];//NO BORRAR
int Entero; //Esta es una variabla NO BORRAR
#define MANUAL 0 //NO BORRAR
#define AUTO 1 //NO BORRAR
int TipoControl=MANUAL; //Iniciar con tipo de control manual
#define Cerrar 0 //Valor de cerrado de la pinza
#define Abrir 40 // Valor de la pinza abierta
unsigned long T; //Esta es un variable NO BORRAR
int Bandera=0; //Esta es una Variable NO BORRAR
int PX=35,CX=90,HX=90; //Variables de las posiciones de los servos
int CajasNegras=0; //Valor de conteo de las cajas negras
int CajasBlancas=0; // Valor de Conteo de las cajas blancas
static Servo Pinza;
static Servo Hombro;
static Servo Codo;
int sensorValue;
//---------- Fin de variables de que no se deben modificar--------


//----- Variables que debes modificar--------  
int UmbralBlanco=100; //Si el sensor lee un valor menor a este se asume que la caja es blanca.
int UmbralNegro=130; //Si el sensor lee un valor mayor a este y menor que UmbralNada se asume que la caja es negra.
int UmbralNada=350; //Si el valor es mayor a este se asume que no hay cajas para recoger.
// ------------------------------------------


void setup(){ 
  /*En este espacio se escriben los pasos iniciales del programa*/
  Serial.begin(9600); //Iniciar comunicación
  Serial.setTimeout(60); //Definir tiempo de espera en la comunicación NO BORRAR
  Serial.println("Reiniciando..."); //Enviar la palabra "Reiniciando..."
  //--------------------------------------------------------------
  Energizar(); //Esta instrucción le da energía a los motores.
  Pinza.write(3); //Mover la pinza al grado 3
  delay(1000);
  Hombro.write(90);
  delay(400);
  Codo.write(90); 
  delay(1000);
  //----------------Fin Inicializar Servo----------------------//
}

void loop(){
  
  sensorValue= analogRead(A0); //Leer el valor del sensor de color
  Serial.print('S'); //Imprimir la letra s
  Serial.println(sensorValue); //Imprimir el valor del sensor
  int numR=0;// NO BORRAR  //Este es un un numero para saber cuantos datos se han recibido
  BUFF[0]=0;
  numR=Serial.readBytesUntil('\n',BUFF,100);//Esos leen los datos
  
  
  //CONTROL MANUAL NO MODIFICAR DURANTE EL RETO----------------------------------
  
  if(numR>0){  //Si hay datos nuevos.
    T=millis();
    BUFF[numR-1]=0; //NO BORRAR //Eliminar caracter \r y finalizar la cadena
    //Preguntar por el tipo de control
    if(BUFF[0]=='T'){
       Entero = atoi(&BUFF[1]);
       if(Entero==AUTO){
         TipoControl=AUTO;
       }else if(Entero==MANUAL){
         TipoControl=MANUAL;
       }
    }else if(BUFF[0]=='E'){
      Serial.println("E");//Enviar echo de la e
    }
     
     //Configuración dependiendo del tipo de control //NO BORRAR NI MODIFICAR
      if(TipoControl==MANUAL){      
       if(BUFF[0]=='H'){ 
         Energizar();
         Entero = atoi(&BUFF[1]);
         Entero = constrain(Entero, 0, 160);
         Serial.print((char)BUFF[0]);
         Serial.println(Entero);
         Hombro.write(Entero); 
       }else if(BUFF[0]=='C'){ 
         Energizar();
         Entero = atoi(&BUFF[1]);
         Entero = constrain(Entero, 0, 160);
         Serial.print((char)BUFF[0]);
         Serial.println(Entero);
         Codo.write(Entero); 
       }else if(BUFF[0]=='P'){ 
         Energizar();
         Entero = atoi(&BUFF[1]);
         Entero = constrain(Entero, 0, 50);
         Serial.print((char)BUFF[0]);
         Serial.println(Entero);
         Pinza.write(Entero); 
       } 
      }
     }
     //FIN DE CODIGO DE CONTROL MANUAL---------------------
     
     //CODIGO PARA CONTROL AUTOMATICO
     if(TipoControl==AUTO){     
       if(sensorValue<UmbralNada){
         CogerCaja();
         if(sensorValue<UmbralBlanco ){ //Blanco
           //Este es el codigo que se ejecuta luego de detectar la caja blanca
           // Para las cajas negras se debe copiar el codigo desde aquí hasta donde estan los guiones---
           MoverLento(&Hombro,0,20,800);
           delay(100);
           Hombro.write(160); 
           Codo.write(160);  //Cuando la caja se negra este valor lo debes cambiar para que el brazo llegue a un angulo menor.
           delay(1500);
           Pinza.write(Abrir);
           delay(1000);
           //Hasta aquí---------------------------------------------
           
           CajasBlancas++;
           Serial.print("W");
           Serial.println(CajasBlancas);
           Bandera=1;
           }else if( sensorValue>UmbralNegro&&sensorValue<UmbralNada){       
            //W=WHITE(Blancas), B=BLACK(Negras)
            //En la siguiente linea es donde debes pegar el codigo --------
          
           //--------------------------------------------------------------
           //CajasNegras++; //Debes descomentar esta linea cuando tengas listo tu codigo para tomar y llevar la caja negra
           //Serial.print("B"); //Debes descomentar esta linea cuando tengas listo tu codigo para tomar y llevar la caja negra
           //Serial.println(CajasNegras); //Debes descomentar esta linea cuando tengas listo tu codigo para tomar y llevar la caja negra
           Bandera=1;
           }else{ //Si la caja no es ni NEGRA ni BLANCA devolvemos el brazo a su posición inicial      
            Pinza.write(5);
            delay(1000);
            Hombro.write(90);
            delay(400);
            Codo.write(90); 
            delay(1000);
            Bandera=1;
          }
        }
      }

//DE AQUI EN ADELANTE NO DEBES MODIFICAR NADA DEL CODIGO 
 //Solo se debe desernegizar si se recibió una caja
 // o despues de un tiempo de espera de 10 segundo
 if(Bandera==1){
   Desenergizar();
   Serial.println("Desernegizado");
   delay(2000);
   Bandera=0;
 }else if(TipoControl==MANUAL&&(millis()-T>10000)){
   Desenergizar();
 }
}

void MoverLento(Servo *mservo,int pi,int pf,unsigned long t){
  unsigned long T0=millis();
  unsigned long DT=0;
  while(DT<=t){
    DT=millis()-T0;
    (*mservo).write(constrain(map(DT,0,t,pi,pf),pi,pf)); //map(DT,0,t,pi,pf)
    }

(*mservo).write(pf);
}

void CogerCaja(){     
      Energizar();
      Pinza.write(Abrir);
      Codo.write(0);  
      Hombro.write(0);
      MoverLento(&Hombro,Hombro.read(),20,1000);
      Hombro.write(20);
      sensorValue=analogRead(A0);
      Serial.print('S'); //Imprimir la letra s
      Serial.println(sensorValue); //Imprimir el valor del sensor
      MoverLento(&Hombro,19,0,1000);
      delay(500);
      Pinza.write(Cerrar);      
      delay(1000);
}

void Desenergizar(){

 if(Hombro.attached()){
   Hombro.detach();
   digitalWrite(11,0);
 }
 
  if(Codo.attached()){
    Codo.detach();
    digitalWrite(10,0);
  }
 
 if(Pinza.attached()){
   PX=Pinza.read();
   Pinza.detach();
   digitalWrite(9,0);
 }

}

void Energizar(){
  if(!Codo.attached()){
   Codo.attach(10);
   digitalWrite(10,0);
   }
  if(!Hombro.attached()){
    Hombro.attach(11);
    digitalWrite(11,0);
   }
   if(!Pinza.attached()){
   Pinza.attach(9); //La pinza se debe energizar al final  
   digitalWrite(9,0);
   }
}
