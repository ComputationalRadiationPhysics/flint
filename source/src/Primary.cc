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
 

#include "Primary.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction( )
{
  /* one particle per shot */
  particleGun = new G4ParticleGun( 1 );
}

PrimaryGeneratorAction::~PrimaryGeneratorAction( )
{
  delete particleGun;
}

void
PrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent )
{
  particleGun->GeneratePrimaryVertex( anEvent );
}

void
PrimaryGeneratorAction::setG( double G )
{
  /* (G - 1) * m * c^2 */
  double E = ( G - 1 ) * particleGun->GetParticleDefinition( )->GetPDGMass( );
  particleGun->SetParticleEnergy( E );
}

void
PrimaryGeneratorAction::setDef( const char* name )
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable( );
  G4String particleName( name );
  particleGun->SetParticleDefinition( particleTable->FindParticle( particleName ) );
}

void
PrimaryGeneratorAction::setDef( G4ParticleDefinition* def )
{
  particleGun->SetParticleDefinition( def );
}
