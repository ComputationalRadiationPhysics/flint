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
 


#ifndef XMLParser_hh
#define XMLParser_hh 1

#include <cstdlib>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>

#include "OutputManager.hh"
#include "ParticleTable.hh"

#include "OutputDose.hh"
#include "OutputTrajectories.hh"

XERCES_CPP_NAMESPACE_USE

class XMLParser {
  public:
    /*Parse the XML file */
    static int parse( );

};

/*This handler reads the 'setup.xml' and extracts particles and
 * output options */
class XMLHandler : public HandlerBase {
  public:
    /*Constructor */
    XMLHandler( );
    /*This function is called at the beginning of each XML-Tag */
    void startElement( const XMLCh* const, AttributeList& );

    /*In case a fatal error occurs, this function is called */
    void fatalError( const SAXParseException& exception );

};

#endif /* XMLParser_hh */
