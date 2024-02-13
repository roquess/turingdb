import AppPopoverMenuItem from '@/components/base/AppPopoverMenuItem'
import AppPopoverMenuDivider from '@/components/base/AppMenuDivider'
import ColorMenuItem from '@/components/visualizer/TheVisualizerContextMenu/ColorMenuItem'
import { COLOR_MODES, DISCRETE_COLORS } from '@/visualizer/constants'
import { Icon } from '@blueprintjs/core'

const NodeColorContextMenuItems = {
  None: () => (
    <ColorMenuItem
      text="None"
      icon="cross"
      elGroup="nodes"
      colorMode={COLOR_MODES.None}
      key="node-none"
    />
  ),
  Preset: () => (
    <AppPopoverMenuItem text="Preset" icon="edit">
      {DISCRETE_COLORS.map((col) => (
        <ColorMenuItem
          text={col}
          icon={<Icon icon="symbol-square" color={col} />}
          elGroup="nodes"
          colorMode={COLOR_MODES.Preset}
          colorData={{ color: col }}
          key={'node-preset-' + col}
        />
      ))}
    </AppPopoverMenuItem>
  ),
  Gradient: ({ propertyTypes }: { propertyTypes: any[] }) => (
    <AppPopoverMenuItem text="Based on property..." icon="property" key="node-gradient-property">
      {propertyTypes.map((propName) => (
        <ColorMenuItem
          text={propName}
          icon="label"
          elGroup="nodes"
          colorMode={COLOR_MODES.GradientProperty}
          colorData={{ propName }}
          key={'node-gradient-' + propName}
        />
      ))}
    </AppPopoverMenuItem>
  ),
  DiscreteProperty: ({ propertyTypes }: { propertyTypes: any[] }) => (
    <AppPopoverMenuItem text="Based on property..." icon="property" key="node-discrete-property">
      {propertyTypes.map((propName) => (
        <ColorMenuItem
          text={propName}
          icon="label"
          elGroup="nodes"
          colorMode={COLOR_MODES.DiscreteProperty}
          colorData={{ propName }}
          key={'node-discrete-' + propName}
        />
      ))}
    </AppPopoverMenuItem>
  ),
  NodeType: () => (
    <ColorMenuItem
      text="Based on node type"
      icon="graph"
      elGroup="nodes"
      colorMode={COLOR_MODES.NodeType}
      key="node-type"
    />
  ),
}

export default function NodeColorContextMenuItem(props: { propertyTypes: any[] }) {
  const { propertyTypes } = props
  return (
    <AppPopoverMenuItem text="Set node colors...">
      <AppPopoverMenuDivider title="General" />
      <NodeColorContextMenuItems.None />
      <NodeColorContextMenuItems.Preset />

      <AppPopoverMenuDivider title="Gradient" />
      <NodeColorContextMenuItems.Gradient propertyTypes={propertyTypes} />

      <AppPopoverMenuDivider title="Discrete" />
      <NodeColorContextMenuItems.DiscreteProperty propertyTypes={propertyTypes} />
      <NodeColorContextMenuItems.NodeType />
    </AppPopoverMenuItem>
  )
}
