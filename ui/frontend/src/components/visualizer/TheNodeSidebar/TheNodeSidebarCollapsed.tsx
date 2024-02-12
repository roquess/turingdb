import { NodePropertyValue, propertyKeyToString } from '@/components/visualizer/common/properties'
import _groupBy from 'lodash.groupby'
import AppBadge from '@/components/base/AppBadge'
import AppLinkButton from '@/components/base/AppLinkButton'
import AppExpandable from '@/components/base/AppExpandable'
import { useEffect, useRef, useState } from 'react'
import clsx from 'clsx'
import { EdgeWithNodes, useVisualizerPageContext } from '@/app/VisualizerPageContext'
import AppSpinner from '@/components/base/AppSpinner'
import { useVisualizerContext } from '@/visualizer'
import { ElementProperties } from '@/visualizer/types'
import { Dictionary } from 'lodash'

function NodePropertiesList({ properties }: { properties: ElementProperties }) {
  const entries = Object.entries(properties)
  if (!entries.length) {
    return null
  }

  return (
    <ul className="grid gap-y-3 border-b border-grey-600 p-4">
      {entries.map(([key, value]) => (
        <li key={key}>
          <p className="mb-1 text-xs font-medium leading-[1.16] text-content-tertiary">
            {propertyKeyToString(key)}
          </p>
          <p
            className="text-sm font-medium"
            style={{
              wordBreak: key === 'schemaClass' ? 'break-all' : undefined,
            }}
          >
            <NodePropertyValue value={value} valueKey={key} />
          </p>
        </li>
      ))}
    </ul>
  )
}

function NodeEdgesGroup({
  edges,
  type,
  selectedNodeIds,
}: {
  edges: EdgeWithNodes[]
  type: string
  selectedNodeIds: string[]
}) {
  const vis = useVisualizerContext()
  if (!vis) {
    throw new Error('Visualizer context is required')
  }

  const isExpandable = edges.length > 8
  const preview = edges.slice(0, 8)
  const rest = edges.slice(8)
  const { getEdgeReferenceNode, getEdgeReferenceNodeId } = useVisualizerPageContext()
  const getDisplayName = (edge: any) => {
    const referenceNode = getEdgeReferenceNode(edge)
    if (!referenceNode) {
      throw new Error(`No reference node found for edge ${edge.data.turing_id}`)
    }
    return referenceNode?.data.properties.displayName
  }

  const isSelected = (edge: EdgeWithNodes) => {
    const referenceNode = getEdgeReferenceNode(edge)
    return referenceNode && selectedNodeIds.includes(referenceNode?.data.turing_id)
  }

  const handleToggleEdge = (edge: EdgeWithNodes) => {
    const nodeId = getEdgeReferenceNodeId(edge)
    const isSelected = selectedNodeIds.includes(nodeId)

    if (isSelected) {
      const selectedNodeIndex = selectedNodeIds.findIndex((v) => {
        return v === nodeId
      })
      const newSelectedNodeIds = [...selectedNodeIds]
      newSelectedNodeIds.splice(selectedNodeIndex, 1)
      vis.callbacks()?.setSelectedNodeIds(newSelectedNodeIds)
      return
    }

    vis.callbacks()?.setSelectedNodeIds([...selectedNodeIds, nodeId])
  }

  return (
    <li>
      <p className="mb-2 text-xs font-medium leading-[1.16] text-content-tertiary">{type}</p>
      <ul className="flex flex-wrap gap-2">
        {preview.map((edge) => (
          <li key={edge.data.turing_id}>
            <AppBadge
              as="button"
              active={isSelected(edge)}
              type="button"
              className="text-left"
              onClick={() => handleToggleEdge(edge)}
            >
              {getDisplayName(edge)}
            </AppBadge>
          </li>
        ))}
      </ul>
      {isExpandable && (
        <AppExpandable
          trigger={({ isExpanded, setIsExpanded }) => (
            <AppLinkButton onClick={() => setIsExpanded(!isExpanded)}>
              + {rest.length} more
            </AppLinkButton>
          )}
          hideTriggerOnExpand
        >
          <ul className="mt-2 flex flex-wrap gap-2">
            {rest.map((edge) => (
              <AppBadge
                as="button"
                key={edge.data.turing_id}
                active={isSelected(edge)}
                type="button"
                className="text-left"
                onClick={() => handleToggleEdge(edge)}
              >
                {getDisplayName(edge)}
              </AppBadge>
            ))}
          </ul>
        </AppExpandable>
      )}
    </li>
  )
}

function NodeEdgesList({
  title,
  groupedEdges,
  selectedNodeIds,
}: {
  title: string
  groupedEdges: Dictionary<EdgeWithNodes[]>
  selectedNodeIds: string[]
}) {
  return (
    <li>
      <p className="mb-2 text-xs font-bold uppercase leading-[1.6] tracking-[0.06em]">{title}</p>
      <ul className="grid gap-y-3">
        {Object.entries(groupedEdges).map(([edgeType, edges]) => (
          <NodeEdgesGroup
            key={edgeType}
            edges={edges}
            type={edgeType}
            selectedNodeIds={selectedNodeIds}
          />
        ))}
      </ul>
    </li>
  )
}

function TheNodeDescription({ value }: { value: string }) {
  const [isExpanded, setIsExpanded] = useState(false)
  const [isTruncated, setIsTruncated] = useState(false)
  const wrapperRef = useRef<HTMLParagraphElement>(null)

  useEffect(() => {
    if (!wrapperRef.current) return
    const wrapper = wrapperRef.current
    const isTruncated = wrapper.clientHeight <= wrapper.scrollHeight
    setIsTruncated(isTruncated)
    setIsExpanded(!isTruncated)
  }, [wrapperRef])

  return (
    <div className="description border-t border-grey-600 p-4">
      <p className="mb-1 text-xs font-medium  leading-[1.16] tracking-[0.06em] text-content-tertiary">
        description
      </p>
      <div
        className={clsx(
          'grid overflow-hidden px-0.5 transition-all duration-500',
          isExpanded ? 'grid-rows-[1fr]' : 'grid-rows-[0fr]'
        )}
      >
        <div
          className={clsx({
            'min-h-[7rem]': isTruncated,
          })}
        >
          <p ref={wrapperRef} className="text-sm font-medium leading-[1.6]">
            <NodePropertyValue value={value} valueKey="text" />
          </p>
        </div>
      </div>
      {isTruncated && !isExpanded && (
        <AppLinkButton className="mt-1" onClick={() => setIsExpanded(!isExpanded)}>
          Read more
        </AppLinkButton>
      )}
    </div>
  )
}

export default function TheNodeSidebarCollapsed({
  turingId,
  properties,
  loading,
  edges = [],
  selectedNodeIds,
}: {
  turingId?: string
  properties?: Record<string, string>
  loading?: boolean
  edges: EdgeWithNodes[]
  selectedNodeIds: string[]
}) {
  const vis = useVisualizerContext()
  if (!vis) {
    throw new Error('Visualizer context is required')
  }

  const inEdges = edges.filter((v) => v.data.turing_target_id === turingId)
  const inEdgesGroupedByEdgeType = _groupBy(inEdges, (v) => v.data.edge_type_name)
  const outEdges = edges.filter((v) => v.data.turing_source_id === turingId)
  const outEdgesGroupedByEdgeType = _groupBy(outEdges, (v) => v.data.edge_type_name)
  const description = properties?.text
  const propertiesWithoutDescription = {
    ...properties,
  }
  delete propertiesWithoutDescription.text

  return (
    <>
      {properties && <NodePropertiesList properties={propertiesWithoutDescription} />}
      {loading ? (
        <div className="flex flex-grow">
          <AppSpinner className="m-auto" />
        </div>
      ) : (
        <div className="edges p-4">
          <ul className="grid gap-y-4">
            {!!inEdges.length && (
              <NodeEdgesList
                title="In edges"
                groupedEdges={inEdgesGroupedByEdgeType}
                selectedNodeIds={selectedNodeIds}
              />
            )}
            {!!outEdges.length && (
              <NodeEdgesList
                title="Out edges"
                groupedEdges={outEdgesGroupedByEdgeType}
                selectedNodeIds={selectedNodeIds}
              />
            )}
          </ul>
        </div>
      )}
      {description && <TheNodeDescription value={description} />}
    </>
  )
}
