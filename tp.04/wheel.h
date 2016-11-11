/*
 * wheel.h
 *
 *  Created on: Nov 11, 2016
 *      Author: lmi
 */

#pragma once
#ifndef WHEEL_H_
#define WHEEL_H_

enum wheel_states {NONE,INCR,DECR,RESET};
void wheel_init();
enum wheel_state wheel_get_state();

#endif /* WHEEL_H_ */
