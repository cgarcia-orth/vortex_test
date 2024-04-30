/**************************************************************************************************
* Module Name    : IJsonMsgListener.h
*
* Description    : Interface to implement to listen to a Json Msg interface.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-10-02  Olivier Battion : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_IJSONMSGLISTENER_H
#define VORTEXCORE_IJSONMSGLISTENER_H

#include <nlohmann/json.hpp>

namespace vortex {

    class IJsonMsgListener {
    public:
        virtual ~IJsonMsgListener() = default;

        virtual std::shared_ptr<nlohmann::json> onNewCmdMsg(int srcPort, std::string method, std::shared_ptr<nlohmann::json> params) = 0;
        virtual void onNewDataMsg(int srcPort, std::string type, float timestamp, std::shared_ptr<nlohmann::json> value) = 0;
    };

}

#endif //VORTEXCORE_IJSONMSGLISTENER_H
