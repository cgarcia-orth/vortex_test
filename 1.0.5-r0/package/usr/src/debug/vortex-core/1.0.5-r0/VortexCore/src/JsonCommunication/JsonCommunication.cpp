/**************************************************************************************************
* Module Name    : JsonCommunication.h
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



#include <sys/poll.h>
#include <sys/socket.h>
#include <cerrno>
#include <regex>
#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <VortexCore/includes/LoggerFactory>
#include <VortexCore/includes/TimeProviderFactory>
#include <VortexCore/includes/ScopedLock>

#include "JsonCommunication.h"
#include <sole.hpp>
#include <utility>


using spjson = std::shared_ptr<nlohmann::json>;

namespace vortex {

JsonCommunication::JsonCommunication() : JsonCommunication("jsonCom") {}

JsonCommunication::JsonCommunication(std::string loggerRoot) {
    aLoggerPtr = LoggerFactory::getLogger(std::move(loggerRoot));
    aTimeProviderPtr = TimeProviderFactory::getTimeProvider();
    aVerbose = false;
    aListenThreadPtr = nullptr;
    aListenerPtr = nullptr;
    Mutex::initialiseRecursiveMutex(&aMutexQueueMsg);

    aEventListenStopRequest.clear();
    aEventSendMsgStopRequest.clear();
    aEventMsgQueue.clear();
}

JsonCommunication::~JsonCommunication() {
    LoggerFactory::dropLogger(aLoggerPtr);
    stop();
}


void JsonCommunication::setVerbose(bool verbose) {
    aVerbose = verbose;
}

spjson JsonCommunication::createJsonResponse(const spjson &pJsonReq, const spjson &pJsonErr) {

    spjson pJsonResp = std::make_shared<nlohmann::json>();
    if (pJsonErr == nullptr) {
        *pJsonResp = {
                {"id",      (*pJsonReq)["id"]},
                {"jsonrpc", "2.0"},
                {"result",  (*pJsonReq)["method"]}};
    } else {
        *pJsonResp = {
                {"id",      (*pJsonReq)["id"]},
                {"jsonrpc", "2.0"},
                {"error",   *pJsonErr}};

    }
    return pJsonResp;
}

spjson JsonCommunication::createJsonResponseErrorCode(int errCode) {
    spjson pRespErr = std::make_shared<nlohmann::json>();
    (*pRespErr)["code"] = errCode;
    return pRespErr;
}

void JsonCommunication::listenOnChannel(eJsonComPort portNum) {
    aPortsToListen.push_back(static_cast<int>(portNum));
}

std::string JsonCommunication::sendData(eJsonComPort portNum,
                                        const std::string &type,
                                        const spjson &pMsg,
                                        std::function<void(void *, eJsonComErr, std::string, spjson)> pCallback,
                                        void *pCaller,
                                        int timeoutMs) {
    return addMsgToQueue(pMsg, portNum, type, false, timeoutMs, std::move(pCallback), pCaller);
}

std::string JsonCommunication::sendCmd(eJsonComPort portNum,
                                       const std::string &method,
                                       const spjson &pMsg,
                                       std::function<void(void *, eJsonComErr, std::string, spjson)> pCallback,
                                       void *pCaller,
                                       int timeoutMs) {
    return addMsgToQueue(pMsg, portNum, method, true, timeoutMs, std::move(pCallback), pCaller);
}

void JsonCommunication::setListener(IJsonMsgListener *listener) {
    aListenerPtr = listener;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Private methods
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// return socket descriptor
int JsonCommunication::getClient(eJsonComPort portNum) {
    int sock;
    auto it = aClientSockets.find(static_cast<int>(portNum));
    if (it == aClientSockets.end()) {
        // new connection
        sock = connectClient(portNum);
        aClientSockets[(int) portNum] = sock;
    } else {
        if (aClientSockets[static_cast<int>(portNum)] == -1) {
            // connection was previously closed
            sock = connectClient(portNum);
            aClientSockets[static_cast<int>(portNum)] = sock;
        } else {
            sock = it->second;
            // make sure socket is still connected
            bool resetConnection = false;
            struct pollfd fds = {.fd = sock, .events = POLLIN | POLLHUP | POLLRDHUP | POLLERR, .revents = 0}; // NOLINT(hicpp-signed-bitwise)
            int rc = poll(&fds, 1, 0);
            if (rc > 0 && fds.revents == POLLIN) {
                char testByte;
                rc = recv(sock, &testByte, sizeof(testByte), MSG_PEEK);
                resetConnection = rc <= 0;
            } else if (rc > 0 && fds.revents != 0) {
                resetConnection = true;
            } else if (rc < 0) {
                resetConnection = true;
            }
            if (resetConnection) {
                close(sock);
                aLoggerPtr->debug("reconnecting socket");
                sock = connectClient(portNum);
                aClientSockets[static_cast<int>(portNum)] = sock;
                aLoggerPtr->debug("reconnected socket {}", sock);
            }
        }
    }
    return sock;
}

int JsonCommunication::connectClient(eJsonComPort portNum) {

    int sock = 0;
    int on = 1;
    struct sockaddr_in addrbind  = {0};
    struct sockaddr_in serv_addr = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    // Allow socket descriptor to be reuseable
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on))) {
        close(sock);
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons((int) portNum);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    addrbind.sin_family = AF_INET;
    addrbind.sin_port = htons((int) portNum+1);
    addrbind.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock, (struct sockaddr*) &addrbind, sizeof(struct sockaddr_in)) != 0) {
        close(sock);
        return -1;
    }

    int connectRet = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (connectRet < 0) {
        close(sock);
        return -1;
    }
    return sock;
}

void JsonCommunication::sendMsg(eJsonComPort portNum,
                                const std::string &msg,
                                const std::string &msgId,
                                bool checkResponse,
                                int timeoutMs,
                                const std::function<void(void *, eJsonComErr, std::string, spjson)> &pCallback,
                                void *pCaller) {
    int sock = getClient(portNum);
    std::string rspStr;

    if (sock == -1) {
        if (pCallback != nullptr && pCaller != nullptr)
            pCallback(pCaller, eJsonComErr::JSON_COM_TX_ERR_CONNECTION, msgId, nullptr);
    } else {
        flushSocketRxBuffer(sock);
        int rc = socketSendJsonMsg(sock, msg);
        if (rc <= 0) {
            if ((nullptr != pCallback) && (nullptr != pCaller))
                pCallback(pCaller, eJsonComErr::JSON_COM_TX_ERR_SEND, msgId, nullptr);
        } else {
            if (!checkResponse) {
                if ((nullptr != pCallback) && (nullptr != pCaller))
                    pCallback(pCaller, eJsonComErr::JSON_COM_NO_ERR, msgId, nullptr);
            } else {
                struct timeval tv{};
                tv.tv_sec = timeoutMs / 1000;
                tv.tv_usec = timeoutMs * 1000 - tv.tv_sec * 1000000;
                setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);

                int rxBytes;
                std::string rxHeader;
                eJsonComErr err = socketRecvJsonMsg(sock, rspStr, rxBytes, rxHeader);
                if (err != eJsonComErr::JSON_COM_NO_ERR || rxBytes == 0) {
                    if ((nullptr != pCallback) && (nullptr != pCaller))
                        pCallback(pCaller, err, msgId, nullptr);
                } else {
                    try {
                        spjson pJsonRsp = std::make_shared<nlohmann::json>();
                        *pJsonRsp = nlohmann::json::parse(rspStr);
                        std::string idStr = (*pJsonRsp)["id"].dump();
                        idStr.erase(remove(idStr.begin(), idStr.end(), '\"'), idStr.end());
                        if ((nullptr != pCallback) && (nullptr != pCaller)) {
                            if (msgId == idStr)
                                pCallback(pCaller, eJsonComErr::JSON_COM_NO_ERR, msgId, pJsonRsp);
                            else {
                                aLoggerPtr->warn("mismatch id Req {} - id Resp {} ", msgId, idStr);
                                pCallback(pCaller, eJsonComErr::JSON_COM_RX_MISMATCH_ID, msgId, pJsonRsp);
                            }

                        }
                    } catch (nlohmann::json::exception &e) {
                        if ((nullptr != pCallback) && (nullptr != pCaller))
                            pCallback(pCaller, eJsonComErr::JSON_COM_RX_INVALID_JSON, msgId, nullptr);
                        aLoggerPtr->warn("json exception {}", e.what());
                    }
                }
            }
        }
    }
}

std::string
JsonCommunication::addMsgToQueue(const spjson &pJsonMsg, eJsonComPort portDest, std::string typeMethod, bool isCommand,
                                 int timeoutMs,
                                 std::function<void(void *, eJsonComErr, std::string, spjson)> pCallback, void *pCaller) {
    ScopedLock lock(&aMutexQueueMsg);

    if (!pJsonMsg->is_object()) {
        aLoggerPtr->error("msg json must be an object");
        return std::string();
    }

    JsonMsg msg;
    msg.aTypeMethod = typeMethod;
    msg.aPortDest = portDest;
    msg.aTimeoutMs = timeoutMs;
    msg.aMsgId = sole::uuid0().str();
    msg.aCallbackPtr = std::move(pCallback);
    msg.aCallerPtr = pCaller;
    msg.aIsCommand = isCommand;

    if (isCommand) {
        spjson pCommand = std::make_shared<nlohmann::json>();
        *pCommand = {
                {"id",      msg.aMsgId},
                {"jsonrpc", "2.0"},
                {"method",  typeMethod},
                {"params",  *pJsonMsg}
        };
        msg.aMsgPtr = pCommand;
    } else {
        spjson pData = std::make_shared<nlohmann::json>();
        *pData = {
                {"data",      typeMethod},
                {"timestamp", std::to_string(aTimeProviderPtr->getTimeMilliSec())},
                {"value",     *pJsonMsg}
        };
        msg.aMsgPtr = pData;
    }
    aMsgQueue.push(msg);
    aEventMsgQueue.set();

    return msg.aMsgId;
}

void JsonCommunication::start(bool bStartTx, bool bStartRx) {
    if (bStartTx) {
        aSendMsgThreadPtr = std::unique_ptr<std::thread>(new std::thread(&JsonCommunication::sendMsgThreadProcess, this));
    }
    if (bStartRx) {
        aEventListenThreadReady.clear();

        if (aPortsToListen.empty()) {
            aLoggerPtr->error("No port to listen to");
        } else {
            aEventListenStopRequest.clear();
            aListenThreadPtr = std::unique_ptr<std::thread>(new std::thread(&JsonCommunication::listenThreadProcess, this));
            aEventListenThreadReady.wait();
        }
    }
}

void JsonCommunication::stop() {

    if (aEventSendMsgStopRequest.isSet()) {
        aLoggerPtr->warn("Stop Sending already requested");
    } else {
        if (aSendMsgThreadPtr != nullptr) {
            aEventSendMsgStopRequest.set();
            aSendMsgThreadPtr->join();
            aEventSendMsgStopRequest.clear();
            aSendMsgThreadPtr = nullptr;
        }
    }

    if (aEventListenStopRequest.isSet()) {
        aLoggerPtr->warn("Stop listening already requested");
    } else {
        if (aListenThreadPtr != nullptr) {
            aEventListenStopRequest.set();
            aListenThreadPtr->join();
            aEventListenStopRequest.clear();
            aListenThreadPtr = nullptr;
        }
    }
}


void JsonCommunication::sendMsgThreadProcess() {

    aLoggerPtr->info("Thread Send Started ");
    pthread_setname_np(pthread_self(), "JsonComTx");

    while (!aEventSendMsgStopRequest.isSet()) {
        aEventMsgQueue.wait(0.5);
        aEventMsgQueue.clear();

        if (aEventSendMsgStopRequest.isSet())
            return; // end of thread

        std::queue<JsonMsg> copyMsgQueue;
        {
            ScopedLock lock(&aMutexQueueMsg);
            if (!aMsgQueue.empty()) {
                swap(aMsgQueue, copyMsgQueue);
            }
        }

        while (!copyMsgQueue.empty()) {
            JsonMsg msgToSend = std::move(copyMsgQueue.front());
            copyMsgQueue.pop();
            sendMsg(msgToSend.aPortDest, msgToSend.aMsgPtr->dump(), msgToSend.aMsgId,
                    msgToSend.aIsCommand, msgToSend.aTimeoutMs,
                    msgToSend.aCallbackPtr, msgToSend.aCallerPtr);
        }

    }
    for (const auto &kv : aClientSockets) {
        close(kv.second);
    }
    aLoggerPtr->info("Thread Send Stopped ");
}

void JsonCommunication::listenThreadProcess() {
    int len, rc, on = 1;
    int listen_sd, new_sd;
    std::vector<int> vectorListenSd; // list of socked descriptor binded to a specific port
    std::map<int, int> mapListenSdToPort;
    std::map<int, int> mapConnectSdToPort;
    bool end_server;
    struct sockaddr_in addr = {0};
    std::vector<pollfd> fdsVector{}; // includes both listen sd and accepted sd

    aLoggerPtr->info("Thread Listen Started ");
    pthread_setname_np(pthread_self(), "JsonComRx");

    for (int portToListen : aPortsToListen) {
        listen_sd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_sd < 0) {
            aLoggerPtr->error("socket() failed");
        }
        // Allow socket descriptor to be reuseable
        rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
        if (rc < 0) {
            aLoggerPtr->error("setsockopt() failed");
            close(listen_sd);
        }
        //Set socket to be nonblocking.
        int flags = fcntl(listen_sd, F_GETFL, 0);
        fcntl(listen_sd, F_SETFL, (unsigned int) flags | (unsigned int) O_NONBLOCK);

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(portToListen);
        rc = bind(listen_sd, (struct sockaddr *) &addr, sizeof(addr));
        if (rc < 0) {
            aLoggerPtr->error("bind failed for port {}", portToListen);
            close(listen_sd);
        }
        rc = listen(listen_sd, 1); // only one connection allowed
        if (rc < 0) {
            aLoggerPtr->error("listen failed for port {}", portToListen);
            close(listen_sd);
        }
        fdsVector.push_back(pollfd{.fd = listen_sd, .events = POLLIN});
        mapListenSdToPort[listen_sd] = portToListen;
    }

    aEventListenThreadReady.set();
    aLoggerPtr->info("Thread Listen Ready to poll");

    // Loop waiting for incoming connects or for incoming data
    do {
        rc = poll(&fdsVector[0], fdsVector.size(), 2 * 1000); // block 2 seconds
        end_server = aEventListenStopRequest.isSet();
        if (rc < 0) {
            aLoggerPtr->error("poll() failed");
            break;
        }
        for (auto &fdIt : fdsVector) {
            // check which sd returned : named sd or connected sd
            if (fdIt.revents == 0)
                continue;

            // these errors can happen when client side stops
            if ((fdIt.revents & POLLHUP) || (fdIt.revents & POLLERR)) {
                aLoggerPtr->warn("Poll revents = {}", fdIt.revents);
                close(fdIt.fd);
                fdIt.fd = -1; // remove socket
                continue;
            }

            bool listenSocketReady = false;
            auto itListenSd = mapListenSdToPort.find(fdIt.fd);
            if (itListenSd != mapListenSdToPort.end()) {
                listenSocketReady = true;
                aLoggerPtr->debug("Listening socket is readable for port {}", itListenSd->second);

                // Accept every incoming connection.
                do {
                    new_sd = accept(itListenSd->first, nullptr, nullptr);
                    if (new_sd < 0) {
                        if (errno != EWOULDBLOCK) {
                            aLoggerPtr->error("  accept() failed for port {}", itListenSd->second);
                            end_server = true;
                        }
                        break;
                    }
                    // Add the new incoming connection to the poll struct
                    aLoggerPtr->debug("New incoming connection on port {} - socket {}", itListenSd->second, new_sd);
                    mapConnectSdToPort[new_sd] = itListenSd->second;
                    fdsVector.push_back(pollfd{.fd = new_sd, .events = POLLIN});
                } while (new_sd != -1);
            }
            if (!listenSocketReady) {
                if (aVerbose)
                    aLoggerPtr->debug("Descriptor is readable for port {} - socket {}", mapConnectSdToPort[fdIt.fd], fdIt.fd);
                std::string msgReceived, msgHeader;
                socketRecvJsonMsg(fdIt.fd, msgReceived, rc, msgHeader);
                // no bytes received means connection closed by client
                if (rc <= 0) {
                    aLoggerPtr->debug(" Connection closed on socket {}", fdIt.fd);
                    // close active connection
                    close(fdIt.fd);
                    fdIt.fd = -1;
                    auto it = mapConnectSdToPort.find(fdIt.fd);
                    if (it != mapConnectSdToPort.end())
                        mapConnectSdToPort.erase(it);
                    // remove entry in fdsVector after the loop
                } else if (rc > 0) {
                    // Data was received
                    len = rc;
                    try {
                        spjson pJsonMsg = std::make_shared<nlohmann::json>();
                        *pJsonMsg = nlohmann::json::parse(msgReceived);
                        if (aVerbose)
                            aLoggerPtr->info("Json bytes Received {} - msg :\n{}{}", len, msgHeader, pJsonMsg->dump(4));

                        int port = mapConnectSdToPort[fdIt.fd];
                        spjson pJsonErr;

                        auto it_jsonrpc = pJsonMsg->find("jsonrpc");
                        // test if the message was a command
                        // filtering can not be done through port (test_link_connection can be done on data channel)
                        if (it_jsonrpc != pJsonMsg->end()) {
                            if (aVerbose) {
                                std::string msgNoHeaderStr = (*pJsonMsg)["params"].dump(4);
                                aLoggerPtr->info("Json msg received sz {} - msg \n{}", msgNoHeaderStr.length(),
                                                 msgNoHeaderStr);
                            }
                            // remove json rpc header
                            pJsonErr = aListenerPtr->onNewCmdMsg(port, (*pJsonMsg)["method"],
                                                                 std::make_shared<nlohmann::json>((*pJsonMsg)["params"]));
                            // Reply JSON RPC success or error to the client
                            spjson pJsonResp = createJsonResponse(pJsonMsg, pJsonErr);
                            std::string respStr = pJsonResp->dump();
                            socketSendJsonMsg(fdIt.fd, respStr);
                        } else {
                            // keep the whole message
                            std::string::size_type sz;
                            std::string tsStr = (*pJsonMsg)["timestamp"].dump();
                            tsStr.erase(remove(tsStr.begin(), tsStr.end(), '\"'), tsStr.end());
                            uint64_t timestamp = std::stof(tsStr, &sz);
                            aListenerPtr->onNewDataMsg(port, (*pJsonMsg)["data"], timestamp,
                                                       std::make_shared<nlohmann::json>((*pJsonMsg)["value"]));
                            if (aVerbose)
                                std::string msgNoHeaderStr = pJsonMsg->dump();
                            // no need to send json rpc response on data channel
                        }
                    }
                    catch (nlohmann::json::exception &e) {
                        aLoggerPtr->warn("json exception {} - drop received message", e.what());
                    }
                }
            }
        }
        // cleanup connections
        auto itRm = std::remove_if(std::begin(fdsVector), std::end(fdsVector), [](pollfd f) { return f.fd == -1; });
        int nbFdTopPop = 0;
        if (itRm != fdsVector.end()) {
            for (; itRm != fdsVector.end(); itRm++)
                nbFdTopPop++;
            for (int i = 0; i < nbFdTopPop; i++)
                fdsVector.pop_back();
        }
    } while (end_server == 0);

    // Clean up  sockets
    for (const auto &kv : mapListenSdToPort) {
        close(kv.first);
    }
    for (const auto &kv : mapConnectSdToPort) {
        close(kv.first);
    }
    mapListenSdToPort.clear();
    mapConnectSdToPort.clear();
    fdsVector.clear();
    aLoggerPtr->info("Thread Listen stopped");
}

int JsonCommunication::socketSendJsonMsg(int sockfd, const std::string &jsonMsg) {
    std::string txStr;
    int msgLength = jsonMsg.length();
    std::string strlength = std::to_string(msgLength);
    if (strlength.length() < 10) // zero padding  limit size to 10 digit
        strlength.insert(0, 10 - strlength.length(), '0');

    txStr = "SZ=" + strlength + ";" + jsonMsg;
    int ret = send(sockfd, txStr.c_str(), txStr.length(), MSG_NOSIGNAL);
    if (aVerbose)
        aLoggerPtr->info("json send msg on socket {} - ret {} - msg sz {} - msg:\n{}",
                sockfd, ret, msgLength, txStr);

    return ret;

}

// return the json message read on the socket, having a header with the size
// there should not be any limitation on message size
// readBytes returns the number of bytes received (0 if none which means close connection)
// return -1 in case of error
eJsonComErr JsonCommunication::socketRecvJsonMsg(int sockfd, std::string &jsonMsg, int &readBytes , std::string &rxHeader) {
    char headerBuff[14+1] = {0};
    char buffer[512] = {0};
    jsonMsg.clear();
    eJsonComErr ret = eJsonComErr::JSON_COM_NO_ERR;
    readBytes = 0;

    int rc = recv(sockfd, headerBuff, sizeof(headerBuff)-1, 0);
    if (rc < 0) {
        aLoggerPtr->warn("socketRecvJsonMsg - error {}", rc);
        ret = eJsonComErr::JSON_COM_RX_TIMEOUT;
    } else if (rc == 0) {
        aLoggerPtr->warn("socketRecvJsonMsg - error no data received on recv");
        ret = eJsonComErr::JSON_COM_RX_NO_DATA;
    }
        // check header exists
    else if (rc != strlen("SZ=xxxxxxxxxx;")) {
        aLoggerPtr->warn("socketRecvJsonMsg - error msg too small recv");
        // drop buffer
        while (recv(sockfd, buffer, sizeof(buffer), 0) > 0);
        ret = eJsonComErr::JSON_COM_RX_INVALID_HEADER;
    } else {
        std::string headerStr(headerBuff);
        std::regex rgx("SZ=([0-9]*);");
        std::smatch match;
        if (!std::regex_search(headerStr, match, rgx)) {
            aLoggerPtr->warn("socketRecvJsonMsg - error invalid header");
            // drop buffer
            while (recv(sockfd, buffer, sizeof(buffer), 0) > 0);
            ret = eJsonComErr::JSON_COM_RX_HEADER_FORMAT;
        } else {
            rxHeader = std::move(headerStr);
            int expectedSz = std::stoi(match.str(1));
            int totalRx = 0;
            do {
                int remainingBytes = expectedSz - totalRx;
                int nbRx = recv(sockfd, buffer,
                                remainingBytes < (sizeof(buffer) - 1) ? remainingBytes : (sizeof(buffer) -
                                                                                          1), // char array must be null terminated
                                0);
                totalRx += nbRx;
                std::string tmp(buffer);
                jsonMsg += tmp;
                memset(buffer, 0, sizeof(buffer));
            } while (totalRx < expectedSz);
            readBytes = totalRx;
        }
    }
    return ret;
}

// make sure RX buffer is empty before starting a request/response any message
void JsonCommunication::flushSocketRxBuffer(int sockfd) {

    int rv;
    struct pollfd ufds[1];
    ufds[0].fd = sockfd;
    ufds[0].events = POLLIN;

    // wait for events on the sockets, 1ms timeout
    rv = poll(ufds, 1, 1);
    if (rv > 0) {
        if (ufds[0].revents & POLLIN) {
            char buffer[20] = {0};
            while (recv(sockfd, buffer, sizeof(buffer), 0) > 0) {}
        }
    }
}

}

