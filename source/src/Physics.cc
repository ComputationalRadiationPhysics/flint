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
 

#include "Physics.hh"

PhysicsList::~PhysicsList( )
{
}

PhysicsList::PhysicsList( ) :
  G4VUserPhysicsList( )
{
  defaultCutValue = 1.0 * cm;
  SetVerboseLevel( 1 );
}

void
PhysicsList::ConstructParticle( )
{

}

void
PhysicsList::ConstructProcess( )
{
  AddTransportation( );
  ConstructEM( );
  /*   ConstructHad(); */
}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4MultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4hIonisation.hh"

#include "StepLimiterProcess.hh"

#include "G4ProtonInelasticProcess.hh"
#include "G4LEProtonInelastic.hh"

#include "G4HadronElasticProcess.hh"
#include "G4HadronElastic.hh"

#include "G4HadronFissionProcess.hh"

void
PhysicsList::ConstructEM( )
{
  theParticleIterator->reset( );
  while ( ( *theParticleIterator )( ) )
  {
    G4ParticleDefinition* particle = theParticleIterator->value( );
    G4ProcessManager* pmanager = particle->GetProcessManager( );
    G4String particleName = particle->GetParticleName( );

    if ( pmanager == 0 )
    {
      pmanager = new G4ProcessManager( particle );
      particle->SetProcessManager( pmanager );
    }

    /* Massive particles are tracked by GPT. Therefore, the step
     * length is set by the tracker, not by G4. */
    if ( particle->GetPDGMass( ) != 0.0 )
      pmanager->AddDiscreteProcess( new StepLimiterProcess( ) );

    if ( particleName == "gamma" )
    {
      /* gamma */
      pmanager->AddDiscreteProcess( new G4PhotoElectricEffect );
      pmanager->AddDiscreteProcess( new G4ComptonScattering );
      pmanager->AddDiscreteProcess( new G4GammaConversion );

    }
    else
    if ( particleName == "e-" ) {
      /* electron */
      pmanager->AddProcess( new G4MultipleScattering, -1, 1, 1 );
      pmanager->AddProcess( new G4eIonisation, -1, 2, 2 );
      pmanager->AddProcess( new G4eBremsstrahlung, -1, 3, 3 );

    }
    else
    if ( particleName == "e+" ) {
      /* positron */
      pmanager->AddProcess( new G4MultipleScattering, -1, 1, 1 );
      pmanager->AddProcess( new G4eIonisation, -1, 2, 2 );
      pmanager->AddProcess( new G4eBremsstrahlung, -1, 3, 3 );
      pmanager->AddProcess( new G4eplusAnnihilation, 0, -1, 4 );

    }
    else
    if ( particleName == "proton" ) {
      G4VProcess* pMultipleScat = new G4MultipleScattering;
      G4VProcess* pIonisation = new G4hIonisation;

      pmanager->AddProcess( pMultipleScat, -1, 1, 1 );
      pmanager->AddProcess( pIonisation, -1, 2, 2 );

      G4ProtonInelasticProcess* theProtonIEProc = new G4ProtonInelasticProcess( );
      G4LEProtonInelastic* theProtonIE = new G4LEProtonInelastic( );
      theProtonIEProc->RegisterMe( theProtonIE );
      pmanager->AddDiscreteProcess( theProtonIEProc );

      G4HadronElasticProcess* theProtonEProc = new G4HadronElasticProcess( );
      G4HadronElastic* theProtonE = new G4HadronElastic( );
      theProtonEProc->RegisterMe( theProtonE );
      pmanager->AddDiscreteProcess( theProtonEProc );

      /*       G4HadronFissionProcess* theProtonFProc = new
       * G4HadronFissionProcess(); */
      /*       pmanager->AddDiscreteProcess(theProtonFProc); */
    }
  }
}

void
PhysicsList::SetCuts( )
{
  /* G4VUserPhysicsList::SetCutsWithDefault method sets the default
   * cut value for all particle types */
  SetCutsWithDefault( );

  double cut = 1.0 * mm;

  SetCutValue( cut, "e-" );
  SetCutValue( cut, "gamma" );
  SetCutValue( cut, "e+" );
  SetCutValue( cut, "proton" );

  G4ProductionCutsTable::GetProductionCutsTable( )->SetEnergyRange( 250 * eV,
                                                                    100 * GeV );

  if ( verboseLevel > 0 )
    DumpCutValuesTable( );
}
