"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const child_process_1 = require("child_process");
const comm = "maestro -v"; // Outputs "Maestro version : 2.4.0-1295";
const maestroVersionRegex = new RegExp("^.*?([\\d\.]+-[\\d]+).*", "giu");
function MaestroVersion() {
    return new Promise((resolve, reject) => {
        child_process_1.exec(comm, {}, (error, stdout, stderr) => {
            if (error !== null) {
                reject();
            }
            else {
                const result = maestroVersionRegex.exec(stderr);
                if (result !== null) {
                    resolve(result[1]);
                }
                else {
                    reject();
                }
            }
        });
    });
}
exports.MaestroVersion = MaestroVersion;
