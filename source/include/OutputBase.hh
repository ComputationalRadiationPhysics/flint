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
 


#ifndef OutputBase_hh
#define OutputBase_hh 1

#include "G4ParticleDefinition.hh"

/*Just another particle-type used for passing particles in output */
struct OutputParticle
{
  int ID;
  double x, y, z;
  double v_x, v_y, v_z;
  double E;
  double E_loss;
  double m;
  double q;
  G4ParticleDefinition* definition;
};

/*This class is the base class for all outputs. */
class OutputBase {
  public:
    OutputBase( )
    {
    };

    virtual ~OutputBase( )
    {
    }

    /*Output initial data like geometry, ... */
    virtual void
    outputInitData( )
    {
    }

    /*This function is used to output information */
    virtual void
    outputData( OutputParticle* p )
    {
    }

    /*Write all unsaved data. Last function to be called before
     * deleting OutputManager. */
    virtual void
    finalize( )
    {
    }
};

#endif /* OutputBase_hh */
