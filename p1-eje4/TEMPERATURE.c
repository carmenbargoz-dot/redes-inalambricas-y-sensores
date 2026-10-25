/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include <stdio.h> /* For printf() */
#include "temperature-sensor.h"
#include "nrf_temp.h"
/*---------------------------------------------------------------------------*/
PROCESS(sensor_process, "lee el sensor");
PROCESS(temporizador_process, "temporizador de 3 segundos"); /*DECLARACION DEL PROCESO Y DESCRIPCION */

AUTOSTART_PROCESSES(&sensor_process, &temporizador_process); /*INDICA QUE PROCESOS DEBEN INICIARSE CUANDO ARRANCA LA APLICACION */
/*---------------------------------------------------------------------------*/


PROCESS_THREAD(sensor_process, ev, data) /*IMPLEMENTACION DEL PROCESO (NOMBRE, IDENTIFICADOE DEL EVENTO RECIBIDO Y PUNTERO A LOS DATOS ASOCIADOS AL EVENTO*/
{

  PROCESS_BEGIN();
 static int32_t temperatura_cruda, temp_grados, temp_resto;
  
  /*Activamos el sensor de temperatura*/
  SENSORS_ACTIVATE(temperature_sensor);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_POLL); /*ESPERA A QUE UN PROCESO SEA PUBLICADO EN EL EVENTO*/
    /*Leemos la temperatura*/
   temperatura_cruda= temperature_sensor.value(0);
   /* Como aparece en nrf_temp.h lo que devuelve la función es el número de 0.25ºC steps que detecta el sensor*/
   temp_grados= temperatura_cruda/4;
   temp_resto= temperatura_cruda%4;
   printf("Temperatura: %ld.%02ld ºC\n", temp_grados, temp_resto); 
  }
  SENSORS_DEACTIVATE(temperature_sensor);
  PROCESS_END(); /*Indica al kernel que el proceso ha concluido para que lo elimine de la lista de procesos activos*/
}
PROCESS_THREAD(temporizador_process, ev, data)
{
  static struct etimer timer; /* DECLARACION DEL EVENTO. etimer PERMITE QUE EL PROCESO ESPERE UN UNTERVALO DE TIEMPO DETERMINADO SIN COMPROBAR CONTINUAMENTE EL RELOJ. static DETERMINA QUE LA VARIABLE MANTIEN Su VALOR */

  PROCESS_BEGIN();

  /* Setup a periodic timer that expires after 5 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 3); /* 3 representa un intervalo de 3 segundos. */
  
  while(1) {
  
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer)); /*ESPERA PARA ESCRIBIR*/
  
    process_poll(&sensor_process); /*llama a un proceso*/
    etimer_reset(&timer); /*RESETEA LA CUENTA*/
    
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
