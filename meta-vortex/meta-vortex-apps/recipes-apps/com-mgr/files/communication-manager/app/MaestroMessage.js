"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function propExists(obj, prop) {
    return (obj.hasOwnProperty(prop) && typeof obj[prop] !== "undefined" && obj[prop] !== null);
}
class MaestroMessage {
    constructor(data, timestamp, value) {
        this.allowedKeys = [
            "activators",
            "activators-logic",
            "alarms",
            "backwash",
            "bypass",
            "data",
            "feature_loop",
            "features",
            "filter",
            "filters",
            "filtration_loop",
            "led_time",
            "meta",
            "purge",
            "rain_diverter",
            "schedule",
            "settings",
            "sequence",
            "splashpad",
            "stats",
            "tasks",
            "tasks_completed_list",
            "tasks_list",
            "sequence_settings",
        ];
        this.data = data;
        this.timestamp = timestamp;
        this.value = value;
    }
    static fromString(msg) {
        const message = JSON.parse(msg);
        if (!propExists(message, "data")) {
            throw new Error("Data property missing");
        }
        if (!propExists(message, "timestamp")) {
            throw new Error("Timestamp property missing");
        }
        if (!propExists(message, "value")) {
            throw new Error("Value property missing");
        }
        return new MaestroMessage(message.data, message.timestamp, message.value);
    }
    getDataAttribute() {
        return this.data.trim().toLowerCase();
    }
    mustUpdateShadow() {
        return (this.isStateMessage() || this.isDataMessage() || this.isMetaMessage());
    }
    toShadowObject() {
        if (this.isStateMessage()) {
            return {
                state: {
                    reported: this.minifyShadow(this.value),
                },
            };
        }
        else if (this.isDataMessage()) {
            return {
                state: {
                    reported: {
                        data: this.value,
                    },
                },
            };
        }
        else if (this.isMetaMessage()) {
            return {
                state: {
                    reported: {
                        meta: this.value,
                    },
                },
            };
        }
        throw new Error("Can't get shadow data");
    }
    isStateMessage() {
        return (this.getDataAttribute() === "state");
    }
    isDataMessage() {
        return (this.getDataAttribute() === "data");
    }
    isMetaMessage() {
        return (this.getDataAttribute() === "meta");
    }
    minifyShadow(object, level = 1) {
        let copy;
        if (Array.isArray(object)) {
            copy = [];
        }
        else {
            copy = {};
        }
        for (const key in object) {
            if (level > 1 || this.allowedKeys.includes(key)) {
                if (typeof object[key] === 'object' && object[key] !== null) {
                    if (level <= 5) { // AWS maximum depth of JSON device state documents is 6
                        copy[key] = this.minifyShadow(object[key], level + 1);
                    }
                }
                else {
                    copy[key] = object[key];
                }
            }
        }
        return copy;
    }
}
exports.MaestroMessage = MaestroMessage;
