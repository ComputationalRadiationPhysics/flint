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
 
/* file G4Virtual.c Included into GPT.
 * A virtual object, that has to be created in order to run Geant4.
 * From here, G4 is launched. */

#include "G4Virtual.h"

/*The time of the previous timestep */
double t_old;

/*Kill G4 */
void
exitGeant4( void* p )
{
  G4_deinit( );
}

/*Update the 'pars_old' array */
void
copyArray( double t, double* x, void* info )
{
  int i;
  for ( i = 0; i < numpar; i++ )
  {
    old_par* p = &( pars_old[pars[i].ID] );
    p->x = pars[i].Wr[0];
    p->y = pars[i].Wr[1];
    p->z = pars[i].Wr[2];
    p->G = pars[i].G;
    p->stored = 1;
  }
  /* New step */
  step++;
}

/*Called after every succesful timestep */
void calcG4( double t, double* dt, double* x, void* info );

/*The init function of flint. */
void
G4Virtual_init( gptinit* init )
{
  printf( "GPT: G4Virtual_init. GPT has PID=%d\n", getpid( ) );

  /* Allocate and set the 'pars_old' array */
  /*\todo Not very nice, but neccesary, as numpars can change during
   * simulation */
  pars_old = gptmalloc( sizeof( old_par ) * NUMPARS_SUPPORTED );
  copyArray( 0, 0, 0 );
  int i;
  for ( i = 0; i < NUMPARS_SUPPORTED; i++ )
  {
    pars_old[i].stored = 0;
  }

  /* Step no. zero */
  step = 0;
  t_old = 0;

  /* Of course... */
  pars_g4 = 0;

  /* Start G4 */
  if ( !G4_init( ) )
    gpterror( "G4 could not init. Aborting...\n" );

  /* Exit-function */
  gptaddmainfunction( GPTMAINFNC_EXT, &exitGeant4, 0 );
  /* Main-function */
  odeaddendfunction( ODEFNC_USR, &calcG4, 0 );
}

/*Main simulation routine in GPT */
void
calcG4( double t, double* dt, double* x, void* info_uncasted )
{
  /* Just a simple integer. Needed very often ;) */
  int i;

  /*\todo Hopefully temporally... See NUMPARS_SUPPORTED. */
  if ( numpar >= NUMPARS_SUPPORTED )
    gpterror(
      "The number of particles (=%d) has exceeded the maximum (=%d). Large numbers are not implemented yet!\n",
      numpar,
      NUMPARS_SUPPORTED );

  /* First step? Do nothing. */
  if ( step == 0 )
  {
    printf( "First step...\n" );
    copyArray( 0.0, NULL, NULL );
    return;
  }

  int alive = 0;

  /* Iterate over all particles */
  /*\todo Check whether gcc3 (GPT) is compatible to gcc4 (OpenMP)... */
  /* #pragma omp parallel for private(i) */
  for ( i = 0; i < numpar; i++ )
  {
    /* Check if we have a problem */
    if ( pars[i].ID >= NUMPARS_SUPPORTED )
      gpterror( "pars[%d].ID=%d >= NUMPARS_SUPPORTED=%d\n",
                i,
                pars[i].ID,
                NUMPARS_SUPPORTED );
     /* Is the particle still alive? */
    if ( !pars[i].alive )
      continue;
    alive++;
    /* Is the particle in old_pars? */
    if ( pars_old[pars[i].ID].stored == 0 )
      continue;
     /* Is the particle inside the area? */
    if ( ( !G4_insideGeometry( pars[i].Wr[0], pars[i].Wr[1],
                               pars[i].Wr[2] ) ) &&
         ( !G4_insideGeometry( pars_old[pars[i].ID].x, pars_old[pars[i].ID].y,
                               pars_old[pars[i].ID].z ) ) )
      continue;

    /* If the particle has not moved, abort */
    if ( ( pars[i].Wr[0] == pars_old[pars[i].ID].x ) &&
         ( pars[i].Wr[1] == pars_old[pars[i].ID].y ) &&
         ( pars[i].Wr[2] == pars_old[pars[i].ID].z ) )
      continue;

    pars_g4++;

    /* The number of secondaries that G4 delivers */
    int n_secondaries;
    /* Position... */
    double* secondaries_Wr;
    /* ...and momentum of the secondaries (both in form of x1, y1, z1,
     * x2, y2, z2, ... in one array) */
    double* secondaries_GBr;
    /* Mass in kg */
    double* secondaries_m;
    /* Charge in C */
    double* secondaries_q;

    double* pos_pre = malloc( 3 * sizeof( double ) );
    pos_pre[0] = pars_old[pars[i].ID].x;
    pos_pre[1] = pars_old[pars[i].ID].y;
    pos_pre[2] = pars_old[pars[i].ID].z;

    /* Needed for output in a particle's first step... */
    if ( pars_old[pars[i].ID].stored == 0 )
      pars_old[pars[i].ID].G = pars[i].G;

    if ( !G4_calculate( pars[i].ID, pos_pre, pars[i].Wr, pars[i].GBr, t - t_old,
                        &pars[i].G, pars[i].q, pars[i].m,
                        &n_secondaries, &secondaries_Wr, &secondaries_GBr,
                        &secondaries_m, &secondaries_q ) )
      gpterror( "G4 exited with error. It should have produced output...\n" );

    free( pos_pre );

    /* Process secondaries */
    int n;
    for ( n = 0; n < n_secondaries; n++ )
    {
      gptaddparmqn( gptgetparset(
                      "__secondaries" ), &secondaries_Wr[n * 3],
                    &secondaries_GBr[n * 3],
                    secondaries_m[n], secondaries_q[n], 1.0 );
    }
    if ( n_secondaries != 0 )
    {
      free( secondaries_m );
      free( secondaries_q );
    }

    /* Is this particle still alive? */
    if ( pars[i].G <= 1.0 )
    {
      /* Remove the particle from the simulation */
      pars_old[pars[i].ID].stored = 0;
      gptremoveparticle( &pars[i] );
      continue;
    }
  }

  /* This is probably deprecated. Trajectories cannot be recorded by
   * G4 and all other output options are useless there. */
  /*
  //Output all particles
  for (i = 0; i < numpar; i++) {
    //Is the particle alive?
    if (!pars[i].alive) continue;
    double E = (pars[i].G - 1.0) * pars[i].m * gpt_c * gpt_c;
    double E_old;
    //In a particle's first step the energy delta is pre-G4 to post-G4. If the particle already existed the delta is
    // post-G4 of the last step to post G4 of this step.
    ///\todo The energy does not fit to position (pre or post delta).
    E_old = (pars_old[pars[i].ID].G - 1.0) * pars[i].m * gpt_c * gpt_c;
    outputData(pars[i].ID, pars[i].Wr[0], pars[i].Wr[1], pars[i].Wr[2], pars[i].GBr[0], pars[i].GBr[1], pars[i].GBr[2],
               E, E_old - E, pars[i].m, pars[i].q);
  }*/

  /* Print a status message once in a while */
  if ( ( step % OUTPUT_INTERVAL ) == 0 )
  {
    printf( "Processing... step=%d, t=%e, dt=%e, G4=%.2f, alive=%d\n",
            step,
            t,
            *dt,
            (double)pars_g4 / (double)alive / OUTPUT_INTERVAL * 100.0,
            alive );
    pars_g4 = 0;
  }

  /* Update old-pars */
  copyArray( 0.0, NULL, NULL );
  /* Update time information */
  t_old = t;
}
