"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const promiseTimeout = (ms, message, promise) => {
    // Create a promise that rejects in <ms> milliseconds
    const timeout = new Promise((resolve, reject) => {
        const id = setTimeout(() => {
            clearTimeout(id);
            reject(message);
        }, ms);
    });
    // Returns a race between our timeout and the passed in promise
    return Promise.race([
        promise,
        timeout,
    ]);
};
exports.promiseTimeout = promiseTimeout;
