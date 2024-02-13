import React from "react";

import {
  Button,
  InputGroup,
  FormGroup,
  Menu,
  MenuItem,
  Tag,
  Icon,
  PopoverProps,
} from "@blueprintjs/core";
import { ItemPredicate, ItemRenderer, Select } from "@blueprintjs/select";

const renderProp: ItemRenderer<string> = (
  prop,
  { handleClick, handleFocus, modifiers }
) => {
  if (!modifiers.matchesPredicate) {
    return null;
  }

  return (
    <MenuItem
      active={modifiers.active}
      disabled={modifiers.disabled}
      key={"item-" + prop}
      onClick={handleClick}
      onFocus={handleFocus}
      roleStructure="listoption"
      text={prop}
    />
  );
};

const renderList = ({ items, renderItem, itemsParentRef }) => {
  const renderedItems = items.map(renderItem);

  return (
    <Menu
      role="listbox"
      ulRef={itemsParentRef}
      className="max-h-[30vh] overflow-auto"
    >
      {renderedItems}
    </Menu>
  );
};

const filterProp: ItemPredicate<string> = (query, prop, _index, exactMatch) => {
  const normalizedProp = prop.toLowerCase();
  const normalizedQuery = query.toLowerCase();

  if (exactMatch) {
    return normalizedProp === normalizedQuery;
  }

  return normalizedProp.indexOf(normalizedQuery) >= 0;
};

export const SelectProperty = ({
  items,
  onItemSelect,
  value,
  className,
  popoverProps,
}: {
  items: string[];
  onItemSelect: (value: string) => void;
  value: string;
  className?: string;
  popoverProps?: PopoverProps;
}) => {
  return (
    <Select
      items={items}
      itemRenderer={renderProp}
      itemListRenderer={renderList}
      itemPredicate={filterProp}
      noResults={
        <MenuItem
          disabled={true}
          text="No results."
          roleStructure="listoption"
        />
      }
      onItemSelect={onItemSelect}
      className={className}
      popoverProps={popoverProps}
    >
      <Button
        text={value}
        rightIcon="double-caret-vertical"
        placeholder="Select property"
      />
    </Select>
  );
};
export default function QueryNodesBar(props: {
  propNames: string[];
  setPropName: React.Dispatch<React.SetStateAction<string>>;
  propName: string;
  setPropValue: React.Dispatch<React.SetStateAction<string>>;
  nodeCount: number;
  propValue: string;
}) {
  return (
    <>
      <FormGroup label="Find by property" inline className="px-5 pt-2.5">
        <SelectProperty
          items={props.propNames}
          onItemSelect={props.setPropName}
          value={props.propName}
        />
      </FormGroup>

      <div className="px-5 pb-5">
        <InputGroup
          id="find-nodes-in-canvas"
          leftElement={<Icon icon="search" />}
          onChange={(e) => props.setPropValue(e.target.value)}
          placeholder="Find nodes"
          rightElement={<Tag minimal>{props.nodeCount}</Tag>}
          value={props.propValue}
        />
      </div>
    </>
  );
}
