import axios from 'axios'
import { Autocomplete, IconButton, TextField } from '@mui/material'
import React from 'react'
import { useDbName } from '../App/AppContext'
import { BorderedContainer } from './'
import SearchIcon from '@mui/icons-material/Search'
import { BorderedContainerTitle } from './BorderedContainer'

export default function PropertyFilterContainer({
    setPropertyName,
    setPropertyValue,
}) {
    const [loading, setLoading] = React.useState(true);
    const [propertyNames, setPropertyNames] = React.useState([]);
    const [currentPropertyName, setCurrentPropertyName] = React.useState(null);
    const [currentValue, setCurrentValue] = React.useState("");
    const [dbName] = useDbName();

    React.useLayoutEffect(() => {
        setLoading(true);
    }, [dbName])

    if (loading) {
        axios
            .post("/api/list_string_property_types", {
                db_name: dbName
            })
            .then(res => {
                setLoading(false);
                setPropertyNames(res.data);
            })
            .catch(err => {
                setLoading(false);
                console.log(err);
            })
    }

    const search = React.useCallback(() => {
        setPropertyName(currentPropertyName);
        setPropertyValue(currentValue);
    }, [setPropertyValue, currentValue, setPropertyName, currentPropertyName]);


    return <form onSubmit={(e) => {
        e.preventDefault();
        search();
    }}
    >
        <BorderedContainer title={
            <BorderedContainerTitle title="Property" noDivider>
                <Autocomplete
                    disablePortal
                    id="property-name-filter"
                    blurOnSelect
                    onChange={(_e, v) => v && setCurrentPropertyName(v)}
                    value={currentPropertyName}
                    options={propertyNames}
                    sx={{ width: "50%", m: 1 }}
                    size="small"
                    renderInput={(params) => <TextField {...params} label="Property name" />}
                />
                <TextField
                    id="outlined-controlled"
                    label="Property value"
                    value={currentValue}
                    onChange={e => setCurrentValue(e.target.value)}
                    size="small"
                />
                <IconButton type="submit"><SearchIcon /></IconButton>
            </BorderedContainerTitle>}>
        </BorderedContainer >
    </form>;
}
