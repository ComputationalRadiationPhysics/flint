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
 


#include <iostream>

#include "TApplication.h"
#include "TGeoManager.h"

int
main( int argc, char** argv )
{
  std::cout << "GDML Viewer" << std::endl;
  TApplication app( "GDML Viewer", &argc, argv );

  std::string filename = "setup.gdml";
  if ( argc >= 2 )
    filename = argv[1];

  TGeoManager::Import( filename.c_str( ) );
  if ( gGeoManager == 0 )
  {
    std::cerr << "File '" << filename << "' could not be read. Exiting..." <<
    std::endl;
    return -1;
  }

  gGeoManager->ViewLeaves( true );
  gGeoManager->GetTopVolume( )->Draw( "ogl" );

  app.Run( );

  std::cout << "GDML Viewer exiting..." << std::endl;

  return 0;
}
