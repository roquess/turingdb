import { useVisualizerContext } from '@/visualizer'
import { GraphNodeData } from '@/visualizer/types'
import { Icon } from '@blueprintjs/core'

import AppPopoverMenu from '@/components/base/AppPopoverMenu'
import AppPopoverMenuItem from '@/components/base/AppPopoverMenuItem'
import AppPopoverMenuDivider from '@/components/base/AppMenuDivider'

import { useVisualizerTabsContext } from '@/app/VisualizerTabsContext'
import { useVisualizerPageContext } from '@/app/VisualizerPageContext'
import { useMenuActions } from '@/visualizer/components/ContextMenu/hooks'
import NodeColorContextMenuItem from './NodeColorContextMenuItem'
import { useCallback } from 'react'

export default function NodeContextMenu(props: { propertyTypes: any[] }) {
  const { propertyTypes } = props
  const vis = useVisualizerContext()
  const menuData = vis.contextMenuData()
  if (!vis || !menuData) {
    throw new Error('No vis context')
  }
  const nodeData = menuData.data as unknown as GraphNodeData
  const { addTab } = useVisualizerTabsContext()
  const { setActiveNode, setPathwaysDialogNodeData } = useVisualizerPageContext()
  const menuActions = useMenuActions()

  const selectAllBySameLayout = useCallback(() => {
    const state = vis.state()
    if (!state) {
      throw new Error('No vis state')
    }

    const mapping = state.layouts.mapping as Record<string, unknown>

    vis
      .cy()
      ?.nodes()
      .filter((n) => mapping[n.id()] === mapping[nodeData.id])
      // @ts-expect-error wrong cytoscape types
      .forEach((n) => n.select())
  }, [vis, nodeData])

  return (
    <AppPopoverMenu>
      <AppPopoverMenuItem text="Show details" onClick={() => setActiveNode(nodeData)} />
      <AppPopoverMenuItem text="Hide" onClick={() => menuActions.hideNodes()} />
      <AppPopoverMenuItem
        text="Show in Pathways"
        onClick={() => setPathwaysDialogNodeData(nodeData)}
      />
      <AppPopoverMenuItem text="Open in new tab" onClick={() => addTab(nodeData.turing_id)} />
      <AppPopoverMenuItem
        text="Collapse neighbors"
        onClick={() => menuActions.collapseNeighbors()}
      />
      <AppPopoverMenuDivider />
      <AppPopoverMenuItem text="Select all...">
        <AppPopoverMenuItem
          icon="form"
          text="by same node type"
          onClick={() => menuActions.selectAllBySameNodeType(nodeData.node_type_name)}
        />
        <AppPopoverMenuItem icon="property" text="by same property...">
          {Object.keys(nodeData.properties).map((propName) => (
            <AppPopoverMenuItem
              key={propName}
              icon="label"
              text={propName}
              onClick={() =>
                menuActions.selectAllBySameNodeProperty(nodeData.properties[propName], propName)
              }
            />
          ))}
        </AppPopoverMenuItem>
        <AppPopoverMenuItem icon="layout" text="by same layout" onClick={selectAllBySameLayout} />
      </AppPopoverMenuItem>
      <AppPopoverMenuDivider />
      <NodeColorContextMenuItem propertyTypes={propertyTypes} />
      <AppPopoverMenuItem text="Set layout...">
        <AppPopoverMenuItem
          text="Vertical line"
          icon={<Icon icon="layout-linear" style={{ rotate: '90deg' }} />}
          onClick={menuActions.setVerticalLine}
        />
        <AppPopoverMenuItem
          icon="layout-auto"
          text="Auto"
          onClick={() => {
            const eles = vis.cy()?.filter((e) => e.selected()) || []
            if (eles.length === 0) return
            vis.callbacks()?.resetLayout(eles.map((e) => e.id()))
          }}
        />
      </AppPopoverMenuItem>
    </AppPopoverMenu>
  )
}
