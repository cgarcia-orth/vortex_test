"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const config = require("config");
const CommunicationManager_1 = require("../app/CommunicationManager");
const Logger_1 = require("../app/Logger");
const Provisioning_1 = require("../app/Provisioning");
const notify = require("sd-notify");
const EnsureConfig_1 = require("../app/utility/EnsureConfig");
if (EnsureConfig_1.ensureConfig()) {
    process.on("uncaughtException", (err) => {
        Logger_1.Logger.debug(err);
    });
    notify.ready();
    notify.startWatchdogMode(30000);
    if (config.get("env") === "production") {
        CommunicationManager_1.communicationManager();
    }
    else {
        Provisioning_1.provisioning().then(() => {
            Logger_1.Logger.debug("Provisioning complete");
            notify.stopWatchdogMode();
        });
    }
}
