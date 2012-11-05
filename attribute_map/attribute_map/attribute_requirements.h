#ifndef LEMMA_ATTRIBUTE_MAP_ATTRIBUTE_REQUIREMENTS_H
#define LEMMA_ATTRIBUTE_MAP_ATTRIBUTE_REQUIREMENTS_H

#include <boost/shared_ptr.hpp>

#include <map>
#include <stdexcept>

namespace lemma {
namespace attribute_map {
namespace detail {

namespace b = boost;

template<typename Key, typename Pred,
  typename Compare=std::less<Key>,
  typename Allocator=std::allocator<std::pair<const Key, Pred> > >
class basic_attribute_requirements {
  private:
    typedef std::map<Key,Pred,Compare,Allocator> map_type;
    
  public:
    typedef typename map_type::key_type key_type;
    typedef typename map_type::mapped_type predicate_type;
    typedef typename map_type::value_type value_type;
    typedef typename map_type::key_compare key_compare;
    typedef typename map_type::allocator_type allocator_type;
    typedef typename map_type::reference reference;
    typedef typename map_type::const_reference const_reference;
    typedef typename map_type::iterator iterator;
    typedef typename map_type::const_iterator const_iterator;
    typedef typename map_type::size_type size_type;
    typedef typename map_type::difference_type difference_type;
    typedef typename map_type::pointer pointer;
    typedef typename map_type::const_pointer const_pointer;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    
    typedef typename map_type::value_compare value_compare;

    class attribute_error : public std::runtime_error {
      public:
        explicit attribute_error(const std::string &what_arg)
          :std::runtime_error(what_arg) {}
    };

    explicit basic_attribute_requirements(const Compare &comp=Compare(),
      const Allocator &alloc = Allocator());
    
    template<typename ReqInputIterator, typename OptInputIterator>
    basic_attribute_requirements(ReqInputIterator reqFirst,
      ReqInputIterator reqLast, OptInputIterator optFirst,
      OptInputIterator optLast, const Compare &comp=Compare(),
      const Allocator &alloc=Allocator());
    
    basic_attribute_requirements(const basic_attribute_requirements &rhs);
    ~basic_attribute_requirements(void);
    
    basic_attribute_requirements<Key,Pred,Compare,Allocator> &
      operator=(const basic_attribute_requirements &rhs);
    
    // required attribute access
    iterator req_begin(void);
    const_iterator req_begin(void) const;
    iterator req_end(void);
    const_iterator req_end(void) const;
    reverse_iterator req_rbegin(void);
    const_reverse_iterator req_rbegin(void) const;
    reverse_iterator req_rend(void);
    const_reverse_iterator req_rend(void) const;

    bool req_empty(void) const;
    size_type req_size(void) const;
    size_type req_max_size(void) const;
    
    // no operator[], where should newly created values go? req or opt?

    std::pair<iterator,bool> req_insert(const value_type &x);
    // reflects C++ LWG issue 233 proposed change
    iterator req_insert(iterator position, const value_type &x);
    template<typename ForwardIterator>
      void req_insert(ForwardIterator first, ForwardIterator last);
    
    void req_erase(iterator position);
    size_type req_erase(const key_type &x);
    void req_erase(iterator first, iterator last);
    void req_clear(void);

    iterator req_find(const key_type &x);
    const_iterator req_find(const key_type &x) const;
    size_type req_count(const key_type &x) const;
    
    iterator req_lower_bound(const key_type &x);
    const_iterator req_lower_bound(const key_type &x) const;
    iterator req_upper_bound(const key_type &x);
    const_iterator req_upper_bound(const key_type &x) const;

    std::pair<iterator,iterator> req_equal_range(const key_type &x);
    std::pair<const_iterator,const_iterator> req_equal_range(const key_type &x) const;


    // optional attribute access
    iterator opt_begin(void);
    const_iterator opt_begin(void) const;
    iterator opt_end(void);
    const_iterator opt_end(void) const;
    reverse_iterator opt_rbegin(void);
    const_reverse_iterator opt_rbegin(void) const;
    reverse_iterator opt_rend(void);
    const_reverse_iterator opt_rend(void) const;

    bool opt_empty(void) const;
    size_type opt_size(void) const;
    size_type opt_max_size(void) const;
    
    // no operator[], where should newly created values go? req or opt?

    std::pair<iterator,bool> opt_insert(const value_type &x);
    // reflects C++ LWG issue 233 proposed change
    iterator opt_insert(iterator position, const value_type &x);
    template<typename ForwardIterator>
      void opt_insert(ForwardIterator first, ForwardIterator last);
    
    void opt_erase(iterator position);
    size_type opt_erase(const key_type &x);
    void opt_erase(iterator first, iterator last);
    void opt_clear(void);

    iterator opt_find(const key_type &x);
    const_iterator opt_find(const key_type &x) const;
    size_type opt_count(const key_type &x) const;
    
    iterator opt_lower_bound(const key_type &x);
    const_iterator opt_lower_bound(const key_type &x) const;
    iterator opt_upper_bound(const key_type &x);
    const_iterator opt_upper_bound(const key_type &x) const;

    std::pair<iterator,iterator> opt_equal_range(const key_type &x);
    std::pair<const_iterator,const_iterator> opt_equal_range(const key_type &x) const;

    void swap(basic_attribute_requirements &rhs);
    
    key_compare key_comp(void) const;
    value_compare value_comp(void) const;
    
  private:
    template<typename _Key, typename _Pred, typename _Compare, typename _Allocator>
    friend bool operator==(const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &lhs,
      const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &rhs);

    template<typename _Key, typename _Pred, typename _Compare, typename _Allocator>
    friend bool operator!=(const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &lhs,
      const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &rhs);

    template<typename _Key, typename _Pred, typename _Compare, typename _Allocator>
    friend bool operator<(const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &lhs,
      const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &rhs);

    template<typename _Key, typename _Pred, typename _Compare, typename _Allocator>
    friend bool operator<=(const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &lhs,
      const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &rhs);
  
    template<typename _Key, typename _Pred, typename _Compare, typename _Allocator>
    friend bool operator>(const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &lhs,
      const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &rhs);

    template<typename _Key, typename _Pred, typename _Compare, typename _Allocator>
    friend bool operator>=(const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &lhs,
      const basic_attribute_requirements<_Key,_Pred,_Compare,_Allocator> &rhs);

    b::shared_ptr<map_type> required_attributes;
    b::shared_ptr<map_type> optional_attributes;
    
    iterator _insert(b::shared_ptr<map_type> &map_ptr, iterator position,
      const value_type &x);
    void _erase(b::shared_ptr<map_type> &map_ptr, iterator position);
    void _erase(b::shared_ptr<map_type> &map_ptr, iterator first, iterator last);

    void _detach(b::shared_ptr<map_type> &map_ptr);
};

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline bool operator==(const basic_attribute_requirements<Key,Pred,Compare,Allocator> &lhs,
  const basic_attribute_requirements<Key,Pred,Compare,Allocator> &rhs)
{
  return
    ((lhs.required_attributes == rhs.required_attributes) ||
      (*(lhs.required_attributes) == *(rhs.required_attributes))) &&
    ((lhs.optional_attributes == rhs.optional_attributes) ||
      (*(lhs.optional_attributes) == *(rhs.optional_attributes)));
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline bool operator!=(const basic_attribute_requirements<Key,Pred,Compare,Allocator> &lhs,
  const basic_attribute_requirements<Key,Pred,Compare,Allocator> &rhs)
{
  return !(lhs == rhs);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline bool operator<(const basic_attribute_requirements<Key,Pred,Compare,Allocator> &lhs,
  const basic_attribute_requirements<Key,Pred,Compare,Allocator> &rhs)
{
  return
    !((lhs.required_attributes == rhs.required_attributes) ||
      (lhs.optional_attributes == rhs.optional_attributes)) &&
    (*(lhs.required_attributes) < *(rhs.required_attributes)) &&
      (*(lhs.optional_attributes) < *(rhs.optional_attributes));
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline bool operator<=(const basic_attribute_requirements<Key,Pred,Compare,Allocator> &lhs,
  const basic_attribute_requirements<Key,Pred,Compare,Allocator> &rhs)
{
  return
    ((lhs.required_attributes == rhs.required_attributes) &&
      (lhs.optional_attributes == rhs.optional_attributes)) ||
    (*(lhs.required_attributes) <= *(rhs.required_attributes)) &&
      (*(lhs.optional_attributes) <= *(rhs.optional_attributes));
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline bool operator>(const basic_attribute_requirements<Key,Pred,Compare,Allocator> &lhs,
  const basic_attribute_requirements<Key,Pred,Compare,Allocator> &rhs)
{
  return
    !((lhs.required_attributes == rhs.required_attributes) ||
      (lhs.optional_attributes == rhs.optional_attributes)) &&
    (*(lhs.required_attributes) > *(rhs.required_attributes)) &&
      (*(lhs.optional_attributes) > *(rhs.optional_attributes));
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline bool operator>=(const basic_attribute_requirements<Key,Pred,Compare,Allocator> &lhs,
  const basic_attribute_requirements<Key,Pred,Compare,Allocator> &rhs)
{
  return
    ((lhs.required_attributes == rhs.required_attributes) &&
      (lhs.optional_attributes == rhs.optional_attributes)) ||
    (*(lhs.required_attributes) >= *(rhs.required_attributes)) &&
      (*(lhs.optional_attributes) >= *(rhs.optional_attributes));
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void swap(basic_attribute_requirements<Key,Pred,Compare,Allocator> &lhs,
  basic_attribute_requirements<Key,Pred,Compare,Allocator> &rhs)
{
  lhs.swap(rhs);
}






template<typename Key, typename Pred, typename Compare, typename Allocator>
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  basic_attribute_requirements(const Compare &comp, const Allocator &alloc)
    :required_attributes(new map_type(comp,alloc)),
      optional_attributes(new map_type(comp,alloc))
{
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
template<typename ReqInputIterator, typename OptInputIterator>
inline basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  basic_attribute_requirements(ReqInputIterator reqFirst,
    ReqInputIterator reqLast, OptInputIterator optFirst,
    OptInputIterator optLast, const Compare &comp, const Allocator &alloc)
      :required_attributes(new map_type(reqFirst,reqLast,comp,alloc)),
      optional_attributes(new map_type(optFirst,optLast,comp,alloc))
{
  iterator cur = optional_attributes->begin();
  while(cur != optional_attributes->end()) {
    if(std::binary_search(required_attributes->begin(),required_attributes->end(),
      *cur++,value_comp()))
    {
      throw attribute_error("Optional attributes duplicate required");
    }
  }
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  basic_attribute_requirements(const basic_attribute_requirements &rhs)
    :required_attributes(rhs.required_attributes),
      optional_attributes(rhs.optional_attributes)
{
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  ~basic_attribute_requirements(void)
{
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline basic_attribute_requirements<Key,Pred,Compare,Allocator> &
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  operator=(const basic_attribute_requirements &rhs)
{
  if(this != &rhs) {
    basic_attribute_requirements tmp(rhs);
    
    this->swap(tmp);
  }
  
  return *this;
}


// required attributes
template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_begin(void)
{
  _detach(required_attributes);

  return required_attributes->begin();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_begin(void) const
{
  return required_attributes->begin();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_end(void)
{
  _detach(required_attributes);

  return required_attributes->end();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_end(void) const
{
  return required_attributes->end();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::reverse_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_rbegin(void)
{
  _detach(required_attributes);

  return required_attributes->rbegin();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_reverse_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_rbegin(void) const
{
  return required_attributes->rbegin();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::reverse_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_rend(void)
{
  _detach(required_attributes);

  return required_attributes->rend();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_reverse_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_rend(void) const
{
  return required_attributes->rend();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline bool basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_empty(void) const
{
  return required_attributes->empty();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::size_type
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_size(void) const
{
  return required_attributes->size();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::size_type
basic_attribute_requirements<Key,Pred,Compare,Allocator>::req_max_size(void) const
{
  return required_attributes->max_size();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline std::pair<
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator,bool>
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_insert(const value_type &x)
{
  if(std::binary_search(optional_attributes->begin(),optional_attributes->end(),
    x,value_comp()))
  {
    throw attribute_error("Required attribute duplicates optional");
  }

  _detach(required_attributes);
  
  return required_attributes->insert(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_insert(iterator position, const value_type &x)
{
  if(std::binary_search(optional_attributes->begin(),optional_attributes->end(),
    x,value_comp()))
  {
    throw attribute_error("Required attribute duplicates optional");
  }

  return _insert(required_attributes,position,x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
template<typename ForwardIterator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_insert(ForwardIterator first, ForwardIterator last)
{
  // provide exception guarantee
  for(ForwardIterator cur = first; cur != last; ++cur) {
    if(std::binary_search(optional_attributes->begin(),optional_attributes->end(),
      *cur,value_comp()))
    {
      throw attribute_error("Required attribute duplicates optional");
    }
  }
    
  if(required_attributes.unique())
    required_attributes->insert(first,last);
  else {
    b::shared_ptr<map_type> tmp(new map_type(*required_attributes));
    tmp->insert(first,last);
    
    required_attributes.swap(tmp);
  }
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_erase(iterator position)
{
  _erase(required_attributes,position);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::size_type
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_erase(const key_type &x)
{
  _detach(required_attributes);

  return required_attributes->erase(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_erase(iterator first, iterator last)
{
  _erase(required_attributes,first,last);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_clear(void)
{
  _detach(required_attributes);

  required_attributes->clear();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_find(const key_type &x)
{
  _detach(required_attributes);

  return required_attributes->find(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_find(const key_type &x) const
{
  return required_attributes->find(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::size_type
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_count(const key_type &x) const
{
  return required_attributes->count(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_lower_bound(const key_type &x)
{
  _detach(required_attributes);

  return required_attributes->lower_bound(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_lower_bound(const key_type &x) const
{
  return required_attributes->lower_bound(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_upper_bound(const key_type &x)
{
  _detach(required_attributes);

  return required_attributes->upper_bound(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_upper_bound(const key_type &x) const
{
  return required_attributes->upper_bound(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline std::pair<
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator,
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator>
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_equal_range(const key_type &x)
{
  _detach(required_attributes);

  return required_attributes->equal_range(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline std::pair<
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator,
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator>
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  req_equal_range(const key_type &x) const
{
  return required_attributes->equal_range(x);
}


// optional attributes
template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_begin(void)
{
  _detach(optional_attributes);

  return optional_attributes->begin();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_begin(void) const
{
  return optional_attributes->begin();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_end(void)
{
  _detach(optional_attributes);

  return optional_attributes->end();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_end(void) const
{
  return optional_attributes->end();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::reverse_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_rbegin(void)
{
  _detach(optional_attributes);

  return optional_attributes->rbegin();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_reverse_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_rbegin(void) const
{
  return optional_attributes->rbegin();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::reverse_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_rend(void)
{
  _detach(optional_attributes);

  return optional_attributes->rend();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_reverse_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_rend(void) const
{
  return optional_attributes->rend();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline bool basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_empty(void) const
{
  return optional_attributes->empty();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::size_type
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_size(void) const
{
  return optional_attributes->size();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::size_type
basic_attribute_requirements<Key,Pred,Compare,Allocator>::opt_max_size(void) const
{
  return optional_attributes->max_size();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline std::pair<
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator,bool>
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_insert(const value_type &x)
{
  if(std::binary_search(required_attributes->begin(),required_attributes->end(),
    x,value_comp()))
  {
    throw attribute_error("Optional attribute duplicates required");
  }
  
  _detach(optional_attributes);

  return optional_attributes->insert(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_insert(iterator position, const value_type &x)
{
  if(std::binary_search(required_attributes->begin(),required_attributes->end(),
    x,value_comp()))
  {
    throw attribute_error("Optional attribute duplicates required");
  }

  return _insert(optional_attributes,position,x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
template<typename ForwardIterator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_insert(ForwardIterator first, ForwardIterator last)
{
  // provide exception guarantee
  for(ForwardIterator cur = first; cur != last; ++cur) {
    if(std::binary_search(required_attributes->begin(),required_attributes->end(),
      *cur,value_comp()))
    {
      throw attribute_error("Optional attribute duplicates required");
    }
  }
    
  if(optional_attributes.unique())
    optional_attributes->insert(first,last);
  else {
    b::shared_ptr<map_type> tmp(new map_type(*optional_attributes));
    tmp->insert(first,last);
    
    optional_attributes.swap(tmp);
  }
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_erase(iterator position)
{
  _erase(optional_attributes,position);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::size_type
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_erase(const key_type &x)
{
  _detach(optional_attributes);

  return optional_attributes->erase(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_erase(iterator first, iterator last)
{
  _erase(optional_attributes,first,last);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_clear(void)
{
  _detach(optional_attributes);

  optional_attributes->clear();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_find(const key_type &x)
{
  _detach(optional_attributes);

  return optional_attributes->find(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_find(const key_type &x) const
{
  return optional_attributes->find(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::size_type
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_count(const key_type &x) const
{
  return optional_attributes->count(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_lower_bound(const key_type &x)
{
  _detach(optional_attributes);

  return optional_attributes->lower_bound(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_lower_bound(const key_type &x) const
{
  return optional_attributes->lower_bound(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_upper_bound(const key_type &x)
{
  _detach(optional_attributes);

  return optional_attributes->upper_bound(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_upper_bound(const key_type &x) const
{
  return optional_attributes->upper_bound(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline std::pair<
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator,
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator>
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_equal_range(const key_type &x)
{
  _detach(optional_attributes);

  return optional_attributes->equal_range(x);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline std::pair<
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator,
  typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::const_iterator>
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  opt_equal_range(const key_type &x) const
{
  return optional_attributes->equal_range(x);
}


template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  swap(basic_attribute_requirements &rhs)
{
  required_attributes.swap(rhs.required_attributes);
  optional_attributes.swap(rhs.optional_attributes);
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::key_compare
basic_attribute_requirements<Key,Pred,Compare,Allocator>::key_comp(void) const
{
  return required_attributes->key_comp();
}

template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::value_compare
basic_attribute_requirements<Key,Pred,Compare,Allocator>::value_comp(void) const
{
  return required_attributes->value_comp();
}



template<typename Key, typename Pred, typename Compare, typename Allocator>
inline typename basic_attribute_requirements<Key,Pred,Compare,Allocator>::iterator
basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  _insert(b::shared_ptr<map_type> &map_ptr, iterator position,
    const value_type &x)
{
  if(map_ptr.unique())
    return map_ptr->insert(position,x);
  
  b::shared_ptr<map_type> tmp(new map_type(map_ptr->begin(),position));
  iterator res = tmp->insert(tmp->end(),x);
  while(position != map_ptr->end())
    tmp->insert(tmp->end(),*position++);

  map_ptr.swap(tmp);
  
  return res;
}









template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  _erase(b::shared_ptr<map_type> &map_ptr, iterator position)
{
  // since this an iterator, we must be unique
  assert(map_ptr.unique());
  return map_ptr->erase(position);
}


template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  _erase(b::shared_ptr<map_type> &map_ptr, iterator first, iterator last)
{
  assert(map_ptr.unique());
  return map_ptr->erase(first,last);
}






template<typename Key, typename Pred, typename Compare, typename Allocator>
inline void basic_attribute_requirements<Key,Pred,Compare,Allocator>::
  _detach(b::shared_ptr<map_type> &map_ptr)
{
  if(!map_ptr.unique()) {
    b::shared_ptr<map_type> tmp(new map_type(*map_ptr));
    map_ptr.swap(tmp);
  }
}


}
}
}

#endif
