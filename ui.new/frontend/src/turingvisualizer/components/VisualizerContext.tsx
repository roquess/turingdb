import React from "react";
import { VisualizerReferences, useVisualizerReferences } from "../refs";
import { DialogInfos, useDialog } from "../tools";
import {
  CxtMenuData,
  HookEventFunction,
  VisualizerCallbacks,
  VisualizerEventHooks,
  VisualizerEvents,
  VisualizerState,
  VisualizerTriggers,
} from "../types";

export type VisualizerContextValueType = {
  refs?: VisualizerReferences;
  themeMode?: string;
  canvasTheme?: string;
  dbName?: string;
  containerId?: string;
  hookEvent: HookEventFunction;
  state: () => VisualizerState | undefined;
  events: () => VisualizerEvents | undefined;
  callbacks: () => VisualizerCallbacks | undefined;
  triggers: () => VisualizerTriggers | undefined;
  cy: () => cytoscape.Core | undefined;
  eventHooks: () => VisualizerEventHooks | undefined;
  contextMenu: () => HTMLElement | undefined;
  contextMenuData: () => CxtMenuData | undefined;
  contextMenuSetData: (data: CxtMenuData) => void;

  searchNodesDialog: DialogInfos;
  showInPathwayDialog: DialogInfos;
};

export const VisualizerContext =
  React.createContext<VisualizerContextValueType>({
    hookEvent: (_eventName, _key, _callback) => {},
    state: () => undefined,
    events: () => undefined,
    callbacks: () => undefined,
    triggers: () => undefined,
    cy: () => undefined,
    eventHooks: () => undefined,
    contextMenu: () => undefined,
    contextMenuData: () => undefined,
    contextMenuSetData: (_data: CxtMenuData) => {},

    searchNodesDialog: {
      isOpen: false,
      open: () => {},
      close: () => {},
      toggle: () => {},
      setIsOpen: () => {},
    },

    showInPathwayDialog: {
      isOpen: false,
      open: () => {},
      close: () => {},
      toggle: () => {},
      setIsOpen: () => {},
    },
  });

export const VisualizerContextProvider = ({
  children,
  canvasTheme,
  themeMode,
  dbName,
  containerId,
}) => {
  const refs = useVisualizerReferences();

  return (
    <VisualizerContext.Provider
      value={{
        refs,
        themeMode,
        canvasTheme,
        dbName,
        containerId,
        state: () => refs.state.current,
        events: () => refs.events.current,
        callbacks: () => refs.callbacks.current,
        triggers: () => refs.triggers.current,
        cy: () => refs.cy.current,
        contextMenu: () => refs.contextMenu.ref.current,
        contextMenuData: () => refs.contextMenu.data.current,
        contextMenuSetData: (data: CxtMenuData) => {
          if (!refs.contextMenu.setData.current) return;
          refs.contextMenu.setData.current(data);
        },
        eventHooks: () => refs.eventHooks.current,
        hookEvent: (eventName: string, key: string, callback: () => void) =>
          refs.hookEvent.current(eventName, key, callback),

        searchNodesDialog: useDialog(),
        showInPathwayDialog: useDialog(),
      }}>
      {children}
    </VisualizerContext.Provider>
  );
};
