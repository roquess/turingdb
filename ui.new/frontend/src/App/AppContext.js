import axios from 'axios'

import {
    GlobalState,
    useGlobalState,
} from "../GlobalState"

const cacheNodes = (state, setState, nodes) => {
    state.cached = new Map([...state.cached, ...nodes]);
    setState({ ...state })
}

const initNodes = () => new GlobalState({
    selected: new Map(),
    cached: new Map(),
}, {
    unselect: (state, setState, node) => {
        state.selected.delete(node.id);
        setState({ ...state })
    },
    select: (state, setState, node) => {
        state.selected.set(node.id, node);
        setState({ ...state })
    },
    selectMany: (state, setState, nodes) => {
        [...nodes.values()].map(n => state.selected.set(n.id, n));
        setState({ ...state })
    },
    selectManyById: (state, _setState, dbName, ids) => {
        state.getNodes(dbName, ids).then(res => {
            state.selectMany(res)
        });
    },
    clear: (state, setState) => {
        state.selected = new Map();
        setState({ ...state })
    },
    has: (state, _setState, node) => {
        if (node.id === undefined) {
            throw new Error(
                "Node has no id, you probably meant to use hasId() instead"
            );
        }
        return state.selected.has(node.id);
    },
    hasId: (state, _setState, id) => {
        return state.selected.has(id);
    },
    getNodes: async (state, setState, dbName, allIds, params = { cache: true }) => {
        return await getNodes(state, setState, dbName, allIds, params);
    },
    getNode: async (state, setState, dbName, id, params = { cache: true }) => {
        return await getNodes(state, setState, dbName, [id], params)
            .then(res => [...res.values()][0]);
    },
    fetchNodes: async (state, setState, dbName, filters) => {
        return await axios
            .post("/api/list_nodes", {
                db_name: dbName,
                ...filters
            })
            .then(res => {
                if (res.data.error) {
                    return res.data
                }
                const nodes = new Map(res.data.map(n => [n.id, n]));
                cacheNodes(state, setState, nodes);
                return nodes;
            });
    },
});

const getNodes = async (state, setState, dbName, allIds, params = { cache: true }) => {
    const ids = allIds.filter(id => !state.cached.has(id));

    if (ids.length !== 0) {
        const res = await axios
            .post('/api/list_nodes', {
                db_name: dbName,
                ids: ids,
            })
            .catch(err => {
                console.log(err)
                return;
            });

        if (res.error) {
            console.log("API error:", res.error)
            return;
        }

        if (params.cache)
            cacheNodes(state, setState, new Map(res.data.map(n => [n.id, n])));
    }

    return new Map(
        allIds
            .map(id => [id, state.cached.get(id)])
    );
}

const initEdges = () => new GlobalState({
    cached: new Map(),
}, {
    get: async (state, setState, dbName, allIds) => {
        const ids = allIds.filter(id => !state.cached.has(id));

        if (ids.length !== 0) {
            const res = await axios
                .post('/api/list_edges', {
                    db_name: dbName,
                    ids: ids,
                })
                .catch(err => {
                    console.log(err)
                    return;
                });

            if (res.error) {
                console.log("API error:", res.error)
                return;
            }

            const edges = new Map(res.data.map(e => [e.id, e]));
            state.cached = new Map([...state.cached, ...edges]);
            setState({ ...state })
        }

        return new Map(
            allIds
                .map(id => [id, state.cached.get(id)])
        );
    },
});

const gDbName = new GlobalState(null);
export const useDbName = (params) => useGlobalState(gDbName, params);

const gInspectedNode = new GlobalState(null);
export const useInspectedNode = (params) => useGlobalState(gInspectedNode, params);

const gCyLayout = new GlobalState("dagre");
export const useCyLaout = (params) => useGlobalState(gCyLayout, params);

const gNodes = initNodes();
export const useNodes = (params) => useGlobalState(gNodes, params);

const gEdges = initEdges();
export const useEdges = (params) => useGlobalState(gEdges, params);
