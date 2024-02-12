import { ContextMenu, useContextMenuData } from '@/visualizer/components/ContextMenu/ContextMenu'
import { useVisualizerContext } from '@/visualizer'
import NodeContextMenu from './NodeContextMenu'
import EdgeContextMenu from './EdgeContextMenu'
import BackgroundContextMenu from './BackgroundContextMenu'
import clsx from 'clsx'
import { useState } from 'react'
import { useCanvasTrigger } from '@/visualizer/useCanvasTrigger'

const getPropertyTypesFromElements = (
  elements: cytoscape.NodeCollection | cytoscape.EdgeCollection | []
) =>
  elements
    .map((e) => Object.keys(e.data().properties))
    .flat()
    .filter((p, i, arr) => arr.indexOf(p) === i)

export default function TheVisualizerContextMenu() {
  useContextMenuData()
  const vis = useVisualizerContext()
  const cy = vis.cy()
  const menuData = vis.contextMenuData()
  const isNode = menuData?.group === 'nodes'
  const isEdge = menuData?.group === 'edges'
  const isBackground = menuData?.group === 'background'

  const [nodePropertyTypes, setNodePropertyTypes] = useState<string[]>([])
  const [edgePropertyTypes, setEdgePropertyTypes] = useState<string[]>([])
  const isVisible = isNode || isEdge || isBackground

  useCanvasTrigger({
    category: 'elements',
    name: 'contextMenu-setPropertyTypes',

    callback: () => {
      setNodePropertyTypes(getPropertyTypesFromElements(cy?.nodes() || []))
      setEdgePropertyTypes(getPropertyTypesFromElements(cy?.edges() || []))
    },
  })
  return (
    <ContextMenu
      popoverProps={{
        popoverClassName: clsx('app-popover visualizer-context-menu-popover', {
          hidden: !isVisible,
        }),
      }}
      className="visualizer-context-menu"
    >
      {isNode && <NodeContextMenu propertyTypes={nodePropertyTypes} />}
      {isEdge && <EdgeContextMenu propertyTypes={edgePropertyTypes} />}
      {isBackground && <BackgroundContextMenu />}
    </ContextMenu>
  )
}
