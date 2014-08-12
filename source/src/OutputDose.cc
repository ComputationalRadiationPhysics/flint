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
 


#include "OutputDose.hh"

OutputDose::OutputDose( double _x_min,
                        double _x_max,
                        double _y_min,
                        double _y_max,
                        double _z_min,
                        double _z_max,
                        int _nx,
                        int _ny,
                        int _nz )
{
  MSG << "Output for dose active..." << NL;
  x_min = _x_min;
  y_min = _y_min;
  z_min = _z_min;
  x_max = _x_max;
  y_max = _y_max;
  z_max = _z_max;
  nx = _nx;
  ny = _ny;
  nz = _nz;
  width_x = fabs( ( x_max - x_min ) / (double)nx );
  width_y = fabs( ( y_max - y_min ) / (double)ny );
  width_z = fabs( ( z_max - z_min ) / (double)nz );

  data = new Field3d< double >( nx, ny, nz );
  data->setAll( 0.0 );
}

OutputDose::~OutputDose( )
{
  delete data;
}

void
OutputDose::outputData( OutputParticle* p )
{
  /*   if (p->definition->GetParticleName() != "gamma") return; */
  if ( p->definition->GetParticleName( ) == "proton" )
    return;
  double x = p->x;
  double y = p->y;
  double z = p->z;
  bool inside_x = ( x < x_max ) && ( x >= x_min );
  bool inside_y = ( y < y_max ) && ( y >= y_min );
  bool inside_z = ( z < z_max ) && ( z >= z_min );
  if ( inside_x && inside_y && inside_z )
  {
    int bin_x = (int)( ( p->x - x_min ) / width_x );
    int bin_y = (int)( ( p->y - y_min ) / width_y );
    int bin_z = (int)( ( p->z - z_min ) / width_z );
    double v = ( p->E_loss > 0 ) ? p->E_loss : 0.0;
    data->addData( bin_x, bin_y, bin_z, v / keV );
  }
}

void
OutputDose::finalize( )
{
  /* Output the energy */
  MSG << "Writing energy information..." << NL;
  data->writeToFile( OUTPUT_ENERGY, "Energy deposition distribution [keV]" );

  /* Scale to dose */
  G4Navigator* navigator = G4Calc::getInstance( )->getCustomNavigator( );
  double bin_size = width_x * width_y * width_z;
  for ( int x = 0; x < data->getNx( ); x++ )
  {
    for ( int y = 0; y < data->getNy( ); y++ )
    {
      for ( int z = 0; z < data->getNz( ); z++ )
      {
        navigator->LocateGlobalPointAndSetup( G4ThreeVector( binToX( x ),
                                                             binToY( y ),
                                                             binToZ( z ) ) );
        G4TouchableHistoryHandle aTouchable =
          navigator->CreateTouchableHistoryHandle( );
        G4LogicalVolume* volume = aTouchable->GetVolume( )->GetLogicalVolume( );
        double voxel_mass = volume->GetMaterial( )->GetDensity( ) * bin_size /
                            kg;
        data->setData( x, y, z, data->getData( x,
                                               y,
                                               z ) * keV / joule / voxel_mass );
      }
    }
  }
  /* Output dose */
  MSG << "Writing dose information..." << NL;
  data->writeToFile( OUTPUT_DOSE, "Dose distribution [Gy = Joule / kg]" );
  MSG << "Done." << NL;
}
