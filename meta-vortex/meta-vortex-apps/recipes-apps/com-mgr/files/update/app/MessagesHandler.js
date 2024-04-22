"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const IotPlatformMessage_1 = require("./IotPlatformMessage");
const Logger_1 = require("./Logger");
const MaestroMessage_1 = require("./MaestroMessage");
const PromiseTimeout_1 = require("./PromiseTimeout");
class MessagesHandler {
    constructor(mClientId, mThingShadow, messagesQueue) {
        this.mClientId = mClientId;
        this.mThingShadow = mThingShadow;
        this.messagesQueue = messagesQueue;
    }
    handleMessage(msg) {
        try {
            const maestroMessage = MaestroMessage_1.MaestroMessage.fromString(msg);
            const topic = `${this.mClientId}/${maestroMessage.getDataAttribute()}`;
            // Simply relay the message to the IoT Platform
            this.mThingShadow.publish(topic, this.replaceEmptyStrings(msg), { qos: 1 }, (err) => {
                if (err) {
                    Logger_1.Logger.debug("Can't send message: ", err);
                }
            });
            // Update thing shadow if necessary
            if (maestroMessage.mustUpdateShadow()) {
                this.messagesQueue.push(maestroMessage.toShadowObject());
            }
        }
        catch (err) {
            Logger_1.Logger.debug("TCP server error", err);
            try {
                const message = IotPlatformMessage_1.IotPlatformMessage.fromString(msg);
                const topic = `${this.mClientId}/debug/${Math.floor(Math.random() * 9001 + 1000)}`;
                if (message.isTestConnectionCommand()) {
                    const debugMessagePromise = new Promise((resolve, reject) => {
                        this.mThingShadow.publish(topic, msg, { qos: 1 }, (err) => {
                            if (err) {
                                reject(message.getCommandId());
                            }
                            else {
                                resolve(message.getCommandId());
                            }
                        });
                    });
                    return PromiseTimeout_1.promiseTimeout(30000, message.getCommandId(), debugMessagePromise);
                }
            }
            catch (e) {
                Logger_1.Logger.debug("TCP server error", e);
            }
        }
    }
    /**
     * DynamoDB cannot save empty strings. Replace them with spaces
     *
     * @param {string} msg
     * @returns {string}
     */
    replaceEmptyStrings(msg) {
        const reg = new RegExp("\"\"", "g");
        return msg.replace(reg, "\" \"");
    }
}
exports.MessagesHandler = MessagesHandler;
