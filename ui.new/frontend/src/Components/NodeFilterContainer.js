import {
    Box,
    Button,
    CircularProgress,
    IconButton,
    TextField,
    Alert,
} from '@mui/material'
import CachedIcon from '@mui/icons-material/Cached'
import React from 'react'
import { useDbName, useNodes } from '../App/AppContext'
import { NodeStack, BorderedContainer, NodeInspector, NodeChip } from './'
import { BorderedContainerTitle } from './BorderedContainer'


const Title = (props) => {
    const { setLoading, setError } = props;

    const [addNodeId, setAddNodeId] = React.useState("")
    const [nodes] = useNodes({ noTrigger: true });
    const [dbName] = useDbName();

    return <BorderedContainerTitle title="Nodes">
        <IconButton onClick={() => setLoading(true)}>
            <CachedIcon />
        </IconButton>

        <form onSubmit={(e) => {
            e.preventDefault();

            const id = parseInt(addNodeId);

            if (isNaN(id)) {
                setError("Please provide a valid node id")
                return;
            }

            if (nodes.hasId(id)) {
                setError("Node already selected");
                return;
            }

            setError(null);
            nodes
                .getNode(dbName, id)
                .then(res => nodes.select(res));
        }}>
            <Box display="flex" >
                <TextField
                    type="number"
                    id="node-id-field"
                    sx={{ p: 1 }}
                    size="small"
                    onChange={e => setAddNodeId(e.target.value)}
                />
                <Button type="submit">Add node</Button>
            </Box>
        </form>
    </BorderedContainerTitle>
}


export default function NodeFilterContainer({
    selectedNodeType,
    propertyName,
    propertyValue
}) {
    const [loading, setLoading] = React.useState(false);
    const [tooManyNodes, setTooManyNodes] = React.useState(false);
    const [error, setError] = React.useState(null);
    const [nodes] = useNodes();
    const [currentNodes, setCurrentNodes] = React.useState(new Map());
    const [dbName] = useDbName();
    const titleProps = { setError, setLoading };

    React.useLayoutEffect(() => {
        setLoading(true);
    }, [selectedNodeType, propertyName, propertyValue]);

    const Content = ({ children }) => {
        return <BorderedContainer title={<Title {...titleProps} />}>
            {error && <Alert severity="error">{error}</Alert>}
            <NodeStack>{children}</NodeStack>
        </BorderedContainer >;
    }

    if (loading) {
        nodes
            .fetchNodes(dbName, {
                ...selectedNodeType ? { node_type_name: selectedNodeType } : {},
                ...propertyName ? { prop_name: propertyName } : {},
                ...propertyValue ? { prop_value: propertyValue } : {},
            })
            .then(res => {
                if (res.error) {
                    setTooManyNodes(true);
                    setCurrentNodes(new Map());
                    setLoading(false);
                    return;
                }

                setLoading(false);
                setCurrentNodes(res);
                setTooManyNodes(false);
            });

        return <Content>
            <Box m={1}>Loading nodes</Box><Box><CircularProgress size={20} /></Box>
        </Content>
    }

    if (tooManyNodes) {
        return <Content><Box m={1}>Too many nodes requested</Box></Content>
    }

    const currentNodeKeys = [...currentNodes.keys()];
    const filteredKeys = currentNodeKeys
        .filter(id => !nodes.hasId(id))
        .slice(0, 100);

    const filteredNodeCount = currentNodes.size - filteredKeys.length;
    const remainingNodeCount = currentNodes.size - filteredNodeCount - filteredKeys.length;

    return <BorderedContainer title={<Title {...titleProps} />}>
        {error && <Alert severity="error">{error}</Alert>}
        {<NodeInspector />}
        <NodeStack>
            {filteredKeys.map((id, _i) => {
                return <NodeChip
                    node={currentNodes.get(id)}
                    key={_i}
                    nodeId={id}
                />;
            })}
        </NodeStack>
        {remainingNodeCount > 0 &&
            <Box p={1}>... {remainingNodeCount} more nodes</Box>}
    </BorderedContainer >;
}
