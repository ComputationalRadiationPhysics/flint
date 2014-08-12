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
 


#include "Geometry.hh"

Geometry::Geometry( G4UserLimits* f_limits )
{
  limits = f_limits;
  parser = new G4GDMLParser( );
  parser->Read( SETUP_FILE );
}

Geometry::~Geometry( )
{

}

/* Set up the geometry. */
/* For instructions how to define the detector-construction refer to */
/*
 *
 *
 *
 * http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch02s02.html */
/* and */
/*
 *
 *
 *
 * http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch04.html */

G4VPhysicalVolume*
Geometry::Construct( )
{
  world_phys = parser->GetWorldVolume( );
  setLimits( world_phys->GetLogicalVolume( ) );
  return world_phys;
}

void
Geometry::setLimits( G4LogicalVolume* volume )
{
  volume->SetUserLimits( limits );
  for ( int i = 0; i < volume->GetNoDaughters( ); i++ )
  {
    setLimits( volume->GetDaughter( i )->GetLogicalVolume( ) );
  }
}
