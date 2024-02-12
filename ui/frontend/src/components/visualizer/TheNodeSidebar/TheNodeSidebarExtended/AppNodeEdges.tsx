import { useCallback, useState } from 'react'
import _groupBy from 'lodash.groupby'
import AppMenuPopover from '@/components/base/AppMenuPopover'
import AppPopoverMenuItem from '@/components/base/AppPopoverMenuItem'
import AppIconButton from '@/components/base/AppIconButton'
import IconFilterSmall from '@/assets/icons/filter-small.svg'
import { useVisualizerContext } from '@/visualizer'
import AppNodeEdgesCard from './AppNodeEdgesCard'
import { EdgeWithNodes, useVisualizerPageContext } from '@/app/VisualizerPageContext'

type EdgeFilter = 'all' | 'active' | 'unselected'

function EdgesFilters({
  value,
  onChange,
}: {
  value: EdgeFilter
  onChange: (value: EdgeFilter) => void
}) {
  const filters: {
    name: string
    value: EdgeFilter
  }[] = [
    {
      name: 'All nodes',
      value: 'all',
    },
    {
      name: 'Active nodes',
      value: 'active',
    },
    {
      name: 'Unselected',
      value: 'unselected',
    },
  ]

  return (
    <div>
      <AppMenuPopover
        popoverClassName="w-[15rem]"
        usePortal={false}
        trigger={
          <AppIconButton label="Filter" ghost>
            <IconFilterSmall />
          </AppIconButton>
        }
        modifiers={{
          offset: {
            enabled: true,
            options: {
              offset: [0, 5],
            },
          },
        }}
        placement="bottom-end"
      >
        {filters.map((filter) => (
          <AppPopoverMenuItem
            key={filter.value}
            text={filter.name}
            active={value === filter.value}
            onClick={() => onChange(filter.value)}
          />
        ))}
      </AppMenuPopover>
    </div>
  )
}

export default function AppNodeEdges({
  title,
  edges,
  search = '',
  selectedNodeIds,
}: {
  title: string
  edges: EdgeWithNodes[]
  search: string
  selectedNodeIds: string[]
}) {
  const vis = useVisualizerContext()

  const [internalSearch, setInternalSearch] = useState('')

  const [selectedFilter, setSelectedFilter] = useState<EdgeFilter>('all')

  const { getEdgeReferenceNode, getEdgeReferenceNodeId } = useVisualizerPageContext()

  const isEdgeSearchMatch = useCallback(
    (search: string, edge: EdgeWithNodes) => {
      if (!search.length) {
        return true
      }
      const node = getEdgeReferenceNode(edge)
      if (!node) {
        return false
      }
      const nodeId = node?.data.turing_id
      const displayName = node.data?.properties?.displayName

      const tests = [nodeId, displayName].filter(Boolean)
      return tests.some((value: string) =>
        String(value).toLowerCase().startsWith(search.toLowerCase())
      )
    },
    [getEdgeReferenceNode]
  )

  const isSearchMatch = useCallback(
    (edge: EdgeWithNodes) => {
      return isEdgeSearchMatch(search, edge) && isEdgeSearchMatch(internalSearch, edge)
    },
    [internalSearch, isEdgeSearchMatch, search]
  )

  const getFilteredEdges = useCallback(() => {
    switch (selectedFilter) {
      case 'active':
        return edges.filter(
          (v) => selectedNodeIds.includes(getEdgeReferenceNodeId(v)) && isSearchMatch(v)
        )
      case 'unselected':
        return edges.filter(
          (v) => !selectedNodeIds.includes(getEdgeReferenceNodeId(v)) && isSearchMatch(v)
        )
      default:
        if (search || internalSearch) {
          return edges.filter((v) => isSearchMatch(v))
        }
        return edges
    }
  }, [
    selectedFilter,
    edges,
    search,
    internalSearch,
    selectedNodeIds,
    getEdgeReferenceNodeId,
    isSearchMatch,
  ])

  const filteredEdges = getFilteredEdges()

  const selectedEdges = filteredEdges.filter((v) =>
    selectedNodeIds.includes(getEdgeReferenceNodeId(v))
  )

  const allEdgesSelected =
    !!filteredEdges.length &&
    filteredEdges.every((edge) => selectedNodeIds.includes(getEdgeReferenceNodeId(edge)))

  const edgesNodeIds = filteredEdges.map((edge) => getEdgeReferenceNodeId(edge))
  const groupedByType = _groupBy(filteredEdges, (v) => v.data.edge_type_name)

  const handleToggleEdges = () => {
    if (!filteredEdges.length) return

    if (allEdgesSelected) {
      const newSelectedNodes = selectedNodeIds.filter((nodeId) => {
        return !edgesNodeIds.includes(nodeId)
      })
      vis.callbacks()?.setSelectedNodeIds(newSelectedNodes)
      return
    }

    const newSelectedEdges = [...filteredEdges]
    const nodesToSelect = newSelectedEdges
      .map((edge: any) => {
        const nodeId = getEdgeReferenceNodeId(edge)
        if (selectedNodeIds.includes(nodeId)) {
          return
        }
        return nodeId
      })
      .filter((v) => !!v) as string[]
    vis.callbacks()?.setSelectedNodeIds([...selectedNodeIds, ...nodesToSelect])
  }

  const handleToggleEdge = (edge: EdgeWithNodes) => {
    const elIndex = selectedEdges.findIndex((v: any) => {
      return v.data.turing_id === edge.data.turing_id
    })

    const nodeId = getEdgeReferenceNodeId(edge)

    if (elIndex !== -1) {
      const selectedNodeIndex = selectedNodeIds.findIndex((v) => {
        return v === nodeId
      })
      const newSelectedNodes = [...selectedNodeIds]
      newSelectedNodes.splice(selectedNodeIndex, 1)
      vis.callbacks()?.setSelectedNodeIds(newSelectedNodes)
      return
    }

    vis.callbacks()?.setSelectedNodeIds([...selectedNodeIds, nodeId])
  }

  return (
    <AppNodeEdgesCard
      title={title}
      selectedEdges={selectedEdges}
      groupedEdges={groupedByType}
      onEdgeClick={(edge: any) => handleToggleEdge(edge)}
      counter={{
        current: selectedEdges.length,
        max: edges.length,
      }}
      onSelectAll={() => handleToggleEdges()}
      selectAllLabel={allEdgesSelected ? 'Deselect all' : 'Select all'}
      filters={
        <EdgesFilters value={selectedFilter} onChange={(value) => setSelectedFilter(value)} />
      }
      onSearch={(v) => setInternalSearch(v)}
      onSearchClear={() => setInternalSearch('')}
    />
  )
}
