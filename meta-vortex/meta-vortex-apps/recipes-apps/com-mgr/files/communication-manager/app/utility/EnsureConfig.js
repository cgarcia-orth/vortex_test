"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const fs = require("fs-extra");
const path = require("path");
function ensureConfig() {
    const configFolderPath = process.env.NODE_CONFIG_DIR;
    const defaultConfig = path.resolve(__dirname, '../..', "config/default.json");
    const productionConfig = path.resolve(__dirname, '../..', "config/production.json");
    if (!fs.existsSync(configFolderPath)) {
        fs.ensureDirSync(configFolderPath);
        fs.copyFileSync(defaultConfig, path.resolve(configFolderPath, "default.json"));
        fs.copyFileSync(productionConfig, path.resolve(configFolderPath, "production.json"));
        return false;
    }
    return true;
}
exports.ensureConfig = ensureConfig;
