#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"
/**************************************************************************************************
* Module Name    : TestJsonCommunication.cpp
*
* Description    : Interface definition to use JSON Communication
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-10-01  Olivier Battion : Initial implementation.
*
**************************************************************************************************/

#include <gmock/gmock.h>
#include <JsonCommunication/JsonCommunication.h>
#include "Time/TimeProvider.h"
#include "includes/TimeProviderFactory"

using json = nlohmann::json;
using namespace vortex;

typedef std::map<int, std::vector<std::string>> JsonMsgMap;


class TestJsonComListener : public vortex::IJsonMsgListener
{
public:
    JsonMsgMap aMsgReceivedPerPort;
    int aReponseCodeInHandler;
    int aNbSecondSleepInHandler;

    TestJsonComListener() {
        aReponseCodeInHandler = 0;
        aNbSecondSleepInHandler = 0;
    }
    spjson onNewCmdMsg(int srcPort, std::string method, spjson pJsonParams) override  {
        if (*pJsonParams != nullptr)
            aMsgReceivedPerPort[srcPort].push_back(pJsonParams->dump());
        sleep(aNbSecondSleepInHandler);

        spjson pJsonErr = nullptr;
        if (aReponseCodeInHandler != 0)
            pJsonErr = JsonCommunication::createJsonResponseErrorCode(aReponseCodeInHandler);
        return pJsonErr;
    }
    void onNewDataMsg(int srcPort, std::string type, float timestamp, spjson pJsonValue) override {
        if (*pJsonValue != nullptr)
            aMsgReceivedPerPort[srcPort].push_back(pJsonValue->dump());
        sleep(aNbSecondSleepInHandler);
    }
};

class TestJsonComSenderWrapper {

public :
    spjson aJsonToSendPtr;
    std::map<std::string,json> aReceivedResp;
    std::map<eJsonComErr,uint> aMapStatus;
    std::list<std::string> aMsgIdTx;
    std::thread *aSenderThread = nullptr;
    bool aRandomDelay = false;
    bool aIsCmd = false;
    uint aNbMsgToSend = 0;
    uint aFixDelayMs = 100;
    uint aRandomMsMax = 1000;
    uint aTimeoutMs = 5000;
    std::string aTypeMethod;
    eJsonComPort aDestPort{};
    std::shared_ptr<JsonCommunication> aCom;

    TestJsonComSenderWrapper(){
        aMapStatus[eJsonComErr::JSON_COM_NO_ERR] = 0;
        aMapStatus[eJsonComErr::JSON_COM_INVALID_PARAM] = 0;
        aMapStatus[eJsonComErr::JSON_COM_TX_ERR_SEND] = 0;
        aMapStatus[eJsonComErr::JSON_COM_TX_ERR_CONNECTION] = 0;
        aMapStatus[eJsonComErr::JSON_COM_RX_TIMEOUT] = 0;
        aMapStatus[eJsonComErr::JSON_COM_RX_INVALID_HEADER] = 0;
        aMapStatus[eJsonComErr::JSON_COM_RX_HEADER_FORMAT] = 0;
        aMapStatus[eJsonComErr::JSON_COM_RX_INVALID_JSON] = 0;
        aMapStatus[eJsonComErr::JSON_COM_RX_MISMATCH_ID] = 0;
    }

    void HandleResponse(eJsonComErr err, std::string msgId, spjson pResponse)
    {
        aMapStatus[err]++;
        if (err == eJsonComErr::JSON_COM_NO_ERR && pResponse != nullptr)
            aReceivedResp[msgId] = *pResponse;
    }

    void CountResults(uint& msgTx,uint& msgRx, uint& msgTxNotRx, uint& msgRxOk, uint& msgRxError) {
        msgTx = aMsgIdTx.size();
        msgRx = aReceivedResp.size();
        msgTxNotRx = msgRxOk = msgRxError = 0;
        for (auto id : aMsgIdTx) {
            if(aReceivedResp.find(id) == aReceivedResp.end())
                msgTxNotRx++;
            else {
                if (aReceivedResp[id].find("error") == aReceivedResp[id].end())
                    msgRxOk++;
                else
                    msgRxError++;
            }
        }
    }
    static void aMsgAcknowledgedCb(void* ptr, eJsonComErr err, std::string msgId, spjson pResponse) {
        auto self = static_cast<TestJsonComSenderWrapper*>(ptr);
        self->HandleResponse(err, msgId, pResponse);
    }
    void sendMsgThreadProcess() {
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_int_distribution<uint> distribution(1, aRandomMsMax);

        for (uint i =0; i < aNbMsgToSend; i++) {
            if(aIsCmd) {
                std::string msgId = aCom->sendCmd(aDestPort, aTypeMethod, aJsonToSendPtr, &aMsgAcknowledgedCb, this, aTimeoutMs);
                if(!msgId.empty())
                    aMsgIdTx.push_back(msgId);
            } else {
                std::string msgId = aCom->sendData(aDestPort, aTypeMethod, aJsonToSendPtr,  &aMsgAcknowledgedCb, this, aTimeoutMs);
                if(!msgId.empty())
                    aMsgIdTx.push_back(msgId);
            }
            if(aRandomDelay) {
                int randomWaitMs = distribution(random_engine);
                usleep(randomWaitMs*1000);
            } else {
                usleep(aFixDelayMs*1000);
            }
        }
    }

    void startSending(bool useThread = true, bool waitThreadToFinish = true){
        if (useThread) {
            if(aSenderThread == nullptr) {
                aSenderThread = new std::thread(&TestJsonComSenderWrapper::sendMsgThreadProcess, this);
                if (waitThreadToFinish) {
                    aSenderThread->join();
                    aSenderThread = nullptr;
                }
            }
        } else {
            sendMsgThreadProcess();
        }
    }

    ~TestJsonComSenderWrapper(){
        if(aSenderThread != nullptr) {
            aSenderThread->join();
            aSenderThread = nullptr;
        }
    }
};

static std::string executeCommand(std::string command) {
    std::array<char, 128> buffer;
    std::string result = "";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe)
        return result;

    while (fgets(buffer.data(), 128, pipe) != NULL) {
        result +=buffer.data();
    }
    pclose(pipe);

    return result;
}


class TestJsonCommunication : public testing::Test

{
protected:

    void SetUp() override ;
    void TearDown() override ;
    static spjson createCommandJson(const std::string &targetID, const std::string &newMode);
    static spjson createDataUsageJson(int usage);

    std::shared_ptr<TimeProvider> aTimeProvider;
};

void TestJsonCommunication::SetUp() {
    aTimeProvider = std::make_shared<TimeProvider>();
    TimeProviderFactory::overrideTimeProvider(aTimeProvider);
}

void TestJsonCommunication::TearDown() {
    TimeProviderFactory::clearTimeProviderOverride();
}

spjson TestJsonCommunication::createCommandJson(const std::string& targetID, const std::string &newMode)
{
    spjson pJsonCmd = std::make_shared<nlohmann::json>(json::object());
    (*pJsonCmd)[targetID] = json::object();
    (*pJsonCmd)[targetID]["data"] = newMode;
    return pJsonCmd;
}

spjson TestJsonCommunication::createDataUsageJson(int usage)
{
    spjson pJsonCmd = std::make_shared<nlohmann::json>(json::object());
    (*pJsonCmd)["usage"] = usage;
    return pJsonCmd;
}

TEST_F(TestJsonCommunication, TestJsonStruct) {
    std::shared_ptr<spdlog::logger> logger = LoggerFactory::getLogger("gtest");
    json jsonCmdToTest = R"(
            {
            "jsonrpc": "2.0",
            "method": "command",
            "params": {
                "toto" : {
                    "data": "theNewvalue"
                }
            },
            "id": "9894"
            })"_json;
    json& jsonmethod = jsonCmdToTest["method"];
    logger->info("method {}", jsonmethod.dump());
    std::string targetID = jsonCmdToTest["params"].begin().key();
    logger->info("targetID {}", targetID);
}

TEST_F(TestJsonCommunication, TestSingleCommand) {
    std::shared_ptr<JsonCommunication> jsonCom = std::make_shared<JsonCommunication>();
    TestJsonComListener listener;
    TestJsonComSenderWrapper msgSender;
    msgSender.aCom = jsonCom;

    jsonCom->setVerbose(true);
    jsonCom->setListener(&listener);

    // simulate Maestro receiving a command from GUI
    jsonCom->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonCom->start();

    msgSender.aIsCmd = true;
    msgSender.aTypeMethod = "command";
    msgSender.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSender.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSender.aRandomDelay = false;
    msgSender.aNbMsgToSend = 1;
    msgSender.startSending();

    sleep(2); //wait to receive messages
    jsonCom->stop(); // can block - up to  2 seconds

    int nbMsgReceivedPortGui = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortGui, 1);
    ASSERT_EQ(msgSender.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , 1);

    std::string txStr = msgSender.aJsonToSendPtr->dump();
    std::string rxStr = listener.aMsgReceivedPerPort[(int)eJsonComPort::GUI_CMD_PORT].at(0);
    ASSERT_EQ(txStr, rxStr);

    // check response received by the caller
    uint msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError;
    msgSender.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, 1);
    ASSERT_EQ(msgRx, 1);   // one command send so one response received
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, 1);
    ASSERT_EQ(msgRxError, 0);
}

TEST_F(TestJsonCommunication, TestSingleData) {
    std::shared_ptr<JsonCommunication> jsonCom = std::make_shared<JsonCommunication>();
    TestJsonComListener listener;
    TestJsonComSenderWrapper msgSender;
    msgSender.aCom = jsonCom;

    jsonCom->setVerbose(true);
    jsonCom->setListener(&listener);

    // simulate Maestro sending a data to GUI
    jsonCom->listenOnChannel(eJsonComPort::GUI_DATA_PORT);
    jsonCom->start();

    msgSender.aIsCmd = false;
    msgSender.aTypeMethod = "data";
    msgSender.aJsonToSendPtr = createDataUsageJson(452);
    msgSender.aDestPort = eJsonComPort::GUI_DATA_PORT;
    msgSender.aRandomDelay = false;
    msgSender.aNbMsgToSend = 1;
    msgSender.startSending();

    sleep(2); //wait to receive messages

    int nbMsgReceivedPortGui = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_DATA_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortGui, 1);
    ASSERT_EQ(msgSender.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , 1);

    std::string txStr = msgSender.aJsonToSendPtr->dump();
    std::string rxStr = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_DATA_PORT)].at(0);
    ASSERT_EQ(rxStr.find(txStr) != std::string::npos, true );

    // check response received by the caller
    uint msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError;
    msgSender.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, 1);
    ASSERT_EQ(msgRx, 0);  // data message dont expect a response
    ASSERT_EQ(msgTxNotRx, 1);
    ASSERT_EQ(msgRxOk, 0);
    ASSERT_EQ(msgRxError, 0);
}

TEST_F(TestJsonCommunication, TestJsonResponseError) {
    std::shared_ptr<JsonCommunication> jsonCom = std::make_shared<JsonCommunication>();
    TestJsonComSenderWrapper msgSender;
    msgSender.aCom = jsonCom;

    TestJsonComListener listenerMpro;

    jsonCom->setVerbose(true);
    jsonCom->setListener(&listenerMpro);

    jsonCom->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonCom->start();

    msgSender.aIsCmd = true;
    msgSender.aTypeMethod = "command";
    msgSender.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSender.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSender.aRandomDelay = false;
    msgSender.aNbMsgToSend = 1;

    listenerMpro.aReponseCodeInHandler = 0; // simulate success
    msgSender.startSending();
    sleep(2); //
    int nbMsgReceivedPortGui = listenerMpro.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortGui, 1);
    ASSERT_EQ(msgSender.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , 1);
    ASSERT_EQ(msgSender.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , 1);

    // check response received by the caller
    uint msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError;
    msgSender.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, 1);
    ASSERT_EQ(msgRx, 1);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, 1);
    ASSERT_EQ(msgRxError, 0);

    listenerMpro.aReponseCodeInHandler = -32001; // simulate error application specific
    msgSender.startSending();
    sleep(2); //
    nbMsgReceivedPortGui = listenerMpro.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortGui, 2);
    ASSERT_EQ(msgSender.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , 2);
    msgSender.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, 2);
    ASSERT_EQ(msgRx, 2);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, 1);
    ASSERT_EQ(msgRxError, 1);

    //jsonCom->stopListening(); // can block - up to  2 seconds

    ASSERT_EQ(listenerMpro.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size(), 2);
}

/**
 * This test takes much too long, enable it to test punctually.
 */
TEST_F(TestJsonCommunication, DISABLED_TestManyCommand)
{
    std::shared_ptr<JsonCommunication> jsonCom = std::make_shared<JsonCommunication>();
    TestJsonComListener listener;
    TestJsonComSenderWrapper msgSender;
    msgSender.aCom = jsonCom;
    msgSender.aIsCmd = true;
    msgSender.aTypeMethod = "command";
    msgSender.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSender.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSender.aRandomDelay = false;
    msgSender.aNbMsgToSend = 5000;
    msgSender.aFixDelayMs = 10;

    jsonCom->setVerbose(true);
    jsonCom->setListener(&listener);

    // simulate Maestro receiving a command from GUI
    jsonCom->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonCom->start();

    msgSender.startSending();
    // wait to receive messages
    sleep(msgSender.aNbMsgToSend * msgSender.aFixDelayMs * 2 / 1000);
    //jsonCom->stopListening(); // can block - up to  2 seconds


    int nbMsgReceivedPortGui = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortGui, msgSender.aNbMsgToSend);
    ASSERT_EQ(msgSender.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , msgSender.aNbMsgToSend);

    uint msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError;
    msgSender.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, msgSender.aNbMsgToSend);
    ASSERT_EQ(msgRx, msgSender.aNbMsgToSend);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, msgSender.aNbMsgToSend);
    ASSERT_EQ(msgRxError, 0);
}

/**
 * This test takes much too long, enable it to test punctually.
 */
TEST_F(TestJsonCommunication, DISABLED_TestMultipleProvider)
{
    std::shared_ptr<JsonCommunication> jsonCom = std::make_shared<JsonCommunication>();
    TestJsonComListener listener;
    TestJsonComSenderWrapper msgSender_1, msgSender_2;
    msgSender_1.aCom = jsonCom;
    msgSender_1.aIsCmd = true;
    msgSender_1.aTypeMethod = "command";
    msgSender_1.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSender_1.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSender_1.aRandomDelay = true;
    msgSender_1.aNbMsgToSend = 200;
    msgSender_1.aRandomMsMax = 40;
    msgSender_2.aCom = jsonCom;
    msgSender_2.aIsCmd = true;
    msgSender_2.aTypeMethod = "command";
    msgSender_2.aJsonToSendPtr = createCommandJson("pump2", "off");
    msgSender_2.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSender_2.aRandomDelay = false;
    msgSender_2.aNbMsgToSend = 100;
    msgSender_2.aRandomMsMax = 80;

    jsonCom->setVerbose(false);
    jsonCom->setListener(&listener);

    // simulate Maestro receiving a command from GUI
    jsonCom->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonCom->start();

    msgSender_1.startSending();
    msgSender_2.startSending();
    // wait to receive messages
    sleep(((msgSender_1.aNbMsgToSend * msgSender_1.aRandomMsMax) + (msgSender_2.aNbMsgToSend * msgSender_2.aRandomMsMax))  * 2 / 1000);
    //jsonCom->stopListening(); // can block - up to  2 seconds

    int nbMsgReceivedPortGui = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortGui, msgSender_1.aNbMsgToSend + msgSender_2.aNbMsgToSend);
    ASSERT_EQ(msgSender_1.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , msgSender_1.aNbMsgToSend);
    ASSERT_EQ(msgSender_2.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , msgSender_2.aNbMsgToSend);

    uint msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError;
    msgSender_1.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, msgSender_1.aNbMsgToSend);
    ASSERT_EQ(msgRx, msgSender_1.aNbMsgToSend);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, msgSender_1.aNbMsgToSend);
    ASSERT_EQ(msgRxError, 0);
    msgSender_2.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, msgSender_2.aNbMsgToSend);
    ASSERT_EQ(msgRx, msgSender_2.aNbMsgToSend);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, msgSender_2.aNbMsgToSend);
    ASSERT_EQ(msgRxError, 0);
}


TEST_F(TestJsonCommunication, TestBigJsonCommand) {
    std::shared_ptr<JsonCommunication> jsonCom = std::make_shared<JsonCommunication>();
    TestJsonComListener listener;
    TestJsonComSenderWrapper msgSender;
    msgSender.aCom = jsonCom;
    msgSender.aIsCmd = true;
    msgSender.aTypeMethod = "command";
    std::string jsnAttributeBig("test");
    std::string strBigNumber("123");
    jsnAttributeBig.insert(4, 40000, 'x');
    strBigNumber.insert(3, 40000, 'a');
    spjson bigJsonCmd = std::make_shared<nlohmann::json>();
    *bigJsonCmd = {{jsnAttributeBig,  strBigNumber}};
    std::string bigJsonCmdStr = bigJsonCmd->dump();
    msgSender.aJsonToSendPtr = bigJsonCmd;
    msgSender.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSender.aRandomDelay = false;
    msgSender.aNbMsgToSend = 1;
    msgSender.aFixDelayMs = 10;

    jsonCom->setVerbose(true);
    jsonCom->setListener(&listener);

    // simulate Maestro receiving a command from GUI
    jsonCom->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonCom->start();

    int bigJsonCmdStrSz = bigJsonCmdStr.length();
    ASSERT_EQ(bigJsonCmdStrSz > 80000, true);

    msgSender.startSending();
    sleep(5);    // wait to receive messages
    //jsonCom->stopListening();

    int nbMsgReceivedPortGui = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortGui, 1);
    ASSERT_EQ(msgSender.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , 1);

    std::string receivedJsonStr = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].at(0);
    ASSERT_EQ(receivedJsonStr.size(), bigJsonCmdStrSz);
    ASSERT_EQ(bigJsonCmdStr, receivedJsonStr);

    uint msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError;
    msgSender.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, msgSender.aNbMsgToSend);
    ASSERT_EQ(msgRx, msgSender.aNbMsgToSend);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, msgSender.aNbMsgToSend);
    ASSERT_EQ(msgRxError, 0);
}

TEST_F(TestJsonCommunication, TestListen2Ports) {
    std::shared_ptr<JsonCommunication> jsonCom = std::make_shared<JsonCommunication>();
    TestJsonComListener listener;

    TestJsonComSenderWrapper msgSenderGui;
    msgSenderGui.aCom = jsonCom;
    msgSenderGui.aIsCmd = true;
    msgSenderGui.aTypeMethod = "command";
    msgSenderGui.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSenderGui.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSenderGui.aRandomDelay = false;
    msgSenderGui.aNbMsgToSend = 10;
    msgSenderGui.aFixDelayMs = 10;
    TestJsonComSenderWrapper msgSenderComMgr;
    msgSenderComMgr.aCom = jsonCom;
    msgSenderComMgr.aIsCmd = true;
    msgSenderComMgr.aTypeMethod = "command";
    msgSenderComMgr.aJsonToSendPtr = createCommandJson("pump2", "off");
    msgSenderComMgr.aDestPort = eJsonComPort::COM_MGR_CMD_PORT;
    msgSenderComMgr.aRandomDelay = false;
    msgSenderComMgr.aNbMsgToSend = 20;
    msgSenderComMgr.aFixDelayMs = 10;

    jsonCom->setVerbose(true);
    jsonCom->setListener(&listener);
    // simulate Maestro receiving a command from GUI and a command from Com MGR
    jsonCom->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonCom->listenOnChannel(eJsonComPort::COM_MGR_CMD_PORT);
    jsonCom->start();

    msgSenderGui.startSending();
    msgSenderComMgr.startSending();


    sleep(4); //wait to receive messages
    //jsonCom->stopListening(); // can block - up to  2 seconds

    int nbMsgReceivedPortGuiCmd = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortGuiCmd, msgSenderGui.aNbMsgToSend);
    ASSERT_EQ(msgSenderGui.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , msgSenderGui.aNbMsgToSend);
    int nbMsgReceivedPortComCmd = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::COM_MGR_CMD_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortComCmd, msgSenderComMgr.aNbMsgToSend);
    ASSERT_EQ(msgSenderComMgr.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , msgSenderComMgr.aNbMsgToSend);

    uint msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError;
    msgSenderGui.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, msgSenderGui.aNbMsgToSend);
    ASSERT_EQ(msgRx, msgSenderGui.aNbMsgToSend);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, msgSenderGui.aNbMsgToSend);
    ASSERT_EQ(msgRxError, 0);
    msgSenderComMgr.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, msgSenderComMgr.aNbMsgToSend);
    ASSERT_EQ(msgRx, msgSenderComMgr.aNbMsgToSend);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, msgSenderComMgr.aNbMsgToSend);
    ASSERT_EQ(msgRxError, 0);
}

/**
 * This test takes much too long, enable it to test punctually.
 */
TEST_F(TestJsonCommunication, DISABLED_TestStartStopListening) {
    const int nbLoop = 25;
    std::shared_ptr<JsonCommunication> jsonCom = std::make_shared<JsonCommunication>();
    TestJsonComListener listener;
    TestJsonComSenderWrapper msgSenderGui;
    msgSenderGui.aCom = jsonCom;
    msgSenderGui.aIsCmd = true;
    msgSenderGui.aTypeMethod = "command";
    msgSenderGui.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSenderGui.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSenderGui.aRandomDelay = false;
    msgSenderGui.aNbMsgToSend = 1;
    msgSenderGui.aFixDelayMs = 10;
    TestJsonComSenderWrapper msgSenderComMgr;
    msgSenderComMgr.aCom = jsonCom;
    msgSenderComMgr.aIsCmd = true;
    msgSenderComMgr.aTypeMethod = "command";
    msgSenderComMgr.aJsonToSendPtr = createCommandJson("pump2", "off");
    msgSenderComMgr.aDestPort = eJsonComPort::COM_MGR_CMD_PORT;
    msgSenderComMgr.aRandomDelay = false;
    msgSenderComMgr.aNbMsgToSend = 1;
    msgSenderComMgr.aFixDelayMs = 10;

    jsonCom->setVerbose(true);
    jsonCom->setListener(&listener);
    // simulate Maestro receiving a command from GUI and a command from Com MGR
    jsonCom->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonCom->listenOnChannel(eJsonComPort::COM_MGR_CMD_PORT);

    for (int iLoop = 1; iLoop < nbLoop+1; iLoop++) {
        jsonCom->start();
        msgSenderGui.startSending();
        msgSenderComMgr.startSending();
        sleep(2); //wait to receive messages
        jsonCom->stop(); // can block - up to  2 seconds

        int nbMsgReceivedPortGuiCmd = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size();
        ASSERT_EQ(nbMsgReceivedPortGuiCmd, 1);
        ASSERT_EQ(msgSenderGui.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , iLoop);
        int nbMsgReceivedPortComCmd = listener.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::COM_MGR_CMD_PORT)].size();
        ASSERT_EQ(nbMsgReceivedPortComCmd, 1);
        ASSERT_EQ(msgSenderComMgr.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , iLoop);
        listener.aMsgReceivedPerPort.clear();
    }

    uint msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError;
    msgSenderGui.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, nbLoop*msgSenderGui.aNbMsgToSend);
    ASSERT_EQ(msgRx, nbLoop*msgSenderGui.aNbMsgToSend);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, nbLoop*msgSenderGui.aNbMsgToSend);
    ASSERT_EQ(msgRxError, 0);
    msgSenderComMgr.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, nbLoop*msgSenderComMgr.aNbMsgToSend);
    ASSERT_EQ(msgRx, nbLoop*msgSenderComMgr.aNbMsgToSend);
    ASSERT_EQ(msgTxNotRx, 0);
    ASSERT_EQ(msgRxOk, nbLoop*msgSenderComMgr.aNbMsgToSend);
    ASSERT_EQ(msgRxError, 0);
}

/**
 * This test depends on the previously ran tests. Disabling.
 */
TEST_F(TestJsonCommunication, DISABLED_TestClosedSocket) {
    const int nbLoop = 5;
    std::shared_ptr<JsonCommunication> jsonComMaestro = std::make_shared<JsonCommunication>("maestro");
    std::shared_ptr<JsonCommunication> jsonComGui = std::make_shared<JsonCommunication>("gui");
    TestJsonComListener listenerMaestro, listenerGui;
    TestJsonComSenderWrapper msgSenderMaestro;
    msgSenderMaestro.aCom = jsonComMaestro;
    msgSenderMaestro.aIsCmd = false;
    msgSenderMaestro.aTypeMethod = "data";
    msgSenderMaestro.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSenderMaestro.aDestPort = eJsonComPort::GUI_DATA_PORT;
    msgSenderMaestro.aRandomDelay = false;
    msgSenderMaestro.aNbMsgToSend = 2;
    msgSenderMaestro.aFixDelayMs = 5;


    jsonComMaestro->setVerbose(true);
    jsonComMaestro->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonComMaestro->setListener(&listenerMaestro);

    jsonComGui->setVerbose(false);
    jsonComGui->listenOnChannel(eJsonComPort::GUI_DATA_PORT);
    jsonComGui->setListener(&listenerGui);
    // simulate Maestro receiving a command from GUI and then GUI stops


    jsonComMaestro->start();
    for (int iLoop = 1; iLoop < nbLoop+1; iLoop++) {
        jsonComGui->start();
        msgSenderMaestro.startSending();
        usleep(500*1000); //wait to receive messages
        jsonComGui->stop(); // can block - up to  2 seconds
    }
    std::string nbSockCloseWaitStr = executeCommand("netstat -t | grep CLOSE_WAIT | wc -l");
    int nbSocketCloseWait = std::stoi( nbSockCloseWaitStr );
    // We shall not have plenty of Close_Wait socket
    // we can have TIME_WAIT though, meaning the socket was closed and socket will be free in 60 seconds
    ASSERT_EQ(nbSocketCloseWait < 2, true);

    jsonComMaestro->stop();
}

/**
 * Disabling this test since it is dependent on previous tests.
 */
TEST_F(TestJsonCommunication, DISABLED_TestGuiSendHeartbeatNoMaestro) {
    std::shared_ptr<JsonCommunication> jsonComGui = std::make_shared<JsonCommunication>("gui");
    TestJsonComListener listenerGui;
    TestJsonComSenderWrapper msgSenderGui;
    msgSenderGui.aCom = jsonComGui;
    msgSenderGui.aIsCmd = true;
    msgSenderGui.aTypeMethod = "heartbeat";
    msgSenderGui.aJsonToSendPtr = createCommandJson("test", "tset");
    msgSenderGui.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSenderGui.aRandomDelay = false;
    msgSenderGui.aNbMsgToSend = 10;
    msgSenderGui.aFixDelayMs = 1000;

    jsonComGui->setVerbose(false);
    jsonComGui->setListener(&listenerGui);
    // simulate Maestro receiving a command from GUI and then GUI stops

    jsonComGui->start();
    msgSenderGui.startSending(false);
    jsonComGui->stop();

    std::string nbSock44400Str = executeCommand("netstat -t | grep localhost:44400 | wc -l");
    int nbSocket44400 = std::stoi( nbSock44400Str );
    ASSERT_EQ(nbSocket44400, 0);
}

TEST_F(TestJsonCommunication, TestDegradedNoRemoteApp)
{
    std::shared_ptr<JsonCommunication> jsonCom_MaestroGUI = std::make_shared<JsonCommunication>("mtest");
    TestJsonComSenderWrapper msgSender;
    msgSender.aCom = jsonCom_MaestroGUI;
    msgSender.aIsCmd = true;
    msgSender.aTypeMethod = "command";
    msgSender.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSender.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSender.aRandomDelay = false;
    msgSender.aNbMsgToSend = 1;
    msgSender.aFixDelayMs = 10;

    jsonCom_MaestroGUI->start(true, false);
    msgSender.startSending();
    sleep(2); //wait to receive messages

    uint msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError;
    msgSender.CountResults(msgTx,msgRx, msgTxNotRx, msgRxOk, msgRxError);
    ASSERT_EQ(msgTx, 1);
    ASSERT_EQ(msgRx, 0);
    ASSERT_EQ(msgTxNotRx, 1);
    ASSERT_EQ(msgRxOk, 0);
    ASSERT_EQ(msgRxError, 0);
    ASSERT_EQ(msgSender.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , 0);
    ASSERT_EQ(msgSender.aMapStatus[eJsonComErr::JSON_COM_TX_ERR_CONNECTION] , 1);
}

TEST_F(TestJsonCommunication, TestTimeout) {
    std::shared_ptr<JsonCommunication> jsonMaestro = std::make_shared<JsonCommunication>("mpro");
    std::shared_ptr<JsonCommunication> jsonGui = std::make_shared<JsonCommunication>("mgui");
    TestJsonComListener listenerMaestro;
    TestJsonComSenderWrapper msgSenderGui;
    msgSenderGui.aCom = jsonGui;
    msgSenderGui.aIsCmd = true;
    msgSenderGui.aTypeMethod = "command";
    msgSenderGui.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSenderGui.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSenderGui.aRandomDelay = false;
    msgSenderGui.aNbMsgToSend = 1;
    msgSenderGui.aFixDelayMs = 10;

    // simulate Maestro receiving a command from GUI
    jsonMaestro->setVerbose(true);
    jsonMaestro->setListener(&listenerMaestro);
    jsonMaestro->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonMaestro->start();
    jsonGui->start();

    listenerMaestro.aNbSecondSleepInHandler = 2;
    msgSenderGui.aTimeoutMs = 4000;
    msgSenderGui.startSending();
    sleep(6);
    int nbMsgReceivedPortGui = listenerMaestro.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size();
    ASSERT_EQ(nbMsgReceivedPortGui, 1); // maestro did receive the message, even if it blocked too long which raised the timeout on GUI side
    ASSERT_EQ(msgSenderGui.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , 1);
    ASSERT_EQ(msgSenderGui.aMapStatus[eJsonComErr::JSON_COM_RX_TIMEOUT] , 0);

    listenerMaestro.aNbSecondSleepInHandler = 6;
    msgSenderGui.aTimeoutMs = 2000;
    msgSenderGui.startSending();
    sleep(4);
    ASSERT_EQ(msgSenderGui.aMapStatus[eJsonComErr::JSON_COM_RX_TIMEOUT] , 1);
}

/**
 * This test takes much too long, enable it to test punctually.
 */
TEST_F(TestJsonCommunication, DISABLED_TestRealScenario)
{
    const int nbMaxLoop = 15;
    std::shared_ptr<spdlog::logger> logger = LoggerFactory::getLogger("gtest");

    std::shared_ptr<JsonCommunication> jsonCom_MaestroPro = std::make_shared<JsonCommunication>("mpro");
    TestJsonComListener listenerMpro;
    jsonCom_MaestroPro->setVerbose(true);
    jsonCom_MaestroPro->setListener(&listenerMpro);
    TestJsonComSenderWrapper msgSenderMaestroToGui;
    msgSenderMaestroToGui.aCom = jsonCom_MaestroPro;
    msgSenderMaestroToGui.aIsCmd = false;
    msgSenderMaestroToGui.aTypeMethod = "testType";
    msgSenderMaestroToGui.aJsonToSendPtr = createDataUsageJson(821);
    msgSenderMaestroToGui.aDestPort = eJsonComPort::GUI_DATA_PORT;
    msgSenderMaestroToGui.aRandomDelay = false;
    msgSenderMaestroToGui.aNbMsgToSend = 5;
    msgSenderMaestroToGui.aFixDelayMs = 0;
    TestJsonComSenderWrapper msgSenderMaestroToCom;
    msgSenderMaestroToCom.aCom = jsonCom_MaestroPro;
    msgSenderMaestroToCom.aIsCmd = false;
    msgSenderMaestroToCom.aTypeMethod = "statistics";
    msgSenderMaestroToCom.aJsonToSendPtr = createDataUsageJson(123);
    msgSenderMaestroToCom.aDestPort = eJsonComPort::COM_MGR_DATA_PORT;
    msgSenderMaestroToCom.aRandomDelay = false;
    msgSenderMaestroToCom.aNbMsgToSend = 15;
    msgSenderMaestroToCom.aFixDelayMs = 0;

    std::shared_ptr<JsonCommunication> jsonCom_MaestroGUI = std::make_shared<JsonCommunication>("mgui");
    TestJsonComListener listenerMgui;
    jsonCom_MaestroGUI->setVerbose(true);
    jsonCom_MaestroGUI->setListener(&listenerMgui);
    TestJsonComSenderWrapper msgSenderGui;
    msgSenderGui.aCom = jsonCom_MaestroGUI;
    msgSenderGui.aIsCmd = true;
    msgSenderGui.aTypeMethod = "command";
    msgSenderGui.aJsonToSendPtr = createCommandJson("pump1", "manu");
    msgSenderGui.aDestPort = eJsonComPort::GUI_CMD_PORT;
    msgSenderGui.aRandomDelay = false;
    msgSenderGui.aNbMsgToSend = 8;
    msgSenderGui.aFixDelayMs = 0;

    std::shared_ptr<JsonCommunication> jsonCom_ComMgr = std::make_shared<JsonCommunication>("comMgr");
    TestJsonComListener listenerComMgr;
    jsonCom_ComMgr->setVerbose(true);
    jsonCom_ComMgr->setListener(&listenerComMgr);
    TestJsonComSenderWrapper msgSenderComMgr;
    msgSenderComMgr.aCom = jsonCom_ComMgr;
    msgSenderComMgr.aIsCmd = true;
    msgSenderComMgr.aTypeMethod = "command";
    msgSenderComMgr.aJsonToSendPtr = createCommandJson("mode", "modevalue");
    msgSenderComMgr.aDestPort = eJsonComPort::COM_MGR_CMD_PORT;
    msgSenderComMgr.aRandomDelay = true;
    msgSenderComMgr.aNbMsgToSend = 12;
    msgSenderComMgr.aFixDelayMs = 60;


    // Maestro Pro register to cmd channel maestro GUI register to data channel
    jsonCom_MaestroPro->listenOnChannel(eJsonComPort::GUI_CMD_PORT);
    jsonCom_MaestroPro->listenOnChannel(eJsonComPort::COM_MGR_CMD_PORT);
    jsonCom_MaestroGUI->listenOnChannel(eJsonComPort::GUI_DATA_PORT);
    jsonCom_ComMgr->listenOnChannel(eJsonComPort::COM_MGR_DATA_PORT);
    jsonCom_MaestroPro->start();
    jsonCom_MaestroGUI->start();
    jsonCom_ComMgr->start();


    for (int iLoop = 1; iLoop < (1+nbMaxLoop); iLoop++) {
        logger->info("loop {} - start send", iLoop);
        msgSenderMaestroToGui.startSending(true, true);
        msgSenderMaestroToCom.startSending(true, true);
        msgSenderGui.startSending(true, true);
        msgSenderComMgr.startSending(true, true);

        logger->info("loop {} - start sleep", iLoop);
        sleep(10); //wait to receive messages
        logger->info("loop {} - start check", iLoop);

        // check listener got the message
        ASSERT_EQ(listenerMpro.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_CMD_PORT)].size(), iLoop * msgSenderGui.aNbMsgToSend);
        ASSERT_EQ(listenerMpro.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::COM_MGR_CMD_PORT)].size(), iLoop * msgSenderComMgr.aNbMsgToSend);
        ASSERT_EQ(listenerMgui.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::GUI_DATA_PORT)].size(), iLoop * msgSenderMaestroToGui.aNbMsgToSend);
        ASSERT_EQ(listenerComMgr.aMsgReceivedPerPort[static_cast<int>(eJsonComPort::COM_MGR_DATA_PORT)].size(), iLoop * msgSenderMaestroToCom.aNbMsgToSend);

        // verify send was acknowledged
        ASSERT_EQ(msgSenderMaestroToCom.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , iLoop*msgSenderMaestroToCom.aNbMsgToSend);
        ASSERT_EQ(msgSenderMaestroToGui.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , iLoop*msgSenderMaestroToGui.aNbMsgToSend);
        ASSERT_EQ(msgSenderComMgr.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , iLoop*msgSenderComMgr.aNbMsgToSend);
        ASSERT_EQ(msgSenderGui.aMapStatus[eJsonComErr::JSON_COM_NO_ERR] , iLoop*msgSenderGui.aNbMsgToSend);
    }

    sleep(2); //wait to receive messages

}


#pragma clang diagnostic pop