#ifndef MCCADEDGEPARABOLA_HXX
#define MCCADEDGEPARABOLA_HXX

#include "McCadEdge.hxx"

class McCadEdgeParabola : public McCadEdge
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

    McCadEdgeParabola();
    McCadEdgeParabola(const TopoDS_Edge &theEdge);
    ~McCadEdgeParabola();

};

#endif // MCCADEDGEPARABOLA_HXX
