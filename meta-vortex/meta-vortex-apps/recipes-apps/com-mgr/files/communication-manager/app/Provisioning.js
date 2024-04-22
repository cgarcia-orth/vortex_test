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
const AdmZip = require("adm-zip");
const axios = require("axios");
const config = require("config");
const fs = require("fs");
const node_machine_id_1 = require("node-machine-id");
const path = require("path");
const led_1 = require("./provisioning/led");
const network_1 = require("./provisioning/network");
const delay_1 = require("./utility/delay");
const UpdatesHandler_1 = require("./UpdatesHandler");
const Logger_1 = require("./Logger");
const STATUS_PENDING = 'pending';
const STATUS_ADOPTING = 'adopting';
const STATUS_COMPLETE = 'complete';
const STATUS_UPDATING = 'updating';
function provisioning() {
    return __awaiter(this, void 0, void 0, function* () {
        const led = new led_1.Led();
        const data = {
            data: {
                attributes: {
                    mac_addresses: network_1.getMacAddresses(),
                    machine_id: node_machine_id_1.machineIdSync(),
                },
                type: "Provision",
            },
        };
        let response = null;
        let fetchDelay = config.get("provisioning.minFetchDelay");
        let status = null;
        let url = null;
        while (!provisionComplete(response)) {
            led.stopLed('spare2'); // means no error
            try {
                response = yield startProvisioning(data, fetchDelay);
                const newStatus = response.data.data.status;
                if (newStatus === STATUS_PENDING) {
                    led.offAll();
                    status = newStatus;
                }
                else if (newStatus.substr(0, 8) === STATUS_ADOPTING) {
                    toggleLed(newStatus, led);
                    status = STATUS_ADOPTING;
                }
                else if (newStatus === STATUS_UPDATING || newStatus === STATUS_COMPLETE) {
                    status = newStatus;
                    url = response.data.data.url;
                }
                fetchDelay = config.get("provisioning.fetchIncrement");
            }
            catch (e) {
                led.errorLed('spare2');
                // TODO retry only if no response, show message if error?
                if (fetchDelay < config.get("provisioning.maxFetchDelay")) {
                    fetchDelay += config.get("provisioning.fetchIncrement");
                }
            }
        }
        led.stopAll();
        led.offAll();
        if (status === STATUS_UPDATING) {
            led.blinkLed('spare1');
            yield update(url);
            led.stopLed('spare1');
            led.offAll();
            led.onLed('spare1');
        }
        if (status === STATUS_COMPLETE) {
            led.onLed('spare1');
            // check update status
            let updateStatus = null;
            let checkDelay = 0;
            let retries = 0;
            while (retries < 11 && updateStatus !== 'SUCCESS' && updateStatus !== 'FAILED') {
                updateStatus = yield getUpdateStatus(checkDelay);
                checkDelay = 30000; // 30 seconds
                retries++;
            }
            Logger_1.Logger.debug('Update status: ', updateStatus);
            if (updateStatus === 'SUCCESS') {
                led.blinkAll();
                yield finish(data, url, led);
                led.stopAll();
                led.offAll();
            }
            else {
                led.onAll();
            }
        }
        return;
    });
}
exports.provisioning = provisioning;
function getUpdateStatus(ms) {
    const updateStatusPath = config.get("production.software.updateStatus");
    return delay_1.delay(ms).then(() => __awaiter(this, void 0, void 0, function* () {
        if (fs.existsSync(updateStatusPath)) {
            return fs.readFileSync(updateStatusPath).toString('utf8').trim().toUpperCase();
        }
        else {
            return null;
        }
    }));
}
function update(url) {
    return UpdatesHandler_1.handleUpdateMessageSimple(url);
}
function finish(data, url, led) {
    return __awaiter(this, void 0, void 0, function* () {
        let response = null;
        let fetchDelay = config.get("provisioning.minFetchDelay");
        while (!response) {
            try {
                response = yield finishProvisioning(data, url, fetchDelay);
            }
            catch (e) {
                led.errorLed('spare2');
                // TODO retry only if no response, show message if error?
                if (fetchDelay < config.get("provisioning.maxFetchDelay")) {
                    fetchDelay += config.get("provisioning.fetchIncrement");
                }
            }
        }
        const savePath = yield save(response);
        const zip = new AdmZip(savePath);
        zip.extractAllTo(config.get("production.comManager.configFolderPath"), true);
        fs.unlinkSync(savePath);
        return;
    });
}
function toggleLed(status, led) {
    const nums = status.substr(8);
    let turnOff = ['1', '2', '3'];
    nums.split(',').forEach((num) => {
        turnOff = turnOff.filter((value) => {
            return value !== num;
        });
        led.onLed(`spare${num}`);
    });
    turnOff.forEach((num) => {
        led.offLed(`spare${num}`);
    });
}
function provisionComplete(response) {
    return response
        && response.status === 200
        && (response.data.data.status === STATUS_UPDATING || response.data.data.status === STATUS_COMPLETE);
}
function save(response) {
    const savePath = path.resolve(__dirname, "..", "config.zip");
    const writer = fs.createWriteStream(savePath);
    response.data.pipe(writer);
    return new Promise((resolve, reject) => {
        writer.on("close", () => {
            resolve(savePath);
        });
        writer.on("error", reject);
    });
}
function startProvisioning(data, ms) {
    return delay_1.delay(ms).then(() => __awaiter(this, void 0, void 0, function* () {
        return axios.default.post(config.get("provisioning.provisioningEndpoint"), data, {
            responseType: "json",
            timeout: config.get("provisioning.httpTimeout"),
        });
    }));
}
function finishProvisioning(data, url, ms) {
    return delay_1.delay(ms).then(() => __awaiter(this, void 0, void 0, function* () {
        return axios.default.post(url, data, {
            responseType: "stream",
            timeout: config.get("provisioning.httpTimeout"),
        });
    }));
}
