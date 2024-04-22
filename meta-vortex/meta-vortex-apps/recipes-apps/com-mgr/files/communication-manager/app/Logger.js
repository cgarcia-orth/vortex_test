"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const config = require("config");
class Logger {
    static debug(...mgs) {
        if (config.get("debug")) {
            console.log(mgs);
        }
    }
}
exports.Logger = Logger;
