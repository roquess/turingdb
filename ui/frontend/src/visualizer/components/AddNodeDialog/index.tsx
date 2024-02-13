import React from "react";
import {
  Button,
  Tooltip,
  Dialog,
  InputGroup,
  Spinner,
  Card,
  Tag,
} from "@blueprintjs/core";

import {
  ttParams,
  dialogParams,
  useDialog,
  useDefinedState,
} from "../../tools";
import {
  getNodePropValue,
  getNodeSecondaryPropValue,
  useNodes,
  usePropertyTypes,
} from "./nodes";
import { useVisualizerContext } from "../../context";
import { SelectProperty } from "./SelectProperty";
import { GraphNodeData } from "../../types";

type DialogProps = {
  isOpen: boolean;
  onClose: () => void;
  onSearch: (v: string) => void;
  onSubmit: () => void;
  loading: boolean;
  nodes: GraphNodeData[];
  getNodePropValue: (node: GraphNodeData) => string;
  getNodeSecondaryPropValue: (node: GraphNodeData) => string;
  addNode: (node: GraphNodeData) => void;
  removeNode: (node: GraphNodeData) => void;
  setInspectedNode: (node: GraphNodeData) => void;
  isNodeSelected: (node: GraphNodeData) => boolean;
  currentPropName: ReturnType<typeof useDefinedState<string>>;
  properties: string[];
} & typeof dialogParams;

export default function AddNodeDialog(props: {
  trigger?: ({ open }: { open: () => void }) => React.ReactNode;
  dialog?: (props: DialogProps) => React.ReactNode;
}) {
  const vis = useVisualizerContext();
  const dialog = useDialog();
  const currentPropName = useDefinedState("");
  const [currentPropValue, setCurrentPropValue] = React.useState("");
  const properties = usePropertyTypes(vis.state()!.dbName);
  const selectedNodeIds = vis.state()!.selectedNodeIds;

  React.useEffect(() => {
    if (properties.includes("displayName")) {
      currentPropName.setOnce("displayName");
      return;
    }
    if (properties.includes("label")) {
      currentPropName.setOnce("label");
      return;
    }
  }, [properties, currentPropName]);

  const { loading, sortedNodes, query } = useNodes(
    vis.state()!.dbName,
    currentPropName.value,
    currentPropValue
  );

  const trigger = props.trigger ? (
    props.trigger({ open: dialog.open })
  ) : (
    <Tooltip {...ttParams} content="Add nodes from the database">
      <Button text="Add nodes" icon="add" onClick={dialog.open} />
    </Tooltip>
  );

  const addNode = (node: GraphNodeData) => {
    vis
      .callbacks()!
      .setSelectedNodeIds([...vis.state()!.selectedNodeIds, node.turing_id]);
  };

  const removeNode = (node: GraphNodeData) => {
    vis
      .callbacks()!
      .setSelectedNodeIds([
        ...vis.state()!.selectedNodeIds.filter((id) => node.turing_id !== id),
      ]);
  };

  const dialogSlotData = {
    ...dialogParams,
    isOpen: dialog.isOpen,
    onClose: dialog.close,
    onSearch: (v: string) => setCurrentPropValue(v),
    onSubmit: query,
    loading,
    nodes: sortedNodes,
    isNodeSelected: (n: GraphNodeData) => selectedNodeIds.includes(n.turing_id),
    getNodePropValue: (n: GraphNodeData) =>
      getNodePropValue(n, currentPropName.value),
    getNodeSecondaryPropValue: (n: GraphNodeData) =>
      getNodeSecondaryPropValue(n, currentPropName.value),
    addNode,
    removeNode,
    setInspectedNode: (node: GraphNodeData) =>
      vis.callbacks()?.setInspectedNode(node),
    currentPropName: currentPropName,
    properties,
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
          title="Add nodes from the database"
        >
          <div className="h-[50vh]">
            <form
              onSubmit={(e) => {
                e.preventDefault();
                query();
              }}
            >
              <div className="flex items-center space-x-2 p-6 pb-4 pt-4">
                <div>Find by property</div>
                <SelectProperty
                  currentProp={currentPropName.value}
                  setCurrentProp={currentPropName.set}
                  properties={properties}
                />
                <InputGroup
                  className="flex-1"
                  onChange={(e) => setCurrentPropValue(e.target.value)}
                />
                <Button type="submit" icon="search" />
              </div>
            </form>
            <div className="h-full w-full overflow-auto">
              {loading ? (
                <div className="flex h-full w-full items-center justify-center">
                  <Spinner size={100} />
                </div>
              ) : (
                sortedNodes.map((n) => {
                  const isSelected = selectedNodeIds.includes(n.turing_id);

                  return (
                    <Card
                      key={n.id}
                      interactive
                      compact
                      className="flex justify-between"
                      onClick={(e) => {
                        e.preventDefault();
                        vis.callbacks()?.setInspectedNode(n);
                        e.stopPropagation();
                      }}
                    >
                      <div className="flex space-x-2">
                        <Tag className="w-20 text-center">{n.turing_id}</Tag>
                        <div>
                          <div className={isSelected ? "primary-light" : ""}>
                            {getNodePropValue(n, currentPropName.value)}
                          </div>
                          <div className="gray3 pl-9 text-sm">
                            {getNodeSecondaryPropValue(
                              n,
                              currentPropName.value
                            )}
                          </div>
                        </div>
                      </div>
                      {isSelected ? (
                        <Button
                          icon="remove"
                          onClick={(
                            e: React.MouseEvent<HTMLElement, MouseEvent>
                          ) => {
                            e.preventDefault();
                            removeNode(n);
                            e.stopPropagation();
                          }}
                        />
                      ) : (
                        <Button
                          icon="add"
                          onClick={(
                            e: React.MouseEvent<HTMLElement, MouseEvent>
                          ) => {
                            e.preventDefault();
                            addNode(n);
                            e.stopPropagation();
                          }}
                        />
                      )}
                    </Card>
                  );
                })
              )}
            </div>
          </div>
        </Dialog>
      )}
    </>
  );
}
