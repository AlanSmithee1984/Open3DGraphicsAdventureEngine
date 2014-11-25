#include "meshtriangleconverter.h"

#include <OgreSubMesh.h>

#include <QDebug>

#include "polyhedron.h"

MeshTriangleConverter::MeshTriangleConverter()
{
}


Polyhedron* MeshTriangleConverter::convert(const Ogre::MeshPtr &mesh)
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



    Polyhedron* poly = new Polyhedron;

    for(quint32 entry = 0; entry < vertBuf->getNumVertices() ; ++entry)
    {
        const quint32 vertPos = vertexSizeInFloats * entry;

        Ogre::Vector3 pos(vertexBuffer[vertPos],
                          vertexBuffer[vertPos + 1],
                vertexBuffer[vertPos + 2]
                );

        poly->addVertex(pos);
    }


    FaceIndices indexStack;

    for(quint32 index = indexData->indexStart ; index < indexData->indexCount ; ++index)
    {
        const quint16 indexBufferEntry = indexBuffer[index];

        indexStack << indexBufferEntry;

        if(indexStack.size() == 3)
        {
            //            std::cout << poly[0]
            //                    << "\t" << poly[1]
            //                    << "\t" << poly[2]
            //            << std::endl;

            Q_ASSERT(indexStack[0] != indexStack[1]);
            Q_ASSERT(indexStack[0] != indexStack[2]);
            Q_ASSERT(indexStack[1] != indexStack[2]);

            poly->addFace(indexStack);
            indexStack.clear();
        }

    }


    poly->create();


    indBuf->unlock();
    vertBuf->unlock();


    return poly;

}

