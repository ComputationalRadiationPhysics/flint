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
 


#ifndef OutputTrajectories_hh
#define OutputTrajectories_hh 1

#include <map>
#include <fstream>

#include "G4String.hh"

#include "OutputBase.hh"
#include "Tools.hh"

/*\todo Attention! A secondary's first step can NOT be outputed by
 * this function as it has no GPT ID before the next GPT step. Use
 * direct GPT output instead. */

/*This class ist used for outputing trajectories */
class OutputTrajectories : public OutputBase {
  private:
    /*The result files are saved in this map */
    std::map< G4String, std::ofstream* > trajectoryFiles;

  public:
    OutputTrajectories( );
    ~OutputTrajectories( );

    void outputData( OutputParticle* p );

    void finalize( );

};

#endif /* OutputTrajectories_hh */
