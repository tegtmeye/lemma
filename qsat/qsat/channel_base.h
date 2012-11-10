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

#ifndef LEMMA_QSAT_CHANNEL_BASE_H
#define LEMMA_QSAT_CHANNEL_BASE_H

#include "basic_channel.h"
#include "detail/value_cast.h"

#include <boost/variant.hpp>

#include <boost/utility/result_of.hpp>

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>

#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>

#include <boost/fusion/include/clear.hpp>

#include <boost/fusion/include/mpl.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/type_traits/is_same.hpp>

#include <iostream>

namespace lemma {
namespace qsat {

namespace b = boost;
namespace bf = boost::fusion;
namespace mpl = boost::mpl;

namespace detail {

template<typename FrequencyT, typename TimeT>
struct make_basic_channel {
  template<typename T>
  struct apply {
    typedef basic_channel<T,FrequencyT,TimeT> type;
  };
};

}


template <typename MagnitudeT, typename FrequencyT, typename TimeT,
  typename InternalPrecisionSequence, template<typename T, typename A>
  class Container = std::vector, template<typename T>
  class Allocator = std::allocator>
class channel_base {
  public:
    /** lvalue of MagnitudeT */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::reference reference;
    /** const lvalue of MagnitudeT */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::const_reference const_reference;
    /** iterator type pointing to MagnitudeT */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::iterator iterator;
    /** iterator type pointing to const MagnitudeT */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::const_iterator const_iterator;
    /** unsigned integral type */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::size_type size_type;
    /** signed integral type */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::difference_type difference_type;
    /** MagnitudeT */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::value_type value_type;
    /** Container */
    typedef Container<MagnitudeT,Allocator<MagnitudeT> > container_type;
    /** Allocator */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::allocator_type allocator_type;
    /** type modeling pointer to MagnitudeT */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::pointer pointer;
    /** type modeling pointer to const MagnitudeT */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::const_pointer const_pointer;
    /** reverse_iterator type pointing to MagnitudeT */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::reverse_iterator reverse_iterator;
    /** reverse_iterator type pointing to const MagnitudeT */
    typedef typename Container<MagnitudeT,Allocator<MagnitudeT> >::const_reverse_iterator const_reverse_iterator;
  
    /** This type, used for making new channels */
    typedef channel_base channel_type;
    /** MagnitudeT */
    typedef MagnitudeT magnitude_type;
    /** FrequencyT */
    typedef FrequencyT frequency_type;
    /** TimeT */
    typedef TimeT time_type;

    /** \brief Default Constructor
     *  \param tp A <code>std::pair<FrequencyT,TimeT></code> where
     *    <em>tp.first</em> represents the sample frequency and
     *    <em>tp.second</em> represents the time sampling started. Default is
     *    <code>std::make_pair<basic_channel::frequency_type(1),basic_channel::time_type(0)></code>.
     *
     *  \note <code>boost::units::quantity</code> type is supported as default
     *    constructed arguments.
     */
    channel_base(const std::pair<FrequencyT,TimeT> &tp =
      std::make_pair(detail::value_cast<FrequencyT>::construct((unsigned char)(1)),
        detail::value_cast<TimeT>::construct((unsigned char)(0))));

    /** \brief Fill Constructor
     *
     *  Make <em>n</em> copies of <em>value</em> with period <em>inter</em>
     *    and start time <em>start</em>
     *
     *  \param n A value of <code>basic_channel::size_type</code> representing
     *    the number of of copies of <em>value</em> to make
     *  \param value A value of <code>basic_channel::value_type</code>
     *  \param freq A value of <code>basic_channel::frequency_type</code>
     *    representing the sample frequency. Default is
     *    <code>basic_channel::frequency_type(1)</code>.
     *  \param start A value of <code>basic_channel::time_type</code>
     *    representing the time sampling started. Default is
     *    <code>basic_channel::time_type(0)</code>.
     *
     *  \note <code>boost::units::quantity</code> type is supported as default
     *    constructed arguments.
     *
     *  \post size() == <em>n</em>
     */
    explicit channel_base(size_type n, const MagnitudeT &value=MagnitudeT(),
      const FrequencyT &freq=detail::value_cast<FrequencyT>::construct(1),
      const TimeT &start=detail::value_cast<TimeT>::construct(0));

    /** \brief Assignment Constructor
     *
     *  Construct a sequence with range <em>[first,last)</em> with period
     *    <em>inter</em> and start time <em>start</em>
     *
     *  \param first A value of InputIterator modeling iterator to
     *    <code>basic_channel::magnitude_type</code>
     *  \param last A value of InputIterator modeling iterator to
     *    <code>basic_channel::magnitude_type</code>
     *  \param freq A value of <code>basic_channel::frequency_type</code>
     *    representing the sample frequency. Default is
     *    <code>basic_channel::frequency_type(1)</code>.
     *  \param start A value of <code>basic_channel::time_type</code>
     *    representing the time sampling started. Default is
     *    <code>basic_channel::time_type(0)</code>.
     *
     *  \post size() == <code>std::distance(first,last)</code>
     *
     *  \note <code>boost::units::quantity</code> type is supported as default
     *    constructed arguments.
     *
     *  \note As outlined in sequence requirements (23.1.1), this constructor
     *    will have the same effect as <pre>
     *      basic_channel(static_cast<typename basic_channel::size_type>(first), 
     *        static_cast<typename basic_channel::magnitude_type>(last))
     *    </pre> if <dfn>InputIterator</dfn> is an integral type
     *
     */
    template<typename T>
    explicit channel_base(size_type n, const T &value,
      const FrequencyT &freq=detail::value_cast<FrequencyT>::construct(1),
      const TimeT &start=detail::value_cast<TimeT>::construct(0));

    /** \brief Copy Constructor
     *
     *  \param rhs rvalue of type channel_base
     */
    template<typename M, typename F, typename T, typename I,
      template<typename _T, typename _A> class C,
      template<typename _T> class A>
    channel_base(const channel_base<M,F,T,I,C,A> &rhs);
    
    /** \brief Destructor
     */
    ~channel_base(void);

    /** \brief Assignment Operator
     *  \param rhs rvalue of type basic_channel
     *  \return <code>*this</code>
     */
    template<typename M, typename F, typename T, typename I,
      template<typename _T, typename _A> class C,
      template<typename _T> class A>
    channel_base<M,F,T,I,C,A> & operator=(const channel_base<M,F,T,I,C,A> &rhs);

    /** \brief Range Assignment Operator
     *  \param first,last <code>InputIterator</code> range pointing to objects
     *    convertable to MagnitudeT
     *  \post size() = <code>std::distance(first,last)</code>
     */
    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last);

    /** \brief Range, Frequencey, and Epoch Assignment Operator
     *  \param first,last <code>InputIterator</code> range pointing to objects
     *    convertable to MagnitudeT
     *  \param freq The sample frequency for samples contained in
     *    <code>[first,last)</code>
     *  \param start The start time associated with value pointed to by
     *    <code>first</code>
     *  \return <code>*this</code>
     *  \post size() = <code>std::distance(first,last)</code>
     */
    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last, const FrequencyT &freq,
      const TimeT &start);

    /** \brief Fill Assignment Operator
     *  \param n The number of <em>val</em> object to assign to this sequence
     *  \param val The value of the <CODE>MagnitudeT</CODE> objects that are
     *    assigned to this
     *  \return <code>*this</code>
     *  \post size() = <em>n</em>
     */
    void assign(size_type n, const MagnitudeT &val);

    /** \brief Fill Assignment Operator
     *  \param n The number of <em>val</em> object to assign to this sequence
     *  \param val The value of the <CODE>MagnitudeT</CODE> objects that are
     *    assigned to this
     *  \param freq The sample frequency for samples contained in
     *    <code>[first,last)</code>
     *  \param start The start time associated with value pointed to by
     *    <code>first</code>
     *  \return <code>*this</code>
     *  \post size() = <em>n</em>
     */
    void assign(size_type n, const MagnitudeT &val, const FrequencyT &freq,
      const TimeT &start);

    /** \brief Obtain iterator to sequence beginning
     *  \return iterator to MagnitudeT
     */
    iterator begin(void);

    /** \brief Obtain iterator to sequence beginning
     *  \return iterator to const MagnitudeT
     */
    const_iterator begin(void) const;

    /** \brief Obtain iterator to one past sequence end
     *  \return iterator to MagnitudeT
     */
    iterator end(void);

    /** \brief Obtain iterator to one past sequence end
     *  \return iterator to const MagnitudeT
     */
    const_iterator end(void) const;

    /** \brief Obtain reverse_iterator to sequence beginning
     *  \return iterator to MagnitudeT
     */
    reverse_iterator rbegin(void);

    /** \brief Obtain reverse_iterator to sequence beginning
     *  \return iterator to const MagnitudeT
     */
    const_reverse_iterator rbegin(void) const;

    /** \brief Obtain reverse_iterator to one past sequence end
     *  \return reverse_iterator to MagnitudeT
     */
    reverse_iterator rend(void);

    /** \brief Obtain reverse_iterator to one past sequence end
     *  \return reverse_iterator to const MagnitudeT
     */
    const_reverse_iterator rend(void) const;

    /** \brief Obtain the number of elements contained in the sequence
     *  \return sequence size
     */
    size_type size(void) const;

    /** \brief Obtain the maximum number of elements able to be stored by
     *    this sequence
     *  \return sequence maximum size
     */
    size_type max_size(void) const;

    /** \brief Resize the sequence
     *  \param sz The new sequence size
     *  \param mag The value of the new objects copied to the end of the sequence
     *    if <em>s</em> > <CODE>size()</CODE>
     */
    void resize(size_type sz, const MagnitudeT &mag = MagnitudeT());

    /** \brief Obtain the total number of elements that this sequence can hold
     *    without requiring reallocation.
     *  \return sequence storage capacity
     */
    size_type capacity(void) const;

    /** \brief Obtain whether or not the sequence contains any values
     *  \return <CODE>size() == 0</CODE>
     */
    bool empty(void) const;
    
    /** \brief Ensure that <EM>n</EM> elements can be inserted into the
     *    sequence without reallocation occuring
     *  \param n the new capacity
     */
    void reserve(size_type n);
    
    /** \brief Obtain a reference to the value stored at location <EM>n</EM>
     *  \param n The element location
     *  \return element reference
     */
    reference operator[](size_type n);

    /** \brief Obtain a const_reference to the value stored at location <EM>n</EM>
     *  \param n The element location
     *  \return element const_reference
     */
    const_reference operator[](size_type n) const;

    /** \brief Obtain a const_reference to the value stored at location <EM>n</EM>
     *  \param n The element location
     *  \return element const_reference
     *  \throws <CODE>std::out_of_range</CODE if <CODE>n >= size()</CODE>
     */
    const_reference at(size_type n) const;

    /** \brief Obtain a reference to the value stored at location <EM>n</EM>
     *  \param n The element location
     *  \return element reference
     *  \throws <CODE>std::out_of_range</CODE if <CODE>n >= size()</CODE>
     */
    reference at(size_type n);
    
    /** \brief Obtain the element at the beginning of the sequence
     *  \return element reference
     */
    reference front(void);

    /** \brief Obtain the element at the beginning of the sequence
     *  \return element const_reference
     */
    const_reference front(void) const;

    /** \brief Obtain the last element of the sequence
     *  \return element reference
     */
    reference back(void);

    /** \brief Obtain the last element of the sequence
     *  \return const_element reference
     */
    const_reference back(void) const;

    /** \brief Add a new element at the end of the sequence
     *  \param val The value of the new element
     */
    void push_back(const MagnitudeT &val);

    /** \brief Remove the last element of the sequence
     */
    void pop_back(void);

    /** \brief Insert a new element into the sequence before <em>position</em>
     *  \param position An iterator pointing to the location the element should
     *    be inserted before
     *  \param val The value of the new element
     *  \return An iterator pointing to the new element
     */
    iterator insert(iterator position, const MagnitudeT &val);

    /** \brief Insert <em>n</em> elements into the sequence before <em>position</em>
     *  \param position An iterator pointing to the location the elements should
     *    be inserted before
     *  \param n The number of elements to insert
     *  \param val The value of the new elements
     */
    void insert(iterator position, size_type n, const MagnitudeT &val);

    /** \brief Insert elements in the range of <code>[first,last)</code>
     *    into the sequence before <em>position</em>
     *  \param position An iterator pointing to the location the elements should
     *    be inserted before
     *  \param first,last An <code>InputIterator</code> pointing to objects
     *    convertable to MagnitudeT
     */
    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);

    /** \brief Erase the element at <em>position</em>
     *  \param position An iterator pointing to the element to erase
     *  \return An iterator to the element immediately after <em>position</em>
     */
    iterator erase(iterator position);
    
    /** \brief Erase elements in the range of <code>[first,last)</code>
     *  \param first,last An iterator range in the sequence
     */
    iterator erase(iterator first, iterator last);

    /** \brief Efficiently swap another basic_channel object with this
     *  \param rhs a reference
     */
    template<typename M, typename F, typename T, typename I,
      template<typename _T, typename _A> class C,
      template<typename _T> class A>
    void swap(channel_base<M,F,T,I,C,A> &rhs);

    /** \brief Remove all elements of the sequence
     */
    void clear(void);

    /** \brief Object equqlity comparison
     *  \param rhs a const_reference
     *  \return <code>[this->begin(),this->end()) == [rhs.begin(),rhs.end())</code>
     *    AND <code>this->frequency() == rhs.frequency()</code>
     *    AND <code>this->epoch() == rhs.epoch()</code>
     */
    template<typename M, typename F, typename T, typename I,
      template<typename _T, typename _A> class C,
      template<typename _T> class A>
    bool operator==(const channel_base<M,F,T,I,C,A> &rhs) const;

    /** \brief Object inequqlity comparison
     *  \param rhs a const_reference
     *  \return !(*this == rhs)
     */
    template<typename M, typename F, typename T, typename I,
      template<typename _T, typename _A> class C,
      template<typename _T> class A>
    bool operator!=(const channel_base<M,F,T,I,C,A> &rhs) const;

    /** \brief Set new sample interval and return the old value
     *
     *  \param freq A value of <code>basic_channel::frequency_type</code>
     *    representing the sample frequency.
     *  \return The current sample frequency
     */
    frequency_type frequency(const FrequencyT &freq);

    /** \brief Obtain the current sample frequency
     *
     *  \return The current sample frequency
     */
    const frequency_type & frequency(void) const;

    /** \brief Set new sample epoch and return the old value
     *
     *  \param inter A value of basic_channel::time_type representing the
     *    time sampling started.
     *  \return The current sample epoch
     */
    time_type epoch(const TimeT &inter);

    /** \brief Obtain the current sample epoch
     *
     *  \return The current sample epoch
     */
    const time_type & epoch(void) const;

  private:
    template<typename M, typename F, typename T, typename I,
      template<typename _T, typename _A> class C,
      template<typename _T> class A>
    struct adapted_copy :public boost::static_visitor<> {
      adapted_copy(const channel_base<M,F,T,I,C,A> &cb) :other(b::cref(cb)) {}
      
      void operator()(basic_channel<M,FrequencyT,TimeT,Container,Allocator> &self)
      {
        self.assign(other.get().begin(),other.get().end());
        self.frequency(b::numeric_cast<FrequencyT>(other.frequency()));
        self.epoch(b::numeric_cast<FrequencyT>(other.epoch()));
      }
      
      template<typename SelfM>
      void operator()(const basic_channel<SelfM,FrequencyT,TimeT,Container,Allocator> &self)
      {
        typedef typename mpl::find<InternalPrecisionSequence,M>::type iter;

        if(boost::is_same<typename mpl::deref<iter>::type,T>::value)
          self.channel_variant = basic_channel<M,FrequencyT,TimeT,Container,Allocator>();
        else
          self.channel_variant = default_channel_type();
        
      }
      
      b::reference_wrapper<const channel_base<M,F,T,I,C,A> > other;
    };
  
    typedef typename mpl::transform<InternalPrecisionSequence,
      detail::make_basic_channel<FrequencyT,TimeT> >::type channel_types;
  
    typedef typename boost::make_variant_over<channel_types>::type variant_type;

    typedef typename mpl::at_c<typename variant_type::types,0>::type default_channel_type;

    variant_type channel_variant;
};

template <typename MagnitudeT, typename FrequencyT, typename TimeT,
  typename InternalPrecisionSequence, template<typename T, typename A>
  class Container, template<typename T> class Allocator>
inline channel_base<MagnitudeT,FrequencyT,TimeT,InternalPrecisionSequence,Container,Allocator>::
  channel_base(const std::pair<FrequencyT,TimeT> &tp)
    :channel_variant(default_channel_type(tp))
{
}

template <typename MagnitudeT, typename FrequencyT, typename TimeT,
  typename InternalPrecisionSequence, template<typename T, typename A>
  class Container, template<typename T> class Allocator>
inline channel_base<MagnitudeT,FrequencyT,TimeT,InternalPrecisionSequence,Container,Allocator>::
  channel_base(size_type n, const MagnitudeT &value, const FrequencyT &freq,
  const TimeT &start) :channel_variant(default_channel_type(n,value,freq,start))
{
}

template <typename MagnitudeT, typename FrequencyT, typename TimeT,
  typename InternalPrecisionSequence, template<typename T, typename A>
  class Container, template<typename T> class Allocator>
template<typename T>
inline channel_base<MagnitudeT,FrequencyT,TimeT,InternalPrecisionSequence,Container,Allocator>::
  channel_base(size_type n, const T &value, const FrequencyT &freq, const TimeT &start)
{
  // if T is one of the InternalPrecisionSequence types, then construct the
  // variant directly, otherwise numeric_cast to MagnitudeT
  typedef typename mpl::find<InternalPrecisionSequence,T>::type iter;

  typedef typename mpl::if_<
    typename boost::is_same<
      typename mpl::deref<iter>::type,
      T>::type,
    T,
    MagnitudeT>::type adapted_type;
  
  channel_variant = basic_channel<adapted_type,FrequencyT,TimeT,Container,Allocator>(n,
    b::numeric_cast<adapted_type>(value),freq,start);
}

template <typename MagnitudeT, typename FrequencyT, typename TimeT,
  typename InternalPrecisionSequence, template<typename T, typename A>
  class Container, template<typename T> class Allocator>
template<typename M, typename F, typename T, typename I,
  template<typename _T, typename _A> class C,
  template<typename _T> class A>
inline channel_base<MagnitudeT,FrequencyT,TimeT,InternalPrecisionSequence,Container,Allocator>::
  channel_base(const channel_base<M,F,T,I,C,A> &rhs)
{
  boost::apply_visitor(adapted_copy<M,F,T,I,C,A>(rhs),channel_variant);
}

}
}

#endif
