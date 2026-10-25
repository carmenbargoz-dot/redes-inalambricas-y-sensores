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
#include "dev/leds.h" /*BIBLIOTECA DE LOS LEDS*/
#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(parpadeo_1_process, "conmuta el led 1");
PROCESS(parpadeo_2_process, "conmuta el led 2"); /*DECLARACION DEL PROCESO Y DESCRIPCION */
PROCESS(timer_process, "envia los eventos");
AUTOSTART_PROCESSES(&timer_process); /*INCA QUE PROCESOS DEBEN INICIARSE CUANDO ARRANCA LA APLICACION */
/*---------------------------------------------------------------------------*/


PROCESS_THREAD(parpadeo_1_process, ev, data) /*IMPLEMENTACION DEL PROCESO (NOMBRE, IDENTIFICADOE DEL EVENTO RECIBIDO Y PUNTERO A LOS DATOS ASOCIADOS AL EVENTO*/
{

  static struct etimer timer; /* DECLARACION DEL EVENTO. etimer PERMITE QUE EL PROCESO ESPERE UN UNTERVALO DE TIEMPO DETERMINADO SIN COMPROBAR CONTINUAMENTE EL RELOJ. static DETERMINA QUE LA VARIABLE MANTIEN SE VALOR */

  PROCESS_BEGIN();

  /* Setup a periodic timer that expires after 5 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 2); /* 3 representa un intervalo de 3 segundos. */
  
  while(1) {
  
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer)); /*ESPERA PARA ESCRIBIR*/
  
    leds_single_toggle(LEDS_LED1); /*INVIERTE EL ESTADO DEL LED*/
    etimer_reset(&timer); /*RESETEA LA CUENTA*/
    
  }

  PROCESS_END();
}
PROCESS_THREAD(parpadeo_2_process, ev, data)
{
  static struct etimer timer; /* DECLARACION DEL EVENTO. etimer PERMITE QUE EL PROCESO ESPERE UN UNTERVALO DE TIEMPO DETERMINADO SIN COMPROBAR CONTINUAMENTE EL RELOJ. static DETERMINA QUE LA VARIABLE MANTIEN SE VALOR */

  PROCESS_BEGIN();

  /* Setup a periodic timer that expires after 5 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 3); /* 3 representa un intervalo de 3 segundos. */
  
  while(1) {
  
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer)); /*ESPERA PARA ESCRIBIR*/
  
    leds_single_toggle(LEDS_LED2); /*INVIERTE EL ESTADO DEL LED*/
    etimer_reset(&timer); /*RESETEA LA CUENTA*/
    
  }

  PROCESS_END();
}
PROCESS_THREAD(timer_process, ev, data)
{
  static struct etimer timer; /* DECLARACION DEL EVENTO. etimer PERMITE QUE EL PROCESO ESPERE UN UNTERVALO DE TIEMPO DETERMINADO SIN COMPROBAR CONTINUAMENTE EL RELOJ. static DETERMINA QUE LA VARIABLE MANTIEN SE VALOR */

  PROCESS_BEGIN();

  /* Setup a periodic timer that expires after 5 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 5); /* etimer_set(&temporizador, intervalo); DONDE &timer indica qué temporizador queremos configurar. CLOCK_SECOND representa la cantidad de unidades de reloj correspondientes a un segundo. CLOCK_SECOND * 5 representa un intervalo de 5 segundos. */

  while(1) {
  
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer)); /*ESPERA PARA ESCRIBIR*/
  
    process_start(&parpadeo_1_process, NULL); /*llama a un proceso*/
    process_start(&parpadeo_2_process, NULL); /*llama a un proceso*/
    
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
