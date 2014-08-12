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
 


#include "mainG4.hh"
#include "svn_version.hh"

/* --------------------------------------------------------------------------- */
/* --- Compability functions for GPT (C++ to C)
 * ------------------------------ */
/* --------------------------------------------------------------------------- */

extern "C" int
G4_init( )
{
  return G4Calc::getInstance( )->init( );
}

extern "C" void
G4_deinit( )
{
  MSG << "G4_deinit() called" << NL;
  OutputManager::getInstance( )->finalize( );
  delete G4Calc::getInstance( );
}

extern "C" int
G4_insideGeometry( double x, double y, double z )
{
  return G4Calc::getInstance( )->insideGeometry( x, y, z );
}

extern "C" int
G4_calculate( int ID,
              double* position_pre,
              double* position_post,
              double* momentum_post,
              double dt,
              double* G,
              double q,
              double m,
              int* n_secondaries,
              double** secondaries_position,
              double** secondaries_momentum,
              double** secondaries_m,
              double** secondaries_q )
{
  return G4Calc::getInstance( )->calculate( ID,
                                            position_pre,
                                            position_post,
                                            momentum_post,
                                            dt,
                                            G,
                                            q,
                                            m,
                                            n_secondaries,
                                            secondaries_position,
                                            secondaries_momentum,
                                            secondaries_m,
                                            secondaries_q );
}

extern "C" void
outputData( int ID,
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
  /*\todo I don't know why this is needed, but it works. */
  if ( E < 0 )
    return;
  G4Calc::getInstance( )->outputData( ID,
                                      x * meter,
                                      y * meter,
                                      z * meter,
                                      v_x,
                                      v_y,
                                      v_z,
                                      E * joule,
                                      E_loss * joule,
                                      m * kg,
                                      q * coulomb );
}

/* --------------------------------------------------------------------------- */
/* --- Real G4 class
 * --------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

G4Calc::G4Calc( )
{
  if ( instance )
    ERROR <<
    "G4Calc already instantiated. The application could show undefined behavior!"
          << ERROR_END;
  instance = this;
}

G4Calc::~G4Calc( )
{
  MSG << "G4 exiting now..." << NL;
  /* Finalize */
  /* 'physics', 'steppingAction' and 'primaryAction' are deleted by
   * G4RunManager... */
  delete runManager;
  delete particleTable;
  delete outputManager;
  instance = 0;
  MSG << "Exit done. G4 is down..." << NL;
}

G4Calc* G4Calc::instance = 0;

bool
G4Calc::init( )
{
  MSG << "G4Calc::init() called..." << NL;
  MSG << "I am FLInT, revision " << flintVersion( ) << ". Going up..." << NL;

  /* Define Run-Manager */
  runManager = new G4RunManager;

  /* Step length is limited to the step used by GPT. */
  /* Geometry */
  limits = new G4UserLimits( );
  geometry = new Geometry( limits );
  runManager->SetUserInitialization( geometry );
  /* Physics */
  particleTable = new ParticleTable( );
  physics = new PhysicsList( );
  runManager->SetUserInitialization( physics );
  /* Particle-Gun: The parameters will be set every call */
  primaryAction = new PrimaryGeneratorAction( );
  runManager->SetUserAction( primaryAction );
  /* 'SteppingAction' will save the data of the call */
  steppingAction = new SteppingAction( limits );
  runManager->SetUserAction( steppingAction );

  /* Create outputManager */
  outputManager = new OutputManager( );

  /* Parse XML file */
  if ( XMLParser::parse( ) != 0 )
  {
    ERROR << "Could not parse XML file!" << ERROR_END;
    return false;
  }

  /* Parse particles */
  particleTable->registerParticleDefs( );

  /* Set verbosities */
  G4UImanager* ui = G4UImanager::GetUIpointer( );
  ui->ApplyCommand( "/control/verbose 0" );
  ui->ApplyCommand( "/process/verbose 0" );
  ui->ApplyCommand( "/material/verbose 0" );
  ui->ApplyCommand( "/run/verbose 0" );
  ui->ApplyCommand( "/event/verbose 0" );
  ui->ApplyCommand( "/tracking/verbose 0" );
  ui->ApplyCommand( "/tracking/storeTrajectory 0" );

  /* Build G4Kernel */
  runManager->Initialize( );

  MSG << "G4 Kernel built..." << NL;

  /* Output initial data */
  outputManager->outputInitData( );

  /* Setup navigator */
  worldVolume = geometry->getWorld( );
  findNavigator = new G4Navigator( );
  findNavigator->SetWorldVolume( worldVolume );

  MSG << "G4Init finished. Ready to calculate..." << NL;
  return true;
}

bool
G4Calc::insideGeometry( double x, double y, double z )
{
  findNavigator->LocateGlobalPointAndSetup( G4ThreeVector( x * meter, y * meter,
                                                           z * meter ) );
  G4TouchableHistoryHandle aTouchable =
    findNavigator->CreateTouchableHistoryHandle( );
  return ( aTouchable->GetHistoryDepth( ) != 0 );
}

bool
G4Calc::calculate( int ID,
                   double* position_pre,
                   double* position_post,
                   double* momentum_post,
                   double dt,
                   double* G,
                   double q,
                   double m,
                   int* n_secondaries,
                   double** secondaries_position,
                   double** secondaries_momentum,
                   double** secondaries_m,
                   double** secondaries_q )
{
  /* Set particle-data */
  G4ParticleDefinition* def = particleTable->getParticleDef( m * kg,
                                                             q * coulomb );
  /* Make sure that def != 0 */
  if ( def == NULL )
  {
    ERROR << "Definition returned by 'particleTable' for m=" << m <<
    " and q=" << q << " is zero." << NL;
    ERROR <<
    "Aborting calculation. Check that you have got every particle you need in your 'setup.xml'."
          << ERROR_END;
    return false;
  }

  /* Convert gpt-vectors */
  G4ThreeVector vec_position_pre( position_pre[0] * meter,
                                  position_pre[1] * meter,
                                  position_pre[2] * meter );
  G4ThreeVector vec_position_post( position_post[0] * meter,
                                   position_post[1] * meter,
                                   position_post[2] * meter );
  G4ThreeVector vec_momentum_post( momentum_post[0],
                                   momentum_post[1],
                                   momentum_post[2] );
  G4ThreeVector vec_momentum_initial( vec_position_post - vec_position_pre );

  primaryAction->setDef( def );
  primaryAction->setPos( vec_position_pre );
  primaryAction->setDir( vec_position_post - vec_position_pre );
  /* G_post is very important here. Do not change it if you are not
   * really sure what you are doing! */
  primaryAction->setG( *G );
  /*\todo Just a debugging variable - could be removed in final
   * release */
  double old_G = *G;

  /* Results are stored here */
  Particle par;
  std::vector< Particle > secondaries;
  steppingAction->setResults( &secondaries, &par );

  /* Set the limits corresponding to GPT's step time */
  limits->SetUserMaxTime( dt * second );

  /* Finally, start the run ;-) */
  /*\todo Not very efficient... */
  runManager->BeamOn( 1 );

  /*\todo Just a debugging check - could be removed in final release
   * (if this is ever released ;) */
  if ( par.G * 0.999 > old_G )
  {
    ERROR << "Particle cannot win energy!!!" << NL;
    ERROR_CONT << "G before = " << old_G << NL;
    ERROR_CONT << "G after  = " << par.G << ERROR_END;
    return false;
  }

  /* Norm the velocity-vector */
  /* Rotate the GPT-resulting vector corrensponding to G4 vector
   * change */
  G4ThreeVector axis = vec_momentum_initial.cross( par.momentum );
  if ( axis.mag2( ) == 0.0 )
    par.momentum = vec_momentum_post;

  else
  {
    double phi = vec_momentum_initial.angle( par.momentum );
    par.momentum = vec_momentum_post.rotate( phi, axis );
  }
  /* Not quite sure if this is perfect, but it seems to work... Would
   * be a point to contact GPT's guys... */
  /* GPT seems to take only the GBr length to calculate momentum and
   * provide information about G only for convienience. */
  /* If this is the case, this approach should work. */
  par.momentum.setR( par.G * sqrt( 1 - 1 / par.G / par.G ) );
  /* Fill GPT's old fashioned arrays */
  for ( int i = 0; i < 3; i++ )
  {
    position_post[i] = par.pos[i] / meter;
  }
  for ( int i = 0; i < 3; i++ )
  {
    momentum_post[i] = par.momentum[i];
  }

  /* Set GPT's G */
  *G = par.G;

  /* Consider secondaries now */
  /* This process may look a bit weird, but communication between GPT
   * and G4 is not easy... */
  int n = secondaries.size( );
  *n_secondaries = n;
  if ( n != 0 )
  {
    /* This memory has to be released by the calling function... */
    *secondaries_position = (double*)malloc( sizeof( double ) * 3 * n );
    *secondaries_momentum = (double*)malloc( sizeof( double ) * 3 * n );
    *secondaries_q = (double*)malloc( sizeof( double ) * n );
    *secondaries_m = (double*)malloc( sizeof( double ) * n );
    /* Iterate all secondaries */
    for ( int i = 0; i < n; i++ )
    {
      Particle* p = &secondaries[i];
      for ( int j = 0; j < 3; j++ )
      {
        ( *secondaries_position )[i * 3 + j] = p->pos[j] / meter;
      }
      for ( int j = 0; j < 3; j++ )
      {
        ( *secondaries_momentum )[i * 3 + j] = p->momentum[j];
      }
      ( *secondaries_q )[i] = p->q;
      ( *secondaries_m )[i] = p->m;
    }
  }

  /* If all bugs failed to produce an error, calculation went right ;) */
  return true;
}

void
G4Calc::outputData( int ID,
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
  outputManager->outputData( ID, x, y, z, v_x, v_y, v_z, E, E_loss, m, q );
}
