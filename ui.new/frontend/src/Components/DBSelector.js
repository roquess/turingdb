import React from 'react'
import axios from 'axios'

import { Autocomplete, TextField } from '@mui/material'
import { useDbName } from '../App/AppContext'

export default function DBSelector() {
    const [availableDbs, setAvailableDbs] = React.useState([]);
    const [dbName, setDbName] = useDbName();

    // See https://mui.com/material-ui/react-autocomplete/ for an async
    // version to wait for the server to respond (Load on open section)
    return <Autocomplete
        disablePortal
        blurOnSelect
        id="db_selector"
        onChange={(_e, newDb) => newDb && setDbName(newDb)}
        onOpen={(_e) => {
            axios.get("/api/list_available_databases")
                .then(res => { setAvailableDbs(res.data.db_names) })
                .catch(err => { console.log(err) });
        }}
        options={availableDbs}
        value={dbName}
        sx={{ width: 300 }}
        size="small"
        renderInput={(params) => <TextField {...params} label="Database" />}
    />;
}

