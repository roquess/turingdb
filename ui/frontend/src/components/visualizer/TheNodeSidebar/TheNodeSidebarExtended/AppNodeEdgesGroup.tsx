import clsx from 'clsx'
import AppBadge from '@/components/base/AppBadge'
import AppExpandable from '@/components/base/AppExpandable'
import AppContextMenu from '@/components/base/AppContextMenu'
import AppCollapseTrigger from './AppCollapseTrigger'
import AppPopoverMenu from '@/components/base/AppPopoverMenu'
import AppPopoverMenuItem from '@/components/base/AppPopoverMenuItem'
import { useVisualizerPageContext } from '@/app/VisualizerPageContext'
import { useVisualizerTabsContext } from '@/app/VisualizerTabsContext'

function NodeEdgeCard({
  edge,
  isSelected,
  onClick,
}: {
  edge: any
  isSelected: boolean
  onClick: (edge: any) => void
}) {
  const { addTab } = useVisualizerTabsContext()
  const { getEdgeReferenceNode } = useVisualizerPageContext()
  const referenceNode = getEdgeReferenceNode(edge)

  if (!referenceNode) {
    throw new Error('Cannot find reference node')
  }
  const referenceNodeId = referenceNode.data.turing_id
  const properties = referenceNode.data.properties

  return (
    <AppContextMenu
      popoverProps={{
        popoverClassName: 'w-[15rem]',
      }}
      content={
        <AppPopoverMenu>
          <AppPopoverMenuItem text="Show in new tab" onClick={() => addTab(referenceNodeId)} />
        </AppPopoverMenu>
      }
    >
      <li
        className={clsx([
          'cursor-pointer rounded-[0.25rem] border bg-grey-700 p-2 text-xs text-content-primary transition-colors hover:bg-grey-600',
          isSelected ? 'border-primary-default' : 'border-white border-opacity-[0.08]',
        ])}
        key={edge.data.turing_id}
        onClick={() => onClick(edge)}
      >
        <p className="pb-2 font-medium leading-[1.16] tracking-[0.06em] transition-colors">
          {properties.displayName}
        </p>
        <AppBadge alt>{referenceNodeId}</AppBadge>
        {properties.definition && (
          <p className="pt-3 leading-[1.33] tracking-[0.02em] text-content-secondary">
            {properties.definition}
          </p>
        )}
      </li>
    </AppContextMenu>
  )
}

export default function AppNodeEdgesGroup({
  edges,
  type,
  onEdgeClick,
  selectedEdges = [],
}: {
  edges: any[]
  type: string
  onEdgeClick: (edge: any) => void
  selectedEdges?: any[]
}) {
  const isSelected = (edge: any) =>
    selectedEdges.findIndex((v: any) => v.data.turing_id === edge.data.turing_id) !== -1
  const isExpandable = edges.length > 8
  const preview = edges.slice(0, 8)
  const rest = edges.slice(8)

  return (
    <div>
      <p className="mb-2 text-xs font-medium">{type}:</p>
      <ul className="grid grid-cols-[11.53125rem_11.53125rem] gap-2">
        {preview.map((edge) => (
          <NodeEdgeCard
            key={edge.data.turing_id}
            edge={edge}
            isSelected={isSelected(edge)}
            onClick={onEdgeClick}
          />
        ))}
      </ul>
      {isExpandable && (
        <AppExpandable
          trigger={({ setIsExpanded }) => (
            <AppCollapseTrigger onClick={() => setIsExpanded(true)}>Show more</AppCollapseTrigger>
          )}
          hideTriggerOnExpand
        >
          <ul className="mt-3 grid grid-cols-[11.53125rem_11.53125rem] gap-2">
            {rest.map((edge) => (
              <NodeEdgeCard
                key={edge.data.turing_id}
                edge={edge}
                isSelected={isSelected(edge)}
                onClick={onEdgeClick}
              />
            ))}
          </ul>
        </AppExpandable>
      )}
    </div>
  )
}
