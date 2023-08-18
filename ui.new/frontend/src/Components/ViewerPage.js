import React from 'react'
import { Box, Autocomplete, CircularProgress, TextField } from '@mui/material';
import CytoscapeComponent from 'react-cytoscapejs';
import { useTheme } from '@emotion/react';
import cytoscape from 'cytoscape';
import cola from 'cytoscape-cola'
import dagre from 'cytoscape-dagre'
import elk from 'cytoscape-elk'
import klay from 'cytoscape-klay'
import cise from 'cytoscape-cise'
import { SelectedNodesContainer } from './'
import { useCyLaout, useDbName, useNodes, useEdges, useInspectedNode } from '../App/AppContext';

const getCyStyle = async () => {
    return await fetch('/cy-style.json')
        .then(res => res.json())
        .catch(err => console.log(err));
}

export default function ViewerPage() {
    const [globalNodes] = useNodes();
    const [globalEdges] = useEdges();
    const [dbName] = useDbName();
    const { hasId, selectManyById } = globalNodes;
    const [, setInspectedNode] = useInspectedNode();
    const theme = useTheme();
    const [cyStyle, setCyStyle] = React.useState(null);
    const [edges, setEdges] = React.useState([]);
    const [cyLayout, setCyLayout] = useCyLaout();
    const [cyState, setCyState] = React.useState(null);
    const { get } = globalEdges;
    const cySetup = React.useCallback(cy => {
        setCyState(cy);
    }, []);

    cytoscape.use(cola);
    cytoscape.use(dagre);
    cytoscape.use(elk);
    cytoscape.use(klay);
    cytoscape.use(cise);

    React.useEffect(() => {
        if (!cyState) {
            return;
        }

        cyState.on('onetap', event => {
            const node = event.target.data();

            if (!node.ins) {
                return;
            }

            setInspectedNode(node);
        })


        cyState.on('dbltap', event => {
            const node = event.target.data();

            if (!node.ins) {
                event.cy.layout({ name: cyLayout }).run();
                return;
            }
            const edgeIds = [...node.ins, ...node.outs];

            if (edgeIds.length === 0) {
                return;
            }

            get(dbName, edgeIds)
                .then(res => {
                    const nodeIds = [...res.values()]
                        .map(e => [e.source_id, e.target_id])
                        .flat()
                        .filter((id, i, arr) => arr.indexOf(id) === i);
                    console.log(nodeIds);
                    setEdges([...res.values()]
                        .filter(e => hasId(e.source_id) && hasId(e.target_id)));
                    selectManyById(dbName, nodeIds);
                });
        });
    }, [cyState, dbName, cyLayout, get, selectManyById, setInspectedNode, hasId]);


    const formatedNodes = React.useMemo(() =>
        [...globalNodes.selected.values()]
            .map(n => {
                return {
                    data: { ...n, id: n.id, turingId: n.id },
                    group: "nodes"
                };
            }), [globalNodes]);

    const formatedEdges = React.useMemo(() => {
        return edges
            .filter(e => hasId(e.source_id) && hasId(e.target_id))
            .map(e => {
                return {
                    data: { turingId: e.id, source: e.source_id, target: e.target_id },
                    group: "edges"
                }
            })
    }, [edges, hasId]);

    const elements = React.useMemo(() => [
        ...formatedNodes,
        ...formatedEdges
    ], [formatedNodes, formatedEdges]);

    React.useEffect(() => {
        if (!cyState) {
            return;
        }

        cyState.layout({ name: cyLayout }).run();
    }, [cyState, cyLayout, elements]);


    if (!dbName) {
        return <Box>Select a database to start</Box>;
    }

    if (!cyStyle) {
        getCyStyle().then(res => setCyStyle(res));
        return <Box>Loading cytoscape <CircularProgress s={20} /></Box>
    }

    const layouts = [
        "cola",
        "dagre",
        "concentric",
        "elk",
        "klay",
        "cise",
    ];


    return <Box>
        <SelectedNodesContainer />
        <Box
            p={1}
            m={1}
            border={1}
            borderRadius={4}
            overflow="hidden"
            borderColor={theme.palette.background.paper}
            bgcolor={theme.palette.background.paper}
            flex={1}
            height="50vh"
        >
            <Autocomplete
                value={cyLayout}
                onChange={(_e, newLayout) => setCyLayout(newLayout)}
                disablePortal
                id="select-layout"
                options={layouts}
                sx={{ width: 300 }}
                renderInput={(params) => <TextField {...params} label="Graph layout" />}
            />
            <CytoscapeComponent
                elements={elements}
                cy={cySetup}
                stylesheet={cyStyle}
                layout={{ name: cyLayout }}
                style={{ width: "100%", height: "100%" }}
            />
        </Box>
    </Box>
}
