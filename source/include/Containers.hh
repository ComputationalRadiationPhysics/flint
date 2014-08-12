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
 


/*\file Containers.hh This file contains several containers mainly
 * used by \ref OutputManager */

#ifndef Containers_hh
#define Containers_hh 1

/*A lightweight class for a 3d field */
template< class T > class Field3d {
  private:
    T* data;
    int nx, ny, nz;

    int getIndex( int x, int y, int z );

  public:
    Field3d( int _nx, int _ny, int _nz );
    ~Field3d( );

    void setAll( T value );

    T getData( int x, int y, int z );

    void setData( int x, int y, int z, T value );

    void addData( int x, int y, int z, T value );

    void writeToFile( const char* filename, const char* header );

    int
    getNx( )
    {
      return nx;
    }
    int
    getNy( )
    {
      return ny;
    }
    int
    getNz( )
    {
      return nz;
    }
};

/* Hack for templates... -,- */
#include "Containers.cc"

#endif /* Containers_hh */
