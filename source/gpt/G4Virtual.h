/**
 * Copyright 2009 Lucas Clemente
 *
 * This file is part of FLINT.
 *
 * FLINT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FLINT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FLINT.
 * If not, see <http://www.gnu.org/licenses/>.
 */
 


#ifndef G4Virtual_h
#define G4Virtual_h 1

#include <stdio.h>
#include <math.h>

#include "elem.h"

#include "mainG4.hh"

/*\todo Hopefully temporally... */
#define NUMPARS_SUPPORTED 10000000

#define OUTPUT_INTERVAL 100

/*The structure in which old information is kept */
typedef struct
{
  double x, y, z;
  double G;
  char stored;
} old_par;

/*Index of the step */
int step;

/*Number of particles calculated using G4 (output) */
int pars_g4;

/*This variable keeps a second particles-array */
old_par* pars_old;

#endif /* G4Virtual_h */
