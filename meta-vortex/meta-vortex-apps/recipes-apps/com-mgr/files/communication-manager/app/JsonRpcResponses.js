"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class JsonRpcResponses {
    /**
     *
     * @param type
     * @param {string} command
     * @returns {string}
     */
    static unknownMessageType(type, command) {
        const jsonRpcMessage = JSON.parse(command);
        return JSON.stringify({
            error: {
                code: -32009,
                data: `Unknown message type: ${type}`,
                message: "Server error",
            },
            id: jsonRpcMessage.id,
            jsonrpc: "2.0",
        });
    }
    /**
     *
     * @param {{id: string | number; params: {}}} message
     * @returns {string}
     */
    static unknownUpdateType(message) {
        return JSON.stringify({
            error: {
                code: -32010,
                data: `Unknown update type: ${JSON.stringify(message.params)}`,
                message: "Server error",
            },
            id: message.id,
            jsonrpc: "2.0",
        });
    }
    /**
     *
     * @param messageId
     * @param errorMessage
     * @returns {string}
     */
    static generalError(messageId, errorMessage) {
        return JSON.stringify({
            error: {
                code: -32010,
                data: `Unable to complete update: ${JSON.stringify(errorMessage)}`,
                message: "Server error",
            },
            id: messageId,
            jsonrpc: "2.0",
        });
    }
    /**
     *
     * @param {string} command
     * @param {boolean} error
     * @param {string} host
     * @param {number} port
     * @returns {string}
     */
    static cannotConnectToMaestro(command, error, host, port) {
        const jsonRpcMessage = JSON.parse(command);
        return JSON.stringify({
            data: { hadError: error },
            error: {
                code: -32011,
                data: `Cannot connect to ${host}:${port}`,
                message: "Server error",
            },
            id: jsonRpcMessage.id,
            jsonrpc: "2.0",
        });
    }
    /**
     *
     * @param {string} command
     * @returns {string}
     */
    static noResponseFromMaestro(command) {
        const jsonRpcMessage = JSON.parse(command);
        return JSON.stringify({
            error: {
                code: -32012,
                data: "Command sent to Maestro, but no response was received.",
                message: "Server error",
            },
            id: jsonRpcMessage.id,
            jsonrpc: "2.0",
        });
    }
    /**
     *
     * @param {string} resultOfUpdate
     * @returns {string}
     */
    static successfulUpdate(resultOfUpdate) {
        return JSON.stringify({
            id: 1,
            jsonrpc: "2.0",
            result: resultOfUpdate,
        });
    }
    /**
     *
     * @param {string} command
     * @returns {string}
     */
    static commandSentToMaestro(command) {
        const jsonRpcMessage = JSON.parse(command);
        return JSON.stringify({
            id: jsonRpcMessage.id,
            jsonrpc: "2.0",
            result: "Command sent to Maestro.",
        });
    }
    static generalSuccess(res) {
        return JSON.stringify({
            id: 1,
            jsonrpc: "2.0",
            result: res,
        });
    }
    static debug(res) {
        return JSON.stringify({
            id: 1,
            jsonrpc: "2.0",
            result: res,
        });
    }
}
exports.JsonRpcResponses = JsonRpcResponses;
