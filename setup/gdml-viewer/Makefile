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

CFLAGS = $(shell root-config --cflags)
GLIBS  = $(shell root-config --glibs) -lGeom
LDFLAGS = $(shell root-config --ldflags)

gdml: gdml.cpp
	g++ -c gdml.cpp $(CFLAGS)
	g++ $(LDFLAGS) $(GLIBS) gdml.o -o gdml

clean:
	@rm -f gdml
