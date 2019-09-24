
#include "skeleton.h"


void skeleton::loadSkeleton(char *fileName)
{
    TiXmlDocument doc(fileName);
    bool loadOkay = doc.LoadFile();
    if(!loadOkay)
    {
        std::cout << "XML load failed" << std::endl;
    }else
    {
        TiXmlElement *root, *model, *characterNode, *characterNodes, *skinList, *Pjoint;

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
            Matrix4 r = Matrix4::getQmat(position);
            sscanf(Pjoint->Attribute("scale"), "%f,%f,%f,%f", &values[0], &values[1], &values[2], &values[3]);
            Vector4D scale(values);
            Matrix4 s = Matrix4::scaleMat(position);

            j.transform = p*r*s;

            joints->push_back(j);
            Pjoint = Pjoint->NextSiblingElement("Joint");

        }
        std::cout << "fin" << std::endl;
    }






}