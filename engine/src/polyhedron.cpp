#include "polyhedron.h"

#include <CGAL/IO/Polyhedron_iostream.h>

#include <OgreRay.h>

typedef CGALPolyhedron::Halfedge_handle         Halfedge_handle;
typedef CGALPolyhedron::Vertex_handle           Vertex_handle;


Polyhedron::Polyhedron()
    : m_cgalPolyhedron(new CGALPolyhedron),
      m_modifier(new PolyhedronModifier<HalfedgeDS>)
{
}

Polyhedron::~Polyhedron()
{
    delete m_cgalPolyhedron;
    m_cgalPolyhedron;
}

void Polyhedron::addVertex(const Ogre::Vector3 &position)
{
    m_modifier->addVertex(position);
}

void Polyhedron::addFace(const FaceIndices &indices)
{
    m_modifier->addFace(indices);
}


struct Plane_equation
{
    template <class Facet>
    typename Facet::Plane_3 operator()( Facet& f)
    {
        typename Facet::Halfedge_handle h = f.halfedge();
        typedef typename Facet::Plane_3 Plane;

        return Plane( h->vertex()->point(),
                      h->next()->vertex()->point(),
                      h->next()->next()->vertex()->point());
    }
};


void Polyhedron::create()
{
    m_cgalPolyhedron->delegate(*m_modifier);

    std::transform( m_cgalPolyhedron->facets_begin(), m_cgalPolyhedron->facets_end(),
                    m_cgalPolyhedron->planes_begin(), Plane_equation());

    std::cout << *m_cgalPolyhedron << std::endl;

    this->calcLocalVertexInfos();

    Q_ASSERT(m_cgalPolyhedron->is_closed());
    Q_ASSERT(m_cgalPolyhedron->is_valid());

    delete m_modifier;
    m_modifier = NULL;

}

quint32 Polyhedron::getNumberFaces() const
{
    return m_cgalPolyhedron->size_of_facets();
}

quint32 Polyhedron::getNumberHalfEdges() const
{
    return m_cgalPolyhedron->size_of_halfedges();
}

quint32 Polyhedron::getNumberVertices() const
{
    return m_cgalPolyhedron->size_of_vertices();
}

VertexInformations Polyhedron::getLocalVertexInformations() const
{
    VertexInformations infos;

    typedef CGALPolyhedron::Vertex_iterator Vertex_iterator;
    Vertex_iterator vertIt = m_cgalPolyhedron->vertices_begin();

    while(vertIt != m_cgalPolyhedron->vertices_end())
    {
        FaceToVertexInfoMap& infoMap = vertIt->vertexInfo;
        foreach(VertexInformation* info, infoMap)
        {
            infos.insert(info);
        }

        ++vertIt;
    }

    return infos;
}

void Polyhedron::clipAtPlane(const Ogre::Plane &clippingPlane, VertexInformations &outputVertexInfos)
{
    outputVertexInfos = this->getLocalVertexInformations();

    typedef CGALPolyhedron::Edge_iterator Edge_iterator;
    Edge_iterator edgeIt = m_cgalPolyhedron->edges_begin();

    while(edgeIt != m_cgalPolyhedron->edges_end())
    {
        typedef CGALPolyhedron::Vertex_handle VertexHandle;

        VertexHandle startVertHandle = edgeIt->prev()->vertex();
        VertexHandle endVertHandle = edgeIt->vertex();

        const Ogre::Vector3 start = this->CGALtoOgreConvert(startVertHandle->point());
        const Ogre::Vector3 end = this->CGALtoOgreConvert(endVertHandle->point());

        const Ogre::Vector3 rayCoPlanar = end - start;
        const Ogre::Vector3 rayDir = rayCoPlanar.normalisedCopy();

        std::cout << start << "\t" << end << "\t" << rayDir << std::endl;

        Ogre::Ray ray(start, rayDir);

        const std::pair<bool, Ogre::Real> result = ray.intersects(clippingPlane);

        if(result.first == true)
        {
            if(result.second <= rayCoPlanar.length())
            {
                // split edge
                const Ogre::Vector3 intersectionPoint = ray.getPoint(result.second);

                typedef CGALPolyhedron::Facet_handle FacetHandle;


                FacetHandle face = edgeIt->facet();
                Plane plane = face->plane();
                Q_ASSERT(plane.is_degenerate() == false);

                Ogre::Plane pl(plane.a(), plane.b(), plane.c(), plane.d());
                const Ogre::Vector3 &currPlaneNormal = pl.normal;
                const Ogre::Vector3 &clippingPlaneNormal = clippingPlane.normal;

                Ogre::Vector3 dirCurrPlane = currPlaneNormal.crossProduct(clippingPlaneNormal);
                Q_ASSERT(dirCurrPlane.isZeroLength() == false);
                dirCurrPlane.normalise();
                Q_ASSERT(dirCurrPlane.isZeroLength() == false);

                FacetHandle faceOpp = edgeIt->opposite()->facet();
                Plane planeOpp = faceOpp->plane();
                Q_ASSERT(planeOpp.is_degenerate() == false);



                Ogre::Plane plOpp(planeOpp.a(), planeOpp.b(), planeOpp.c(), planeOpp.d());
                const Ogre::Vector3 &oppositePlaneNormal = plOpp.normal;

                Ogre::Vector3 dirOppPlane = oppositePlaneNormal.crossProduct(clippingPlaneNormal);
                Q_ASSERT(dirOppPlane.isZeroLength() == false);
                dirOppPlane.normalise();
                Q_ASSERT(dirOppPlane.isZeroLength() == false);



                Ogre::Vector3 dirEdge = rayDir;
                if(dirEdge.dotProduct(clippingPlaneNormal) > 0)
                {
                    dirEdge = -dirEdge;
                    dirOppPlane = -dirOppPlane;
                    std::swap(dirCurrPlane, dirOppPlane);
                }
                else
                {
                    dirCurrPlane = -dirCurrPlane;
                }

                Q_ASSERT(dirEdge.isZeroLength() == false);
                Q_ASSERT((dirEdge.squaredLength() - 1.0) < std::numeric_limits<Ogre::Real>::epsilon());


                // clipping plane coplanar
                // assure that cross product points in same dir as clipping plane
//                Q_ASSERT(dirCurrPlane.crossProduct(dirOppPlane).dotProduct(clippingPlaneNormal) >= 0);
                outputVertexInfos.insert(new VertexInformation(intersectionPoint, dirCurrPlane, dirOppPlane));

                // curr face
                // assure that cross product points in same dir as curr face
//                Q_ASSERT(dirEdge.crossProduct(currPlaneNormal).dotProduct(dirCurrPlane) >= 0);
                outputVertexInfos.insert(new VertexInformation(intersectionPoint, dirEdge, dirCurrPlane));

                // opp face
                // assure that cross product points in same dir as oppoisite face
//                Q_ASSERT(dirOppPlane.crossProduct(dirEdge).dotProduct(oppositePlaneNormal) >= 0);
                outputVertexInfos.insert(new VertexInformation(intersectionPoint, dirOppPlane, dirEdge));

            }
        }

        ++edgeIt;
    }


    this->removeInvalidVertexInfos(clippingPlane, outputVertexInfos);
}

void Polyhedron::calcLocalVertexInfos()
{

    typedef CGALPolyhedron::Face_iterator Face_iterator;
    Face_iterator faceIt = m_cgalPolyhedron->facets_begin();

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
            //            std::cout << prevPoint << "\t" << currPoint << "\t" << nextPoint << std::endl;



            const Ogre::Vector3 pos = this->CGALtoOgreConvert(currPoint);
            const Ogre::Vector3 next = this->CGALtoOgreConvert(nextPoint);
            const Ogre::Vector3 prev = this->CGALtoOgreConvert(prevPoint);

            Ogre::Vector3 edgeP = next - pos;
            edgeP.normalise();
            Q_ASSERT(edgeP.isZeroLength() == false);

            Ogre::Vector3 edgeQ = prev - pos;
            edgeQ.normalise();
            Q_ASSERT(edgeQ.isZeroLength() == false);

            Q_ASSERT(edgeP.crossProduct(edgeQ).length() > 0);

            FaceToVertexInfoMap& infoMap = currVertHandle->vertexInfo;

            void* faceVoid = static_cast<void*>(&face);
            Q_ASSERT(infoMap.contains(faceVoid) == false);
            infoMap[faceVoid] = new VertexInformation(pos, edgeP, edgeQ);


            currEdge = nextEdge;
        }

        ++faceIt;
    }

}

void Polyhedron::removeInvalidVertexInfos(const Ogre::Plane &clippingPlane, VertexInformations &vertexInfos)
{

    foreach(VertexInformation* info, vertexInfos)
    {
        if(clippingPlane.getSide(info->getPosition()) == Ogre::Plane::POSITIVE_SIDE)
        {
            vertexInfos.remove(info);
            continue;
        }

        typedef CGALPolyhedron::Face_iterator Face_iterator;

        for(Face_iterator faceIt = m_cgalPolyhedron->facets_begin();
            faceIt != m_cgalPolyhedron->facets_end(); ++faceIt)
        {

            const Plane &plane = faceIt->plane();
            const Ogre::Plane pl(plane.a(), plane.b(), plane.c(), plane.d());

            if(pl.getSide(info->getPosition()) == Ogre::Plane::POSITIVE_SIDE)
            {
                vertexInfos.remove(info);
                break;
            }

        }

    }
}

Ogre::Vector3 Polyhedron::CGALtoOgreConvert(const Point &cgalPoint)
{
    const Ogre::Vector3 pos(cgalPoint.x(),
                            cgalPoint.y(),
                            cgalPoint.z());

    return pos;
}

