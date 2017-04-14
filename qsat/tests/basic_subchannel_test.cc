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
#include <boost/test/floating_point_comparison.hpp>

#include "test_types.h"

#include <qsat/basic_channel.h>

/** \file
 *  \brief Unit tests for basic_subchannel
 */

namespace lemma {
namespace qsat {
namespace test {

BOOST_AUTO_TEST_SUITE( channel_suite )

/** \test subchannel check
 */
BOOST_AUTO_TEST_CASE( basic_channel_subchannel_test )
{
  float_basic_channel bc1(mags,mags+sizeof(mags)/sizeof(float),.5,-.2);
  const float_basic_channel &cbc1 = bc1;
  float_basic_channel bc2 = bc1;
  const float_basic_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  
  float_basic_channel::iterator start = bc2.begin();
  std::advance(start,2);
  float_basic_channel::iterator stop = bc2.begin();
  std::advance(stop,4);
  float_basic_channel::subchannel_type sub = bc2.subchannel(start,stop);
  
  BOOST_CHECK(&cbc1.front() != &cbc2.front());
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),cbc2.begin(),cbc2.end());

  BOOST_CHECK(sub.begin() == start);
  BOOST_CHECK(sub.end() == stop);
  BOOST_CHECK(sub.rbegin().base() == sub.end());
  BOOST_CHECK(sub.rend().base() == sub.begin());
  BOOST_CHECK_EQUAL(sub.front(), *start);
  
  float_basic_channel::iterator back = stop;
  --stop;
  BOOST_CHECK_EQUAL(sub.back(), *stop);
  *sub.begin() = 42.0;
  BOOST_CHECK_EQUAL(*start,42.0);
  
  BOOST_CHECK_EQUAL( &sub[0], &*sub.begin());
}

/** \test subchannel assign check
 */
BOOST_AUTO_TEST_CASE( basic_channel_subchannel_assign_test )
{
  float values[] = {
    99,99,99,99,99
  };

  float values2[] = {
    -1,-1,-1,-1
  };

  float_basic_channel bc1(mags,mags+sizeof(mags)/sizeof(float),.5,-.2);
  const float_basic_channel &cbc1 = bc1;
  float_basic_channel bc2 = bc1;
  const float_basic_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  
  float_basic_channel::iterator start = bc2.begin();
  std::advance(start,2);
  float_basic_channel::iterator stop = bc2.begin();
  std::advance(stop,4);
  float_basic_channel::subchannel_type sub = bc2.subchannel(start,stop);
  
  sub.assign(values,values+std::distance(start,stop));
  BOOST_CHECK_EQUAL_COLLECTIONS(start,stop,sub.begin(),sub.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(values,values+sub.size(),sub.begin(),sub.end());

  sub.assign(-1.0f);
  BOOST_CHECK_EQUAL_COLLECTIONS(start,stop,sub.begin(),sub.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(values2,values2+sub.size(),sub.begin(),sub.end());
}

/** \test subchannel check (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_subchannel_test )
{
  float_list_channel bc1(mags,mags+sizeof(mags)/sizeof(float),.5,-.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  
  float_list_channel::iterator start = bc2.begin();
  std::advance(start,2);
  float_list_channel::iterator stop = bc2.begin();
  std::advance(stop,4);
  float_list_channel::subchannel_type sub = bc2.subchannel(start,stop);
  
  BOOST_CHECK(&cbc1.front() != &cbc2.front());
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),cbc2.begin(),cbc2.end());

  BOOST_CHECK(sub.begin() == start);
  BOOST_CHECK(sub.end() == stop);
  BOOST_CHECK(sub.rbegin().base() == sub.end());
  BOOST_CHECK(sub.rend().base() == sub.begin());
  BOOST_CHECK_EQUAL(sub.front(), *start);

  BOOST_CHECK(sub.begin() == start);
  BOOST_CHECK(sub.end() == stop);
  *sub.begin() = 42.0;
  BOOST_CHECK_EQUAL(*start,42.0);
}

/** \test subchannel assign check (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_subchannel_assign_test )
{
  float values[] = {
    99,99,99,99,99
  };

  float values2[] = {
    -1,-1,-1,-1
  };

  float_list_channel bc1(mags,mags+sizeof(mags)/sizeof(float),.5,-.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  
  float_list_channel::iterator start = bc2.begin();
  std::advance(start,2);
  float_list_channel::iterator stop = bc2.begin();
  std::advance(stop,4);
  float_list_channel::subchannel_type sub = bc2.subchannel(start,stop);
  
  sub.assign(values,values+std::distance(start,stop));
  BOOST_CHECK_EQUAL_COLLECTIONS(start,stop,sub.begin(),sub.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(values,values+sub.size(),sub.begin(),sub.end());

  sub.assign(-1.0f);
  BOOST_CHECK_EQUAL_COLLECTIONS(start,stop,sub.begin(),sub.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(values2,values2+sub.size(),sub.begin(),sub.end());
}

/** \test const subchannel check
 */
BOOST_AUTO_TEST_CASE( basic_channel_const_subchannel_test )
{
  float_basic_channel bc1(mags,mags+sizeof(mags)/sizeof(float),.5,-.2);
  const float_basic_channel &cbc1 = bc1;
  float_basic_channel bc2 = bc1;
  const float_basic_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  
  float_basic_channel::const_iterator start = cbc2.begin();
  std::advance(start,2);
  float_basic_channel::const_iterator stop = cbc2.begin();
  std::advance(stop,4);
  float_basic_channel::const_subchannel_type sub = cbc2.subchannel(start,stop);
  
  BOOST_CHECK(&cbc1.front() == &cbc2.front());
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),cbc2.begin(),cbc2.end());

  BOOST_CHECK(sub.begin() == start);
  BOOST_CHECK(sub.end() == stop);
  BOOST_CHECK(sub.rbegin().base() == sub.end());
  BOOST_CHECK(sub.rend().base() == sub.begin());
  BOOST_CHECK_EQUAL(sub.front(), *start);

  BOOST_CHECK(sub.begin() == start);
  BOOST_CHECK(sub.end() == stop);

  BOOST_CHECK_EQUAL( &sub[0], &*sub.begin());
}

/** \test const subchannel check (list)
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_const_subchannel_test )
{
  float_list_channel bc1(mags,mags+sizeof(mags)/sizeof(float),.5,-.2);
  const float_list_channel &cbc1 = bc1;
  float_list_channel bc2 = bc1;
  const float_list_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  
  float_list_channel::const_iterator start = cbc2.begin();
  std::advance(start,2);
  float_list_channel::const_iterator stop = cbc2.begin();
  std::advance(stop,4);
  float_list_channel::const_subchannel_type sub = cbc2.subchannel(start,stop);
  
  BOOST_CHECK(&cbc1.front() == &cbc2.front());
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),cbc2.begin(),cbc2.end());

  BOOST_CHECK(sub.begin() == start);
  BOOST_CHECK(sub.end() == stop);
  BOOST_CHECK(sub.rbegin().base() == sub.end());
  BOOST_CHECK(sub.rend().base() == sub.begin());
  BOOST_CHECK_EQUAL(sub.front(), *start);

  BOOST_CHECK(sub.begin() == start);
  BOOST_CHECK(sub.end() == stop);
}

/** \test subchannel frequency check
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_subchannel_frequency_test )
{
  float_basic_channel bc1(mags,mags+sizeof(mags)/sizeof(float),.5,-.2);
  const float_basic_channel &cbc1 = bc1;
  float_basic_channel bc2 = bc1;
  const float_basic_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  
  float_basic_channel::const_iterator start = cbc2.begin();
  std::advance(start,2);
  float_basic_channel::const_iterator stop = cbc2.begin();
  std::advance(stop,4);
  float_basic_channel::const_subchannel_type sub = cbc2.subchannel(start,stop);

  BOOST_CHECK_EQUAL(&(cbc2.frequency()), &sub.frequency());
}

/** \test subchannel epoch check
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_subchannel_epoch_test )
{
  float_basic_channel bc1(mags,mags+sizeof(mags)/sizeof(float),10,-.2);
  const float_basic_channel &cbc1 = bc1;
  float_basic_channel bc2 = bc1;
  const float_basic_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  
  float_basic_channel::const_iterator start = cbc2.begin();
  std::advance(start,2);
  float_basic_channel::const_iterator stop = cbc2.begin();
  std::advance(stop,4);
  float_basic_channel::const_subchannel_type sub = cbc2.subchannel(start,stop);

  BOOST_CHECK_SMALL(cbc2.epoch()+float(2)/10-sub.epoch(), .00000001f);
}

/** \test subchannel subchannel check
 */
BOOST_AUTO_TEST_CASE( basic_channel_list_subchannel_subchannel_test )
{
  float_basic_channel bc1(mags,mags+sizeof(mags)/sizeof(float),10,-.2);
  const float_basic_channel &cbc1 = bc1;
  float_basic_channel bc2 = bc1;
  const float_basic_channel &cbc2 = bc2;

  BOOST_CHECK_EQUAL_COLLECTIONS(cbc1.begin(),cbc1.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  BOOST_CHECK_EQUAL_COLLECTIONS(cbc2.begin(),cbc2.end(),
    mags,mags+sizeof(mags)/sizeof(float));
  
  float_basic_channel::const_iterator start = cbc2.begin();
  std::advance(start,2);
  float_basic_channel::const_iterator stop = cbc2.begin();
  std::advance(stop,4);
  
  typedef float_basic_channel::const_subchannel_type const_subchannel_type;
  const_subchannel_type sub = cbc2.subchannel(start,stop);

  const_subchannel_type::iterator first = sub.begin();
  const_subchannel_type::iterator last = sub.end();
  
  const_subchannel_type sub2 = sub.subchannel(first,last);
  BOOST_CHECK(sub == sub2);
}

BOOST_AUTO_TEST_SUITE_END()

}
}
}
