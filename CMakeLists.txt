# Copyright 2009 Lucas Clemente
#
# This file is part of FLINT.
#
# FLINT is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FLINT is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FLINT.
# If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.6)

PROJECT(geant4_gpt CXX)

include(config/config.txt)

LINK_LIBRARIES(Geant4 CLHEP pthread xerces-c)

INCLUDE_DIRECTORIES(source/include)
INCLUDE_DIRECTORIES(source/src) # Needed for templates -,-
LINK_DIRECTORIES(lib)

# 32 bit architecture - GPT is built in 32 bit
# Debug / Release
# set(CMAKE_CXX_FLAGS "-O3 -m32 -g")
set(CMAKE_CXX_FLAGS "-O3 -m32")

# Version
execute_process(COMMAND svnversion -n . OUTPUT_VARIABLE version)
message("-- Detected revision ${version}")
file(WRITE source/include/svn_version.hh "\#ifndef svn_version_hh\n")
file(APPEND source/include/svn_version.hh "\#define svn_version_hh 1\n")
file(APPEND source/include/svn_version.hh "const char* flintVersion () { return \"")
file(APPEND source/include/svn_version.hh ${version})
file(APPEND source/include/svn_version.hh "\";};\n")
file(APPEND source/include/svn_version.hh "\#endif\n")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./lib/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "./lib/")

ADD_LIBRARY(flint STATIC ./source/src/Geometry.cc ./source/src/ParticleTable.cc ./source/src/mainG4.cc ./source/src/Physics.cc ./source/src/Primary.cc ./source/src/Stepping.cc ./source/include/Physics.hh ./source/include/Primary.hh ./source/include/Stepping.hh ./source/include/Geometry.hh ./source/include/ParticleTable.hh ./source/include/mainG4.hh ./source/include/Particle.hh ./source/include/Tools.hh ./source/src/OutputManager.cc ./source/include/OutputManager.hh ./source/include/OutputBase.hh ./source/include/OutputTrajectories.hh ./source/src/OutputTrajectories.cc ./source/include/OutputDose.hh ./source/src/OutputDose.cc ./source/include/Containers.hh ./source/include/XMLParser.hh ./source/src/XMLParser.cc ./source/include/StepLimiterProcess.hh ./source/src/StepLimiterProcess.cc)
