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
 


/*\file Tools.hh This file contains general tools used in
 * programs... */
/*Use MSG and NL instead of std::cout and std::endl! */

#ifndef Tools_hh
#define Tools_hh 1

/*The configuration file */
/*\todo Future: Commandline option? */
#define SETUP_FILE "setup/setup.gdml"

#define TRAJECTORY_PREFIX "./output/traj_"
#define OUTPUT_DOSE "./output/dose"
#define OUTPUT_ENERGY "./output/energy"

#include <iostream>

static const char setblack[] = "\033[m";
static const char setred[] = "\033[31m";
static const char setlightred[] = "\033[1;31m";
static const char setgreen[] = "\033[32m";
static const char setyellow[] = "\033[33m";
static const char setblue[] = "\033[34m";
static const char setlightblue[] = "\033[1;34m";
static const char setmagenta[] = "\033[35m";
static const char setlightmagenta[] = "\033[1;35m";
static const char setcyan[] = "\033[36m";
static const char bell[] = "\007\007\007";

/*The default color for text */
#define stdcolor setlightblue

#define NL setblack << std::endl << std::flush
#define MSG std::cout << setlightblue << ">> " << __FILE__ << "(" << \
  __LINE__ << "): "

#define WARN \
  std::cout << NL << setlightmagenta << \
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << \
  std::endl \
  << "!!!!! WARNING! " << __FILE__ << "(" << __LINE__ << ")" << \
  std::endl << "!!!!! "
#define WARN_CONT std::cout << setlightmagenta << "!!!!! "
#define WARN_END NL << setlightmagenta << \
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << \
  NL << \
  NL

#define ERROR \
  std::cout << NL << setlightred << \
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << \
  std::endl \
  << "!!!!! ERROR! " << __FILE__ << "(" << __LINE__ << ")" << \
  std::endl << "!!!!! "
#define ERROR_CONT std::cout << setlightred << "!!!!! "
#define ERROR_END NL << setlightred << \
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << \
  NL << \
  NL

#endif /* Tools_hh */
