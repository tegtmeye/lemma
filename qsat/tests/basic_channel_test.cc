/**
 *  Copyright (c) 2012, Mike Tegtmeyer
 *  All rights reserved.
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      * Neither the name of the author nor the names of its contributors may
 *        be used to endorse or promote products derived from this software
 *        without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <boost/test/unit_test.hpp>

#include "test_types.h"

#include <lemma/qsat/basic_channel.h>

/** \file
 *  \brief Unit tests for basic_channel as model of Channel and std::sequence
 */

namespace lemma {
namespace qsat {
namespace test {

BOOST_AUTO_TEST_SUITE( channel_suite )

/** \test Check additions to std::sequence handled std::vector
 */
BOOST_AUTO_TEST_CASE( basic_channel_default_constructors_test )
{
  float_channel bc1;
  BOOST_CHECK( bc1.empty() );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );

  float_channel bc2(std::make_pair(.5,-.2));
  BOOST_CHECK( bc2.empty() );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), -.2f );
}

/** \test Check additions to std::sequence (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_default_constructors_test )
{
  float_list_channel bc1;
  BOOST_CHECK( bc1.empty() );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );

  float_list_channel bc2(std::make_pair(.5,-.2));
  BOOST_CHECK( bc2.empty() );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), -.2f );
}

/** \test Check to ensure fill dispatch is working
 */
BOOST_AUTO_TEST_CASE( basic_channel_fill_construction_test )
{
  float_channel bc1(1);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(1) );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );
  BOOST_CHECK_EQUAL( bc1.front(), 0.0f );

  float_channel bc2(1,2);
  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(1) );
  BOOST_CHECK_EQUAL( bc2.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc2.epoch(), 0 );
  BOOST_CHECK_EQUAL( bc2.front(), 2.0f );

  float_channel bc3(1,2,3);
  BOOST_CHECK_EQUAL( bc3.size(), std::size_t(1) );
  BOOST_CHECK_EQUAL( bc3.frequency(), 3 );
  BOOST_CHECK_EQUAL( bc3.epoch(), 0 );
  BOOST_CHECK_EQUAL( bc3.front(), 2.0f );

  float_channel bc4(1,2,3,4);
  BOOST_CHECK_EQUAL( bc4.size(), std::size_t(1) );
  BOOST_CHECK_EQUAL( bc4.frequency(), 3 );
  BOOST_CHECK_EQUAL( bc4.epoch(), 4 );
  BOOST_CHECK_EQUAL( bc4.front(), 2.0f );
}

/** \test Check to ensure fill dispatch is working (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_fill_construction_test )
{
  float_list_channel bc1(1);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(1) );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );
  BOOST_CHECK_EQUAL( bc1.front(), 0.0f );

  float_list_channel bc2(1,2);
  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(1) );
  BOOST_CHECK_EQUAL( bc2.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc2.epoch(), 0 );
  BOOST_CHECK_EQUAL( bc2.front(), 2.0f );

  float_list_channel bc3(1,2,3);
  BOOST_CHECK_EQUAL( bc3.size(), std::size_t(1) );
  BOOST_CHECK_EQUAL( bc3.frequency(), 3 );
  BOOST_CHECK_EQUAL( bc3.epoch(), 0 );
  BOOST_CHECK_EQUAL( bc3.front(), 2.0f );

  float_list_channel bc4(1,2,3,4);
  BOOST_CHECK_EQUAL( bc4.size(), std::size_t(1) );
  BOOST_CHECK_EQUAL( bc4.frequency(), 3 );
  BOOST_CHECK_EQUAL( bc4.epoch(), 4 );
  BOOST_CHECK_EQUAL( bc4.front(), 2.0f );
}

/** \test Check to assignemnt with new frequence and epoch
 */
BOOST_AUTO_TEST_CASE( basic_channel_assignment_construction_test )
{
  float_channel bc1(mags,mags+5);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),mags,mags+5 );

  float_channel bc2(mags,mags+5,.5);
  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), 0 );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc2.begin(),bc2.end(),mags,mags+5 );

  float_channel bc3(mags,mags+5,.5,-.2);
  BOOST_CHECK_EQUAL( bc3.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc3.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc3.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc3.begin(),bc3.end(),mags,mags+5 );
}


/** \test Check to assignemnt with new frequence and epoch (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_assignment_construction_test )
{
  float_list_channel bc1(mags,mags+5);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),mags,mags+5 );

  float_list_channel bc2(mags,mags+5,.5);
  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), 0 );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc2.begin(),bc2.end(),mags,mags+5 );

  float_list_channel bc3(mags,mags+5,.5,-.2);
  BOOST_CHECK_EQUAL( bc3.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc3.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc3.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc3.begin(),bc3.end(),mags,mags+5 );
}

/** \test Check copy construction
 */
BOOST_AUTO_TEST_CASE( basic_channel_copy_construction_test )
{
  float_channel bc1(mags,mags+5,.5,-.2);
  const float_channel &cbc1 = bc1;

  float_channel bc2(bc1);
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), -.2f );
  
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
}

/** \test Check copy construction (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_copy_construction_test )
{
  float_list_channel bc1(mags,mags+5,.5,-.2);
  const float_list_channel &cbc1 = bc1;

  float_list_channel bc2(bc1);
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), -.2f );
  
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
}

/** \test Check assignment
 */
BOOST_AUTO_TEST_CASE( basic_channel_assignment_test )
{
  float_channel bc1(mags,mags+5,.5,-.2);
  const float_channel &cbc1 = bc1;

  float_channel bc2;
  const float_channel &cbc2 = bc2;
  
  bc2 = bc1;
  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), -.2f );
  
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
}

/** \test Check assignment (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_assignment_test )
{
  float_list_channel bc1(mags,mags+5,.5,-.2);
  const float_list_channel &cbc1 = bc1;

  float_list_channel bc2;
  const float_list_channel &cbc2 = bc2;
  
  bc2 = bc1;
  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), -.2f );
  
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
}

/** \test Check assign
 */
BOOST_AUTO_TEST_CASE( basic_channel_assign_test )
{
  float_channel bc1;
  const float_channel &cbc1 = bc1;
  
  bc1.assign(mags,mags+5);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),mags,mags+5 );

  bc1.assign(mags,mags+5,.5,-.2);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc1.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),mags,mags+5 );

  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  //check new base
  bc2.assign(mags,mags+5,.5,-.2);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check assign (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_assign_test )
{
  float_list_channel bc1;
  const float_list_channel &cbc1 = bc1;
  
  bc1.assign(mags,mags+5);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),mags,mags+5 );

  bc1.assign(mags,mags+5,.5,-.2);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc1.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),mags,mags+5 );

  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  //check new base
  bc2.assign(mags,mags+5,.5,-.2);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check fill assignment
 */
BOOST_AUTO_TEST_CASE( basic_channel_fill_test )
{
  float_channel bc1;
  const float_channel &cbc1 = bc1;
  
  bc1.assign(5,.2);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),fill,fill+5 );

  bc1.assign(5,.2,.5,-.2);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc1.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),fill,fill+5 );

  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  //check new base
  bc2.assign(5,.2,.5,-.2);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check fill assignment (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_fill_test )
{
  float_list_channel bc1;
  const float_list_channel &cbc1 = bc1;
  
  bc1.assign(5,.2);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), 1 );
  BOOST_CHECK_EQUAL( bc1.epoch(), 0 );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),fill,fill+5 );

  bc1.assign(5,.2,.5,-.2);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc1.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),fill,fill+5 );

  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  //check new base
  bc2.assign(5,.2,.5,-.2);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check access
 */
BOOST_AUTO_TEST_CASE( basic_channel_access_test )
{
  float_channel bc1(5,.2,.5,-.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;
  
  // begin
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.begin();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.begin();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // end
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.end();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.end();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // rbegin
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.rbegin();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.rbegin();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // rend
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.rend();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.rend();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // operator[]
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2[0];
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2[0];
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // at()
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.at(0);
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.at(0);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // front()
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.front();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.front();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // back()
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.back();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.back();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check access (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_access_test )
{
  float_list_channel bc1(5,.2,.5,-.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;
  
  // begin
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.begin();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.begin();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // end
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.end();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.end();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // rbegin
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.rbegin();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.rbegin();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // rend
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.rend();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.rend();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // front()
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.front();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.front();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  // back()
  // reset
  bc2 = bc1;
  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
  cbc2.back();
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.back();
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check resize
 */
BOOST_AUTO_TEST_CASE( basic_channel_resize_test )
{
  float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.resize(10,.1);
  BOOST_CHECK_EQUAL( bc1.size(), 5u);
  BOOST_CHECK_EQUAL( bc2.size(), 10u);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check resize (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_resize_test )
{
  float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.resize(10,.1);
  BOOST_CHECK_EQUAL( bc1.size(), 5u);
  BOOST_CHECK_EQUAL( bc2.size(), 10u);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check reserve
 */
BOOST_AUTO_TEST_CASE( basic_channel_reserve_test )
{
  float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.reserve(10);
  BOOST_CHECK_EQUAL( bc1.capacity(), 5u);
  BOOST_CHECK_EQUAL( bc2.capacity(), 10u);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check stack operations
 */
BOOST_AUTO_TEST_CASE( basic_channel_stack_test )
{
  float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.push_back(.2);
  BOOST_CHECK_EQUAL( bc1.size(), 5u);
  BOOST_CHECK_EQUAL( bc2.size(), 6u);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  bc2 = bc1;
  bc2.pop_back();
  BOOST_CHECK_EQUAL( bc1.size(), 5u);
  BOOST_CHECK_EQUAL( bc2.size(), 4u);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check stack operations (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_stack_test )
{
  float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  bc2.push_back(.2);
  BOOST_CHECK_EQUAL( bc1.size(), 5u);
  BOOST_CHECK_EQUAL( bc2.size(), 6u);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );

  bc2 = bc1;
  bc2.pop_back();
  BOOST_CHECK_EQUAL( bc1.size(), 5u);
  BOOST_CHECK_EQUAL( bc2.size(), 4u);
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check single insert operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_single_insert_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  float ivalues[] = {
    .2,.2,.2,.5,.2,.2
  };
  
  const float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_channel::iterator res = bc2.insert(bc2.begin()+3,.5);

  BOOST_CHECK_EQUAL( *res, .5 );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    ivalues,ivalues+sizeof(ivalues)/sizeof(float));
}

/** \test Check single insert operation (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_single_insert_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  float ivalues[] = {
    .2,.2,.2,.5,.2,.2
  };
  
  const float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_list_channel::iterator iter = bc2.begin();
  std::advance(iter,3);
  float_list_channel::iterator res = bc2.insert(iter,.5);

  BOOST_CHECK_EQUAL( *res, .5 );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    ivalues,ivalues+sizeof(ivalues)/sizeof(float));
}

/** \test Check fill insert operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_fill_insert_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  float ivalues[] = {
    .2,.2,.2,.5,.5,.5,.2,.2
  };
  
  const float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  bc2.insert(bc2.begin()+3,3,.5);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    ivalues,ivalues+sizeof(ivalues)/sizeof(float));
}

/** \test Check fill insert operation (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_fill_insert_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  float ivalues[] = {
    .2,.2,.2,.5,.5,.5,.2,.2
  };
  
  const float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_list_channel::iterator iter = bc2.begin();
  std::advance(iter,3);
  bc2.insert(iter,3,.5);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    ivalues,ivalues+sizeof(ivalues)/sizeof(float));
}

/** \test Check random access iterator, random access base container
 *  range insert operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_RA_RA_range_insert_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  float ivalues[] = {
    .2,.2,.2,.5,.5,.5,.2,.2
  };
  
  const float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  bc2.insert(bc2.begin()+3,ivalues+3,ivalues+6);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    ivalues,ivalues+sizeof(ivalues)/sizeof(float));
}

/** \test Check bidirectional iterator, random access base container
 *  range insert operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_BI_RA_range_insert_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  float ivalues[] = {
    .2,.2,.2,.5,.5,.5,.2,.2
  };
  
  const float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  std::list<float> ilist(3,.5);

  bc2.insert(bc2.begin()+3,ilist.begin(),ilist.end());

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    ivalues,ivalues+sizeof(ivalues)/sizeof(float));
}

/** \test Check random access iterator, bidirectional base container
 *  range insert operation (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_RA_BI_range_insert_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  float ivalues[] = {
    .2,.2,.2,.5,.5,.5,.2,.2
  };
  
  const float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_list_channel::iterator iter = bc2.begin();
  std::advance(iter,3);
  bc2.insert(iter,ivalues+3,ivalues+6);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    ivalues,ivalues+sizeof(ivalues)/sizeof(float));
}

/** \test Check bidirectional iterator, bidirectional base container
 *  range insert operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_BI_BI_range_insert_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  float ivalues[] = {
    .2,.2,.2,.5,.5,.5,.2,.2
  };
  
  const float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  std::list<float> ilist(3,.5);

  float_list_channel::iterator iter = bc2.begin();
  std::advance(iter,3);
  bc2.insert(iter,ilist.begin(),ilist.end());

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    ivalues,ivalues+sizeof(ivalues)/sizeof(float));
}

/** \test Check erase operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_erase_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_channel::iterator res = bc2.erase(bc2.begin()+3);
  BOOST_CHECK(res == cbc2.begin()+3);
  BOOST_CHECK_EQUAL(cbc2.size(),sizeof(values)/sizeof(float)-1);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float)-1);
}

/** \test Check erase operation (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_erase_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_list_channel::iterator iter = bc2.begin();
  std::advance(iter,3);
  float_list_channel::iterator res = bc2.erase(iter);
  iter = bc2.begin();
  std::advance(iter,3);
  BOOST_CHECK(iter == res);
  BOOST_CHECK_EQUAL(cbc2.size(),sizeof(values)/sizeof(float)-1);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float)-1);
}

/** \test Check range erase end operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_erase_range_end_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_channel::iterator res = bc2.erase(bc2.begin()+3,bc2.end());
  BOOST_CHECK_EQUAL(cbc2.size(),3u);
  BOOST_CHECK(res == cbc2.end());

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),values,values+3);
}

/** \test Check range erase end operation (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_erase_range_end_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_list_channel::iterator iter = bc2.begin();
  std::advance(iter,3);
  float_list_channel::iterator res = bc2.erase(iter,bc2.end());
  BOOST_CHECK_EQUAL(cbc2.size(),3u);
  BOOST_CHECK(res == cbc2.end());

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),values,values+3);
}

/** \test Check range erase middle operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_erase_range_middle_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_channel::iterator res = bc2.erase(bc2.begin()+2,bc2.begin()+4);
  BOOST_CHECK_EQUAL(cbc2.size(),3u);
  BOOST_CHECK(res == cbc2.begin()+2);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),values,values+3);
}

/** \test Check range erase middle operation (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_erase_range_middle_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_list_channel::iterator start = bc2.begin();
  std::advance(start,2);
  float_list_channel::iterator stop = bc2.begin();
  std::advance(stop,4);
  float_list_channel::iterator res = bc2.erase(start,stop);
  BOOST_CHECK_EQUAL(cbc2.size(),3u);
  start = bc2.begin();
  std::advance(start,2);
  BOOST_CHECK(res == start);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),values,values+3);
}

/** \test Check range erase beginning operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_erase_range_beginning_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_channel::iterator res = bc2.erase(bc2.begin(),bc2.begin()+2);
  BOOST_CHECK_EQUAL(cbc2.size(),3u);
  BOOST_CHECK(res == cbc2.begin());

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),values,values+3);
}

/** \test Check range erase beginning operation (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_erase_range_beginning_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  float_list_channel::iterator stop = bc2.begin();
  std::advance(stop,2);
  float_list_channel::iterator res = bc2.erase(bc2.begin(),stop);
  BOOST_CHECK_EQUAL(cbc2.size(),3u);
  BOOST_CHECK(res == cbc2.begin());

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),values,values+3);
}

/** \test Check clear operation
 */
BOOST_AUTO_TEST_CASE( basic_channel_clear_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_channel bc1(5,.2);
  const float_channel &cbc1 = bc1;
  float_channel bc2 = bc1;
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  bc2.clear();
  BOOST_CHECK(!cbc1.empty());
  BOOST_CHECK(cbc2.empty());
  BOOST_CHECK(cbc1.begin() != cbc1.end());
  BOOST_CHECK(cbc2.begin() == cbc2.end());
  BOOST_CHECK_EQUAL(cbc1.size(),5u);
  BOOST_CHECK_EQUAL(cbc2.size(),0u);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check clear operation (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_clear_test )
{
  float values[] = {
    .2,.2,.2,.2,.2
  };
  
  const float_list_channel bc1(5,.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    values,values+sizeof(values)/sizeof(float));

  bc2.clear();
  BOOST_CHECK(!cbc1.empty());
  BOOST_CHECK(cbc2.empty());
  BOOST_CHECK(cbc1.begin() != cbc1.end());
  BOOST_CHECK(cbc2.begin() == cbc2.end());
  BOOST_CHECK_EQUAL(cbc1.size(),5u);
  BOOST_CHECK_EQUAL(cbc2.size(),0u);

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    values,values+sizeof(values)/sizeof(float));
  BOOST_CHECK( &cbc1.front() != &cbc2.front() );
}

/** \test Check swap
 */
BOOST_AUTO_TEST_CASE( basic_channel_swap_test )
{
  float_channel bc1(5,.2,.5,-.2);
  const float_channel &cbc1 = bc1;

  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc1.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),fill,fill+5 );

  float_channel bc2(mags,mags+5,.5,-.2);
  const float_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc2.begin(),bc2.end(),mags,mags+5 );

  swap(bc1,bc2);

  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc2.begin(),bc2.end(),fill,fill+5 );

  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc1.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),mags,mags+5 );
  
  bc2 = bc1;

  //check shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );

  swap(bc1,bc2);

  //recheck shared base
  BOOST_CHECK_EQUAL( &cbc1.front(), &cbc2.front() );
}

/** \test Check equality
 */
BOOST_AUTO_TEST_CASE( basic_channel_equality_test )
{
  float_channel bc1(mags,mags+5,.5,-.2);
  BOOST_CHECK_EQUAL( bc1.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc1.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc1.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc1.begin(),bc1.end(),mags,mags+5 );

  float_channel bc2(mags,mags+5,.5,-.2);
  BOOST_CHECK_EQUAL( bc2.size(), std::size_t(5) );
  BOOST_CHECK_EQUAL( bc2.frequency(), .5f );
  BOOST_CHECK_EQUAL( bc2.epoch(), -.2f );
  BOOST_CHECK_EQUAL_COLLECTIONS( bc2.begin(),bc2.end(),mags,mags+5 );

  float_channel bc3(1,2,3,4);
  BOOST_CHECK_EQUAL( bc3.size(), std::size_t(1) );
  BOOST_CHECK_EQUAL( bc3.frequency(), 3 );
  BOOST_CHECK_EQUAL( bc3.epoch(), 4 );
  BOOST_CHECK_EQUAL( bc3.front(), 2.0f );

  BOOST_CHECK( bc1 == bc2 );
  BOOST_CHECK( bc2 != bc3 );
}

BOOST_AUTO_TEST_SUITE_END()

}
}
}
