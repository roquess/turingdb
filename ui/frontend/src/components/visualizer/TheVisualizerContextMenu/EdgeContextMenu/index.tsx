import { useVisualizerContext } from '@/visualizer'
import { GraphNodeData } from '@/visualizer/types'
import { Icon } from '@blueprintjs/core'

import AppPopoverMenu from '@/components/base/AppPopoverMenu'
import AppPopoverMenuItem from '@/components/base/AppPopoverMenuItem'
import AppPopoverMenuDivider from '@/components/base/AppMenuDivider'

import { useVisualizerTabsContext } from '@/app/VisualizerTabsContext'
import { useVisualizerPageContext } from '@/app/VisualizerPageContext'
import { useMenuActions } from '@/visualizer/components/ContextMenu/hooks'
import NodeColorContextMenu from '../NodeContextMenu/NodeColorContextMenuItem'
import EdgeColorContextMenuItem from './EdgeColorContextMenuItem'

export default function EdgeContextMenu(props: { propertyTypes: any[] }) {
  const { propertyTypes } = props

  return (
    <AppPopoverMenu>
      <EdgeColorContextMenuItem propertyTypes={propertyTypes} />
    </AppPopoverMenu>
  )
}
