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
 


/*\file mainG4.hh This file is included from both, GPT and G4. Thus,
 * it has to be compatible to C *and* C++. Watch out for preprocessor
 * commands... */

#ifndef mainG4_hh
#define mainG4_hh 1

/* The cpp header follows */
#ifdef __cplusplus

/* ------------------------------------------------------------------------// */
/* ---- C++ header here
 * ---------------------------------------------------// */
/* ------------------------------------------------------------------------// */
/* ------------------------------------------------------------------------// */

# include "G4RunManager.hh"
# include "G4UserLimits.hh"
# include "G4Navigator.hh"
# include "G4UImanager.hh"

# include "Primary.hh"
# include "Stepping.hh"
# include "Physics.hh"
# include "Geometry.hh"
# include "ParticleTable.hh"
# include "OutputManager.hh"
# include "XMLParser.hh"

# include "Tools.hh"

/*The main class for G4 calculation. */
/*\todo Make sure that all members are really needed (static
 * instances?) */
class G4Calc {
  private:
    G4RunManager* runManager;
    G4UserLimits* limits;
    Geometry* geometry;
    ParticleTable* particleTable;
    PhysicsList* physics;
    SteppingAction* steppingAction;
    PrimaryGeneratorAction* primaryAction;
    G4VPhysicalVolume* worldVolume;
    G4Navigator* findNavigator;
    OutputManager* outputManager;

    static G4Calc* instance;
  public:
    /*Does nothing... */
    G4Calc( );
    /*Shutdown G4. */
    ~G4Calc( );

    /*Init G4. Has to be called before any other function, otherwise
     * your cluster will explode. */
    bool init( );

    /*\brief Calculates whether a given point is inside of a G4
     * geometry and thus needs to be calculated */
    /*Attention: This function does NOT cover worlds filled with
     * anything! Information about the world is lost (intended)... */
    bool insideGeometry( double x, double y, double z );

    /*This is a heavy function doing everything... It just can't
     * make cake, as the cake is a lie. */
    bool calculate( int ID,                          /*!< The
                                                      *particle's ID,
                                                      *used for
                                                      *output.*/
                    double* position_pre,         /*!< The particle's
                                                   *position before
                                                   *the GPT-step.*/
                    double* position_post,        /*!< The particle's
                                                   *position after the
                                                   *GPT-step. Will be
                                                   *changed by this
                                                   *funtion.*/
                    double* momentum_post,        /*!< The particle's
                                                   *momentum after the
                                                   *GPT-step. Will be
                                                   *changed by this
                                                   *funtion.*/
                    double dt,                    /*!< The time that
                                                   *G4 should
                                                   *calculate.*/
                    double* G,                    /*!< The particle's
                                                   *lorentz-factor.
                                                   *Will be changed by
                                                   *this function.*/
                    double q,                     /*!< The particle's
                                                   *charge.*/
                    double m,                     /*!< The particle's
                                                   *mass (Before AND
                                                   *after GPT. If not,
                                                   *write to
                                                   *Fleischmann and
                                                   *Pons).*/
                    int* n_secondaries,           /*!< Number of
                                                   *secondaries
                                                   *produced by G4.
                                                   *Will be changed by
                                                   *this function.*/
                    double** secondaries_position, /*!< Position of
                                                    *all secondaries
                                                    *(array). Will be
                                                    *changed by this
                                                    *function.*/
                    double** secondaries_momentum, /*!< Momentum of
                                                    *all secondaries
                                                    *(array). Will be
                                                    *changed by this
                                                    *function.*/
                    double** secondaries_m,       /*!< Mass of all
                                                   *secondaries
                                                   *(array). Will be
                                                   *changed by this
                                                   *function.*/
                    double** secondaries_q        /*!< Charge of all
                                                   *secondaries
                                                   *(array). Will be
                                                   *changed by this
                                                   *function.*/
                  );

    void outputData( int ID,
                     double x,
                     double y,
                     double z,
                     double v_x,
                     double v_y,
                     double v_z,
                     double E,
                     double E_loss,
                     double m,
                     double q );

    /*Return a pointer to the (custom) geometry navigator */
    G4Navigator*
    getCustomNavigator( )
    {
      if ( !findNavigator )
        WARN <<
        "This will fail. 'findNavigator' is zero when requesting it!" <<
        WARN_END;

      return findNavigator;
    }

    /*Returns a pointer to the global instance of G4Calc. If it does
     * not exist, this function creates a new one. */
    static G4Calc*
    getInstance( )
    {
      return ( instance == NULL ) ? ( instance = new G4Calc( ) ) : instance;
    }
};

/* Mark the next functions to be compiled compatible to C */
extern "C" {
/* End of C++ part, the following is parsed by both */
#endif /* __cplusplus */

/* ------------------------------------------------------------------------// */
/* ---- Wrapper functions here
 * --------------------------------------------// */
/* ------------------------------------------------------------------------// */
/* ------------------------------------------------------------------------// */

int G4_init( );

void G4_deinit( );

int G4_insideGeometry( double x, double y, double z );

int G4_calculate( int ID,
                  double* position_pre,
                  double* position_post,
                  double* momentum_post,
                  double dt,
                  double* G_post,
                  double q,
                  double m,
                  int* n_secondaries,
                  double** secondaries_position,
                  double** secondaries_momentum,
                  double** secondaries_m,
                  double** secondaries_q );

void outputData( int ID,
                 double x,
                 double y,
                 double z,
                 double v_x,
                 double v_y,
                 double v_z,
                 double E,
                 double E_loss,
                 double m,
                 double q );

#ifdef __cplusplus
} /* This brace belongs to "extern C". */
#endif /* __cplusplus */

#endif /* mainG4_hh */
