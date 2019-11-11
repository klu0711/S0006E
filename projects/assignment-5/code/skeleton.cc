#include "skeleton.h"
using namespace CoreGraphics;
skeleton::skeleton()
{

}
skeleton::~skeleton()
{
    delete joints;
}

void skeleton::loadMesh(char *fileName)
{
    std::ifstream file;
    file.open(fileName, std::ifstream::in | std::ifstream::binary);
    file.seekg(0, file.end);
    unsigned int length = file.tellg();
    file.seekg(0, file.beg);
    char* ptr = new char[length];
    file.read(ptr, length);
    file.close();

    struct Nvx2Header* h = (struct Nvx2Header*) ptr;
    header = h;
    header->numIndices *= 3;
    ptr += sizeof(Nvx2Header);


    this->numGroups = h->numGroups;
    this->numVertices = h->numVertices;
    this->vertexWidth = h->vertexWidth;
    this->numIndices = h->numIndices;
    this->numEdges = h->numEdges;
    this->vertexComponentMask = h->vertexComponentMask;
    this->groupDataSize = 6 * sizeof(uint) * this->numGroups;
    this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(GLfloat);
    this->indexDataSize = header->numIndices * sizeof(int);

    this->vertexDataPtr = ((uchar*)ptr) + this->groupDataSize;
    this->indexDataPtr = (((uchar*)this->vertexDataPtr) + this->vertexDataSize);
    

    Nvx2Group * g = (Nvx2Group*) ptr;
    for (int i = 0; i < (size_t)this->numGroups; ++i)
    {
        PrimitiveGroup p;
        p.numVerticies = g->numVertices;
        p.baseIndex = g->firstTriangle * 3;
        p.numIndicies = g->numTriangles * 3;
        p.primitiveTopology = 4;
        primGroups.push_back(p);

        g++;
    }

    size_t i;
    for (int i = 0; i < N2NumVertexComponents; ++i)
    {
        CoreGraphics::SemanticName sem;
        CoreGraphics::Format fmt;
        size_t index = 0;

        if(this->vertexComponentMask & (1<<i))
        {
            switch(1<<i)
            {
                case N2Coord: sem = CoreGraphics::SemanticName::Position; fmt = CoreGraphics::Format::Float3; break;
                case N2Normal: sem = CoreGraphics::SemanticName::Normal; fmt = CoreGraphics::Format::Float3; break;
                case N2NormalB4N: sem = CoreGraphics::SemanticName::Normal; fmt = CoreGraphics::Format::Byte4N; break;
                case N2Uv0: sem = CoreGraphics::SemanticName::TexCoord1; fmt = CoreGraphics::Format::Float2; index = 0; break;
                case N2Uv0S2: sem = CoreGraphics::SemanticName::TexCoord1; fmt = CoreGraphics::Format::Short2; index = 0; break;
                case N2Uv1: sem = CoreGraphics::SemanticName::TexCoord2; fmt = CoreGraphics::Format::Float2; index = 1; break;
                case N2Uv1S2: sem = CoreGraphics::SemanticName::TexCoord2; fmt = CoreGraphics::Format::Short2; index = 1; break;

                case N2Color: sem = CoreGraphics::SemanticName::Color; fmt = CoreGraphics::Format::Float4; break;
                case N2ColorUB4N: sem = CoreGraphics::SemanticName::Color; fmt = CoreGraphics::Format::UByte4N; break;
                case N2Tangent: sem = CoreGraphics::SemanticName::Tangent; fmt = CoreGraphics::Format::Float3; break;
                case N2TangentB4N: sem = CoreGraphics::SemanticName::Tangent; fmt = CoreGraphics::Format::Byte4N; break;
                case N2Binormal: sem = CoreGraphics::SemanticName::Binormal; fmt = CoreGraphics::Format::Float3; break;
                case N2BinormalB4N: sem = CoreGraphics::SemanticName::Binormal; fmt = CoreGraphics::Format::Byte4N; break;
                case N2Weights: sem = CoreGraphics::SemanticName::SkinWeights; fmt = CoreGraphics::Format::Float4; break;
                case N2WeightsUB4N: sem = CoreGraphics::SemanticName::SkinWeights; fmt = CoreGraphics::Format::UByte4N; break;
                case N2JIndices: sem = CoreGraphics::SemanticName::SkinJIndices; fmt = CoreGraphics::Format::Float4; break;
                case N2JIndicesUB4: sem = CoreGraphics::SemanticName::SkinJIndices; fmt = CoreGraphics::Format::UByte4; break;
                default:
                    sem = CoreGraphics::SemanticName::Position;
                    fmt = CoreGraphics::Format::Float3;
                    break;
            }
            vertexComponent vertexC;
            vertexC.name = sem;
            vertexC.index = index;
            vertexC.format = fmt;

            this->vertexComponents.push_back(vertexC);
        }
    }
   // std::cout << this->vertexComponents.size() << std::endl;
}

void skeleton::moveJoint(Matrix4 transform, int joint)
{
    joints->at(joint).localTransform = joints->at(joint).localTransform * transform;
    updateJoints(joint);

}

void skeleton::updateJoints(int joint)
{
    if(joints->at(joint).parent == -1)
    {
        joints->at(joint).transform = joints->at(joint).localTransform;
    }else
    {
        joints->at(joint).transform = joints->at(joints->at(joint).parent).transform * joints->at(joint).localTransform;
    }

    for (int i = 0; i < joints->at(joint).children.size(); ++i)
    {
        updateJoints(joints->at(joint).children.at(i));
    }
}

void skeleton::worldSpaceConversion()
{
    for (int i = 0; i < joints->size(); ++i) {
        if(joints->at(i).parent != -1)
        {
            //joint* j = &joints->at(i);
            joints->at(i).transform = joints->at(joints->at(i).parent).transform * joints->at(i).localTransform;
        }

    }
    for (int j = 0; j < joints->size(); ++j) {
        joints->at(j).inverseBindPose = Matrix4::inverse(joints->at(j).transform);
    }
}



void skeleton::loadSkeleton(char *fileName)
{
    TiXmlDocument doc(fileName);
    bool loadOkay = doc.LoadFile();
    if(!loadOkay)
    {
        std::cout << "XML load failed" << std::endl;
    }else
    {
        TiXmlElement *p_joint;
        TiXmlElement *p_skinJoints;

        p_joint = doc.FirstChildElement("Nebula3")->FirstChildElement("Model")->FirstChildElement("CharacterNodes")->FirstChildElement("CharacterNode")->FirstChildElement("Joint");
        while(p_joint)
        {
            joint j;
            j.parent = atoi(p_joint->Attribute("parent"));
            j.index = atoi(p_joint->Attribute("index"));
            if(j.parent != -1)
            {
                joints->at(j.parent).children.push_back(j.index);
            }

            float values[4];
            sscanf(p_joint->Attribute("position"), "%f,%f,%f,%f", &values[0], &values[1], &values[2], &values[3]);
            vec4 position(values);
            Matrix4 p = Matrix4::getPositionMatrix(position);
            sscanf(p_joint->Attribute("rotation"), "%f,%f,%f,%f", &values[0], &values[1], &values[2], &values[3]);
            vec4 rotation(values);
            Matrix4 r = Matrix4::getQmat(rotation);
            sscanf(p_joint->Attribute("scale"), "%f,%f,%f,%f", &values[0], &values[1], &values[2], &values[3]);
            vec4 scale(values);
            Matrix4 s = Matrix4::scaleMat(scale);

            j.transform = p*r*s;
            j.localTransform = j.transform;
            joints->push_back(j);
            p_joint = p_joint->NextSiblingElement("Joint");

        }
        worldSpaceConversion();

        p_skinJoints = doc.FirstChildElement("Nebula3")->FirstChildElement("Model")->FirstChildElement("Skins")->FirstChildElement("Skin")->FirstChildElement("Fragment")->FirstChildElement("Joints");

        char* a = (char*)p_skinJoints->GetText();
        skinJoints.reserve(21);
         sscanf(a, "%i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i", &skinJoints[0], &skinJoints[1], &skinJoints[2], &skinJoints[3], &skinJoints[4], &skinJoints[5], &skinJoints[6], &skinJoints[7], &skinJoints[8], &skinJoints[9], &skinJoints[10], &skinJoints[11], &skinJoints[12],&skinJoints[13], &skinJoints[14], &skinJoints[15], &skinJoints[16],&skinJoints[17], &skinJoints[18], &skinJoints[19], &skinJoints[20]);
    }
}

int skeleton::convertJointIndex(uint index)
{
    return skinJoints[index];
}