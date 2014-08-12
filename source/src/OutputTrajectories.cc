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
 


#include "OutputTrajectories.hh"

OutputTrajectories::OutputTrajectories( )
{
  MSG << "Output for trajectories active..." << NL;
}

OutputTrajectories::~OutputTrajectories( )
{
  for ( std::map< G4String, std::ofstream* >::iterator it =
          trajectoryFiles.begin( );
        it != trajectoryFiles.end( );
        it++ )
  {
    delete( *it ).second;
  }
}

void
OutputTrajectories::outputData( OutputParticle* p )
{
  G4String definitionName = p->definition->GetParticleName( );

  /* If an output file does not exist, create one */
  if ( trajectoryFiles.find( definitionName ) == trajectoryFiles.end( ) )
  {
    MSG << "Creating output file for new particle type '" << definitionName
        << "'... Trajectories will be stored in '" << TRAJECTORY_PREFIX <<
    definitionName << "'." << NL;
    trajectoryFiles[definitionName] = new std::ofstream( );

    /* Attention: Do not insert directly to definition name (e.g.
     * G4String::prepend()) */
    std::string filename = definitionName;
    filename.insert( 0, TRAJECTORY_PREFIX );

    trajectoryFiles[definitionName]->open( filename.c_str( ), std::ios::out );
    ( *trajectoryFiles[definitionName] ) << "# Trajectories for particle '" <<
    definitionName << "'." << std::endl
                                         <<
    "ID, x[m], y[m], z[m], vx[GB], vy[GB], vz[GB], E[keV]" << std::endl <<
    std::endl;
  }

  /*\todo Where does this come from? */
  if ( p->v_x != p->v_x )
    p->v_x = 0;
  if ( p->v_y != p->v_y )
    p->v_y = 0;
  if ( p->v_z != p->v_z )
    p->v_z = 0;

  /* Output intereting values */
  ( *trajectoryFiles[definitionName] ) << p->ID << " " << p->x / meter <<
  " " << p->y / meter << " " << p->z / meter
                                       << " " << p->v_x << " " << p->v_y <<
  " " << p->v_z
                                       << " " << p->E / keV << std::endl;
}

void
OutputTrajectories::finalize( )
{
  for ( std::map< G4String, std::ofstream* >::iterator it =
          trajectoryFiles.begin( );
        it != trajectoryFiles.end( );
        it++ )
  {
    ( *it ).second->close( );
  }
}
