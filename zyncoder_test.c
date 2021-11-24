/*
 * ******************************************************************
 * ZYNTHIAN PROJECT: Zyncoder Library Test
 * 
 * Test switches & encoders for zynthian kit configurations
 * 
 * Copyright (C) 2015-2021 Fernando Moyano <jofemodo@zynthian.org>
 *
 * ******************************************************************
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the LICENSE.txt file.
 * 
 * ******************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "zynpot.h"
#include "zyncoder.h"
#include "zyncontrol.h"

#ifndef HAVE_WIRINGPI_LIB
	#define NUM_ZYNSWITCHES 4
	unsigned int zyncoder_pin_a[4]={4,5,6,7};
	unsigned int zyncoder_pin_b[4]={8,9,10,11};
	unsigned int zynswitch_pin[NUM_ZYNSWITCHES]={0,1,2,3};
#else
	#ifdef MCP23017_ENCODERS
		#define NUM_ZYNSWITCHES 8
		unsigned int zyncoder_pin_a[4] = { 102, 105, 110, 113 };
		unsigned int zyncoder_pin_b[4] = { 101, 104, 109, 112 };
		unsigned int zynswitch_pin[NUM_ZYNSWITCHES]  = { 100, 103, 108, 111, 106, 107, 114, 115 };
	#else
		#define NUM_ZYNSWITCHES 4
		//PROTOTYPE-3
		//unsigned int zyncoder_pin_a[4]={27,21,3,7};
		//unsigned int zyncoder_pin_b[4]={25,26,4,0};
		//PROTOTYPE-4
		unsigned int zyncoder_pin_a[4]={26,25,0,4};
		unsigned int zyncoder_pin_b[4]={21,27,7,3};
		unsigned int zynswitch_pin[NUM_ZYNSWITCHES]={107,23,106,2};
	#endif
#endif

int main() {
	int i;

	printf("Starting ZynCore...\n");
	init_zyncontrol();
	init_zynmidirouter();

	printf("Setting-up %d x Zynswitches\n", NUM_ZYNSWITCHES);
	for (i=0;i<NUM_ZYNSWITCHES;i++) {
		setup_zynswitch(i,zynswitch_pin[i]);
	}

	printf("Setting-up 4 x Zyncoders\n");
	for (i=0;i<4;i++) {
		setup_zyncoder(i,zyncoder_pin_a[i],zyncoder_pin_b[i]);
		setup_zynpot(i,ZYNPOT_ZYNCODER,i);
		setup_rangescale_zynpot(i,0,127,64,0);
	}

	printf("Testing switches & rotaries...\n");
	while(1) {
		for (i=0;i<NUM_ZYNSWITCHES;i++) {
			int ts = get_zynswitch(i,2000000);
			if (ts>0) fprintf(stdout, "SW-%d = %d\n", i, ts);
		}
		for (i=0;i<4;i++) {
			if (get_value_flag_zynpot(i)) {
				printf("PT-%d = %d\n", i, get_value_zynpot(i));
			}
		}
		usleep(5000);
	}

	return 0;
}
