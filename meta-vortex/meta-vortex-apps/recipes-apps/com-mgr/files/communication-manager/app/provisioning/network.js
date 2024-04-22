"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const os = require("os");
const crypto = require("crypto");
function getMacAddresses() {
    const networkInterfaces = os.networkInterfaces();
    const macAddresses = [];
    Object.keys(networkInterfaces).forEach((interfaceId) => {
        const interfaceInfo = networkInterfaces[interfaceId];
        interfaceInfo.forEach((iFace) => {
            const mac = iFace.mac;
            if (mac !== "00:00:00:00:00:00" && !macAddresses.includes(mac)) {
                macAddresses.push(mac);
            }
        });
    });
    return macAddresses.map((mac) => {
        const hash = crypto.createHash("sha256");
        return hash.update(mac).digest("hex");
    });
}
exports.getMacAddresses = getMacAddresses;
