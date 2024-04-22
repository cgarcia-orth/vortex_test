"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const child_process_1 = require("child_process");
const config = require("config");
function sysCom(command) {
    child_process_1.exec(command, (error, stdout, stderr) => {
        if (error || stderr) {
            return;
        }
    });
}
class Led {
    constructor() {
        this.intervalsOn = {
            spareLed1: null,
            spareLed2: null,
            spareLed3: null,
        };
        this.intervalsOff = {
            spareLed1: null,
            spareLed2: null,
            spareLed3: null,
        };
        this.timeoutsOn = {
            spareLed1: null,
            spareLed2: null,
            spareLed3: null,
        };
        this.timeoutsOff = {
            spareLed1: null,
            spareLed2: null,
            spareLed3: null,
        };
        this.ledInterval = config.get("provisioning.ledInterval");
        this.ledErrorInterval = config.get("provisioning.ledErrorInterval");
    }
    blinkLed(ledId) {
        this.stopLed(ledId);
        this.startLed(ledId, this.ledInterval);
    }
    blinkAll() {
        this.blinkLed('spare1');
        this.blinkLed('spare2');
        this.blinkLed('spare3');
    }
    errorLed(ledId) {
        this.stopLed(ledId);
        this.startLed(ledId, this.ledErrorInterval);
    }
    onAll() {
        this.onLed('spare1');
        this.onLed('spare2');
        this.onLed('spare3');
    }
    offAll() {
        this.offLed('spare1');
        this.offLed('spare2');
        this.offLed('spare3');
    }
    onLed(ledId) {
        this.stopLed(ledId);
        sysCom(config.get(`provisioning.led.${ledId}.on`));
    }
    offLed(ledId) {
        this.stopLed(ledId);
        sysCom(config.get(`provisioning.led.${ledId}.off`));
    }
    startLed(ledId, interval) {
        this.timeoutsOn[ledId] = setTimeout(() => {
            sysCom(config.get(`provisioning.led.${ledId}.on`));
            this.intervalsOn[ledId] = setInterval(() => {
                sysCom(config.get(`provisioning.led.${ledId}.on`));
            }, interval * 2);
        }, 0);
        this.timeoutsOff[ledId] = setTimeout(() => {
            sysCom(config.get(`provisioning.led.${ledId}.off`));
            this.intervalsOff[ledId] = setInterval(() => {
                sysCom(config.get(`provisioning.led.${ledId}.off`));
            }, interval * 2);
        }, interval);
    }
    stopAll() {
        this.stopLed('spare1');
        this.stopLed('spare2');
        this.stopLed('spare3');
    }
    stopLed(ledId) {
        if (this.intervalsOn[ledId]) {
            clearInterval(this.intervalsOn[ledId]);
        }
        if (this.timeoutsOn[ledId]) {
            clearTimeout(this.timeoutsOn[ledId]);
        }
        if (this.intervalsOff[ledId]) {
            clearInterval(this.intervalsOff[ledId]);
        }
        if (this.timeoutsOff[ledId]) {
            clearTimeout(this.timeoutsOff[ledId]);
        }
    }
}
exports.Led = Led;
