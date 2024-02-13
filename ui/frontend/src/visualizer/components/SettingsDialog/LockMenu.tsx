// Core
import React from "react";
import { useVisualizerContext } from "../../context";

// @blueprintjs
import {
  Tooltip,
  TooltipProps,
  MenuItem,
  Button,
  PopoverProps,
  ButtonProps,
} from "@blueprintjs/core";
import { SelectMenu } from "../ContextMenu/select";
import { lockBehaviours } from "../../reducers/layouts";

const ttParams = {
  hoverCloseDelay: 40,
  hoverOpenDelay: 400,
};

const LockMenu = (props: {
  tooltip?: (props: TooltipProps) => React.ReactNode;
  button?: (props: ButtonProps) => React.ReactNode;
  className?: string;
  selectClassName?: string;
  selectPopoverProps?: PopoverProps;
}) => {
  const vis = useVisualizerContext();
  const lockValues = ["Main nodes", "Interacted nodes", "None"];
  const [lock, setLock] = React.useState("Interacted nodes");

  React.useEffect(() => {
    let lockValue = 0;

    switch (lock) {
      case "Main nodes": {
        lockValue = lockBehaviours.ALL_SELECTED;
        break;
      }
      case "Interacted nodes": {
        lockValue = lockBehaviours.INTERACTED;
        break;
      }
      case "None": {
        lockValue = lockBehaviours.DO_NOT_LOCK;
        break;
      }
    }

    if (vis.state()!.layouts.definitions[0]!.lockBehaviour !== lockValue) {
      vis.callbacks()!.setDefaultCyLayout({
        ...vis.state()!.layouts.definitions[0]!,
        lockBehaviour: lockValue,
      });
    }
  }, [vis, lock]);

  const renderValue = (behaviour: string) => {
    return (
      <MenuItem
        key={"lock-behaviour-" + behaviour}
        text={behaviour}
        onClick={() => setLock(behaviour)}
      />
    );
  };

  const CustomTooltip = props.tooltip
    ? (tooltipProps: TooltipProps) =>
        props.tooltip && props.tooltip(tooltipProps)
    : Tooltip;

  const CustomButton = props.button
    ? (buttonProps: ButtonProps) => props.button && props.button(buttonProps)
    : Button;

  return (
    <div className={`flex items-center space-x-2 ${props.className}`}>
      <div>Lock nodes during layout run</div>
      <CustomTooltip content="Lock nodes during layout run" {...ttParams}>
        <SelectMenu
          key={"select-menu-set-node-label"}
          className={props.selectClassName}
          popoverProps={props.selectPopoverProps}
          renderChild={renderValue}
          items={lockValues}
          placement={"bottom"}
          Parent={
            <CustomButton
              key="set-node-label"
              fill
              style={{ width: "180px" }}
              alignText="left"
              icon="lock"
              text={lock}
              rightIcon="caret-down"
            />
          }
        />
      </CustomTooltip>
    </div>
  );
};

export default LockMenu;
