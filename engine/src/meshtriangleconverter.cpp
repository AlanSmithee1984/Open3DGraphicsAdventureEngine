#include "meshtriangleconverter.h"

#include <OgreSubMesh.h>

#include <QDebug>

MeshTriangleConverter::MeshTriangleConverter()
{
}


Polygons MeshTriangleConverter::convert(const Ogre::MeshPtr &mesh)
{
    Ogre::SubMesh* subMesh = mesh->getSubMesh(0);

    Ogre::IndexData* indexData = subMesh->indexData;
    Ogre::HardwareIndexBufferSharedPtr indBuf = indexData->indexBuffer;
    quint16* indexBuffer = static_cast<quint16*>(indBuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

    Ogre::VertexData* vertexData = subMesh->vertexData;
    Ogre::HardwareVertexBufferSharedPtr vertBuf = vertexData->vertexBufferBinding->getBuffer(0);
    Ogre::Real* vertexBuffer = static_cast<Ogre::Real*>(vertBuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

    const quint32 vertexSizeInBytes = vertexData->vertexDeclaration->getVertexSize(0);

    Q_ASSERT(vertexSizeInBytes % sizeof(Ogre::Real) == 0);
    const quint32 vertexSizeInFloats = vertexSizeInBytes / sizeof(Ogre::Real);

//    const quint32 max = vertexData->vertexCount * vertexSizeInFloats;

//    qDebug() << vertexData->vertexCount << vertexSizeInBytes << max;

//    for(quint32 i = 0 ; i < max; ++i)
//    {
//        qDebug() << i << vertexBuffer[i];
//    }


//    for(quint32 i = 0 ; i < max; i += vertexSizeInFloats )
//    {
//        qDebug() << i << vertexBuffer[i] << vertexBuffer[i + 1] << vertexBuffer[i + 2];
//    }

    Polygons polys;
    Polygon poly;


    for(quint32 index = indexData->indexStart ; index < indexData->indexCount ; ++index)
    {
        const quint16 indexBufferEntry = indexBuffer[index];
        // Q_ASSERT(indexBufferEntry + 2 < vertexData->vertexCount);

        const quint32 vertPos = vertexSizeInFloats* indexBufferEntry;


//        std::cout << index << "\t" << indexBufferEntry
//                  << "\t" << vertPos
//                  << "\t" << vertexBuffer[vertPos]
//                    << "\t" << vertexBuffer[vertPos + 1]
//                    << "\t" << vertexBuffer[vertPos + 2]
//                    << std::endl;

        poly.push_back(Ogre::Vector3(vertexBuffer[vertPos],
                                     vertexBuffer[vertPos + 1],
                       vertexBuffer[vertPos + 2])
                );

        if(poly.size() == 3)
        {
//            std::cout << poly[0]
//                    << "\t" << poly[1]
//                    << "\t" << poly[2]
//            << std::endl;

            Q_ASSERT(poly[0] != poly[1]);
            Q_ASSERT(poly[0] != poly[2]);
            Q_ASSERT(poly[1] != poly[2]);

            polys << poly;
            poly.clear();
        }

    }


    indBuf->unlock();
    vertBuf->unlock();


    return polys;

}

