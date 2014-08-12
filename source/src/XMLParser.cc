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
 


#include "XMLParser.hh"

int
XMLParser::parse( )
{
  MSG << "XMLParser::parse('" << SETUP_FILE << "')" << NL;
  /* Init XML */
  try
  {
    XMLPlatformUtils::Initialize( );
  }
  catch( const XMLException& toCatch )
  {
    char* message = XMLString::transcode( toCatch.getMessage( ) );
    ERROR << "Error during initialization! Exception message is:" << NL;
    ERROR_CONT << message << ERROR_END;
    XMLString::release( &message );
    return 1;
  }
  /* Setup the parser */
  char* xmlFile = SETUP_FILE;
  SAXParser* parser = new SAXParser( );
  parser->setDoValidation( true );
  parser->setValidationScheme( SAXParser::Val_Always );
  parser->setDoNamespaces( true );    /* optional */

  /* Setup the handler */
  HandlerBase* handler = new XMLHandler( );
  parser->setDocumentHandler( handler );
  parser->setErrorHandler( handler );

  /* Parse */
  try
  {
    parser->parse( xmlFile );
  }
  catch( const XMLException& toCatch )
  {
    char* message = XMLString::transcode( toCatch.getMessage( ) );
    ERROR << "Exception message is:" << NL;
    ERROR_CONT << message << ERROR_END;
    XMLString::release( &message );
    return -1;
  }
  catch( const SAXParseException& toCatch )
  {
    char* message = XMLString::transcode( toCatch.getMessage( ) );
    ERROR << "Exception message is:" << NL;
    ERROR_CONT << message << ERROR_END;
    XMLString::release( &message );
    return -1;
  }
  catch( ... )
  {
    ERROR << "Unexpected Exception" << ERROR_END;
    return -1;
  }
  /* Clear up */
  delete parser;
  delete handler;
  return 0;
}

XMLHandler::XMLHandler( )
{

}

/*\todo Place to add additional output */
void
XMLHandler::startElement( const XMLCh* const name, AttributeList& attributes )
{
  if ( !name )
    return;
  XMLCh* particle_str = XMLString::transcode( "particle" );
  XMLCh* output_str = XMLString::transcode( "output" );
  /* Is this a 'particle' tag? */
  if ( XMLString::equals( name, particle_str ) )
  {
    /* Particle information */
    const XMLCh* p_name = attributes.getValue( "name" );
    ParticleTable::getInstance( )->addParticleName( XMLString::transcode(
                                                      p_name ) );
  }
  else
  if ( XMLString::equals( name, output_str ) ) {
    /* Output information */
    const XMLCh* output_name = attributes.getValue( "name" );
    MSG << "Found output '" << XMLString::transcode( output_name ) <<
    "' in '" << SETUP_FILE << "'." << NL;
    XMLCh* trajectories_str = XMLString::transcode( "trajectories" );
    XMLCh* dose_str = XMLString::transcode( "dose" );
    /* Check all possibe outputs */
    if ( XMLString::equals( output_name, trajectories_str ) )
      /* Trajectories */
      OutputManager::getInstance( )->addOutput( new OutputTrajectories( ) );

    else
    if ( XMLString::equals( output_name, dose_str ) ) {
      /* Dose */
      /* This was the point where I decided not to mind any leaks. */
      /* The leakless code would be about 10 times larger ;) */
      double x_min = strtod( XMLString::transcode( attributes.getValue(
                                                     "x_min" ) ), 0 );
      double x_max = strtod( XMLString::transcode( attributes.getValue(
                                                     "x_max" ) ), 0 );
      double y_min = strtod( XMLString::transcode( attributes.getValue(
                                                     "y_min" ) ), 0 );
      double y_max = strtod( XMLString::transcode( attributes.getValue(
                                                     "y_max" ) ), 0 );
      double z_min = strtod( XMLString::transcode( attributes.getValue(
                                                     "z_min" ) ), 0 );
      double z_max = strtod( XMLString::transcode( attributes.getValue(
                                                     "z_max" ) ), 0 );
      int nx = atoi( XMLString::transcode( attributes.getValue( "nx" ) ) );
      int ny = atoi( XMLString::transcode( attributes.getValue( "ny" ) ) );
      int nz = atoi( XMLString::transcode( attributes.getValue( "nz" ) ) );

      OutputManager::getInstance( )->addOutput( new OutputDose( x_min * mm,
                                                                x_max * mm,
                                                                y_min * mm,
                                                                y_max * mm,
                                                                z_min * mm, z_max * mm, nx, ny, nz ) );
    }
    else
      /* Not found */
      WARN << "Output option '" << output_name <<
      "' not recognized. Ignoring it!" << WARN_END;

    XMLString::release( &trajectories_str );
    XMLString::release( &dose_str );
  }
  XMLString::release( &particle_str );
  XMLString::release( &output_str );
}

void
XMLHandler::fatalError( const SAXParseException& exception )
{
  char* message = XMLString::transcode( exception.getMessage( ) );
  ERROR << "Fatal Error: " << message
        << " at line: " << exception.getLineNumber( )
        << ERROR_END;
}
