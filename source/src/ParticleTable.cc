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

#include "ParticleTable.hh"

ParticleTable* ParticleTable::instance = 0;

ParticleTable::ParticleTable( )
{
  MSG << "ParticleTable::ParticleTable()" << NL;
  instance = this;
}

ParticleTable::~ParticleTable( )
{
  MSG << "ParticleTable::~ParticleTable()" << NL;
  instance = 0;
}

void
ParticleTable::addParticleName( char* name )
{
  definition_names.push_back( G4String( name ) );
}

/* Beware of automatic indenting ;) */
void
ParticleTable::registerParticleDefs( )
{
  /* Iterate over all names and try to find a particle with that name */
  for ( std::vector< G4String >::iterator it = definition_names.begin( );
        it != definition_names.end( );
        it++ )
  {
    G4String name = ( *it );
    /* Bosons */
    if ( name == "gamma" )
      addParticleDef( G4Gamma::GammaDefinition( ) );
    else
      /* Leptons */
      if ( name == "e-" )
        addParticleDef( G4Electron::ElectronDefinition( ) );
      else
        if ( name == "e+" )
          addParticleDef( G4Positron::PositronDefinition( ) );
        else
          /* mu+/- */
          if ( name == "mu+" )
            addParticleDef( G4MuonPlus::MuonPlusDefinition( ) );
          else
            if ( name == "mu-" )
              addParticleDef( G4MuonMinus::MuonMinusDefinition( ) );
            else
              /* nu_e */
              if ( name == "neutrino_e" )
                addParticleDef( G4NeutrinoE::NeutrinoEDefinition( ) );
              else
                if ( name == "antineutrino_e" )
                  addParticleDef( G4AntiNeutrinoE::AntiNeutrinoEDefinition( ) );
                else
                  /* nu_mu */
                  if ( name == "neutrino_mu" )
                    addParticleDef( G4NeutrinoMu::NeutrinoMuDefinition( ) );
                  else
                    if ( name == "antineutrino_mu" )
                      addParticleDef(
                         G4AntiNeutrinoMu::AntiNeutrinoMuDefinition( ) );
                    else
                      /* mesons */
                      /* light mesons */
                      if ( name == "pion+" )
                        addParticleDef( G4PionPlus::PionPlusDefinition( ) );
                      else
                        if ( name == "pion-" )
                          addParticleDef( G4PionMinus::PionMinusDefinition( ) );
                        else
                          if ( name == "pion0" )
                            addParticleDef( G4PionZero::PionZeroDefinition( ) );
                          else
                            if ( name == "eta" )
                              addParticleDef( G4Eta::EtaDefinition( ) );
                            else
                              if ( name == "eta_prime" )
                                addParticleDef( G4EtaPrime::EtaPrimeDefinition( ) );
                              else
                                if ( name == "kaon+" )
                                  addParticleDef(
                                     G4KaonPlus::KaonPlusDefinition( ) );
                                else
                                  if ( name == "kaon-" )
                                    addParticleDef(
                                       G4KaonMinus::KaonMinusDefinition( ) );
                                  else
                                    if ( name == "kaon0" )
                                      addParticleDef(
                                         G4KaonZero::KaonZeroDefinition( ) );
                                    else
                                      if ( name == "antikaon0" )
                                        addParticleDef(
                                           G4AntiKaonZero::
                                          AntiKaonZeroDefinition( ) );
                                      else
                                        if ( name == "kaon0_long" )
                                          addParticleDef(
                                             G4KaonZeroLong::
                                            KaonZeroLongDefinition( ) );
                                        else
                                          if ( name == "kaon0_short" )
                                            addParticleDef(
                                               G4KaonZeroShort::
                                              KaonZeroShortDefinition( ) );
                                          else
                                            /* barions */
                                            if ( name == "proton" )
                                              addParticleDef(
                                                 G4Proton::ProtonDefinition( ) );
                                            else
                                              if ( name == "antiproton" )
                                                addParticleDef(
                                                   G4AntiProton::
                                                  AntiProtonDefinition( ) );
                                              else
                                                if ( name == "neutron" )
                                                  addParticleDef(
                                                     G4Neutron::
                                                    NeutronDefinition( ) );
                                                else
                                                  if ( name == "antineutron" )
                                                    addParticleDef(
                                                       G4AntiNeutron::
                                                      AntiNeutronDefinition( ) );
                                                  else
                                                    if ( name == "alpha" )
                                                      addParticleDef(
                                                         G4Alpha::
                                                        AlphaDefinition( ) );
                                                    else
                                                      if ( name == "alpha+" )
                                                        addParticleDef(
                                                           G4Alpha::
                                                          AlphaDefinition( ) );
                                                      else
                                                        if ( name == "deuteron" )
                                                          addParticleDef(
                                                             G4Deuteron::
                                                            DeuteronDefinition( ) );
                                                        else
                                                          if ( name == "triton" )
                                                            addParticleDef(

                                                               G4Triton::
                                                              TritonDefinition( ) );
                                                          else
                                                            /* Ions -
                                                             * experimental */
                                                            if ( name == "ion" )
                                                              addParticleDef(
                                                                 G4GenericIon::
                                                                GenericIonDefinition( ) );
                                                            else
                                                            {
                                                              /* Not
                                                               * found */
                                                              WARN <<
                                                              "Particle '" <<
                                                              name <<
                                                              "' could not be found."
                                                              << NL;
                                                              WARN_CONT <<
                                                              "Physics is continuing without it."
                                                                        <<
                                                              WARN_END;
                                                            }
  }
}

void
ParticleTable::addParticleDef( G4ParticleDefinition* def )
{
  definitions.push_back( def );
  MSG << "Adding particle definition '" << def->GetParticleName( ) <<
  "' to Physics." << NL;
}

double
sqr( double x )
{
  return x * x;
}

G4ParticleDefinition*
ParticleTable::getParticleDef( double m, double q )
{
  /* Minimum abberation */
  double min = DBL_MAX;
  /* Index of the temporally best definition */
  int index;
  /* Check all definitions */
  for ( int i = 0; i < definitions.size( ); i++ )
  {
    /*\todo Use relative errors */
    /* Calculate the difference as '|m_1 - m| * |q_1 - q|' */
    double diff = fabs( m * c_squared - definitions[i]->GetPDGMass( ) ) * fabs(
       q - definitions[i]->GetPDGCharge( ) );
    if ( diff < min )
    {
      min = diff;
      index = i;
    }
  }
  /* return the best particle */
  return definitions[index];
}
