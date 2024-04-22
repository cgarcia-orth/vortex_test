"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const AdmZip = require("adm-zip");
const fs = require("fs");
const path = require("path");
const regExp = new RegExp("^path=(.*)$");
let param = null;
process.argv.forEach((val, index, array) => {
    if (regExp.test(val)) {
        param = val;
    }
});
if (param === null) {
    throw new Error("No path specified");
}
const found = param.match(regExp);
const filePath = found[1];
// check that file exists
fs.lstatSync(filePath).isFile();
const dirPath = path.dirname(filePath);
const zip = new AdmZip(filePath);
zip.extractAllTo(dirPath, true);
