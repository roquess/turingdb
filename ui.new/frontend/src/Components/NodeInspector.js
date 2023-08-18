import React from 'react'
import { Box, Button, CircularProgress, Dialog, DialogContent, DialogTitle, Grid, Typography } from '@mui/material'
import { useTheme } from '@emotion/react';
import axios from 'axios'
import BorderedContainer, { BorderedContainerTitle } from './BorderedContainer';
import NodeChip from './NodeChip';
import NodeStack from './NodeStack';
import { useDbName, useInspectedNode, useNodes } from '../App/AppContext';
import { useReactive } from '../Hooks';

const containerHeight = "25vh";

const Edges = (props) => {
    const theme = useTheme();
    const { edgeIds, type } = props;
    const [dbName] = useDbName({ noTrigger: true });
    const [loading, setLoading] = useReactive(true, [edgeIds]);
    const [edgeData, setEdgeData] = React.useState({});
    const [{ getNodes }] = useNodes({ noTrigger: true });
    const done = React.useRef(false);

    React.useEffect(() => {
        if (!loading.current) return;
        done.current = false;

        axios
            .post("/api/list_edges", {
                db_name: dbName,
                ids: edgeIds,
            })
            .then(res => {
                if (res.data.error) {
                    setEdgeData({});
                    return;
                }
                setEdgeData({ ...res.data });
            })
            .catch(err => {
                console.log(err);
                setLoading(false);
                setEdgeData({});
            });
    }, [loading, dbName, edgeIds, setLoading])

    const nodeIds = React.useMemo(() => {
        var currentNodeIds = {};

        if (type === "in") {
            for (const e of Object.values(edgeData)) {
                if (e.edge_type_name in currentNodeIds === false) {
                    currentNodeIds[e.edge_type_name] = [];
                }
                currentNodeIds[e.edge_type_name].push(e.source_id);
            }
        }
        else {
            for (const e of Object.values(edgeData)) {
                if (e.edge_type_name in currentNodeIds === false) {
                    currentNodeIds[e.edge_type_name] = [];
                }
                currentNodeIds[e.edge_type_name].push(e.target_id);
            }

        }
        return currentNodeIds;
    }, [type, edgeData])

    React.useEffect(() => {
        done.current = true;

        const ids = [...Object.values(nodeIds)]
            .flat()
            .filter((id, i, arr) => arr.indexOf(id) === i);

        getNodes(dbName, ids).then(() => {
            setLoading(false);
        })
    }, [dbName, getNodes, nodeIds, setLoading, loading]);

    return <BorderedContainer
        height={containerHeight}
        title={<BorderedContainerTitle
            title={type === "in" ? "In edges" : "Out edges"}
        />}
    >
        <Grid container>
            {/*TODO refactor here to have a grid container wrapping to edge type names
            and one wrapping the nodeStack, so that the first column fits the width of
            edge types*/}
            {loading.current ? <CircularProgress /> : <>
                {Object.keys(nodeIds).map(edge_type =>
                    <Grid item container xs={12} key={"global-edge-type-" + edge_type}>
                        <Grid item xs={3.5}
                            key={edge_type}
                            color={theme.palette.secondary.main}
                            display="flex"
                            overflow="auto"
                            alignItems="center"
                        >
                            <Box display="flex"
                                alignItems="center"
                                key={"edge-type-container-" + edge_type}
                            >
                                {edge_type}
                            </Box>
                        </Grid>
                        <Grid item xs
                            key={"grid-item-" + edge_type}
                            maxHeight="20vh"
                            overflow="auto"
                        >
                            <NodeStack key={"node-stack-" + edge_type}>
                                {nodeIds[edge_type].map((id, i) =>
                                    <NodeChip key={i} nodeId={id}/>
                                )}
                            </NodeStack>
                        </Grid>
                    </Grid>
                )}
            </>
            }
        </Grid>
    </BorderedContainer >
}

const Properties = (props) => {
    const theme = useTheme()
    const { node } = props;
    const [loading, setLoading] = useReactive(true, [node]);
    const [properties, setProperties] = React.useState({});
    const [dbName] = useDbName({ noTrigger: true });

    if (loading.current && node) {
        axios
            .post("/api/list_node_properties", {
                db_name: dbName,
                id: node.id,
            })
            .then(res => {
                setProperties(res.data);
                setLoading(false);
            })
            .catch(err => {
                console.log(err);
                setProperties({});
                setLoading(false);
            });
    }

    return <BorderedContainer
        title={<BorderedContainerTitle title="Node properties" />}
        height={containerHeight}
    >
        {loading.current
            ? <CircularProgress size={20} />
            : Object.keys(properties).map(pName =>
                <Box
                    key={"box-key-" + pName}
                    display="flex"
                    alignItems="center"
                >
                    <Typography
                        variant="body2"
                        color={theme.palette.secondary.main}
                        key={pName + "-name"}
                        p={1}
                    >
                        {pName}:
                    </Typography>
                    <Typography key={pName + "-value"}>
                        {properties[pName]}
                    </Typography>
                </Box>)}
    </BorderedContainer>
}

export default function NodeInspector() {
    const [inspectedNode, setInspectedNode] = useInspectedNode();
    const [nodes] = useNodes({ noTrigger: true });

    if (!inspectedNode) {
        return null;
    }

    return <Dialog
        open={inspectedNode !== null}
        onClose={() => setInspectedNode(null)}
        maxWidth="md"
        fullWidth
    >
        <DialogTitle >
            Node id: {inspectedNode.id}
        </DialogTitle>
        <DialogContent dividers>
            {<Edges edgeIds={inspectedNode.ins} type="in" />}
            {<Edges edgeIds={inspectedNode.outs} type="out" />}
            <Properties node={inspectedNode} />
            <Box display="flex" flexDirection="column" alignItems="center">
                {!nodes.has(inspectedNode) &&
                    <Button variant="contained" onClick={() => {
                        nodes.select(inspectedNode);
                        setInspectedNode(null);
                    }}>Add node to selection</Button>}
            </Box>
        </DialogContent>
    </Dialog>

}
