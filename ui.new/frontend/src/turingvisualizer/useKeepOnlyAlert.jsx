import { Alert } from "@blueprintjs/core";
import { useDialog } from "./dialogs";
import { useVisualizerContext } from ".";

const useKeepOnlyAlert = () => {
  const vis = useVisualizerContext();

  const nodes =
    vis
      .cy()
      ?.nodes()
      .filter((n) => n.selected()) || [];

  useDialog({
    name: "keep-only-alert",
    content: () => {
      return (
        <p>
          Are you sure you want to keep only the selected nodes ? You will start
          a new network exploration from this set.
        </p>
      );
    },
    isAlert: true,
    alertProps: {
      cancelButtonText: "Cancel",
      confirmButtonText: `Keep only ${nodes.length} nodes`,
      icon: "trash",
      intent: "danger",
      onConfirm: ({ data, toggle }) => {
        data().actions.keepOnly();
        toggle();
      },
      onCancel: ({ toggle }) => toggle(),
    },
  });
};

export default useKeepOnlyAlert;
