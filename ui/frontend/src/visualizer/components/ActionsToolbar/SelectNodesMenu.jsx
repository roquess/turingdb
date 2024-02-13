// @blueprintjs
import { Menu } from "@blueprintjs/core";

// Turing
import * as items from "../ContextMenu/items";
import { useMenuActions } from "../ContextMenu/hooks";

const SelectNodesMenu = ({
  className,
  selectClassName,
  selectPopoverProps,
  button
} = {}) => {
  const actions = useMenuActions();

  return (
    <Menu className={className}>
      <items.ItemSelectAllBySameNodeTypeNoData
        actions={actions}
        className={selectClassName}
        popoverProps={selectPopoverProps}
        button={button}
      />
      <items.ItemSelectAllBySamePropertyNoData
        actions={actions}
        className={selectClassName}
        popoverProps={selectPopoverProps}
        button={button}
      />
    </Menu>
  );
};

export default SelectNodesMenu;
