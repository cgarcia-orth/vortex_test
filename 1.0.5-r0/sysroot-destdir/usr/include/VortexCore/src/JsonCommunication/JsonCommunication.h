/**************************************************************************************************
* Module Name    : SystemdUtilsInterface.h
*
* Description    : Interface definition to use JSON Communication
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-09-30  Olivier Battion : Initial implementation.
*
**************************************************************************************************/

#ifndef VORTEXCORE_JSONCOMMUNICATION_H
#define VORTEXCORE_JSONCOMMUNICATION_H

#include <pthread.h>
#include <list>
#include <queue>

#include <nlohmann/json.hpp>

#include <VortexCore/includes/LoggerFactory>
#include <VortexCore/includes/TimeProviderFactory>
#include <VortexCore/includes/Mutex>
#include <VortexCore/includes/Event>

#include "IJsonMsgListener.h"

using spjson = std::shared_ptr<nlohmann::json>;

namespace vortex {

enum class eJsonComPort : int32_t {
    COM_MGR_CMD_PORT = 44200,
    COM_MGR_DATA_PORT = 44300,
    GUI_CMD_PORT = 44400,
    GUI_DATA_PORT = 44500,
};

enum class eJsonComErr : int32_t {
    JSON_COM_NO_ERR,
    JSON_COM_INVALID_PARAM,
    JSON_COM_TX_ERR_SEND,
    JSON_COM_TX_ERR_CONNECTION,
    JSON_COM_RX_NO_DATA,
    JSON_COM_RX_TIMEOUT,
    JSON_COM_RX_INVALID_HEADER,
    JSON_COM_RX_HEADER_FORMAT,
    JSON_COM_RX_INVALID_JSON,
    JSON_COM_RX_MISMATCH_ID,
};

/// Msg format for the TX message queue
class JsonMsg {
public:
    spjson aMsgPtr;
    eJsonComPort aPortDest{};
    std::string aTypeMethod;
    int aTimeoutMs{};
    bool aIsCommand{};
    std::string aMsgId{};
    std::function<void(void *, eJsonComErr, std::string, spjson)> aCallbackPtr;
    void *aCallerPtr{};
};

class JsonCommunication {
public:
    JsonCommunication();

    explicit JsonCommunication(std::string loggerRoot);

    virtual ~JsonCommunication();

    void listenOnChannel(eJsonComPort portNum);

    /*!
      \brief send JSON data message to selected TCP port
             automatically fill the timestamp in the message
      \param portNum numero of the port to send data to.
      \param type is inserted in JSON first line "data": [type],
      \param shared pointer on JSON message will be added under "value" object
      \param callback provide the transmission status - message ID, json response received (nullptr if no response)
                    optional - can be set to nullptr
      \param caller  object that owns the callback
                    optional - can be set to nullptr
      \param timeoutMs Optional - millisecs to wait for response
      \return message UUID
    */
    std::string sendData(eJsonComPort portNum, const std::string& type, const spjson& msg,
                 std::function<void(void *, eJsonComErr, std::string, spjson)> callback, void *caller, int timeoutMs = 250);

    /*!
      \brief send JSON command message to selected TCP port
             jsonrpc method and id are automatically filled
             a random int is created for id field
      \param portNum numero of the port to send command to.
      \param method is inserted in JSON "method": "[method]"
      \param shared pointer on  JSON message will be added under "params" object
      \param callback provide the transmission status - message ID, json response received (nullptr if no response)
                    optional - can be set to nullptr
      \param caller  object that owns the callback
                    optional - can be set to nullptr
      \param timeoutMs Optional - millisecs to wait for response
      \return message UUID
    */
    std::string sendCmd(eJsonComPort portNum, const std::string& method, const spjson& msg,
                std::function<void(void *, eJsonComErr, std::string, spjson)> callback, void *caller, int timeoutMs = 250);

    /*!
    \brief start both tx and rx threads
     At least on port must be listened before calling this function
    */
    void start(bool bStartTx = true, bool bStartRx = true);
    // for both
    /*!
    \brief stop both tx and rx threads
    */
    void stop();

    void setListener(IJsonMsgListener *listener);

    void setVerbose(bool verbose);

    // utilities
    static spjson createJsonResponseErrorCode(int errCode);

private :
    std::shared_ptr<spdlog::logger> aLoggerPtr;
    bool aVerbose{};

    std::unique_ptr<std::thread> aListenThreadPtr;
    std::unique_ptr<std::thread> aSendMsgThreadPtr;
    std::vector<int> aPortsToListen;
    IJsonMsgListener *aListenerPtr;
    std::map<int, int> aClientSockets;
    Event aEventListenStopRequest;
    Event aEventListenThreadReady;
    std::shared_ptr<ITimeProvider> aTimeProviderPtr;

    int getClient(eJsonComPort portNum);

    int connectClient(eJsonComPort portNum);

    static spjson createJsonResponse(const spjson& pJsonReq, const spjson& pJsonErr);

    void listenThreadProcess();

    void sendMsgThreadProcess();

    void sendMsg(eJsonComPort portNum, const std::string &msg, const std::string &msgId, bool checkResponse, int timeoutMs,
                 const std::function<void(void *, eJsonComErr, std::string, spjson)>& callbackRx,
                 void *caller);

    int socketSendJsonMsg(int sockfd, const std::string &jsonMsg);

    eJsonComErr socketRecvJsonMsg(int sockfd, std::string &jsonMsg, int &readBytes, std::string &rxHeader);
    static void flushSocketRxBuffer(int sockfd);

    // Tx handling
    Event aEventSendMsgStopRequest;
    Event aEventMsgQueue;
    std::queue<JsonMsg> aMsgQueue;
    pthread_mutex_t aMutexQueueMsg;

    std::string addMsgToQueue(const spjson& pJson, eJsonComPort portDest, std::string typeMethod, bool isCommand, int timeoutMs,
                       std::function<void(void *, eJsonComErr, std::string, spjson)> pCallback = nullptr,
                       void *caller = nullptr);
};

}

#endif //VORTEXCORE_JSONCOMMUNICATION_H
