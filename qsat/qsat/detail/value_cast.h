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

#ifndef LEMMA_QSAT_DETAIL_VALUE_CAST_H
#define LEMMA_QSAT_DETAIL_VALUE_CAST_H

#include <boost/numeric/conversion/cast.hpp>

#include <boost/units/quantity.hpp>

/** \file
 *  \brief Functions to genericaly and safely construct numeric types with a
 *    specific value
 */

namespace lemma {
namespace qsat {
namespace detail {

/** \brief Generic construction cast
 *  \tparam T The type to construct
 *  \internal Generically construct one type from another.
 */
template<typename T>
struct value_cast {
  /** construct T from \e val */
  template<typename U>
  static T construct(const U &val) {
    return boost::numeric_cast<T>(val);
  }
};

/** \brief Generic construction cast
 *  \tparam Unit The unit type of boost::units::quantity
 *  \tparam Y The value_type (or precision) of boost::units::quantity
 *  \internal Specialization to construct a boost::unit from it's value_type
 */
template<typename Unit, typename Y>
struct value_cast<boost::units::quantity<Unit,Y> > {
  /** construct <tt>quantity<Unit,Y></tt> from \e val */
  static boost::units::quantity<Unit,Y> construct(const Y &val) {
    return boost::units::quantity<Unit,Y>(val*Unit());
  }
};



}
}
}


#endif

