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
 
#include "Stepping.hh"

SteppingAction::SteppingAction( G4UserLimits* f_limits )
{
  limits = f_limits;
}

SteppingAction::~SteppingAction( )
{
}

void
SteppingAction::storeInfo( Particle* p, const G4Step* step )
{
  p->pos.setX( step->GetPostStepPoint( )->GetPosition( ).x( ) );
  p->pos.setY( step->GetPostStepPoint( )->GetPosition( ).y( ) );
  p->pos.setZ( step->GetPostStepPoint( )->GetPosition( ).z( ) );
  /*   MSG << "z=" << p->pos.z() << ", dmax=" <<
   * limits->GetMaxAllowedStep(G4Track()) << NL; */

  p->momentum.setX( step->GetPostStepPoint( )->GetMomentumDirection( ).x( ) );
  p->momentum.setY( step->GetPostStepPoint( )->GetMomentumDirection( ).y( ) );
  p->momentum.setZ( step->GetPostStepPoint( )->GetMomentumDirection( ).z( ) );

  /* Cutoff under 10 eV */
  if ( step->GetPostStepPoint( )->GetKineticEnergy( ) < 10.0 * eV )
    p->G = 1.0;

  else
    p->G = step->GetPostStepPoint( )->GetGamma( );

}

void
SteppingAction::UserSteppingAction( const G4Step* step )
{
  /* If this particle is massless, it should be tracked by Geant4 for */
  /* GPT cannot track particles with m=0. */
  if ( step->GetTrack( )->GetDynamicParticle( )->GetMass( ) == 0.0 )
  {
    /* Here is the place to output any data. */
    /* If this is the first step, information about the start point is
     * required */
    if ( step->GetTrack( )->GetCurrentStepNumber( ) == 1 )
      /*\todo Only works for up to 100 secondary photons per event! */
      OutputManager::getInstance( )->outputData( step->GetTrack( )->GetTrackID(
                                                                              )
                                                 +
                                                 G4RunManager::GetRunManager( )
                                                 ->GetCurrentRun( )->GetRunID( )
                                                 * 100,
                                                 step->GetPreStepPoint( )->
                                                 GetPosition( ).x( ),
                                                 step->GetPreStepPoint( )->
                                                 GetPosition( ).y( ),
                                                 step->GetPreStepPoint( )->
                                                 GetPosition( ).z( ),
                                                 step->GetPreStepPoint( )->
                                                 GetMomentum( ).x( ),
                                                 step->GetPreStepPoint( )->
                                                 GetMomentum( ).y( ),
                                                 step->GetPreStepPoint( )->
                                                 GetMomentum( ).z( ),
                                                 step->GetPreStepPoint( )->
                                                 GetKineticEnergy( ), 0.0, 0.0,
                                                 0.0 );

  }
  else
  if ( step->GetTrack( )->GetLocalTime( ) * 1.01 >=
       limits->GetUserMaxTime( *( step->GetTrack( ) ) ) )
  {
    /* Abort tracking in G4, return to GPT */
    storeInfo( pResult, step );
    /* Kill this track (not the secondaries, they will be seperatley
     * killed afterwards). */
    /*\todo Perhaps configure secondary handling via XML? */
    step->GetTrack( )->SetTrackStatus( fStopAndKill );
    /*
     *
     *
     *     step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries); */
  }

  /* All Geant output is done here. For trajectories of other
   * particles, please refer to GPT output. */
  OutputManager::getInstance( )->outputData( step->GetTrack( )->GetTrackID( ) +
                                             G4RunManager::GetRunManager( )->
                                             GetCurrentRun( )->GetRunID( ) *
                                             100,
                                             step->GetPostStepPoint( )->
                                             GetPosition( ).x( ),
                                             step->GetPostStepPoint( )->
                                             GetPosition( ).y( ),
                                             step->GetPostStepPoint( )->
                                             GetPosition( ).z( ),
                                             step->GetPostStepPoint( )->
                                             GetMomentum( ).x( ),
                                             step->GetPostStepPoint( )->
                                             GetMomentum( ).y( ),
                                             step->GetPostStepPoint( )->
                                             GetMomentum( ).z( ),
                                             step->GetPostStepPoint( )->
                                             GetKineticEnergy( ),
                                             step->GetTotalEnergyDeposit( ),
                                             step->GetPostStepPoint( )->GetMass( ),
                                             step->GetPostStepPoint( )->
                                             GetCharge( ) );

  /*\todo Also configure via XML. */

  /* Massless secondaries will be tracked by G4, massive one will be
   * deleted and sent to GPT. */
  /* Secondaries organized in a vector */
  G4TrackVector* fSecondary = step->GetSecondary( );
  for ( G4TrackVector::iterator it = fSecondary->begin( );
        it != fSecondary->end( );
        it++ )
  {
    /* Massive particle? */
    if ( ( *it )->GetDynamicParticle( )->GetDefinition( )->GetPDGMass( ) != 0.0 )
    {
      /* Kill this particle ('fKillTrackAndSecondaries' is here equal
       * to 'fStopAndKill') */
      ( *it )->SetTrackStatus( fKillTrackAndSecondaries );
      /* Resulting particle */
      Particle p;
      /* Set the particle's properties */
      p.q = ( *it )->GetDynamicParticle( )->GetCharge( ) / coulomb;
      /*\todo Why factor 1e-12? */
      p.m = ( *it )->GetDynamicParticle( )->GetDefinition( )->GetPDGMass( ) *
            1e-12 / joule / c_light / c_light;
      /* Write coordinates, dir and G */
      storeInfo( &p, step );
      /* An ID of -1 signalized GPT it has to deal with a secondary */
      p.ID = -1;
      /* Save */
      results->push_back( p );
    }
  }

}
