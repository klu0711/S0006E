
#include "skeleton.h"

skeleton::skeleton()
{

}
skeleton::~skeleton()
{
    delete joints;
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
        TiXmlElement *Pjoint;

        Pjoint = doc.FirstChildElement("Nebula3")->FirstChildElement("Model")->FirstChildElement("CharacterNodes")->FirstChildElement("CharacterNode")->FirstChildElement("Joint");
        while(Pjoint)
        {
            joint j;
            j.parent = atoi(Pjoint->Attribute("parent"));
            j.index = atoi(Pjoint->Attribute("index"));
            if(j.parent != -1)
            {
                joints->at(j.parent).children.push_back(j.index);
            }
            // TODO: Add the position scale and rotation to the joint struct
            // position * rotation * scale
            float values[4];
            sscanf(Pjoint->Attribute("position"), "%f,%f,%f,%f", &values[0], &values[1], &values[2], &values[3]);
            Vector4D position(values);
            Matrix4 p = Matrix4::getPositionMatrix(position);
            sscanf(Pjoint->Attribute("rotation"), "%f,%f,%f,%f", &values[0], &values[1], &values[2], &values[3]);
            Vector4D rotation(values);
            Matrix4 r = Matrix4::getQmat(rotation);
            sscanf(Pjoint->Attribute("scale"), "%f,%f,%f,%f", &values[0], &values[1], &values[2], &values[3]);
            Vector4D scale(values);
            Matrix4 s = Matrix4::scaleMat(scale);

            j.transform = p*r*s;
            j.localTransform = j.transform;
            joints->push_back(j);
            Pjoint = Pjoint->NextSiblingElement("Joint");

        }
        worldSpaceConversion();
        std::cout << "fin" << std::endl;
    }






}