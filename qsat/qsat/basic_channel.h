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

#ifndef LEMMA_QSAT_BASIC_CHANNEL_H
#define LEMMA_QSAT_BASIC_CHANNEL_H

#include "detail/value_cast.h"

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>

#include <vector>
#include <iterator>

/** \file
 *  \brief Implementation of basic_channel modeling Channel concept
 */

namespace lemma {
namespace qsat {

namespace b = boost;
namespace mpl = boost::mpl;

namespace detail {

/** \brief Utility function to determine if the given type has reserve ability
 *  \internal
 */
template<typename T>
class has_reserve_capability {
  private:
    template<typename U, void (U::*)(size_t)>
    struct SFINAE {};
    
    template<typename U>
    static char test(SFINAE<U,&U::reserve>*);
    
    template<typename U>
    static int test(...);

  public:
    /** Test to determine reserve ability */
    typedef mpl::bool_<sizeof(test<T>(0)) == sizeof(char)> type;
};


}

/** \brief Reference into a basic_channel interval
 *
 *  \par Discussion
 *  An "almost sequence" container with reference semantics into an existing
 *  basic_channel. The container is bound to the parent channel at construction
 *  or assignment and cannot be reassigned by any other means. The lifetime
 *  of the bound parent channel is not controlled by the subchannel container.
 *  Therefore, for a bound subchannel object to be valid, the parent channel
 *  must also be valid. ie the lifetime of the parent channel must be at last
 *  as long as its subchannels for them to be valid.
 *
 *  \par 
 *  As a byproduct of this requirement, for subchannels to const channels,
 *  will be invalidated by \e any potentially mutating parent channel operation.
 *  \code
 *  typedef basic_channel<T> channel_type;
 *  basic_subchannel<const channel_type> subchannel_type;
 *  
 *  channel_type channel; // assume filled with values
 *  subchannel_type sub(channel,first,last); // for some [first,last)
 *  
 *  channel_type dup = channel;
 *  channel.begin(); // mutable operation, invalidates sub
 *  \endcode
 *  
 */
template<typename ChannelT>
class basic_subchannel {
  public:
    /** lvalue of ChannelT::value_type
     *  (const-ness dependent on ChannelT)
     */
    typedef typename mpl::if_<
      b::is_const<ChannelT>,
      typename ChannelT::const_reference,
      typename ChannelT::reference>::type reference;

    /** const lvalue of Channel::value_type */
    typedef typename ChannelT::const_reference const_reference;

    /** iterator type pointing to ChannelT::value_type
     *  (const-ness dependent on ChannelT)
     */
    typedef typename mpl::if_<
      b::is_const<ChannelT>,
      typename ChannelT::const_iterator,
      typename ChannelT::iterator>::type iterator;

    /** iterator type pointing to ChannelT::value_type */
    typedef typename ChannelT::const_iterator const_iterator;

    /** unsigned integral type */
    typedef typename ChannelT::size_type size_type;
    /** signed integral type */
    typedef typename ChannelT::difference_type difference_type;
    /** MagnitudeT */
    typedef typename ChannelT::value_type value_type;

    /** type modeling pointer to Channel::value_type
     *  (const-ness dependent on ChannelT)
     */
    typedef typename mpl::if_<
      b::is_const<ChannelT>,
      typename ChannelT::const_pointer,
      typename ChannelT::pointer>::type pointer;

    /** type modeling pointer to const Channel::value_type */
    typedef typename ChannelT::const_pointer const_pointer;

    /** reverse_iterator type pointing to Channel::value_type
     *  (const-ness dependent on ChannelT)
     */
    typedef typename mpl::if_<
      b::is_const<ChannelT>,
      typename ChannelT::const_reverse_iterator,
      typename ChannelT::reverse_iterator>::type reverse_iterator;

    /** reverse_iterator type pointing to const Channel::value_type */
    typedef typename ChannelT::const_reverse_iterator const_reverse_iterator;

    /** Underlying channel type */
    typedef ChannelT channel_type;
    /** Subchannel type */
    typedef basic_subchannel subchannel_type;
    /** Subchannel type */
    typedef basic_subchannel<
      typename b::add_const<ChannelT>::type> const_subchannel_type;
    
    /** MagnitudeT */
    typedef typename ChannelT::magnitude_type magnitude_type;
    /** FrequencyT */
    typedef typename ChannelT::frequency_type frequency_type;
    /** TimeT */
    typedef typename ChannelT::time_type time_type;

    /** \brief Constructor
     *
     *  Bind to \e ch with interval [\e first, \e last)
     *
     *  \param ch Parent channel
     *  \param first,last Interval over \e ch to bind to
     *
     *  \post size() == <code>std::distance(first,last)</code>
     */
    explicit basic_subchannel(ChannelT &ch, iterator first, iterator last);

    /** \brief Copy Constructor
     *
     *  \param rhs rvalue of type basic_channel
     *  \post size() = rhs.size()
     */
    basic_subchannel(const basic_subchannel &rhs);

    /** \brief Distructor
     */
    ~basic_subchannel(void);
    
    /** \brief Copy Constructor
     *
     *  \param rhs rvalue of type basic_subchannel
     */
    basic_subchannel & operator=(const basic_subchannel &rhs);
    
    /** \brief Range Assignment Operator
     *  \param first,last <code>InputIterator</code> range pointing to objects
     *    convertable to MagnitudeT
     *  \post size() = <code>std::distance(first,last)</code>
     */
    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last);
    
    /** \brief Fill Assignment Operator
     *  \param val A value of <code>basic_subchannel::value_type</code>
     */
    void assign(const value_type &val);
    
    /** \brief Obtain iterator to subsequence beginning
     *  \return iterator to MagnitudeT
     */
    iterator begin(void) const;

    /** \brief Assign \a pos to the new interval beginning
     *  \param pos The new interval beginning
     *  \return An iterator to the new interval beginning
     */
    iterator begin(iterator pos);

    /** \brief Obtain iterator to one past subsequence end
     *  \return iterator to MagnitudeT
     */
    iterator end(void) const;
    
    /** \brief Assign \a pos to the new interval ending
     *  \param pos The new interval ending
     *  \return An iterator to the new interval ending
     */
    iterator end(iterator pos);

    /** \brief Obtain reverse_iterator to subsequence beginning
     *  \return iterator to MagnitudeT
     */
    reverse_iterator rbegin(void) const;

    /** \brief Obtain reverse_iterator to one past subsequence end
     *  \return reverse_iterator to MagnitudeT
     */
    reverse_iterator rend(void) const;

    /** \brief Obtain the number of elements contained in the subsequence
     *  \return subsequence size
     */
    size_type size(void) const;

    /** \brief Obtain whether or not the subsequence contains any values
     *  \return <CODE>size() == 0</CODE>
     */
    bool empty(void) const;
  
    /** \brief Obtain a reference to the value stored at sublocation <EM>n</EM>
     *  \param n The element location
     *  \return element reference
     */
    reference operator[](size_type n) const;

    /** \brief Obtain the element at the beginning of the subsequence
     *  \return element reference
     */
    reference front(void) const;

    /** \brief Obtain the last element of the subsequence
     *  \return element reference
     */
    reference back(void) const;

    /** \brief Obtain the current sample frequency
     *
     *  \return The current sample frequency
     */
    const frequency_type & frequency(void) const;

    /** \brief Obtain the current sample epoch
     *
     *  \return The current sample epoch
     */
    time_type epoch(void) const;

    /** \brief Obtain a refrence to the channel that this subchannel is an
     *  interval over
     *  \return Channel reference
     */
    channel_type & channel(void) const;

    /** \brief Obtain a mutable subset of this channel
     *
     *  \par Discussion
     *  The lifetime of the underlying channel is not bound to the returned
     *  subchannel. Make sure that the channel bound to the subchannel has
     *  a longer lifetime than the subchannel to be used.
     *
     *  \par As per the semantics of the Channel concept, obtaining a mutable
     *  subset will cause a copy operation of this channel's underlying bound
     *  representation.
     *
     *  \return A subset of this subchannel.
     */
    subchannel_type subchannel(iterator first, iterator last) const;
    
    /** \brief Compare two subchannels for equality
     *  \return this->channel() == rhs.channel() && this->begin() == rhs.begin()
     *    && this->end() == rhs.end()
     */
    bool operator==(const basic_subchannel &rhs) const;

    /** \brief Compare two subchannels for inequality
     *  \return !(*this == rhs)
     */
    bool operator!=(const basic_subchannel &rhs) const;
    
  private:
    iterator sub_first;
    iterator sub_last;
    b::reference_wrapper<ChannelT> base;
};

template<typename ChannelT>
inline basic_subchannel<ChannelT>::basic_subchannel(ChannelT &ch,
  iterator first, iterator last) :sub_first(first), sub_last(last), base(ch)
{
}

template<typename ChannelT>
inline basic_subchannel<ChannelT>::basic_subchannel(const basic_subchannel &rhs)
  :sub_first(rhs.sub_first), sub_last(rhs.sub_last), base(rhs.base)
{
}

template<typename ChannelT>
inline basic_subchannel<ChannelT>::~basic_subchannel(void)
{
}

template<typename ChannelT>
inline basic_subchannel<ChannelT> &
basic_subchannel<ChannelT>::operator=(const basic_subchannel &rhs)
{
  if(this != &rhs) {
    sub_first = rhs.sub_first;
    sub_last = rhs.sub_last;
    base = rhs.base;
  }
  
  return *this;
}

template<typename ChannelT>
template<typename InputIterator>
inline void basic_subchannel<ChannelT>::assign(InputIterator first,
  InputIterator last)
{
  iterator loc = sub_first;
  while(first != last) {
    assert(loc != sub_last);
    *loc++ = *first++;
  }
}

template<typename ChannelT>
inline void basic_subchannel<ChannelT>::assign(const value_type &val)
{
  std::fill(sub_first,sub_last,val);
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::iterator
basic_subchannel<ChannelT>::begin(void) const
{
  return sub_first;
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::iterator
basic_subchannel<ChannelT>::begin(iterator pos)
{
  return (sub_first = pos);
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::iterator
basic_subchannel<ChannelT>::end(void) const
{
  return sub_last;
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::iterator
basic_subchannel<ChannelT>::end(iterator pos)
{
  return (sub_last = pos);
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::reverse_iterator
basic_subchannel<ChannelT>::rbegin(void) const
{
  return reverse_iterator(sub_last);
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::reverse_iterator
basic_subchannel<ChannelT>::rend(void) const
{
  return reverse_iterator(sub_first);
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::size_type
basic_subchannel<ChannelT>::size(void) const
{
  return std::distance(sub_first,sub_last);
}

template<typename ChannelT>
inline bool basic_subchannel<ChannelT>::empty(void) const
{
  return sub_first == sub_last;
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::reference
basic_subchannel<ChannelT>::operator[](size_type n) const
{
  return sub_first[n];
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::reference
basic_subchannel<ChannelT>::front(void) const
{
  return *sub_first;
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::reference
basic_subchannel<ChannelT>::back(void) const
{
  return *reverse_iterator(sub_last);
}

template<typename ChannelT>
inline const typename basic_subchannel<ChannelT>::frequency_type &
basic_subchannel<ChannelT>::frequency(void) const
{
  typedef typename b::add_const<ChannelT>::type const_channel_type;
  return static_cast<const_channel_type&>(base).frequency();
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::time_type
basic_subchannel<ChannelT>::epoch(void) const
{
  typedef typename b::add_const<ChannelT>::type const_channel_type;
  const_channel_type &c_base = static_cast<const_channel_type&>(base);

  return c_base.epoch() +
    double(std::distance(c_base.begin(),
      typename const_channel_type::const_iterator(sub_first))) / frequency();
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::channel_type &
basic_subchannel<ChannelT>::channel(void) const
{
  return static_cast<ChannelT&>(base);
}

template<typename ChannelT>
inline typename basic_subchannel<ChannelT>::subchannel_type
basic_subchannel<ChannelT>::subchannel(iterator first, iterator last) const
{
  return static_cast<ChannelT>(base).subchannel(first,last);
}

template<typename ChannelT>
inline bool basic_subchannel<ChannelT>::operator==(const basic_subchannel &rhs) const
{
  return (this == &rhs) ||
    (static_cast<ChannelT&>(base) == static_cast<ChannelT&>(rhs.base) && 
    sub_first == rhs.sub_first && sub_last == rhs.sub_last);
}

template<typename ChannelT>
inline bool basic_subchannel<ChannelT>::operator!=(const basic_subchannel &rhs) const
{
  return !(*this == rhs);
}

/** \brief Core signal container
 *  \tparam MagnitudeT The signal magnitude type
 *  \tparam FrequencyT The signal frequency type
 *  \tparam TimeT The type used to represent the channels time quantum
 *
 *  Model of std::sequence with optional members
 *  
 *  A basic_channel is the base object used to encapsulate signal data. It is
 *  basically a STL random-access sequence with an extra sampling interval
 *  and start time member.
 */
template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container = std::vector,
  template<typename T> class Allocator = std::allocator>
class basic_channel {
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
    typedef basic_channel channel_type;
    /** MagnitudeT */
    typedef MagnitudeT magnitude_type;
    /** FrequencyT */
    typedef FrequencyT frequency_type;
    /** TimeT */
    typedef TimeT time_type;
    
    /** Subchannel type representing mutable interval of basic_channel */
    typedef basic_subchannel<basic_channel> subchannel_type;
    /** Subchannel type representing const interval of basic_channel */
    typedef basic_subchannel<const basic_channel> const_subchannel_type;
    
    /** \brief Default Constructor
     *  \param tp A <code>std::pair<FrequencyT,TimeT></code> where
     *    <em>tp.first</em> represents the sample frequency and
     *    <em>tp.second</em> represents the time sampling started. Default is
     *    <code>std::make_pair<basic_channel::frequency_type(1),basic_channel::time_type(0)></code>.
     *
     *  \note <code>boost::units::quantity</code> type is supported as default
     *    constructed arguments.
     */
    basic_channel(const std::pair<FrequencyT,TimeT> &tp =
      std::make_pair(detail::value_cast<FrequencyT>::construct(1),
        detail::value_cast<TimeT>::construct(0)));

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
    explicit basic_channel(size_type n, const MagnitudeT &value = MagnitudeT(),
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
    template<typename InputIterator>
    basic_channel(InputIterator first, InputIterator last,
      const FrequencyT &freq=detail::value_cast<FrequencyT>::construct(1),
      const TimeT &start=detail::value_cast<TimeT>::construct(0));

    /** \brief Copy Constructor
     *
     *  \param rhs rvalue of type basic_channel
     */
    basic_channel(const basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> &rhs);
    
    /** \brief Destructor
     */
    ~basic_channel(void);
    
    /** \brief Assignment Operator
     *  \param rhs rvalue of type basic_channel
     *  \return <code>*this</code>
     */
    basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> &
    operator=(const basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> &rhs);
    
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
    void swap(basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> &rhs);

    /** \brief Remove all elements of the sequence
     */
    void clear(void);

    /** \brief Object equqlity comparison
     *  \param rhs a const_reference
     *  \return <code>[this->begin(),this->end()) == [rhs.begin(),rhs.end())</code>
     *    AND <code>this->frequency() == rhs.frequency()</code>
     *    AND <code>this->epoch() == rhs.epoch()</code>
     */
    bool operator==(const basic_channel &rhs) const;

    /** \brief Object inequqlity comparison
     *  \param rhs a const_reference
     *  \return !(*this == rhs)
     */
    bool operator!=(const basic_channel &rhs) const;

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
    
    /** \brief Obtain a mutable subset of this channel
     *
     *  \par Discussion
     *  The lifetime of the channel is not bound to the returned
     *  subchannel. Make sure that the channel bound to the subchannel has
     *  a longer lifetime than the subchannel to be used.
     *
     *  \par As per the semantics of the Channel concept, obtaining a mutable
     *  subset will cause a copy operation of this channel's underlying bound
     *  representation.
     *
     *  \return A mutable subset into this channel.
     */
    subchannel_type subchannel(iterator first, iterator last);

    /** \brief Obtain a constant subset of this channel
     *
     *  \par Discussion
     *  The lifetime of the channel is not bound to the returned
     *  subchannel. Make sure that the channel bound to the subchannel has
     *  a longer lifetime than the subchannel to be used.
     *
     *  \return A constant subset into this channel.
     */
    const_subchannel_type subchannel(const_iterator first,
      const_iterator last) const;

  private:
    FrequencyT _sample_frequency;
    TimeT _time_start;
    
    b::shared_ptr<container_type> sequence;

    void resize_impl(size_type sz, const MagnitudeT &mag, mpl::false_);
    void resize_impl(size_type sz, const MagnitudeT &mag, mpl::true_);

    void push_back_impl(const MagnitudeT &val, mpl::false_);
    void push_back_impl(const MagnitudeT &val, mpl::true_);
    
    iterator insert_impl(iterator position, const MagnitudeT &val, mpl::false_);
    iterator insert_impl(iterator position, const MagnitudeT &val, mpl::true_);

    void fill_insert_impl(iterator position, size_type n, const MagnitudeT &val, mpl::false_);
    void fill_insert_impl(iterator position, size_type n, const MagnitudeT &val, mpl::true_);

    template <typename InputIterator, typename IteratorTag>
    void range_insert_impl(iterator position, InputIterator first,
      InputIterator last, IteratorTag, mpl::false_);

    template <typename InputIterator, typename IteratorTag>
    void range_insert_impl(iterator position, InputIterator first,
      InputIterator last, IteratorTag, mpl::true_);
    
    template <typename InputIterator>
    void range_insert_impl(iterator position, InputIterator first,
      InputIterator last, std::random_access_iterator_tag, mpl::true_);

    iterator erase_impl(iterator position, mpl::false_);
    iterator erase_impl(iterator position, mpl::true_);

    iterator erase_impl(iterator first, iterator last, mpl::false_);
    iterator erase_impl(iterator first, iterator last, mpl::true_);
    
    template <typename InputIterator>
    void range_insert_thunk(iterator position, InputIterator first,
      InputIterator last, mpl::true_);

    template <typename InputIterator>
    void range_insert_thunk(iterator position, InputIterator first,
      InputIterator last, mpl::false_);
};

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  basic_channel(const std::pair<FrequencyT,TimeT> &tp) :_sample_frequency(tp.first),
  _time_start(tp.second), sequence(new container_type()) {}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::basic_channel(size_type n,
  const MagnitudeT &value, const FrequencyT &freq, const TimeT &start)
    :_sample_frequency(freq), _time_start(start), sequence(new container_type(n,value)) {}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
template<typename InputIterator>
inline basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::basic_channel(InputIterator first,
  InputIterator last, const FrequencyT &freq, const TimeT &start)
    :_sample_frequency(freq), _time_start(start), sequence(new container_type(first,last)) {}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  basic_channel(const basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> &rhs)
    :_sample_frequency(rhs._sample_frequency), _time_start(rhs._time_start), sequence(rhs.sequence) {}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::~basic_channel(void) {}
    
template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> & basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  operator=(const basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> &rhs)
{
  if(&rhs != this) {
    _sample_frequency = rhs._sample_frequency;
    _time_start = rhs._time_start;
    sequence = rhs.sequence;
  }

  return *this;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
template<typename InputIterator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::assign(InputIterator first,
  InputIterator last)
{
  if(sequence.unique())
    sequence->assign(first,last);
  else
    sequence.reset(new container_type(first,last));
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
template<typename InputIterator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::assign(InputIterator first,
  InputIterator last, const FrequencyT &freq, const TimeT &start)
{
  _sample_frequency = freq;
  _time_start = start;

  if(sequence.unique())
    sequence->assign(first,last);
  else
    sequence.reset(new container_type(first,last));
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::assign(size_type n,
  const MagnitudeT &val)
{
  if(sequence.unique())
    sequence->assign(n,val);
  else
    sequence.reset(new container_type(n,val));
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::assign(size_type n,
  const MagnitudeT &val, const FrequencyT &freq, const TimeT &start)
{
  _sample_frequency = freq;
  _time_start = start;

  if(sequence.unique())
    sequence->assign(n,val);
  else
    sequence.reset(new container_type(n,val));
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::begin(void)
{
  if(!sequence.unique())
    sequence.reset(new container_type(*sequence));
  
  return sequence->begin();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::const_iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::begin(void) const
{
  return sequence->begin();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::end(void)
{
  if(!sequence.unique())
    sequence.reset(new container_type(*sequence));
  
  return sequence->end();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::const_iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::end(void) const
{
  return sequence->end();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::reverse_iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::rbegin(void)
{
  if(!sequence.unique())
    sequence.reset(new container_type(*sequence));
  
  return sequence->rbegin();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::const_reverse_iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::rbegin(void) const
{
  return sequence->rbegin();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::reverse_iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::rend(void)
{
  if(!sequence.unique())
    sequence.reset(new container_type(*sequence));
  
  return sequence->rend();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::const_reverse_iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::rend(void) const
{
  return sequence->rend();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::size_type
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::size(void) const
{
  return sequence->size();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::size_type
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::max_size(void) const
{
  return sequence->max_size();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::resize(size_type sz,
  const MagnitudeT &mag)
{
  if(sequence.unique())
    sequence->resize(sz,mag);
  else
    resize_impl(sz,mag,typename detail::has_reserve_capability<container_type>::type());
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::size_type
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::capacity(void) const
{
  return sequence->capacity();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline bool basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::empty(void) const
{
  return sequence->empty();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::reserve(size_type sz)
{
  if(sz <= sequence->capacity())
    return;

  if(sequence.unique())
    sequence->reserve(sz);
  else {
    b::shared_ptr<container_type> tmp(new container_type());
    tmp->reserve(sz);
    tmp->assign(sequence->begin(),sequence->end());
    sequence.swap(tmp);
  }
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::reference
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::operator[](size_type n)
{
  if(!sequence.unique())
    sequence.reset(new container_type(*sequence));

  return sequence->operator[](n);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::const_reference
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::operator[](size_type n) const
{
  return sequence->operator[](n);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::const_reference
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::at(size_type n) const
{
  return sequence->at(n);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::reference
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::at(size_type n)
{
  if(!sequence.unique())
    sequence.reset(new container_type(*sequence));

  return sequence->at(n);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::reference
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::front(void)
{
  if(!sequence.unique())
    sequence.reset(new container_type(*sequence));

  return sequence->front();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::const_reference
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::front(void) const
{
  return sequence->front();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::reference
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::back(void)
{
  if(!sequence.unique())
    sequence.reset(new container_type(*sequence));

  return sequence->back();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::const_reference
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::back(void) const
{
  return sequence->back();
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::push_back(const MagnitudeT &val)
{
  if(sequence.unique())
    sequence->push_back(val);
  else
    push_back_impl(val,
      typename detail::has_reserve_capability<container_type>::type());
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::pop_back(void)
{
  if(sequence.unique())
    sequence->pop_back();
  else {
    b::shared_ptr<container_type> tmp(new container_type(*sequence));
    tmp->pop_back();
    sequence.swap(tmp);
  }
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::insert(iterator position, const MagnitudeT &val)
{
  if(sequence.unique())
    return sequence->insert(position,val);
  
  return insert_impl(position,val,
    typename detail::has_reserve_capability<container_type>::type());
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::insert(iterator position,
  size_type n, const MagnitudeT &val)
{
  if(sequence.unique())
    sequence->insert(position,n,val);
  else
    fill_insert_impl(position,n,val,
      typename detail::has_reserve_capability<container_type>::type());
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
template <class InputIterator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::insert(iterator position,
  InputIterator first, InputIterator last)
{
  range_insert_thunk(position,first,last,b::is_integral<InputIterator>());
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::erase(iterator position)
{
  if(sequence.unique())
    return sequence->erase(position);

  return erase_impl(position,
    typename detail::has_reserve_capability<container_type>::type());
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::erase(iterator first, iterator last)
{
  if(sequence.unique())
    return sequence->erase(first,last);

  return erase_impl(first,last,
    typename detail::has_reserve_capability<container_type>::type());
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  swap(basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> &rhs)
{
  std::swap(_sample_frequency,rhs._sample_frequency);
  std::swap(_time_start,rhs._time_start);
  sequence.swap(rhs.sequence);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::clear(void)
{
  if(sequence.unique())
    sequence->clear();
  else
    sequence.reset(new container_type());
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline bool basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  operator==(const basic_channel &rhs) const
{
  return (
    this == &rhs || 
    (_sample_frequency == rhs._sample_frequency &&
      _time_start == rhs._time_start && 
      (sequence == rhs.sequence || *sequence == *rhs.sequence)));
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline bool basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  operator!=(const basic_channel &rhs) const
{
  return !(*this == rhs);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<
  MagnitudeT,FrequencyT,TimeT,Container,Allocator>::frequency_type
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::frequency(const FrequencyT &freq)
{
  FrequencyT prev(_sample_frequency);
  _sample_frequency = freq;
  return prev;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline const typename basic_channel<
  MagnitudeT,FrequencyT,TimeT,Container,Allocator>::frequency_type &
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::frequency(void) const
{
  return _sample_frequency;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<
  MagnitudeT,FrequencyT,TimeT,Container,Allocator>::time_type
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::epoch(const TimeT &start)
{
  TimeT prev(_time_start);
  _time_start = start;
  return prev;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline const typename basic_channel<
  MagnitudeT,FrequencyT,TimeT,Container,Allocator>::time_type &
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::epoch(void) const
{
  return _time_start;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<
  MagnitudeT,FrequencyT,TimeT,Container,Allocator>::subchannel_type
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  subchannel(iterator first, iterator last)
{
  if(!sequence.unique())
    sequence.reset(new container_type(*sequence));
  
  return subchannel_type(*this,first,last);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::const_subchannel_type
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  subchannel(const_iterator first, const_iterator last) const
{
  return const_subchannel_type(*this,first,last);
}




/** \brief Free function to efficiently swap two basic_channel objects
 *  \param lhs,rhs to basic_channel references
 */
template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void swap(basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> &lhs, basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator> &rhs)
{
  lhs.swap(rhs);
}




template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  resize_impl(size_type sz, const MagnitudeT &mag, mpl::false_)
{
  b::shared_ptr<container_type> tmp(new container_type(*sequence));
  tmp->resize(sz,mag);
  sequence.swap(tmp);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  resize_impl(size_type sz, const MagnitudeT &mag, mpl::true_)
{
  b::shared_ptr<container_type> tmp(new container_type());
  tmp->reserve(sequence->size()+sz);
  tmp->assign(sequence->begin(),sequence->end());
  tmp->resize(sz,mag);
  sequence.swap(tmp);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  push_back_impl(const MagnitudeT &val, mpl::false_)
{
  b::shared_ptr<container_type> tmp(new container_type(*sequence));
  tmp->push_back(val);
  sequence.swap(tmp);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  push_back_impl(const MagnitudeT &val, mpl::true_)
{
  b::shared_ptr<container_type> tmp(new container_type());
  tmp->reserve(sequence->size()+1);
  tmp->assign(sequence->begin(),sequence->end());
  tmp->push_back(val);
  sequence.swap(tmp);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  insert_impl(iterator position, const MagnitudeT &val, mpl::false_)
{
  b::shared_ptr<container_type> tmp(new container_type(sequence->begin(),position));

  iterator res = tmp->end();
  reverse_iterator rfirst = sequence->rbegin();
  while(rfirst.base() != position)
    res = tmp->insert(res,*rfirst++);

  if(position != sequence->end())
    res = tmp->insert(res,*position);
  
  res = tmp->insert(res,val);
  sequence.swap(tmp);
  return res;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  insert_impl(iterator position, const MagnitudeT &val, mpl::true_)
{
  b::shared_ptr<container_type> tmp(new container_type());
  tmp->reserve(sequence->size()+1);
  tmp->assign(sequence->begin(),position);
  iterator res = tmp->insert(tmp->end(),val);
  tmp->insert(tmp->end(),position,sequence->end());
  sequence.swap(tmp);
  return res;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  fill_insert_impl(iterator position, size_type n, const MagnitudeT &val, mpl::false_)
{
  b::shared_ptr<container_type> tmp(new container_type(sequence->begin(),position));
  tmp->insert(tmp->end(),n,val);
  tmp->insert(tmp->end(),position,sequence->end());
  sequence.swap(tmp);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  fill_insert_impl(iterator position, size_type n, const MagnitudeT &val, mpl::true_)
{
  b::shared_ptr<container_type> tmp(new container_type());
  tmp->reserve(sequence->size()+n);
  tmp->assign(sequence->begin(),position);
  tmp->insert(tmp->end(),n,val);
  tmp->insert(tmp->end(),position,sequence->end());
  sequence.swap(tmp);
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
template <class InputIterator, typename IteratorTag>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  range_insert_impl(iterator position, InputIterator first, InputIterator last,
    IteratorTag, mpl::false_)
{
  if(sequence.unique())
    sequence->insert(position,first,last);
  else {
    b::shared_ptr<container_type> tmp(new container_type(sequence->begin(),position));
    tmp->insert(tmp->end(),first,last);
    tmp->insert(tmp->end(),position,sequence->end());
    sequence.swap(tmp);
  }
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
template <class InputIterator, typename IteratorTag>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  range_insert_impl(iterator position, InputIterator first, InputIterator last,
    IteratorTag, mpl::true_)
{
  if(sequence.unique())
    sequence->insert(position,first,last);
  else {
    b::shared_ptr<container_type> tmp(new container_type());
    tmp->reserve(sequence->size());
    tmp->assign(sequence->begin(),position);
    size_type extra = std::distance(position,sequence->end());
    while(first != last) {
      tmp->reserve(sequence->size()+extra+1);
      tmp->push_back(*first++);
    }
    tmp->insert(tmp->end(),position,sequence->end());
  }
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
template <class InputIterator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  range_insert_impl(iterator position, InputIterator first, InputIterator last,
    std::random_access_iterator_tag, mpl::true_)
{
  if(sequence.unique())
    sequence->insert(position,first,last);
  else {
    b::shared_ptr<container_type> tmp(new container_type());
    tmp->reserve(sequence->size()+std::distance(first,last));
    tmp->assign(sequence->begin(),position);
    tmp->insert(tmp->end(),first,last);
    tmp->insert(tmp->end(),position,sequence->end());
  }
}


template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  erase_impl(iterator position, mpl::false_)
{
  size_type len = std::distance(sequence->begin(),position);
  b::shared_ptr<container_type> tmp(new container_type(sequence->begin(),position));
  tmp->insert(tmp->end(),++position,sequence->end());
  sequence.swap(tmp);
  iterator res = sequence->begin();
  std::advance(res,len);
  return res;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  erase_impl(iterator position, mpl::true_)
{
  b::shared_ptr<container_type> tmp(new container_type());
  tmp->reserve(sequence->size()-1);
  tmp->assign(sequence->begin(),position);
  iterator res = tmp->insert(tmp->end(),*(++position));
  tmp->assign(position,sequence->end());
  sequence.swap(tmp);
  return res;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  erase_impl(iterator first, iterator last, mpl::false_)
{
  b::shared_ptr<container_type> tmp(new container_type(sequence->begin(),first));
  
  iterator res = tmp->end();
  reverse_iterator rfirst = sequence->rbegin();
  while(rfirst.base() != last)
    res = tmp->insert(res,*rfirst++);

  sequence.swap(tmp);
  return res;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
inline typename basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::iterator
basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
  erase_impl(iterator first, iterator last, mpl::true_)
{
  b::shared_ptr<container_type> tmp(new container_type());
  tmp->reserve(sequence->size()-std::distance(first,last));
  tmp->assign(sequence->begin(),first);
  
  iterator res = tmp->end();
  reverse_iterator rfirst = sequence->rbegin();
  while(rfirst.base() != last)
    res = tmp->insert(res,*rfirst++);

  sequence.swap(tmp);
  return res;
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
template <class InputIterator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
range_insert_thunk(iterator position, InputIterator first, InputIterator last,
  mpl::true_)
{
  fill_insert(position,static_cast<size_type>(first),
    static_cast<const value_type &>(last));
}

template<typename MagnitudeT, typename FrequencyT, typename TimeT,
  template<typename T, typename A> class Container,
  template<typename T> class Allocator>
template <class InputIterator>
inline void basic_channel<MagnitudeT,FrequencyT,TimeT,Container,Allocator>::
range_insert_thunk(iterator position, InputIterator first, InputIterator last,
  mpl::false_)
{
  range_insert_impl(position,first,last,
    typename std::iterator_traits<InputIterator>::iterator_category(),
    typename detail::has_reserve_capability<container_type>::type());
}


}
}


#endif
