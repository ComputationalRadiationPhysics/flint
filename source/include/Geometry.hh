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
 


#ifndef Geometry_hh
#define Geometry_hh 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UserLimits.hh"
#include "G4Tubs.hh"
#include "G4GDMLParser.hh"

#include "Tools.hh"

/* This class defines the geometry of the world. */

class Geometry : public G4VUserDetectorConstruction {
  public:
    Geometry( G4UserLimits * f_limits );
    ~Geometry( );

    /*Build the geometry */
    G4VPhysicalVolume* Construct( );

    /*Returns a pointer to the world volume (only if previously
     * constructed...) */
    G4VPhysicalVolume*
    getWorld( )
    {
      return world_phys;
    }
  private:
    /*Step limits need to be saved here, we need them during the
     * construction of the geometry as an pointer, */
    /* as they will be updated ever step. */
    G4UserLimits* limits;

    /*Physical placement of the world */
    G4VPhysicalVolume* world_phys;

    G4GDMLParser* parser;

    /*Recursive function to set the limits in every logical volume
     * in the tree */
    void setLimits( G4LogicalVolume* volume );

};

#endif /* Geometry_hh */
