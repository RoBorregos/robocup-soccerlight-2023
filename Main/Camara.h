#pragma once

//#include <openmvrpc.h>
//openmv::rpc_scratch_buffer<256> scratch_buffer; // All RPC objects share this buffer.
//openmv::rpc_hardware_serial3_uart_master interface(115200);


class Camara {
  public: 
    int porteria;
    //openmv::rpc_hardware_serial3_uart_master interface(115200);
    
    Camara(){
      
    }
    
    void iniciar(){
      interface.begin();
      Serial.begin(115200);
    }
    


void exe_porteria_detection(openmv::rpc_hardware_serial3_uart_master &interface){
  int8_t color_thresholds[6] = {52, 97, -25, 10, 33, 90}; // generic_red_thresholds
    int8_t color_thresholdsBlue[6] = {21, 60, -6, 30, -74, -28}; // generic_green_thresholds
    // int8_t color_thresholds[6] = {0, 30, 0, 64, -128, 0}; // generic_blue_thresholds
    struct { 
      uint16_t cx, cy; 
    }
    ﻿detectar_porteria_result;
    
    if (interface.call(F("﻿detectar_porteria"), color_thresholds, sizeof(color_thresholds), &﻿detectar_porteria_result, sizeof(detectar_porteria_result))) {
        Serial.print(F("Largest Color Detected [cx="));
        Serial.print(detectar_porteria_result.cx);
        Serial.print(F(", cy="));
        Serial.print(detectar_porteria_result.cy);
        Serial.println(F("]"));

        if (detectar_porteria_result.cx > 200){
          //motoresRobot.movimientoLineal(45, velocidades);  
          Serial.println("Derecha");
        } else if (detectar_porteria_result.cx < 40) {
          //motoresRobot.movimientoLineal(-45, velocidades);
          Serial.println("Izquierda");
        } else {
          // motoresRobot.setAllMotorSpeed(velocidades);
           //motoresRobot.adelante();
           Serial.println("Adelante");
        }
    }
  }
//void exe_color_detection()
//{
//    int8_t color_thresholds[6] = {52, 97, -25, 10, 33, 90}; // generic_red_thresholds
//    int8_t color_thresholdsBlue[6] = {21, 60, -6, 30, -74, -28}; // generic_green_thresholds
//    // int8_t color_thresholds[6] = {0, 30, 0, 64, -128, 0}; // generic_blue_thresholds
//    struct { 
//      uint16_t cx, cy; 
//    }
//    color_detection_result;
//    
//    if (interface.call(F("color_detection"), color_thresholds, sizeof(color_thresholds), &color_detection_result, sizeof(color_detection_result))) {
//        Serial.print(F("Largest Color Detected [cx="));
//        Serial.print(color_detection_result.cx);
//        Serial.print(F(", cy="));
//        Serial.print(color_detection_result.cy);
//        Serial.println(F("]"));
//
//        if (color_detection_result.cx > 200){
//          //motoresRobot.movimientoLineal(45, velocidades);  
//          Serial.println("Derecha");
//        } else if (color_detection_result.cx < 40) {
//          //motoresRobot.movimientoLineal(-45, velocidades);
//          Serial.println("Izquierda");
//        } else {
//          // motoresRobot.setAllMotorSpeed(velocidades);
//           //motoresRobot.adelante();
//           Serial.println("Adelante");
//        }
//    }
//}

//char porteria (String input, char last){
//      char lastSeen = last;
//      //int digitIn = input - 48;
//
//      //Si se ve la portería (recibe un valor 0-9)
//      if (input != 'a'){
//  
//            if (digitIn > 7){  //Derecha
//              motoresRobot.movimientoLineal(45, velocidades);  
//              lastSeen  = 'd';
//              digitalWrite(ledRojo, HIGH);
//              //Serial.println(digitIn);
//              Serial.println("Derecha");
//        
//                    
//            } else if (digitIn < 2) {  //Izquierda
//              motoresRobot.movimientoLineal(-45, velocidades);
//              lastSeen = 'i';
//              digitalWrite(ledVerde, HIGH);
//              //Serial.println(digitIn);
//              Serial.println("Izq");
//
//
//            } else {  //Centro
//              motoresRobot.setAllMotorSpeed(velocidades);
//              motoresRobot.adelante();
//              digitalWrite(ledVerde, LOW);
//              //digitalWrite(ledRojo, LOW);
//              Serial.println("Adelante");
//                          
//            }
//          
//      } else {
//              motoresRobot.setAllMotorSpeed(120);
//              Serial.println("nada");
//
//
//              if (lastSeen = 'i') {
//                    motoresRobot.giroAH();
//                  
//               } else {
//                    motoresRobot.giroH();
//          
//                 }
//     }
//
//  //Retorna el último lugar en donde vió la portería (izquierda o derecha)
//  return lastSeen;
//}
};
