"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
Object.defineProperty(exports, "__esModule", { value: true });
const awsIot = require("aws-iot-device-sdk");
const fs = require("fs");
const path = require("path");
const FirmwareCommand_1 = require("./FirmwareCommand");
const IotPlatformMessage_1 = require("./IotPlatformMessage");
const JsonRpcResponses_1 = require("./JsonRpcResponses");
const Logger_1 = require("./Logger");
const MaestroVersion_1 = require("./MaestroVersion");
const MessagesHandler_1 = require("./MessagesHandler");
const SystemMessagesHandler_1 = require("./SystemMessagesHandler");
const TcpServer_1 = require("./TcpServer");
const UpdatesHandler_1 = require("./UpdatesHandler");
const config = require("config");
function communicationManager() {
    const version = '3.0.6';
    const tmpFile = path.resolve(config.get("production.comManager.path"), "update-progress.json");
    const PORT = Number.parseInt(config.get("production.maestro.port"), 10);
    const HOST = config.get("production.maestro.host");
    const TIMEOUT = 30000; // milliseconds
    const tcpPort = parseInt(config.get("production.comManager.port"), 10);
    const mClientId = config.get("production.iot.clientId");
    const tcpServer = new TcpServer_1.TcpServer(tcpPort);
    const TOPICS_TO_SUBSCRIBE = [
        mClientId + "/commands/+",
        mClientId + "/updates/+",
        mClientId + "/system/+",
    ];
    const THING_NAME = config.get("production.iot.thingName");
    const messagesQueue = [];
    const mThingShadow = new awsIot.thingShadow({
        caPath: path.join(config.get("production.comManager.configFolderPath"), config.get("production.iot.caPath")),
        certPath: path.join(config.get("production.comManager.configFolderPath"), config.get("production.iot.certificatePath")),
        clean: true,
        clientId: mClientId,
        debug: false,
        host: config.get("production.iot.host"),
        keyPath: path.join(config.get("production.comManager.configFolderPath"), config.get("production.iot.privateKeyPath")),
        keepalive: 40,
    });
    Logger_1.Logger.debug("Connecting with client ID: ", mClientId);
    function clearSavedState() {
        fs.writeFileSync(tmpFile, JSON.stringify({}), { encoding: "utf8" });
    }
    function reportSavedState() {
        try {
            const obj = JSON.parse(fs.readFileSync(tmpFile, { encoding: "utf8" }).toString());
            Object.keys(obj).forEach((topic) => {
                const message = IotPlatformMessage_1.IotPlatformMessage.fromString(obj[topic]);
                if (message.isJsonRpcResult()) {
                    mThingShadow.publish(topic, obj[topic], { qos: 1 }, (err) => {
                        Logger_1.Logger.debug("Can't send message: ", err);
                    });
                }
            });
        }
        catch (err) {
            Logger_1.Logger.debug("reportSavedState: ", err);
        }
        finally {
            clearSavedState();
        }
    }
    function reportMaestroVersion() {
        MaestroVersion_1.MaestroVersion().then((mVersion) => {
            Logger_1.Logger.debug("Maestro Version: ", mVersion);
            const state = { meta: { firmware_version: mVersion } };
            messagesQueue.push({ state: { reported: state } });
        }).catch((err) => {
            Logger_1.Logger.debug("Can't get Maestro version");
        });
    }
    mThingShadow.on("connect", () => {
        // subscribe to messages
        mThingShadow.subscribe(TOPICS_TO_SUBSCRIBE, { qos: 1 }, (err, granted) => {
            Logger_1.Logger.debug("subscribed to : ", granted);
        });
        mThingShadow.register(THING_NAME, {}, () => {
            Logger_1.Logger.debug("registered to : ", THING_NAME);
            const getStateCommand = JSON.stringify({
                id: Math.floor(Math.random() * 9001 + 1000),
                jsonrpc: "2.0",
                method: "shadow",
                params: {},
            });
            const firmwareCommand = new FirmwareCommand_1.FirmwareCommand(HOST, PORT, TIMEOUT);
            firmwareCommand.send(getStateCommand, 10, true).then((res) => {
                const state = JSON.parse(res);
                if (!state.hasOwnProperty("meta")) {
                    state.meta = {};
                }
                state.meta.comm_manager_version = version;
                messagesQueue.push({
                    state: {
                        reported: {
                            "activators": null,
                            "activators-logic": null,
                            "bypass": null,
                            "featureloop": null,
                            "features": null,
                            "filtrationloop": null,
                            "rain-diverter": null,
                            "schedule": null,
                        },
                    },
                });
                messagesQueue.push({ state: { reported: state } });
                reportMaestroVersion();
            }).catch((err) => {
                const state = { meta: { comm_manager_version: version } };
                messagesQueue.push({ state: { reported: state } });
                Logger_1.Logger.debug("register: ", err);
                reportMaestroVersion();
            });
            reportSavedState();
        });
    });
    mThingShadow.on("status", (thingName, stat, clientToken, stateObject) => {
        Logger_1.Logger.debug("status", thingName, stat, clientToken, stateObject);
    });
    mThingShadow.on("timeout", (thingName, clientToken) => {
        Logger_1.Logger.debug("timeout", thingName, clientToken);
    });
    mThingShadow.on("reconnect", () => {
        Logger_1.Logger.debug("reconnect");
    });
    mThingShadow.on("close", () => {
        Logger_1.Logger.debug("close");
    });
    mThingShadow.on("offline", () => {
        Logger_1.Logger.debug("offline");
    });
    mThingShadow.on("error", (error) => {
        Logger_1.Logger.debug("error: ", error);
    });
    /**
     * Handle messages from IoT Platform
     */
    mThingShadow.on("message", (topic, payload) => __awaiter(this, void 0, void 0, function* () {
        // match topic name: {CLIENT_ID}/{TYPE}/{MESSAGE_ID}
        const regExp = new RegExp(`^${mClientId}\/(.*)\/[a-zA-Z0-9]+$`, "gui");
        const match = regExp.exec(topic);
        if (match === null) {
            Logger_1.Logger.debug("Unknown topic: ", topic);
        }
        else {
            try {
                const message = IotPlatformMessage_1.IotPlatformMessage.fromString(payload.toString().trim());
                Logger_1.Logger.debug("New message", JSON.stringify(message));
                if (message.isJsonRpcResult()) {
                    // Communication manager will receive its own responses, just ignore them
                }
                else if (message.isJsonRpcCommand()) {
                    const result = yield handleMessage(match[1], topic, payload.toString().trim());
                    Logger_1.Logger.debug(`Message ${JSON.stringify(message)} processes with result ${result}`);
                    mThingShadow.publish(topic, result, { qos: 1 }, (err) => {
                        if (err) {
                            Logger_1.Logger.debug("Can't send message: ", err);
                        }
                        else {
                            clearSavedState();
                        }
                    });
                }
                else {
                    Logger_1.Logger.debug("Unknown message type", message);
                }
            }
            catch (err) {
                clearSavedState();
                Logger_1.Logger.debug(JSON.stringify(err));
                if (err instanceof Error) {
                    mThingShadow.publish(topic, JsonRpcResponses_1.JsonRpcResponses.generalError(null, err.message), { qos: 1 }, (err) => {
                        if (err) {
                            Logger_1.Logger.debug("message: ", err);
                        }
                    });
                }
                else {
                    mThingShadow.publish(topic, JsonRpcResponses_1.JsonRpcResponses.generalError(null, err), { qos: 1 }, (err) => {
                        if (err) {
                            Logger_1.Logger.debug("message: ", err);
                        }
                    });
                }
            }
        }
    }));
    /**
     *
     * @param {string} type
     * @param {string} topic
     * @param {string} msg
     * @returns {Promise<string>}
     */
    function handleMessage(type, topic, msg) {
        switch (type) {
            case "updates":
                return UpdatesHandler_1.handleUpdateMessage(msg, topic, tmpFile, (status) => __awaiter(this, void 0, void 0, function* () {
                    return new Promise((resolve, reject) => {
                        mThingShadow.publish(topic, status, { qos: 1 }, (err) => {
                            if (err) {
                                Logger_1.Logger.debug("Can't send message: ", err);
                            }
                            resolve();
                        });
                    });
                }));
            case "commands":
                const firmwareCommand = new FirmwareCommand_1.FirmwareCommand(HOST, PORT, TIMEOUT);
                return firmwareCommand.send(msg, 10, true);
            case "system":
                return SystemMessagesHandler_1.handleSystemMessage(msg);
        }
        return new Promise((resolve, reject) => {
            reject(JsonRpcResponses_1.JsonRpcResponses.unknownMessageType(type, msg));
        });
    }
    /**
     * Handle messages from firmware
     */
    const messagesHandler = new MessagesHandler_1.MessagesHandler(mClientId, mThingShadow, messagesQueue);
    tcpServer.start((msg) => {
        Logger_1.Logger.debug("TCP message", msg);
        return messagesHandler.handleMessage(msg);
    });
    const intervalId = setInterval(() => {
        if (messagesQueue.length > 0) {
            const msq = messagesQueue.shift();
            const res = mThingShadow.update(THING_NAME, msq);
            if (res === null) {
                Logger_1.Logger.debug("update shadow failed, operation still in progress");
                messagesQueue.unshift(msq);
            }
        }
    }, 100);
    const pingInterval = setInterval(() => {
        mThingShadow.publish(`${mClientId}/ping`, JSON.stringify({ online: true }), { qos: 1 }, (err) => {
            if (err) {
                Logger_1.Logger.debug("Can't send message: ", err);
            }
        });
    }, 60000);
}
exports.communicationManager = communicationManager;
