"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class IotPlatformMessage {
    constructor(message) {
        this.message = message;
    }
    static fromString(msg) {
        const message = JSON.parse(msg);
        if (!message.hasOwnProperty("jsonrpc")) {
            throw new Error("Invalid message");
        }
        return new IotPlatformMessage(message);
    }
    isJsonRpcResult() {
        return (this.message.hasOwnProperty("result"));
    }
    isJsonRpcCommand() {
        return (this.message.hasOwnProperty("method") && this.message.hasOwnProperty("params"));
    }
    isTestConnectionCommand() {
        return this.isJsonRpcCommand() && this.message.method === "test_link_connection";
    }
    getCommandId() {
        return this.message.id || null;
    }
}
exports.IotPlatformMessage = IotPlatformMessage;
