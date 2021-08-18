#include "McCadEdgeParabola.hxx"

McCadEdgeParabola::McCadEdgeParabola()
{
}


McCadEdgeParabola::~McCadEdgeParabola()
{
}


McCadEdgeParabola::McCadEdgeParabola(const TopoDS_Edge &theEdge):McCadEdge(theEdge)
{
    m_CurveType = Spline;
}
