import React from 'react'

export const useReactive = (initialValue, deps) => {
    const ref = React.useRef(initialValue);
    const [, triggerRender] = React.useReducer(x => x + 1, 0);

    const setRef = React.useCallback((newValue) => {
        if (ref.current !== newValue) {
            ref.current = newValue;
            triggerRender();
        }
    }, []);

    React.useMemo(() => {
        if (ref.current !== initialValue) {
            ref.current = initialValue;
        }
    // eslint-disable-next-line
    }, [...deps, initialValue]);

    return [ref, setRef];
}
