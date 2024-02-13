import AppPopoverMenuItem from '@/components/base/AppPopoverMenuItem'
import AppPopoverMenuDivider from '@/components/base/AppMenuDivider'
import ColorMenuItem from '@/components/visualizer/TheVisualizerContextMenu/ColorMenuItem'
import { COLOR_MODES, DISCRETE_COLORS } from '@/visualizer/constants'
import { Icon } from '@blueprintjs/core'

const EdgeColorContextMenuItems = {
  None: () => (
    <ColorMenuItem
      text="None"
      icon="cross"
      elGroup="edges"
      colorMode={COLOR_MODES.None}
      colorData={{}}
      key="edge-none"
    />
  ),
  Preset: () => (
    <AppPopoverMenuItem text="Preset" icon="edit">
      {DISCRETE_COLORS.map((col) => (
        <ColorMenuItem
          text={col}
          icon={<Icon icon="symbol-square" color={col} />}
          elGroup="edges"
          colorMode={COLOR_MODES.Preset}
          colorData={{ color: col }}
          key={'edge-preset-' + col}
        />
      ))}
    </AppPopoverMenuItem>
  ),
  Gradient: ({ propertyTypes }: { propertyTypes: any[] }) => (
    <AppPopoverMenuItem text="Based on property..." icon="property" key="edge-gradient-property">
      {propertyTypes.map((propName) => (
        <ColorMenuItem
          text={propName}
          icon="label"
          elGroup="edges"
          colorMode={COLOR_MODES.GradientProperty}
          colorData={{ propName }}
          key={'edge-gradient-' + propName}
        />
      ))}
    </AppPopoverMenuItem>
  ),
  DiscreteProperty: ({ propertyTypes }: { propertyTypes: any[] }) => (
    <AppPopoverMenuItem text="Based on property..." icon="property" key="edge-discrete-property">
      {propertyTypes.map((propName) => (
        <ColorMenuItem
          text={propName}
          icon="label"
          elGroup="edges"
          colorMode={COLOR_MODES.DiscreteProperty}
          colorData={{ propName }}
          key={'edge-discrete-' + propName}
        />
      ))}
    </AppPopoverMenuItem>
  ),
  NodeType: () => (
    <ColorMenuItem
      text="Based on edge type"
      icon="one-to-one"
      elGroup="edges"
      colorMode={COLOR_MODES.EdgeType}
      colorData={{}}
      key="edge-type"
    />
  ),
}

export default function EdgeColorContextMenuItem(props: { propertyTypes: any[] }) {
  const { propertyTypes } = props
  return (
    <AppPopoverMenuItem text="Set edge colors...">
      <AppPopoverMenuDivider title="General" />
      <EdgeColorContextMenuItems.None />
      <EdgeColorContextMenuItems.Preset />

      <AppPopoverMenuDivider title="Gradient" />
      <EdgeColorContextMenuItems.Gradient propertyTypes={propertyTypes} />

      <AppPopoverMenuDivider title="Discrete" />
      <EdgeColorContextMenuItems.DiscreteProperty propertyTypes={propertyTypes} />
      <EdgeColorContextMenuItems.NodeType />
    </AppPopoverMenuItem>
  )
}
