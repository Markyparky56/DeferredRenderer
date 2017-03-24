#pragma once
#include "UniquePtr.hpp"

// Non-nullable variant of UniquePtr (Like SharedRef and SharedPtr)

template< typename T >
class UniqueObj
{
public:
    UniqueObj(const UniqueObj& other)
        : Obj(MakeUnique<T>(*other))
    {
    }

    // Becuase UniqueObj's internal pointer can't be null, we can't move it
    // However we can call the move constructor of T
    UniqueObj(UniqueObj&& other)
        : Obj(MakeUnique<T>(MoveTemp(*other)))
    {
    }

    template< typename... Args >
    explicit UniqueObj(Args&&... args)
        : Obj(MakeUnique<T>(Forward<Args>(args)...))
    {
    }

    // Disallow copy-assignment
    UniqueObj& operator=(const UniqueObj&) = delete;

    // Move-assignment is implemented as swapping
    UniqueObj& operator=(UniqueObj&& other)
    {
        Swap(Obj, other.Obj);
        return *this;
    }

    template< typename Arg >
    UniqueObj& operator=(Arg&& other)
    {
        *Obj = Forward<Arg>(other);
        return *this;
    }

    // Accessor functions
          T& Get()       { return *Obj; }
    const T& Get() const { return *Obj; }

          T* operator->()       { return Obj.Get(); }
    const T* operator->() const { return Obj.Get(); }

          T& operator*()       { return *Obj; }
    const T& operator*() const { return *Obj; }

private:
    UniquePtr<T> Obj;
};