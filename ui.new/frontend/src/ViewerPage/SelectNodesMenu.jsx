// @blueprintjs
import { Menu } from "@blueprintjs/core";

// Turing
import * as items from "@turingvisualizer/ContextMenu/items";
import { useMenuActions } from "@turingvisualizer/ContextMenu/hooks";

const SelectNodesMenu = () => {
  const actions = useMenuActions();

  return (
    <Menu>
      <items.ItemSelectAllBySameNodeTypeNoData actions={actions} />
      <items.ItemSelectAllBySamePropertyNoData actions={actions} />
    </Menu>
  );
};

export default SelectNodesMenu;
