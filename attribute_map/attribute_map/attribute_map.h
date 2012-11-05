#ifndef LEMMA_ATTRIBUTE_MAP_ATTRIBUTE_MAP_H
#define LEMMA_ATTRIBUTE_MAP_ATTRIBUTE_MAP_H

#include <lemma/attribute_map/attribute_requirements.h>

#include <functional>
#include <map>
#include <memory>
#include <utility>

#include <stdexcept>

#include <sstream>

namespace lemma {
namespace attribute_map {
namespace detail {


/** \brief Associative Container with discriminated entry based on \c Pred
 *
 *  map values are values that are actuallly 'in' the container,
 *  the req & opt values are the 'rules' that applied to determine
 *  if the map value is allowed to be added...
 *
 *  Must enforce the rules s.t. they never change over the course of the object's
 *  lifetime. This way we ensure that whatever values are in the container,
 *  the rules apply to them, ie if we didn't we could get ourselves into
 *  a situation through assignment where the contents to comply with the
 *  current rules for the container
 *
 *  Therefore, designed as a base class for other objects that provide
 *  this as a 'view'
 */
template<typename Key, typename T, typename Pred,
  typename Compare=std::less<Key>,
  typename Allocator=std::allocator<std::pair<const Key,const T> > >
class basic_attribute_map :private std::map<Key,const T,Compare,Allocator> {
  private:
    typedef std::map<Key,const T,Compare,Allocator> map_base;
    typedef typename
      Allocator::template rebind<std::pair<const Key,Pred> >::other other_alloc;

  public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef Pred predicate_type;
    typedef typename map_base::value_type value_type;
    typedef typename map_base::key_compare key_compare;
    typedef typename map_base::allocator_type allocator_type;
    typedef typename map_base::reference reference;
    typedef typename map_base::const_reference const_reference;
    typedef typename map_base::iterator iterator;
    typedef typename map_base::const_iterator const_iterator;
    typedef typename map_base::size_type size_type;
    typedef typename map_base::difference_type difference_reference;
    typedef typename map_base::pointer pointer;
    typedef typename map_base::const_pointer const_pointer;
    typedef typename map_base::reverse_iterator reverse_iterator;
    typedef typename map_base::const_reverse_iterator const_reverse_iterator;
    typedef typename map_base::value_compare value_compare;

    typedef basic_attribute_requirements<Key,Pred,Compare,other_alloc> attribute_requirements_type; 
              
    class predicate_error : public std::runtime_error {
      public:
        explicit predicate_error(const std::string &what_arg)
          :std::runtime_error(what_arg) {}
    };

    explicit basic_attribute_map(const Compare &comp = Compare(),
      const Allocator &alloc = Allocator());

    // only applicable if req only contains optional requirements
    basic_attribute_map(const attribute_requirements_type &req,
      const Allocator &alloc=Allocator());

    template<typename InputIterator>
    basic_attribute_map(InputIterator first, InputIterator last,
      const attribute_requirements_type &req = attribute_requirements_type(),
      const Compare &comp = Compare(), const Allocator &alloc=Allocator());

    basic_attribute_map(const basic_attribute_map<Key,T,Pred,Compare,Allocator> &rhs);
    ~basic_attribute_map(void);
    
    basic_attribute_map<Key,T,Pred,Compare,Allocator> &
      operator=(const basic_attribute_map<Key,T,Pred,Compare,Allocator> &rhs);

    using map_base::begin;
    using map_base::end;
    using map_base::rbegin;
    using map_base::rend;
    
    using map_base::empty;
    using map_base::size;
    using map_base::max_size;
    
    //no operator[], can't check insert if have reference to storage
    
    std::pair<iterator,bool> insert(const value_type &x);
    iterator insert(iterator position, const value_type &x);

    template<typename InputIterator>
      void insert(InputIterator first, InputIterator last);
    
    void erase(iterator position);
    size_type erase(const key_type &x);
    void erase(iterator first, iterator last);
    
    // similar to insert with the exception that if the key already exists,
    // attempt to replace the value with x.second and report false
    // otherwise report true
    std::pair<iterator,bool> replace(const value_type &x);
    
    void clear(void);
    
    using map_base::key_comp;
    using map_base::value_comp;
    
    using map_base::find;
    using map_base::count;
    
    using map_base::lower_bound;
    using map_base::upper_bound;
    
    using map_base::equal_range;

    void swap(basic_attribute_map<Key,T,Pred,Compare,Allocator> &rhs);
  
    const attribute_requirements_type & requirements(void) const;
  
    friend bool operator==(const basic_attribute_map &lhs, const basic_attribute_map &rhs) {
      return (lhs._attribute_requirements == rhs._attribute_requirements) &&
        (static_cast<const map_base &>(lhs) == static_cast<const map_base &>(rhs));
    }

    friend bool operator!=(const basic_attribute_map &lhs, const basic_attribute_map &rhs) {
      return !(lhs == rhs);
    }

    friend bool operator<(const basic_attribute_map &lhs, const basic_attribute_map &rhs) {
      return (lhs._attribute_requirements < rhs._attribute_requirements) &&
        (static_cast<const map_base &>(lhs) < static_cast<const map_base &>(rhs));
    }

    friend bool operator<=(const basic_attribute_map &lhs, const basic_attribute_map &rhs) {
      return (lhs._attribute_requirements <= rhs._attribute_requirements) &&
        (static_cast<const map_base &>(lhs) <= static_cast<const map_base &>(rhs));
    }

    friend bool operator>(const basic_attribute_map &lhs, const basic_attribute_map &rhs) {
      return (lhs._attribute_requirements > rhs._attribute_requirements) &&
        (static_cast<const map_base &>(lhs) > static_cast<const map_base &>(rhs));
    }

    friend bool operator>=(const basic_attribute_map &lhs, const basic_attribute_map &rhs) {
      return (lhs._attribute_requirements >= rhs._attribute_requirements) &&
        (static_cast<const map_base &>(lhs) >= static_cast<const map_base &>(rhs));
    }
  
  private:
    attribute_requirements_type _attribute_requirements;
    
    const attribute_requirements_type &attribute_requirements;
};

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  basic_attribute_map(const Compare &comp, const Allocator &alloc)
    :map_base(comp,alloc), _attribute_requirements(comp,other_alloc(alloc)),
      attribute_requirements(_attribute_requirements)
{
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  basic_attribute_map(const attribute_requirements_type &req,
    const Allocator &alloc) :map_base(req.key_comp(),alloc),
      _attribute_requirements(req),attribute_requirements(_attribute_requirements)
{
  if(!req.req_empty())
    throw predicate_error("Empty value list with required attributes");
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
template<typename InputIterator>
inline basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  basic_attribute_map(InputIterator first, InputIterator last,
  const attribute_requirements_type &req, const Compare &comp,
    const Allocator &alloc) :map_base(comp,alloc),_attribute_requirements(req),
      attribute_requirements(_attribute_requirements)
{
  typedef typename attribute_requirements_type::const_iterator req_iterator;

  insert(first,last);

  // should make sure that all of the required attributes are satisfied!
  for(req_iterator rfirst = req.req_begin(); rfirst != req.req_end(); ++rfirst) {
    if(map_base::find(rfirst->first) == map_base::end()) {
      std::stringstream err;
      err << "Missing required attribute '" << rfirst->first << "'";
      throw predicate_error(err.str());
    }
  }
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  basic_attribute_map(const basic_attribute_map<Key,T,Pred,Compare,Allocator> &rhs)
    :map_base(static_cast<const map_base&>(rhs)),
      _attribute_requirements(rhs._attribute_requirements),
      attribute_requirements(_attribute_requirements)
{
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  ~basic_attribute_map(void)
{
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline basic_attribute_map<Key,T,Pred,Compare,Allocator> &
basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  operator=(const basic_attribute_map<Key,T,Pred,Compare,Allocator> &rhs)
{
  if(this != &rhs) {
    basic_attribute_map tmp(rhs);
    this->swap(tmp);
  }
  
  return *this;
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline std::pair<
  typename basic_attribute_map<Key,T,Pred,Compare,Allocator>::iterator,bool>
basic_attribute_map<Key,T,Pred,Compare,Allocator>::insert(const value_type &x)
{
  typedef typename attribute_requirements_type::const_iterator req_iterator;
  
  iterator pos = map_base::find(x.first);
  if(pos != map_base::end())
    return std::make_pair(pos,false);
  
  req_iterator loc = attribute_requirements.req_find(x.first);
  if(loc != attribute_requirements.req_end() && !loc->second(x.second)) {
    std::stringstream err;
    err << "element '" << x.first << "' with value '" << x.second
      << "' does not meet required predicate '" << loc->second << "' during insert";
    throw predicate_error(err.str());
  }
  else {
    loc = attribute_requirements.opt_find(x.first);
    if(loc != attribute_requirements.opt_end() && !loc->second(x.second)) {
      std::stringstream err;
      err << "element '" << x.first << "' with value '" << x.second
        << "' does not meet optional predicate '" << loc->second << "' during insert";
      throw predicate_error(err.str());
    }
  }
  
  return map_base::insert(x);
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
typename basic_attribute_map<Key,T,Pred,Compare,Allocator>::iterator
basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  insert(iterator position, const value_type &x)
{
  typedef typename attribute_requirements_type::const_iterator req_iterator;
  
  iterator pos = map_base::find(x.first);
  if(pos != map_base::end())
    return pos;

  req_iterator req_loc = attribute_requirements.req_find(x.first);
  if(req_loc != attribute_requirements.req_end() && !req_loc->second(x.second)) {
    std::stringstream err;
    err << "element '" << x.first << "' with value '" << x.second
      << "' does not meet required predicate during insert";
    throw predicate_error(err.str());
  }
  else {
    req_loc = attribute_requirements.opt_find(x.first);
    if(req_loc != attribute_requirements.opt_end() && !req_loc->second(x.second)) {
      std::stringstream err;
      err << "element '" << x.first << "' with value '" << x.second
        << "' does not meet optional predicate during insert";
      throw predicate_error(err.str());
    }
  }
  
  return map_base::insert(position,x);
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
template<typename InputIterator>
inline void basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  insert(InputIterator first, InputIterator last)
{
  while(first != last)
    insert(*first++);
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline void basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  erase(iterator position)
{
  if(attribute_requirements.req_find(position->first) != attribute_requirements.req_end()) {
    std::stringstream err;
    err << "unable to erase element containing required attribute '" << position->first << "'";
    throw predicate_error(err.str());
  }

  map_base::erase(position);
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline typename basic_attribute_map<Key,T,Pred,Compare,Allocator>::size_type
basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  erase(const key_type &x)
{
  std::pair<iterator,iterator> range = map_base::equal_range(x);
  
  size_type n = 0;
  for(iterator cur = range.first; cur != range.second; ++cur, ++n) {
    if(attribute_requirements.req_find(cur->first) != attribute_requirements.req_end()) {
      std::stringstream err;
      err << "unable to erase element containing required attribute '" << cur->first << "'";
      throw predicate_error(err.str());
    }
  }
  
  map_base::erase(range.first,range.second);
  
  return n;
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline void basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  erase(iterator first, iterator last)
{
  for(iterator cur = first; cur != last; ++cur) {
    if(attribute_requirements.req_find(cur->first) != attribute_requirements.req_end()) {
      std::stringstream err;
      err << "unable to erase element containing required attribute '" << cur->first << "'";
      throw predicate_error(err.str());
    }
  }

  map_base::erase(first,last);
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline std::pair<
  typename basic_attribute_map<Key,T,Pred,Compare,Allocator>::iterator,bool>
basic_attribute_map<Key,T,Pred,Compare,Allocator>::replace(const value_type &x)
{
  using std::swap;
  typedef typename attribute_requirements_type::const_iterator req_iterator;
  
  req_iterator loc = attribute_requirements.req_find(x.first);
  if(loc != attribute_requirements.req_end() && !loc->second(x.second)) {
    std::stringstream err;
    err << "element '" << x.first << "' with value '" << x.second
      << "' does not meet required predicate during insert";
    throw predicate_error(err.str());
  }
  else {
    loc = attribute_requirements.opt_find(x.first);
    if(loc != attribute_requirements.opt_end() && !loc->second(x.second)) {
      std::stringstream err;
      err << "element '" << x.first << "' with value '" << x.second
        << "' does not meet optional predicate during insert";
      throw predicate_error(err.str());
    }
  }

  iterator pos = map_base::find(x.first);
  if(pos != map_base::end()) {
    mapped_type tmp(x.second);
    swap(const_cast<mapped_type&>(pos->second),tmp);
    return std::make_pair(pos,false);
  }

  return map_base::insert(x);
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline void basic_attribute_map<Key,T,Pred,Compare,Allocator>::clear(void)
{
  if(!attribute_requirements.req_empty())
    throw predicate_error("unable to clear with required elements");
  
  map_base::clear();
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline void basic_attribute_map<Key,T,Pred,Compare,Allocator>::
  swap(basic_attribute_map<Key,T,Pred,Compare,Allocator> &rhs)
{
  if(this != &rhs) {
    map_base::swap(rhs);
    _attribute_requirements.swap(rhs._attribute_requirements);
  }
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline const typename basic_attribute_map<Key,T,Pred,Compare,Allocator>::attribute_requirements_type &
basic_attribute_map<Key,T,Pred,Compare,Allocator>::requirements(void) const
{
  return attribute_requirements;
}

template<typename Key, typename T, typename Pred, typename Compare,
  typename Allocator>
inline void swap(basic_attribute_map<Key,T,Pred,Compare,Allocator> &lhs,
  basic_attribute_map<Key,T,Pred,Compare,Allocator> &rhs)
{
  lhs.swap(rhs);
}

}
}
}

#endif
