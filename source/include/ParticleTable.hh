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
 

#ifndef ParticleTable_hh
#define ParticleTable_hh 1

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "Tools.hh"

/*Particle Table */
class ParticleTable {
  public:
    ParticleTable( );

    ~ParticleTable( );

    /*Add a definition name to the table */
    void addParticleName( char* name );

    /*Register the particle definitions to G4 */
    void registerParticleDefs( );

    /*Find a definition for the supplied mass 'm' and charge 'q' */
    G4ParticleDefinition* getParticleDef( double m, double q );

    /*Returns a pointer to the global instance of ParticleTable. If
     * it does not exist, this function creates a new one. */
    static ParticleTable*
    getInstance( )
    {
      return ( instance == NULL ) ? ( instance =
                                        new ParticleTable( ) ) : instance;
    }
  private:
    /*The names of the definitions */
    std::vector< G4String > definition_names;

    /*Definitions */
    std::vector< G4ParticleDefinition* > definitions;

    /*Add a definition to the table */
    void addParticleDef( G4ParticleDefinition* def );

    static ParticleTable* instance;
};
#endif /* ParticleTable_hh */
