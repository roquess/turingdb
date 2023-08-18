import React from 'react'
import axios from 'axios'
import { Box, Button, CircularProgress, Typography } from '@mui/material'
import { useDbName } from '../App/AppContext'
import DBInspector from './DBInspector'
import { useReactive } from '../Hooks'
import { Secondary } from '../Components'

function DBLoader() {
    const [dbName] = useDbName();
    const [loaded, setLoaded] = useReactive(false, [dbName]);
    const [loading, setLoading] = React.useState(false);

    React.useMemo(() => {
        axios.get('/api/is_db_loaded', { params: { db_name: dbName } })
            .then(res => {
                if (res.data.error) {
                    console.log("Error bioserver not started");
                    return;
                }
                setLoaded(res.data.loaded)
            })
            .catch(error => {
                console.log("Error", error);
                setLoaded(false);
            });
    }, [dbName, setLoaded]);

    const loadDatabase = React.useCallback(() => {
        setLoading(true);
        axios
            .post('/api/load_database', { db_name: dbName })
            .then(() => {
                setLoading(false);
                setLoaded(true);
            });
    }, [dbName, setLoaded]);

    if (!dbName) {
        return;
    }

    return (
        <Box>
            {loading
                ? <Box display="flex" justifyContent="center" alignItems="center">
                    <Typography p={2} variant="h4">Loading <Secondary variant="h4">{dbName}</Secondary></Typography>
                    <CircularProgress size={30}/>
                </Box>
                : loaded.current
                    ? <DBInspector />
                    : <Box display="flex" flexDirection="column" alignItems="center">
                        <Typography p={2}>Database <Secondary>{dbName}</Secondary> not loaded.</Typography>
                        <Button variant="contained" color="primary" onClick={loadDatabase}>
                            Load Database
                        </Button>
                    </Box>}
        </Box>
    );
}

export default function DatabasePage() {
    return <DBLoader />;
}
