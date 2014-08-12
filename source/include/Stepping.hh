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
 

#ifndef Stepping_hh
#define Stepping_hh 1

#include "Tools.hh"
#include "G4UserSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "Particle.hh"
#include "OutputManager.hh"

/* The 'SteppingAction' is called after every (and the only) step.
 * In 'UserSteppingAction', */
/* all relevant data for GPT can be extracted and saved to the
 * 'results'-vector as a 'Particle'. */
class SteppingAction : public G4UserSteppingAction {
  private:
    /*The main particle where to save the resulting data */
    Particle* pResult;
    /*Vector of particles to send back to GPT (main particle +
     * secondaries) */
    std::vector< Particle >* results;
    /*Limits */
    G4UserLimits* limits;
    /*Total track limit */
    double limit;
  public:
    SteppingAction( G4UserLimits * f_limits );
    ~SteppingAction( );

    /* Set the pointers where to save the results. */
    /* 'pResult' still has to be put to the vector, it is not
     * included. */
    /* 'results' should be empty here... */
    void inline
    setResults( std::vector< Particle >* f_results, Particle* f_pResult )
    {
      pResult = f_pResult;
      results = f_results;
    }

    /*Set the total track limit */
    void setLimit( double f_limit );

    /*Write the data of one step to the 'results'-array */
    void storeInfo( Particle* p, const G4Step* step );

    /* This function is called after every step. */
    /* Primary-particles should be killed here, if they are massive.
     * They will be tracked by GPT. Massless particles */
    /* must not be killed, as they would get lost then. This
     * function is also the place where to read secondaries. */
    void UserSteppingAction( const G4Step* step );

};

#endif /* Stepping_hh */
