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

#ifndef LEMMA_QSAT_TEST_TEST_TYPES_H
#define LEMMA_QSAT_TEST_TEST_TYPES_H

#include <qsat/basic_channel.h>
#include <qsat/channel_base.h>

#include <boost/mpl/vector.hpp>

#include <list>

/** \file
 *  Files with the name '(*)/tests/test_types.h/ are ignored by Doxygen. Put types and
 *  other things that don't make sense to document into a file named this.
 */

namespace lemma {
namespace qsat {
namespace test {

namespace mpl = boost::mpl;

/** \file
 *  Files with the name '(*)/tests/test_types.h/ are ignored by Doxygen. Put types and
 *  other things that don't make sense to document into a file named this.
 */

/** basic_channel with float precision */
typedef basic_channel<float,float,float> float_basic_channel;

/** basic_channel with float precision using std::list as an underlying container*/
typedef basic_channel<float,float,float,std::list> float_list_channel;


/** channel_base with float precision and float underlying type*/
typedef channel_base<float,float,float,mpl::vector<float> > float_channel_base;

/** channel_base with float precision using std::list as an underlying container*/
typedef channel_base<float,float,float,mpl::vector<float>,std::list> float_list_channel_base;


static const float mags[] = {1.0,2.0,3.0,4.0,5.0};
static const float fill[] = {0.2,0.2,0.2,0.2,0.2};


}
}
}

#endif
