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
 


#ifndef StepLimiterProcess_hh
#define StepLimiterProcess_hh 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VProcess.hh"

class G4LossTableManager;

/*\brief This process sets step / time limits according to the
 * logical volume's G4UserLimits. */
/*StepLimiterProcess is basically taken from G4UserSpecialCuts. */
/*The PostStepDoIt routine is modified so that the particle is not
 * killed completly and other processes may still work. */
class StepLimiterProcess : public G4VProcess {
  public:

    StepLimiterProcess( const G4String& processName = "UserSpecialCut" );

    virtual ~StepLimiterProcess( );

    virtual G4double PostStepGetPhysicalInteractionLength(
      const G4Track& track,
      G4double
      previousStepSize,
      G4ForceCondition*
      condition );

    virtual G4VParticleChange* PostStepDoIt( const G4Track&, const G4Step& );

    virtual G4double
    AtRestGetPhysicalInteractionLength( const G4Track&, G4ForceCondition* )
    {
      return -1.0;
    }

    virtual G4VParticleChange*
    AtRestDoIt( const G4Track&, const G4Step& )
    {
      return 0;
    }

    virtual G4double
    AlongStepGetPhysicalInteractionLength( const G4Track&,
                                           G4double,
                                           G4double,
                                           G4double&,
                                           G4GPILSelection* )
    {
      return -1.0;
    }

    virtual G4VParticleChange*
    AlongStepDoIt( const G4Track&, const G4Step& )
    {
      return 0;
    }

  private:

    StepLimiterProcess( StepLimiterProcess & );
    StepLimiterProcess& operator = ( const StepLimiterProcess &right );

    G4LossTableManager* theLossTableManager;

};

#endif /* StepLimiterProcess_hh */
