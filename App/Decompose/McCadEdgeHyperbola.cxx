#include "McCadEdgeHyperbola.hxx"

McCadEdgeHyperbola::McCadEdgeHyperbola()
{
}

McCadEdgeHyperbola::~McCadEdgeHyperbola()
{
}


McCadEdgeHyperbola::McCadEdgeHyperbola(const TopoDS_Edge &theEdge):McCadEdge(theEdge)
{
    m_CurveType = Spline;
}
