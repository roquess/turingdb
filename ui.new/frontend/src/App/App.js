import { ThemeProvider, createTheme } from '@mui/material/styles';
import React from 'react'
import { styled } from '@mui/material/styles';
import Box from '@mui/material/Box';
import CssBaseline from '@mui/material/CssBaseline';
import { themeOptions } from '../theme'

import DatabasePage from '../Components/DatabasePage';
import ViewerPage from '../Components/ViewerPage';
import CustomAppBar from '../Components/CustomAppBar'
import { NodeInspector } from '../Components';
import { useDbName, useNodes } from './AppContext';

export const DrawerHeader = styled('div')(({ theme }) => ({
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'flex-end',
    padding: theme.spacing(0, 1),
    ...theme.mixins.toolbar,
}));


function Page({ currentMenu }) {
    const [dbName] = useDbName();
    const [nodes] = useNodes();
    const nodesRef = React.useRef(nodes);
    React.useEffect(() => nodesRef.current.clear(), [dbName]);
    switch (currentMenu) {
        case 'Database':
            return <>
                <DatabasePage />
                <NodeInspector />
            </>
        case 'Viewer':
            return <>
                <ViewerPage />
                <NodeInspector />
            </>
        default:
            return "404";
    }
}

export default function App() {
    const theme = createTheme(themeOptions.dark);
    const [currentMenu, setCurrentMenu] = React.useState("Database");
    const [selectedCyLayout, setSelectedCyLayout] = React.useState("dagre");

    const context = {
        DrawerHeader,
        selectedCyLayout,
        setSelectedCyLayout,
    };

    return <ThemeProvider theme={theme}>
        <CssBaseline />
        <Box display="flex" sx={{ flexGrow: 1 }}>
            <CustomAppBar setCurrentMenu={setCurrentMenu} />
            <Box component="main" flex={1} sx={{
                p: 2,
                display: "flex",
                flexDirection: "column",
                bgcolor: theme.palette.background.default,
                minHeight: "100vh"
            }}>
                <context.DrawerHeader />
                <Page currentMenu={currentMenu} />
            </Box>
        </Box>
    </ThemeProvider>;
}

