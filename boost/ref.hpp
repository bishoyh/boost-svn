#ifndef BOOST_REF_HPP_INCLUDED
# define BOOST_REF_HPP_INCLUDED

# if _MSC_VER+0 >= 1020
#  pragma once
# endif

# include <boost/config.hpp>

//
//  ref.hpp - ref/cref, useful helper functions
//
//  Copyright (C) 1999, 2000 Jaakko J�rvi (jaakko.jarvi@cs.utu.fi)
//  Copyright (C) 2001 Peter Dimov
//  Copyright (C) 2002 David Abrahams
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/bind/ref.html for documentation.
//

namespace boost
{

template<class T> class reference_wrapper
{ 
public:
    typedef T type;

    explicit reference_wrapper(T& t): t_(&t) {}

    operator T& () const { return *t_; }

    T& get() const { return *t_; }

private:

    T* t_;
};

# if defined(__BORLANDC__) && (__BORLANDC__ <= 0x551)
#  define BOOST_REF_CONST
# else
#  define BOOST_REF_CONST const
# endif

template<class T> inline reference_wrapper<T> BOOST_REF_CONST ref(T & t)
{ 
    return reference_wrapper<T>(t);
}

template<class T> inline reference_wrapper<T const> BOOST_REF_CONST cref(T const & t)
{
    return reference_wrapper<T const>(t);
}

# undef BOOST_REF_CONST

# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template<typename T>
class is_reference_wrapper
{
 public:
    BOOST_STATIC_CONSTANT(bool, value = false); 
};

template<typename T>
class is_reference_wrapper<reference_wrapper<T> >
{
 public:
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template<typename T>
class unwrap_reference
{
 public:
    typedef T type;
};

template<typename T>
class unwrap_reference<reference_wrapper<T> >
{
 public:
    typedef T type;
};
# else // no partial specialization

} // namespace boost

#include <boost/type.hpp>

namespace boost
{

namespace detail
{
  typedef char (&yes_reference_wrapper_t)[1];
  typedef char (&no_reference_wrapper_t)[2];
      
  no_reference_wrapper_t is_reference_wrapper_test(...);

  template<typename T>
  yes_reference_wrapper_t is_reference_wrapper_test(type< reference_wrapper<T> >);

  template<bool wrapped>
  struct reference_unwrapper
  {
      template <class T>
      struct apply
      {
          typedef T type;
      };
  };

  template<>
  struct reference_unwrapper<true>
  {
      template <class T>
      struct apply
      {
          typedef typename T::type type;
      };
  };
}

template<typename T>
class is_reference_wrapper
{
 public:
    BOOST_STATIC_CONSTANT(
        bool, value = (
            sizeof(detail::is_reference_wrapper_test(type<T>()))
            == sizeof(detail::yes_reference_wrapper_t)));
};

template <typename T>
class unwrap_reference
    : public detail::reference_unwrapper<
        is_reference_wrapper<T>::value
      >::template apply<T>
{};

# endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace boost

#endif // #ifndef BOOST_REF_HPP_INCLUDED
