// Core
import React from "react";
import { useCanvasTrigger } from "../../useCanvasTrigger";
import { useVisualizerContext } from "../../context";

// @blueprintjs
import { Tooltip, MenuItem, Button } from "@blueprintjs/core";
import { SelectMenu } from "../ContextMenu/select";

const titleSizeLimit = 14;

const ttParams = {
  hoverCloseDelay: 40,
  hoverOpenDelay: 400,
  compact: true,
  openOnTargetFocus: false,
};

const useDefinedState = (initValue) => {
  const [value, set] = React.useState(initValue);
  const ready = React.useRef(false);

  return {
    value, // The actual value
    set, // Sets the value
    ready, // Holds true if value was initialized
    setOnce: (v) => {
      // Sets the value only the first time it is called
      if (!ready.current) {
        ready.current = true;
        set(v);
      }
    },
  };
};

export const LabelMenus = (props) => {
  const vis = useVisualizerContext();
  const [nodeLabels, setNodeLabels] = React.useState([]);
  const [edgeLabels, setEdgeLabels] = React.useState([]);
  const currentNodeLabel = useDefinedState("None");
  const currentEdgeLabel = useDefinedState("None");
  const priorityLabels = React.useMemo(
    () => ["displayName", "name", "label", "Node Type", "Edge Type", "None"],
    []
  );

  React.useEffect(() => {
    vis.callbacks().setNodeLabel(currentNodeLabel.value);
  }, [vis, currentNodeLabel.value]);

  React.useEffect(() => {
    vis.callbacks().setEdgeLabel(currentEdgeLabel.value);
  }, [vis, currentEdgeLabel.value]);

  React.useEffect(() => {
    if (nodeLabels.length > 2) {
      for (const label of priorityLabels) {
        if (nodeLabels.includes(label)) {
          currentNodeLabel.setOnce(label);
          break;
        }
      }
    }
    if (edgeLabels.length > 2) {
      for (const label of priorityLabels) {
        if (edgeLabels.includes(label)) {
          currentEdgeLabel.setOnce(label);
          break;
        }
      }
    }
  }, [
    currentNodeLabel,
    currentEdgeLabel,
    nodeLabels,
    edgeLabels,
    priorityLabels,
  ]);

  // NodeLabel
  useCanvasTrigger({
    category: "elements",
    name: "setNodeLabels",

    callback: () => {
      const properties = [
        ...new Set(
          vis
            .state()
            .elements.filter((e) => e.group === "nodes")
            .map((e) => Object.keys(e.data.properties))
            .flat()
        ),
        "None",
        "Node Type",
      ];

      if (JSON.stringify(properties) !== JSON.stringify(nodeLabels)) {
        setNodeLabels(properties);
      }
    },
  });

  useCanvasTrigger({
    category: "elements",
    name: "setEdgeLabels",

    callback: () => {
      const properties = [
        ...new Set(
          vis
            .state()
            .elements.filter((e) => e.group === "edges")
            .map((e) => Object.keys(e.data.properties))
            .flat()
        ),
        "None",
        "Edge Type",
      ];

      if (JSON.stringify(properties) !== JSON.stringify(edgeLabels)) {
        setEdgeLabels(properties);
      }
    },
  });

  const renderNodeLabel = (label) => {
    return (
      <MenuItem
        key={"node-label-" + label}
        text={
          label.length < titleSizeLimit
            ? label
            : label.slice(0, titleSizeLimit) + "..."
        }
        onClick={() => currentNodeLabel.set(label)}
      />
    );
  };

  const renderEdgeLabel = (label) => {
    return (
      <MenuItem
        key={"edge-label-" + label}
        text={
          label.length < titleSizeLimit
            ? label
            : label.slice(0, titleSizeLimit) + "..."
        }
        onClick={() => currentEdgeLabel.set(label)}
      />
    );
  };

  const CustomTooltip = props.tooltip
    ? ({ children, content }) =>
        props.tooltip({ content, ...ttParams, children })
    : ({ children, content }) => (
        <Tooltip content={content} {...ttParams}>
          {children}
        </Tooltip>
      );

  const CustomButton = props.button
    ? ({ children, text, icon }) => props.button({ children, text, icon })
    : ({ children, text, icon }) => (
        <Button
          fill
          style={{ width: "180px" }}
          alignText="left"
          icon={icon}
          text={text}
          rightIcon="caret-down"
        >
          {children}
        </Button>
      );

  return (
    <>
      <CustomTooltip content="Node labels" {...ttParams}>
        <SelectMenu
          key={"select-menu-set-node-label"}
          renderChild={renderNodeLabel}
          items={nodeLabels}
          placement={"bottom"}
          popoverProps={props.selectPopoverProps}
          className={props.selectClassName}
          Parent={
            <CustomButton
              key="set-node-label"
              icon="graph"
              text={
                currentNodeLabel.value.length < titleSizeLimit
                  ? currentNodeLabel.value
                  : currentNodeLabel.value.slice(0, titleSizeLimit - 3) + "..."
              }
            />
          }
        />
      </CustomTooltip>
      <CustomTooltip content="Edge labels" {...ttParams}>
        <SelectMenu
          key={"select-menu-set-edge-label"}
          renderChild={renderEdgeLabel}
          items={edgeLabels}
          placement={"bottom"}
          popoverProps={props.selectPopoverProps}
          className={props.selectClassName}
          Parent={
            <CustomButton
              key="set-edge-label"
              icon="one-to-one"
              text={
                currentEdgeLabel.value.length < titleSizeLimit
                  ? currentEdgeLabel.value
                  : currentEdgeLabel.value.slice(0, titleSizeLimit - 3) + "..."
              }
            />
          }
        />
      </CustomTooltip>
    </>
  );
};

export default LabelMenus;
