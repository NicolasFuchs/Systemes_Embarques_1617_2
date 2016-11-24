#pragma once
#ifndef SEG7_H
#define SEH7_H
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
 * Project:  HEIA-FR / Embedded Systems 1 Laboratory
 *
 * Abstract: Introduction the C programming language
 *
 * Purpose:  Module for managing the 7 segments displays of the HEIA-FR extension
 *           card of the Beaglebone Black board
 *
 * Author: 	Jonathan Rial & Alan Sueur
 * Date: 	November 2016
 */

/**
 * Méthode d'initialisation de l'affichage 7 segments
 */
void seg7_init();

/**
 * Méthode pour afficher une valeur sur l'arffichage 7 segments
 *
 * @param chiffre à afficher
 */
void seg7_display_value(int32_t counter);

#endif
