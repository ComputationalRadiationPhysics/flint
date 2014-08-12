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
 


#include "OutputManager.hh"

OutputManager::OutputManager( )
{
  MSG << "OutputManager::OutputManager()" << NL;
  if ( instance )
    ERROR << "OutputManager::OuotputManager: instance != 0" << ERROR_END;

  instance = this;
  /* Delete all old outputs */
  system( "rm -rf output" );
  system( "mkdir output" );
}

OutputManager::~OutputManager( )
{
  for ( std::vector< OutputBase* >::iterator it = outputs.begin( );
        it != outputs.end( );
        it++ )
  {
    delete( *it );
  }
}

OutputManager* OutputManager::instance = 0;

void
OutputManager::addOutput( OutputBase* output )
{
  outputs.push_back( output );
}

void
OutputManager::outputInitData( )
{
  for ( std::vector< OutputBase* >::iterator it = outputs.begin( );
        it != outputs.end( );
        it++ )
  {
    ( *it )->outputInitData( );
  }
}

void
OutputManager::outputData( int ID,
                           double x,
                           double y,
                           double z,
                           double v_x,
                           double v_y,
                           double v_z,
                           double E,
                           double E_loss,
                           double m,
                           double q )
{
  /* Prepare particle */
  OutputParticle p;
  p.ID = ID;
  p.x = x;
  p.y = y;
  p.z = z;
  p.v_x = v_x;
  p.v_y = v_y;
  p.v_z = v_z;
  p.E = E;
  p.E_loss = E_loss;
  p.m = m;
  p.q = q;
  p.definition = ParticleTable::getInstance( )->getParticleDef( m, q );

  for ( std::vector< OutputBase* >::iterator it = outputs.begin( );
        it != outputs.end( );
        it++ )
  {
    ( *it )->outputData( &p );
  }
}

void
OutputManager::finalize( )
{
  MSG << "OutputManager::finalize()" << NL;
  for ( std::vector< OutputBase* >::iterator it = outputs.begin( );
        it != outputs.end( );
        it++ )
  {
    ( *it )->finalize( );
  }
}
