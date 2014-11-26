#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <QList>
#include <QDebug>


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include "vertexinformation.h"

typedef CGAL::Simple_cartesian<double>        Kernel;

typedef typename Kernel::Point_3 Point;

class Facet;
typedef QHash<void*, VertexInformation> FaceToVertexInfoMap;

template <class Refs>
struct CustomVertex : public CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point>
{
    typedef  CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point> Base;

    CustomVertex()
    {

    }


    CustomVertex(const Point &p)
        : Base(p)
    {
//        qDebug() << "ctor:" << this;
    }

    //    CustomVertex(const CustomVertex &other)
    //        : Base(other),
    //          a(other.a)
    //    {
    //        qDebug() << "copy:" << this;
    //    }


    FaceToVertexInfoMap vertexInfo;
};

struct PolyhedronItems : public CGAL::Polyhedron_items_3
{
    template <class Refs, class Traits>
    struct Vertex_wrapper
    {
        typedef CustomVertex<Refs> Vertex;
    };
};

typedef CGAL::Polyhedron_3<Kernel, PolyhedronItems>    CGALPolyhedron;
typedef CGALPolyhedron::HalfedgeDS              HalfedgeDS;


typedef QList<quint32> FaceIndices;

template <class HDS>
class PolyhedronModifier : public CGAL::Modifier_base<HDS>
{
public:

    PolyhedronModifier()
    {
    }

    void addVertex(const Ogre::Vector3 &position)
    {
        m_positions.append(position);
    }

    void addFace(const FaceIndices &indices)
    {
        m_faces.append(indices);
    }


    void operator()( HDS& hds)
    {
        // Postcondition: hds is a valid polyhedral surface.
        typedef CGAL::Polyhedron_incremental_builder_3<HDS> Builder;
        Builder builder( hds, true);

        builder.begin_surface(m_positions.size(), m_faces.size());
        Q_ASSERT(builder.error() == false);

        QListIterator<Ogre::Vector3> vertIt(m_positions);
        while(vertIt.hasNext())
        {
            const Ogre::Vector3 &pos = vertIt.next();

            typedef typename HDS::Vertex   Vertex;
            typedef typename Vertex::Point Point;

            Point p( pos.x, pos.y, pos.z);

            builder.add_vertex( p );
            Q_ASSERT(builder.error() == false);
        }

        QListIterator<FaceIndices> faceIt(m_faces);
        while(faceIt.hasNext())
        {
            const FaceIndices& indices = faceIt.next();

            Q_ASSERT(indices.isEmpty() == false);

            typename Builder::Facet_handle faceHnd = builder.begin_facet();
            Q_ASSERT(builder.error() == false);


            QListIterator<quint32> indexIt(indices);
            while(indexIt.hasNext())
            {
                quint32 index = indexIt.next();
                builder.add_vertex_to_facet( index );
                Q_ASSERT(builder.error() == false);
            }

            builder.end_facet();
            Q_ASSERT(builder.error() == false);
        }


        builder.end_surface();
        Q_ASSERT(builder.error() == false);

    }

private:
    QList<Ogre::Vector3> m_positions;
    QList<FaceIndices> m_faces;
};



class Polyhedron
{
public:
    Polyhedron();
    ~Polyhedron();

    void addVertex(const Ogre::Vector3 &position);
    void addFace(const FaceIndices &indices);

    void create();


private:    
    void calcVertexInfos();

    CGALPolyhedron* m_cgalPolyhedron;
    PolyhedronModifier<HalfedgeDS> m_modifier;
};

#endif // POLYHEDRON_H
