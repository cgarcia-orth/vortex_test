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
const child_process_1 = require("child_process");
const fs = require("fs");
const JsonRpcResponses_1 = require("./JsonRpcResponses");
const fsExtra = require("fs-extra");
const config = require("config");
const download = require("download");
/**
 * Handle update message
 *
 * @param {string} msg
 * @param {string} topic
 * @param {string} tmpFile
 * @param {() => void} callback
 * @returns {Promise<string>}
 */
function handleUpdateMessage(msg, topic, tmpFile, callback) {
    return __awaiter(this, void 0, void 0, function* () {
        const message = JSON.parse(msg);
        clearUpdateDir();
        yield downloadPackage(message.params.url);
        return runUpdate(topic, tmpFile, message.id);
    });
}
exports.handleUpdateMessage = handleUpdateMessage;
function handleUpdateMessageSimple(url) {
    return __awaiter(this, void 0, void 0, function* () {
        clearUpdateDir();
        if (url) {
            yield downloadPackage(url);
        }
        return runUpdateSimple();
    });
}
exports.handleUpdateMessageSimple = handleUpdateMessageSimple;
function clearUpdateDir() {
    const updateDir = config.get("production.software.updateDir");
    fsExtra.emptyDirSync(updateDir);
}
function downloadPackage(url) {
    const updateDir = config.get("production.software.updateDir");
    return new Promise((resolve, reject) => {
        download(url, updateDir, { extract: true }).then(() => {
            resolve();
        }).catch((err) => {
            reject(err);
        });
    });
}
function runUpdateSimple() {
    return __awaiter(this, void 0, void 0, function* () {
        const updateDir = config.get("production.software.updateDir");
        const updateScript = config.get("production.software.updateScript");
        // perform update
        yield ex(`/bin/sh ${updateScript}`, { cwd: updateDir });
    });
}
function runUpdate(topic, tmpFile, messageId) {
    return __awaiter(this, void 0, void 0, function* () {
        const updateDir = config.get("production.software.updateDir");
        const updateScript = config.get("production.software.updateScript");
        try {
            const result = JsonRpcResponses_1.JsonRpcResponses.successfulUpdate(`Update complete`);
            const data = { [topic]: result };
            fs.writeFileSync(tmpFile, JSON.stringify(data), { encoding: "utf8" });
            // perform update
            yield ex(`/bin/sh ${updateScript}`, { cwd: updateDir });
            return result;
        }
        catch (error) {
            return JsonRpcResponses_1.JsonRpcResponses.generalError(messageId, error);
        }
    });
}
function ex(command, options) {
    return new Promise((resolve, reject) => {
        child_process_1.exec(command, options, (error, stdout, stderr) => {
            if (error || stderr) {
                reject("" + (error || stderr));
            }
            else {
                resolve("Success");
            }
        });
    });
}
