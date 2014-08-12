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
 


template < class T >
int
Field3d< T >::getIndex( int x, int y, int z )
{
  return x * nx * ny + y * ny + z;
}

template < class T >
Field3d< T >::Field3d( int _nx, int _ny, int _nz )
{
  nx = _nx;
  ny = _ny;
  nz = _nz;
  data = (T*)malloc( sizeof( T ) * _nx * _ny * _nz );
}

template < class T >
Field3d< T >::~Field3d( )
{
  free( data );
}

template < class T >
void
Field3d< T >::setAll( T value )
{
  for ( int x = 0; x < nx; x++ )
  {
    for ( int y = 0; y < ny; y++ )
    {
      for ( int z = 0; z < nz; z++ )
      {
        setData( x, y, z, value );
      }
    }
  }
}

template < class T >
T
Field3d< T >::getData( int x, int y, int z )
{
  return data[getIndex( x, y, z )];
}

template < class T >
void
Field3d< T >::setData( int x, int y, int z, T value )
{
  data[getIndex( x, y, z )] = value;
}

template < class T >
void
Field3d< T >::addData( int x, int y, int z, T value )
{
  data[getIndex( x, y, z )] += value;
}

template < class T >
void
Field3d< T >::writeToFile( const char* filename, const char* header )
{
  std::ofstream file;
  file.open( filename );
  file << "# " << header << std::endl;
  file << "# Format: x, y, z, data" << std::endl;
  for ( int x = 0; x < nx; x++ )
  {
    for ( int y = 0; y < ny; y++ )
    {
      for ( int z = 0; z < nz; z++ )
      {
        file << x << ", " << y << ", " << z << ", " <<
        getData( x, y, z ) << std::endl;
      }
    }
  }
  file.close( );
}
