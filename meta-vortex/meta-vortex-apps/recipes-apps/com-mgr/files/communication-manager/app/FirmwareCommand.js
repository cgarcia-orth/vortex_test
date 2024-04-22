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
const net_1 = require("net");
const JsonRpcResponses_1 = require("./JsonRpcResponses");
const Logger_1 = require("./Logger");
function delay(milliseconds) {
    return __awaiter(this, void 0, void 0, function* () {
        return new Promise((resolve) => {
            setTimeout(resolve, milliseconds);
        });
    });
}
class FirmwareCommand {
    constructor(host, port, timeout) {
        this.reconnectInterval = 1000;
        this.process = false;
        this.state = "HEADER";
        this.payloadLength = 0;
        this.bufferedBytes = 0;
        this.queue = [];
        this.headerRegex = /SZ=(\d{10});/mi;
        this.host = host;
        this.port = port;
        this.timeout = timeout;
    }
    /**
     * Send command to Maestro Firmware through TCP socket
     *
     * @param {string} command
     * @param {number} numberOrRetries
     * @param {boolean} waitForResponse
     * @returns {Promise<string>}
     */
    send(command, numberOrRetries = 0, waitForResponse = false) {
        return new Promise((resolve, reject) => {
            const sock = new net_1.Socket();
            this.socket = sock;
            sock.setTimeout(this.timeout, () => {
                sock.destroy(); // kill client after timeout
                reject(JsonRpcResponses_1.JsonRpcResponses.noResponseFromMaestro(command));
            });
            sock.on("connect", () => {
                this.reconnectInterval = 1000;
                numberOrRetries = 0;
                const length = 10;
                const l = command.length.toString(10);
                const header = `SZ=${l.padStart(length, "0")};`;
                sock.write(header + command);
                if (!waitForResponse) {
                    sock.destroy(); // kill client
                    resolve(JsonRpcResponses_1.JsonRpcResponses.commandSentToMaestro(command));
                }
                // wait for response from firmware, see `sock.on("data")` below
            });
            sock.connect(this.port, this.host);
            sock.on("error", (err) => {
                Logger_1.Logger.debug(err);
            });
            sock.on("data", (data) => {
                this.bufferedBytes += data.length;
                this.queue.push(data);
                this.process = true;
                this.onData();
            });
            sock.on("served", (data) => {
                sock.destroy(); // kill client after server's response
                resolve(data.toString().trim());
            });
            sock.on("close", (error) => __awaiter(this, void 0, void 0, function* () {
                if (numberOrRetries > 0) {
                    yield delay(this.reconnectInterval);
                    if (this.reconnectInterval < 60000) {
                        this.reconnectInterval += 100;
                    }
                    numberOrRetries--;
                    sock.connect(this.port, this.host);
                }
                else {
                    reject(JsonRpcResponses_1.JsonRpcResponses.cannotConnectToMaestro(command, error, this.host, this.port));
                }
            }));
        });
    }
    onData() {
        while (this.process) {
            switch (this.state) {
                case "HEADER":
                    this.getHeader();
                    break;
                case "PAYLOAD":
                    this.getPayload();
                    break;
            }
        }
    }
    getHeader() {
        if (this.hasEnough(14)) {
            const header = this.readBytes(14).toString();
            const match = header.match(this.headerRegex);
            if (match) {
                this.payloadLength = parseInt(match[1], 10);
                this.state = "PAYLOAD";
            }
            else {
                // TODO
            }
        }
    }
    getPayload() {
        if (this.hasEnough(this.payloadLength)) {
            const received = this.readBytes(this.payloadLength);
            this.socket.emit("served", received);
            this.state = "HEADER";
        }
    }
    hasEnough(size) {
        if (this.bufferedBytes >= size) {
            return true;
        }
        this.process = false;
        return false;
    }
    readBytes(size) {
        let result;
        this.bufferedBytes -= size;
        if (size === this.queue[0].length) {
            return this.queue.shift();
        }
        if (size < this.queue[0].length) {
            result = this.queue[0].slice(0, size);
            this.queue[0] = this.queue[0].slice(size);
            return result;
        }
        result = Buffer.allocUnsafe(size);
        let offset = 0;
        let length;
        while (size > 0) {
            length = this.queue[0].length;
            if (size >= length) {
                this.queue[0].copy(result, offset);
                offset += length;
                this.queue.shift();
            }
            else {
                this.queue[0].copy(result, offset, 0, size);
                this.queue[0] = this.queue[0].slice(size);
            }
            size -= length;
        }
        return result;
    }
}
exports.FirmwareCommand = FirmwareCommand;
