"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const net = require("net");
/**
 * Listen for firmware events
 */
class TcpServer {
    constructor(port) {
        this.process = false;
        this.state = "HEADER";
        this.payloadLength = 0;
        this.bufferedBytes = 0;
        this.queue = [];
        this.headerRegex = /SZ=(\d{10});/mi;
        this.socket = null;
        this.headerLength = 10;
        this.port = port;
    }
    start(callback) {
        const server = net.createServer((sock) => {
            this.socket = sock;
            sock.on("end", () => {
                // console.log("client disconnected");
            });
            // Handle incoming messages
            sock.on("data", (data) => {
                this.bufferedBytes += data.length;
                this.queue.push(data);
                this.process = true;
                this.onData();
            });
            sock.on("served", (data) => {
                const res = callback(data.toString().trim());
                if (res instanceof Promise) {
                    res.then((messageId) => {
                        const resp = JSON.stringify({ jsonrpc: "2.0", result: true, id: messageId });
                        const respLength = resp.length.toString(10);
                        const header = `SZ=${respLength.padStart(this.headerLength, "0")};`;
                        sock.write(header + resp);
                    }).catch((messageId) => {
                        const resp = JSON.stringify({ jsonrpc: "2.0", error: {}, id: messageId });
                        const respLength = resp.length.toString(10);
                        const header = `SZ=${respLength.padStart(this.headerLength, "0")};`;
                        sock.write(header + resp);
                    });
                }
            });
        });
        server.on("error", (err) => {
            // console.log(err);
            // TODO: try to reopen a socket ??
            throw err;
        });
        server.listen(this.port, () => {
            // console.log("server bound to: ", port);
        });
        return server;
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
}
exports.TcpServer = TcpServer;
