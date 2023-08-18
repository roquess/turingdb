import React from 'react';

export class GlobalState {
    constructor(initialValue, methods = {}) {
        this.value = initialValue;  // Actual value of a global state
        this.subscribers = [];     // List of subscribers

        for (const [name, method] of Object.entries(methods)) {
            this.value[name] = (...args) => method(
                this.value, this.setValue.bind(this), ...args
            );
        }
    }

    getValue = function() {
        return this.value;
    }

    setValue = function(newState) {
        if (this.getValue() === newState) {
            // No new update
            return
        }

        this.value = newState;  // Update global state value
        this.subscribers.forEach(subscriber => {
            // Notify subscribers that the global state has changed
            subscriber(this.value);
        });
    }

    subscribe = function(itemToSubscribe) {
        // This is a function for subscribing to a global state
        if (this.subscribers.indexOf(itemToSubscribe) > -1) {
            // Already subsribed
            return
        }
        // Subscribe a component
        this.subscribers.push(itemToSubscribe);
    }

    unsubscribe = function(itemToUnsubscribe) {
        // This is a function for unsubscribing from a global state
        this.subscribers = this.subscribers.filter(
            subscriber => subscriber !== itemToUnsubscribe
        );
    }
}

export function useGlobalState(globalState, params = {}) {
    const [, render] = React.useState();
    const state = globalState.getValue();

    function reRender(_newState) {
        // This will be called when the global state changes
        if (!params.noTrigger) {
            render({});
        }
    }

    React.useEffect(() => {
        // Subscribe to a global state when a component mounts
        globalState.subscribe(reRender);

        return () => {
            // Unsubscribe from a global state when a component unmounts
            globalState.unsubscribe(reRender);
        }
    })

    const setState = React.useCallback((newState) => {
        // Send update request to the global state and let it
        // update itself
        globalState.setValue(newState);
    }, [globalState]);

    return [state, setState];
}
