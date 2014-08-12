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
 

#ifndef Physics_hh
#define Physics_hh 1

#include "G4VUserPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "Tools.hh"

/*The PhysicsList is responsible for generating processes in Geant4. */

class PhysicsList : public G4VUserPhysicsList {
  public:
    PhysicsList( );
    ~PhysicsList( );
  protected:
    /*Construct particles (not done here but in ParticleTable) */
    void ConstructParticle( );

    /* Construct physics */
    void ConstructProcess( );

    /*Set the particle cuts by G4 (these are NOT the step cuts!) */
    void SetCuts( );

  protected:
    /* Iterate over all particle-types and add the corresponding
     * processes. */
    void ConstructEM( );

    void ConstructHad( );

};

#endif /* Physics_hh */
