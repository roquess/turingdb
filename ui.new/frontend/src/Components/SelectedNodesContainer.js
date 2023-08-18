import { IconButton } from '@mui/material'
import { useNodes } from '../App/AppContext'
import DeleteIcon from '@mui/icons-material/Delete'
import { BorderedContainer, NodeChip, NodeStack } from './'
import React from 'react'
import { BorderedContainerTitle } from './BorderedContainer';

export default function SelectedNodesContainer() {
    const [nodes] = useNodes();

    var rows = [];
    nodes.selected.forEach(
        (node, i) => {
            rows.push(<NodeChip
                key={i}
                nodeId={node.id}
            />)
        }
    )

    return <BorderedContainer title={
        <BorderedContainerTitle title="Selected nodes">
            {nodes.selected.size !== 0 &&
                <IconButton onClick={() => {
                    nodes.clear()
                }}>
                    <DeleteIcon />
                </IconButton>}
        </BorderedContainerTitle>
    }>
        <NodeStack>
            {rows}
        </NodeStack>
    </BorderedContainer >;
}
