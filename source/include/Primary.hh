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
 

#ifndef Primary_hh
#define Primary_hh 1

#include "Tools.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"

/* The PrimaryAction generates particles recieved from GPT. */
/* It supplies functions to set G, direction, position and the type
 * of the particle */
/* to be produced next. 'GeneratePrimaries' is called by Geant4
 * every event. */

/* ATTENTION!\n */
/* 'setDef' has to be called BEFORE setG!\n */
/* (The definition is used to get the particles mass...)\n */

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
    PrimaryGeneratorAction( );
    ~PrimaryGeneratorAction( );
  public:
    /*Set the particles pre-step position */
    void inline
    setPos( G4ThreeVector pos )
    {
      particleGun->SetParticlePosition( pos );
    }
    /*Set the direction of the particle. */
    void inline
    setDir( G4ThreeVector dir )
    {
      particleGun->SetParticleMomentumDirection( dir );
    }
    /*Set the particles definition */
    void setDef( const char* name );

    /*Set the particles definition */
    void setDef( G4ParticleDefinition* def );

    /*Set the lorentz-factor of the particle. */
    /* ATTENTION!\n */
    /* 'setDef' has to be called BEFORE setG!\n */
    /* (The definition is used to get the particle's mass...)\n */
    void setG( double G );

    /*Generate the particle (this function is called by G4 Kernel) */
    void GeneratePrimaries( G4Event* anEvent );

  private:
    /*The particleGun is used to generate particles. */
    G4ParticleGun* particleGun;
};

#endif /* Primary_hh */
