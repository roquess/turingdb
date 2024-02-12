import React from "react";

import {
  Tooltip,
  Checkbox,
  TooltipProps,
  CheckboxProps,
} from "@blueprintjs/core";
import { useVisualizerContext } from "../../context";

function useFilters() {
  const vis = useVisualizerContext();
  const filtersState = React.useState(vis.state()!.filters);

  React.useEffect(() => {
    Object.keys(filtersState[0]).forEach((key) => {
      if (filtersState[0][key] !== vis.state()!.filters[key]) {
        vis.callbacks()!.setFilters({
          ...vis.state()!.filters,
          ...filtersState[0],
        });
        return;
      }
    });
  }, [vis, filtersState]);

  return filtersState;
}

export default function NodeFilters(props: {
  checkbox?: (props: CheckboxProps) => React.ReactNode;
  tooltip?: (props: TooltipProps) => React.ReactNode;
}) {
  const [filters, setFilters] = useFilters();

  const CustomTooltip = props.tooltip
    ? (tooltipProps: TooltipProps) =>
        props.tooltip && props.tooltip(tooltipProps)
    : Tooltip;
  const CustomCheckbox = props.checkbox
    ? (checkboxProps: CheckboxProps) =>
        props.checkbox && props.checkbox(checkboxProps)
    : Checkbox;

  return (
    <div className="flex flex-col">
      <CustomTooltip content="Hide publications" className="w-max">
        <CustomCheckbox
          label="Hide publications"
          checked={filters.hidePublications}
          onChange={(e) => {
            setFilters({
              ...filters,
              hidePublications: e.target.checked,
            });
          }}
        />
      </CustomTooltip>

      <CustomTooltip
        content='Hide compartments such as "extracellular region"'
        className="w-max"
      >
        <CustomCheckbox
          label="Hide compartments"
          checked={filters.hideCompartments}
          onChange={(e) => {
            setFilters({
              ...filters,
              hideCompartments: e.target.checked,
            });
          }}
        />
      </CustomTooltip>
      <CustomTooltip
        content='Hide species nodes such as "Homo sapiens"'
        className="w-max"
      >
        <CustomCheckbox
          label="Hide species"
          checked={filters.hideSpecies}
          onChange={(e) => {
            setFilters({
              ...filters,
              hideSpecies: e.target.checked,
            });
          }}
        />
      </CustomTooltip>
      <CustomTooltip content="Hide database references" className="w-max">
        <CustomCheckbox
          label="Hide database references"
          checked={filters.hideDatabaseReferences}
          onChange={(e) => {
            setFilters({
              ...filters,
              hideDatabaseReferences: e.target.checked,
            });
          }}
        />
      </CustomTooltip>
      <CustomTooltip content="Show Homo sapiens only nodes" className="w-max">
        <CustomCheckbox
          label="Homo sapiens only"
          checked={filters.showOnlyHomoSapiens}
          onChange={(e) => {
            setFilters({
              ...filters,
              showOnlyHomoSapiens: e.target.checked,
            });
          }}
        />
      </CustomTooltip>
    </div>
  );
}
