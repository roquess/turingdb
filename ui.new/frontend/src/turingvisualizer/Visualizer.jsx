import React from "react";
import useVisualizerState from "./state";
import useSearchNodesWindow from "./useSearchNodesWindow";
import { useVisualizerContext } from ".";
import useKeepOnlyAlert from "./useKeepOnlyAlert";

const Visualizer = (props) => {
  useVisualizerState(props.cyStyle);
  const vis = useVisualizerContext();
  useSearchNodesWindow();
  useKeepOnlyAlert();

  return (
    <div
      style={{
        flexGrow: 1,
        position: "relative",
        overflow: "hidden",
      }}>
      {props.canvas}
      {props.contextMenu}
      {Object.values(vis.dialogs()).map((d) => d.Content())}
      <div
        style={{
          position: "absolute",
          width: "100%",
          height: "100%",
          pointerEvents: "none",
        }}>
        {React.Children.map(props.children, (child) =>
          React.cloneElement(child, {
            ...child.props,
            style: {
              ...child.props.style,
              pointerEvents: "auto",
            },
          })
        )}
      </div>
    </div>
  );
};
export default Visualizer;
