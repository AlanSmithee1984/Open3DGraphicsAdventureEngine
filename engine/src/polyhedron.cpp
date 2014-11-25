#include "polyhedron.h"

#include <CGAL/IO/Polyhedron_iostream.h>

typedef CGALPolyhedron::Halfedge_handle         Halfedge_handle;
typedef CGALPolyhedron::Vertex_handle           Vertex_handle;


Polyhedron::Polyhedron()
    : m_cgalPolyhedron(new CGALPolyhedron)
{
}

Polyhedron::~Polyhedron()
{
    delete m_cgalPolyhedron;
    m_cgalPolyhedron;
}

void Polyhedron::addVertex(const Ogre::Vector3 &position)
{
    m_modifier.addVertex(position);
}

void Polyhedron::addFace(const FaceIndices &indices)
{
    m_modifier.addFace(indices);
}

void Polyhedron::create()
{
    m_cgalPolyhedron->delegate(m_modifier);

    std::cout << *m_cgalPolyhedron << std::endl;

    this->calcVertexInfos();

}

void Polyhedron::calcVertexInfos()
{

    typedef CGALPolyhedron::Face_iterator Vertex_iterator;
    Vertex_iterator faceIt = m_cgalPolyhedron->facets_begin();

    while(faceIt != m_cgalPolyhedron->facets_end())
    {
        typedef CGALPolyhedron::Facet Facet;
        Facet& face = *faceIt;

        typedef CGALPolyhedron::Halfedge_handle HalfEdgeHandle;
        HalfEdgeHandle currEdge = face.halfedge();
        const HalfEdgeHandle startEdge = currEdge;

        bool firstTime = true;

        while(currEdge != startEdge || firstTime == true)
        {
            firstTime = false;

            HalfEdgeHandle prevEdge = currEdge->prev();
            HalfEdgeHandle nextEdge = currEdge->next();

            typedef CGALPolyhedron::Vertex_handle VertexHandle;

            VertexHandle prevVertHandle = prevEdge->vertex();
            VertexHandle currVertHandle = currEdge->vertex();
            VertexHandle nextVertHandle = nextEdge->vertex();

            const Point& prevPoint = prevVertHandle->point();
            const Point& currPoint = currVertHandle->point();
            const Point& nextPoint = nextVertHandle->point();
            std::cout << prevPoint << "\t" << currPoint << "\t" << nextPoint << std::endl;

            FaceToVertexInfoMap& infoMap = currVertHandle->vertexInfo;


            CGALPolyhedron::Facet_handle faceHandle = currVertHandle->facet();
            VertexInformation &info = infoMap[faceHandle];

            currEdge = nextEdge;
        }










        ++faceIt;
    }
}

