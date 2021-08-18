#ifndef MCCADEDGEHYPERBOLA_HXX
#define MCCADEDGEHYPERBOLA_HXX

#include "McCadEdge.hxx"

class McCadEdgeHyperbola : public McCadEdge
{
public:    

    void* operator new(size_t,void* anAddress)
    {
        return anAddress;
    }
    void* operator new(size_t size)
    {
        return Standard::Allocate(size);
    }
    void  operator delete(void *anAddress)
    {
        if (anAddress) Standard::Free((Standard_Address&)anAddress);
    }

public:
    McCadEdgeHyperbola();
    McCadEdgeHyperbola(const TopoDS_Edge &theEdge);
    ~McCadEdgeHyperbola();
};

#endif // MCCADEDGEHYPERBOLA_HXX
