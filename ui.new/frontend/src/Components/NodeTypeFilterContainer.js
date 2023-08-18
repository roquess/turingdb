import React from 'react'
import axios from 'axios'

import { Autocomplete, CircularProgress, TextField } from '@mui/material'

import { useDbName } from '../App/AppContext'
import BorderedContainer from './BorderedContainer'
import { BorderedContainerTitle } from './BorderedContainer'

export default function NodeTypeFilterContainer({ selected, setSelected }) {
    const [loading, setLoading] = React.useState(true);
    const [nodeTypes, setNodeTypes] = React.useState([]);
    const [dbName] = useDbName();

    React.useEffect(() => {
        setLoading(true);
    }, [dbName])

    const Content = () => {
        return <BorderedContainer title={
            <BorderedContainerTitle title="NodeType" noDivider>
                <Autocomplete
                    id="node-type-filter"
                    onChange={(_e, nt) => {
                        setSelected(nt);
                    }}
                    value={selected}
                    options={nodeTypes}
                    sx={{ width: "100%", p: 1 }}
                    size="small"
                    renderInput={(params) => <TextField {...params} />}
                />
            </BorderedContainerTitle>
        }>
        </BorderedContainer >;
    }

    if (loading) {
        axios
            .post("/api/list_node_types", {
                db_name: dbName
            })
            .then(res => {
                setLoading(false);
                setNodeTypes(res.data)
            })
            .catch(err => {
                setLoading(false);
                console.log(err);
            })
        return <Content><CircularProgress size={20} /></Content>
    }

    return <Content>
    </Content>
}
