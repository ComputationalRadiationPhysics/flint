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
 


#ifndef OutputManager_hh
#define OutputManager_hh 1

#include <vector>

#include "G4Step.hh"
#include "G4String.hh"

#include "Containers.hh"
#include "ParticleTable.hh"
#include "OutputBase.hh"

/*This class manages the output of all important data from the
 * simulation. */
class OutputManager {
  private:
    /*All outputs */
    std::vector< OutputBase* > outputs;

    /*Global instance */
    static OutputManager* instance;
  public:
    OutputManager( );

    ~OutputManager( );

    /*Add an output */
    void addOutput( OutputBase* output );

    /*Output initial data like geometry, ... */
    void outputInitData( );

    /*This function is used directly by G4Calc to output general
     * information */
    void outputData( int ID,
                     double x,
                     double y,
                     double z,
                     double v_x,
                     double v_y,
                     double v_z,
                     double E,
                     double E_loss,
                     double m,
                     double q );

    /*Write all unsaved data. Last function to be called before
     * deleting OutputManager. */
    void finalize( );

    /*Returns a pointer to the global instance of OutputManager. If
     * it does not exist, this function creates a new one. */
    static OutputManager*
    getInstance( )
    {
      return ( instance == NULL ) ? ( instance =
                                        new OutputManager( ) ) : instance;
    }
};

#endif /* OutputManager_hh */
