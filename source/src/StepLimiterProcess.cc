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
 


#include "StepLimiterProcess.hh"

#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"
#include "G4LossTableManager.hh"

StepLimiterProcess::StepLimiterProcess( const G4String& aName ) :
  G4VProcess( aName )
{
  if ( verboseLevel > 0 )
    G4cout << GetProcessName( ) << " is created " << G4endl;

  theLossTableManager = G4LossTableManager::Instance( );
}

StepLimiterProcess::~StepLimiterProcess( )
{
}

StepLimiterProcess::StepLimiterProcess( StepLimiterProcess& right ) :
  G4VProcess( right )
{
}

G4double
StepLimiterProcess::PostStepGetPhysicalInteractionLength(
  const G4Track& aTrack,
  G4double,
  G4ForceCondition*
  condition )
{
  /* condition is set to "Not Forced" */
  *condition = NotForced;

  G4double ProposedStep = DBL_MAX;
  G4UserLimits* pUserLimits =
    aTrack.GetVolume( )->GetLogicalVolume( )->GetUserLimits( );
  if ( pUserLimits )
  {
    /* max track length */
    /*  */
    ProposedStep = ( pUserLimits->GetUserMaxTrackLength( aTrack )
                     - aTrack.GetTrackLength( ) );
    if ( ProposedStep < 0. )
      return 0.;

    /* max time limit */
    /*  */
    G4double beta = ( aTrack.GetDynamicParticle( )->GetTotalMomentum( ) )
                    / ( aTrack.GetTotalEnergy( ) );
    G4double dTime = ( pUserLimits->GetUserMaxTime( aTrack )
                       - aTrack.GetGlobalTime( ) );
    G4double temp = beta * c_light * dTime;
    if ( temp < 0. )
      return 0.;
    if ( ProposedStep > temp )
      ProposedStep = temp;

    /* min remaining range */
    /* (only for charged particle except for chargedGeantino) */
    /*  */
    G4ParticleDefinition* Particle = aTrack.GetDefinition( );
    if ( ( Particle->GetPDGCharge( ) != 0. )
         && ( Particle->GetParticleType( ) != "geantino" ) )
    {
      G4double Ekine = aTrack.GetKineticEnergy( );
      const G4MaterialCutsCouple* couple = aTrack.GetMaterialCutsCouple( );
      G4double RangeNow = theLossTableManager->GetRange( Particle,
                                                         Ekine,
                                                         couple );
      G4double Rmin = pUserLimits->GetUserMinRange( aTrack );
      if ( Rmin > DBL_MIN )
      {
        temp = RangeNow - Rmin;
        if ( temp < 0. )
          return 0.;
        if ( ProposedStep > temp )
          ProposedStep = temp;
      }

      /* min kinetic energy (only for charged particle) */
      /*  */
      G4double Emin = pUserLimits->GetUserMinEkine( aTrack );
      if ( Emin > DBL_MIN )
      {
        Rmin = theLossTableManager->GetRange( Particle, Emin, couple );
        temp = RangeNow - Rmin;
        if ( temp < 0. )
          return 0.;
        if ( ProposedStep > temp )
          ProposedStep = temp;
      }
    }
  }
  return ProposedStep;
}

G4VParticleChange*
StepLimiterProcess::PostStepDoIt( const G4Track& aTrack, const G4Step& )
{
  aParticleChange.Initialize( aTrack );
  /*    aParticleChange.ProposeEnergy(0.); */
  /*
   *
   *
   *
   * aParticleChange.ProposeLocalEnergyDeposit(aTrack.GetKineticEnergy()); */
  /*    aParticleChange.ProposeTrackStatus(fStopAndKill); */
  return &aParticleChange;
}
