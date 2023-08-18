import React from 'react'

import { Grid, Chip } from '@mui/material'
import AddCircleSharpIcon from '@mui/icons-material/AddCircleSharp'
import CircleSharpIcon from '@mui/icons-material/CircleSharp';

import { useNodes, useDbName, useInspectedNode } from '../App/AppContext'

const NodeChip = (props) => {
    const propNode = props.node;
    const propNodeId = props.nodeId;
    const [node, setNode] = React.useState(propNode);
    const [nodes] = useNodes({ noTrigger: true })
    const { getNode } = nodes;
    const [dbName] = useDbName();
    const [, setInspectedNode] = useInspectedNode()

    React.useEffect(() => {
        getNode(dbName, propNodeId)
            .then(node => setNode(node));
    }, [getNode, dbName, propNodeId]);

    return <Grid item p={0.5}><Chip {...{
        label: props.nodeId,
        variant: "outlined",
        ...node
            ? {
                onClick: () => setInspectedNode(node),
                ...(nodes.has(node)
                    ? {
                        onDelete: () => {
                            nodes.unselect(node);
                            setNode({ ...node }); // trigger render
                        }
                    }
                    : {
                        onDelete: () => {
                            nodes.select(node)
                            setNode({ ...node }); // trigger render
                        },
                        deleteIcon: <AddCircleSharpIcon />
                    }
                )
            }
            : {
                onClick: () => { },
                onDelete: () => { },
                deleteIcon: <CircleSharpIcon />
            }
    }} /></Grid>;
}

export default NodeChip
