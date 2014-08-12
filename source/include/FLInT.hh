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
 


/*\file FLInT.hh This file is currently not in use. It may be needed
 * in a future version of flint, possibly in combination with a 4dim
 * tracker... Until then, it will be kept in devel trunk. It may be
 * ignored ;) */

class Particle;

/*This abstract class should be used to create
 * communicator-policies. */
class CommunicatorPolicyBase {
  public:
    /* Init and deinit methods */
    virtual int initComm( );

    virtual void deinitComm( );

    /* Send and recieve methods for data communication */
    virtual void i2tsend( Particle* p, int dest );

    virtual void i2trecv( Particle* p, int src );

    virtual void t2isend( Particle* p, int dest );

    virtual void t2irecv( Particle* p, int src );

    /* Send and recieve methods for job communication */
    virtual void t2isend( int job, int dest );

    virtual void t2irecv( int job, int src );

};

/*This abstract class should be used to create tracker-policies. */
template< class CommunicatorPolicy >
class TrackerPolicyBase {
  public:
    /* Init and deinit calculation */
    virtual void initTracker( );

    virtual void deinitTracker( );

    /* Start calculation */
    virtual void run( );

};

/*This abstract class should be used to create interactor-policies. */
template< class CommunicatorPolicy >
class InteractorPolicyBase {
  public:
    /* Init calculation */
    virtual void initInteractor( );

    virtual void deinitInteractor( );

    /* Launch ready-mode */
    virtual void run( );

};

/*The main class for a calculation */
template
<
  class CommunicatorPolicy,
  template< class > class TrackerPolicy,
  template< class > class InteractorPolicy
>
class FLInT :
public CommunicatorPolicy,
public TrackerPolicy< CommunicatorPolicy >,
public InteractorPolicy< CommunicatorPolicy >
{
  public:
    FLInT( );
    ~FLInT( );

    int startCalc( );

}
