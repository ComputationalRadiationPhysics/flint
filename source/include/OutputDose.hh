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
 


#ifndef OutputDose_hh
#define OutputDose_hh 1

#include <fstream>

#include "G4Navigator.hh"

#include "mainG4.hh"
#include "OutputBase.hh"
#include "Containers.hh"
#include "Tools.hh"

/*This class ist used for outputing dose distributions */
class OutputDose : public OutputBase {
  private:
    /*The dose distribution */
    Field3d< double >* data;

    double x_min, x_max, y_min, y_max, z_min, z_max;
    int nx, ny, nz;
    double width_x, width_y, width_z;

    double
    binToX( int bin )
    {
      return x_min + bin * width_x;
    }
    double
    binToY( int bin )
    {
      return y_min + bin * width_y;
    }
    double
    binToZ( int bin )
    {
      return z_min + bin * width_z;
    }

  public:
    OutputDose( double _x_min,
                double _x_max,
                double _y_min,
                double _y_max,
                double _z_min,
                double _z_max,
                int _nx,
                int _ny,
                int _nz );
    ~OutputDose( );

    void outputData( OutputParticle* p );

    void finalize( );

};

#endif /* OutputDose_hh */
