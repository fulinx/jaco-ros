

#include "vrep_jaco_sim/JacoVREPInterface.h"
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>

JacoVREPInterface::JacoVREPInterface() : nh("~") {
    for (int i = 0; i < 6; i++) {
        char topic_name[1024];
        sprintf(topic_name,"/vrep/J%dEncoder",i+1);
        jointSub[i] = nh.subscribe(topic_name,1,&VelPosData::cb,vp+i);
        sprintf(topic_name,"/vrep/J%dPositionCommand",i+1);
        jointPosPub[i] = nh.advertise<std_msgs::Float64>(topic_name,1);
        sprintf(topic_name,"/vrep/J%dVelocityCommand",i+1);
        jointVelPub[i] = nh.advertise<std_msgs::Float64>(topic_name,1);
    }
    controlPub = nh.advertise<std_msgs::Int32>("/vrep/armControlMode",1);
    positionControl = false;
    setVelocityControl();
    vp[0].offset = 180.0;
    vp[1].offset = 180.0;
    vp[2].offset = 71.0;
    vp[3].offset = 0.0;
    vp[4].offset = 0.0;
    vp[5].offset = 90.0;
    
}



bool JacoVREPInterface::setPositionControl() {
    std_msgs::Int32 b;
    positionControl = b.data = 1;
    controlPub.publish(b);
    return true;
}

bool JacoVREPInterface::setVelocityControl(){
    std_msgs::Int32 b;
    positionControl = b.data = 0;
    controlPub.publish(b);
    return true;
}

bool JacoVREPInterface::publishControl(float a1, float a2, float a3,
        float a4, float a5, float a6) {
    float control[6] = {a1,a2,a3,a4,a5,a6};
    if (positionControl) {
        for (int i = 0; i < 6; i++) {
            std_msgs::Float64 f; f.data = control[i];
            jointPosPub[i].publish(f);
        }
    } else {
        for (int i = 0; i < 6; i++) {
            std_msgs::Float64 f; f.data = control[i];
            jointVelPub[i].publish(f);
        }
    }
}


