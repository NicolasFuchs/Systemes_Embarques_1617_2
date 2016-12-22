#pragma once
#ifndef DMTIMER1_H
#define DMTIMER1_H
/**
 * Copyright 2016 University of Applied Sciences Western Switzerland / Fribourg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Project:	HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract:	Introduction to device driver development in C
 *
 * Purpose:	 Implementation of a basic timer based on the AM335x DMTimer1 timer.
 *
 * Author: 	Jonathan Rial, Alan Sueur
 * Date: 	december 2016
 */

/**
 * Cette méthode permet d'initialiser le dmtimer1.
 */
void dmtimer1_init();

/**
 * Cette méthode permet d'actualiser le temps.
 */
void dmtimer1_refresh_time();

/**
 * Cette méthode permet de démarrer le compteur.
 */
void dmtimer1_start();

/**
 * Cette méthode permet d'arrêter le compteur.
 */
void dmtimer1_stop();

/**
 * Cette méthode permet de réinitialiser le compteur.
 */
void dmtimer1_reset();

/**
 * Getter retournant le temps en heures.
 *
 * @return le temps en heures
 */
uint32_t dmtimer1_get_hours();

/**
 * Getter retournant le temps en minutes.
 *
 * @return le temps en minutes
 */
uint32_t dmtimer1_get_minutes();

/**
 * Getter retournant le temps en secondes.
 *
 * @return le temps en secondes
 */
uint32_t dmtimer1_get_seconds();

/**
 * Getter retournant le temps en millisecondes.
 *
 * @return le temps en millisecondes
 */
uint32_t dmtimer1_get_milliseconds();

/**
 * Getter retournant le temps en microsecondes.
 *
 * @return le temps en microsecondes
 */
uint32_t dmtimer1_get_microseconds();

#endif
