#pragma once
//
// see http://stackoverflow.com/questions/6172361/c-cli-wrapping-a-function-that-returns-a-stdshared-ptr
//
#include <memory>
#include <boost/shared_ptr.hpp>


namespace og
{
namespace net
{

template <class T>
public ref class OGSharedPtr sealed
{
  boost::shared_ptr<T>* pPtr;

public:
  OGSharedPtr()
    : pPtr(new boost::shared_ptr<T>())
  {}

  OGSharedPtr(T* t)
  {
    pPtr = new boost::shared_ptr<T>(t);
  }

  OGSharedPtr(boost::shared_ptr<T> t)
  {
    pPtr = new boost::shared_ptr<T>(t);
  }

  OGSharedPtr(const OGSharedPtr<T>% t)
  {
    pPtr = new boost::shared_ptr<T>(*t.pPtr);
  }

  !OGSharedPtr()
  {
    delete pPtr;
  }

  ~OGSharedPtr()
  {
    delete pPtr;
  }

  operator boost::shared_ptr<T>()
  {
    return *pPtr;
  }

  OGSharedPtr<T>% operator=(T* ptr)
  {
    delete pPtr;
    pPtr = new boost::shared_ptr<T>(ptr);
    return *this;
  }

  T* operator->()
  {
    return (*pPtr).get();
  }

  T* get()
  {
    return (*pPtr).get();
  }

  void reset()
  {
    pPtr->reset();
  }
};

}
}
