import React from "react";
import {
  Button,
  Dialog,
  Tooltip,
  Checkbox,
  Slider,
  FormGroup,
} from "@blueprintjs/core";
import LockMenu from "./LockMenu";
import { useVisualizerContext } from "../../";
import { useDialog, dialogParams, ttParams } from "../../tools";
import { useWidgetValue } from "./hooks";
import NodeFilters from "./NodeFilters";

const LENGTH_RATIO = 0.1;

type DialogProps = {
  isOpen: boolean;
  onClose: () => void;
  centerOnDoubleClicked: {
    value: boolean;
    onRelease: () => void;
    onChange: (v: boolean) => void;
  };
  edgeLengthVal: {
    value: number;
    onRelease: () => void;
    onChange: (v: number) => void;
  };
  nodeSpacing: {
    value: number | undefined;
    onRelease: () => void;
    onChange: (v: number | undefined) => void;
  };
  min: number;
  max: number;
  labelValues: number[];
  LENGTH_RATIO: number;
} & typeof dialogParams;

export default function SettingsDialog(props: {
  trigger?: ({ open }: { open: () => void }) => React.ReactNode;
  dialog?: (props: DialogProps) => React.ReactNode;
}) {
  const vis = useVisualizerContext();
  const dialog = useDialog();

  const centerOnDoubleClicked = useWidgetValue({
    getCanvasValue: () => vis.state()!.layouts.centerOnDoubleClicked,
    setCanvasValue: vis.callbacks()!.centerOnDoubleClicked,
  });

  const edgeLengthVal = useWidgetValue({
    getCanvasValue: () => vis.state()!.layouts.definitions[0]!.edgeLengthVal!,
    setCanvasValue: (v) => {
      vis.callbacks()!.setDefaultCyLayout({
        ...vis.state()!.layouts.definitions[0]!,
        edgeLengthVal: v,
      });
    },
  });

  const nodeSpacing = useWidgetValue({
    getCanvasValue: () => vis.state()!.layouts.definitions[0]!.nodeSpacing,
    setCanvasValue: (v) => {
      vis.callbacks()!.setDefaultCyLayout({
        ...vis.state()!.layouts.definitions[0]!,
        nodeSpacing: v,
      });
    },
  });

  const trigger = props.trigger ? (
    props.trigger({ open: dialog.open })
  ) : (
    <Tooltip {...ttParams} content="Canvas settings">
      <Button text="Settings" icon="settings" onClick={dialog.open} />
    </Tooltip>
  );

  const dialogSlotData: DialogProps = {
    ...dialogParams,
    isOpen: dialog.isOpen,
    onClose: dialog.close,
    centerOnDoubleClicked,
    edgeLengthVal,
    nodeSpacing,
    min: 1,
    max: 100,
    labelValues: [1, 50, 100],
    LENGTH_RATIO,
  };

  return (
    <>
      {trigger}

      {props.dialog ? (
        props.dialog(dialogSlotData)
      ) : (
        <Dialog
          {...dialogParams}
          isOpen={dialog.isOpen}
          onClose={dialog.close}
          title="Canvas settings"
        >
          <div className="flex h-[60vh] flex-col overflow-auto p-10">
            <FormGroup label="Node spacing">
              <Slider
                min={1}
                labelValues={[1, 50, 100]}
                max={100}
                initialValue={nodeSpacing.value}
                value={nodeSpacing.value}
                onRelease={nodeSpacing.onRelease}
                onChange={nodeSpacing.onChange}
              />
            </FormGroup>

            <FormGroup label="Edge length">
              <Slider
                min={1}
                labelValues={[1, 50, 100]}
                max={100}
                initialValue={edgeLengthVal.value * LENGTH_RATIO}
                value={edgeLengthVal.value * LENGTH_RATIO}
                onRelease={edgeLengthVal.onRelease}
                onChange={(v) => edgeLengthVal.onChange(v / LENGTH_RATIO)}
              />
            </FormGroup>

            <NodeFilters />
            <LockMenu />
            <Tooltip
              content="Double clicking a node centers the canvas onto it"
              className="w-max"
            >
              <Checkbox
                label="Center on double click"
                checked={centerOnDoubleClicked.value}
                onChange={(e) => {
                  centerOnDoubleClicked.onChange(e.target.checked);
                  centerOnDoubleClicked.onRelease();
                }}
              />
            </Tooltip>
          </div>
        </Dialog>
      )}
    </>
  );
}
