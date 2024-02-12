import clsx from 'clsx'
import { endpoints } from '@/visualizer/queries'
import { useQuery } from '@tanstack/react-query'
import { useState, useEffect } from 'react'
import TheNodeSidebarCollapsed from './TheNodeSidebarCollapsed'
import TheNodeSidebarExtended from './TheNodeSidebarExtended'
import IconChevronRight from '@/assets/icons/chevron-right.svg'
import AppIconButton from '@/components/base/AppIconButton'
import IconClose from '@/assets/icons/close.svg'
import Image from 'next/image'
import { GraphNodeData, GraphEdge, GraphNode } from '@/visualizer/types'
import { useCanvasTrigger } from '@/visualizer/useCanvasTrigger'
import { useVisualizerContext } from '@/visualizer'
import { EdgeWithNodes } from '@/app/VisualizerPageContext'

export default function TheNodeSidebar({
  visible = true,
  node,
}: {
  visible?: boolean
  node?: GraphNodeData | null
}) {
  const vis = useVisualizerContext()
  if (!vis) {
    throw new Error('Visualizer context is required')
  }

  const properties = node?.properties
  const turingId = node?.turing_id
  const nodeTypeName = node?.node_type_name

  async function getEdgesNodes(edgesData: GraphEdge[]) {
    const nodesSet = new Set(
      edgesData.flatMap((edge) => [edge.data.turing_target_id, edge.data.turing_source_id])
    )
    const nodes = Array.from(nodesSet)
    const data = await endpoints['get_nodes']({
      nodeIds: nodes,
      yield_edges: false,
    })

    const nodesMap: Record<string, GraphNode> = {}
    data.forEach((node) => {
      nodesMap[node.data.turing_id] = node
    })
    return nodesMap
  }

  const { data: edges = [], isFetching: areEdgesLoading } = useQuery({
    queryKey: ['list_edges', turingId],
    enabled: !!turingId,
    queryFn: async () => {
      if (!turingId) {
        throw new Error('No turingId')
      }

      const data = await endpoints['list_edges']({
        nodeId: turingId, // List the edges of the node

        nodeFilters: {
          // Filter edges according to the other node
          // showOnlyHomoSapiens: true,
          // hidePublications: true,
          // hideCompartments: true,
          // hideSpecies: true,
          // hideDatabaseReferences: true,
        },

        edgeTypeNames: [
          // 'input'
        ], // Show only edges of type "input"

        additionalNodePropFilterIn: [
          // Filter edges according to the other node
          // ['prop1', 'prop1value'],
          // ['prop2', 'prop2value'],
          // ['prop3', 'prop3value'],
        ],

        additionalNodePropFilterOut: [
          // ['prop1', 'prop1value'],
          // ['prop2', 'prop2value'],
          // ['prop3', 'prop3value'],
        ],

        edgePropFilterOut: [
          // ['prop1', 'prop1value'],
          // ['prop2', 'prop2value'],
          // ['prop3', 'prop3value'],
        ],

        edgePropFilterIn: [],
      })

      const nodesMap = await getEdgesNodes(data)

      const dataWithNodes: EdgeWithNodes[] = data.map((edge) => {
        return {
          ...edge,
          source: nodesMap[edge.data.turing_source_id],
          target: nodesMap[edge.data.turing_target_id],
        }
      })

      return dataWithNodes
    },
  })

  const [isExtended, setIsExtended] = useState(false)

  useEffect(() => {
    setIsExtended(false)
  }, [turingId])

  useEffect(() => {
    if (!visible) {
      setIsExtended(false)
    }
  }, [visible])

  const [selectedNodeIds, setSelectedNodeIds] = useState<string[]>(
    vis.state()?.selectedNodeIds ?? []
  )

  useCanvasTrigger({
    category: 'selectedNodeIds',
    name: 'TheNodeSidebar - set selectedNodeIds',
    callback() {
      setSelectedNodeIds(vis.state()?.selectedNodeIds ?? [])
    },
  })

  return (
    <div
      className={clsx(
        'pointer-events-auto absolute bottom-0 left-[0.0625rem] top-[0.0625rem] z-[100] flex transform-gpu flex-col overflow-hidden bg-grey-800 shadow-dark transition-all duration-300',
        [visible ? 'translate-x-[0]' : 'translate-x-[calc(-100%-2px)]'],
        [isExtended ? 'w-[28.125rem]' : 'w-[15.625rem]']
      )}
    >
      <div
        className={clsx(
          'flex flex-shrink-0 items-center justify-between gap-x-4 border-b border-r border-grey-600 px-4 py-3 transition-[height] duration-200 ease-linear',
          [isExtended ? 'h-[3.75rem]' : 'h-[3rem]']
        )}
      >
        <div className="overflow-hidden">
          <div className="flex items-center gap-x-2.5">
            <Image
              className="flex-shrink-0"
              src="/img/svg/node-blue-small.svg"
              alt=""
              width="25"
              height="24"
            />
            <div className="flex min-h-[1.5rem] items-center overflow-hidden">
              <div className="overflow-hidden">
                <h2
                  className="line-clamp-1 text-sm font-medium leading-[1.42] text-content-primary"
                  title={properties?.displayName}
                >
                  {properties?.displayName}
                </h2>
                {isExtended && (
                  <p className="mt-0.5 line-clamp-1 block text-ellipsis whitespace-nowrap text-xs font-medium leading-[1.16] text-content-secondary">
                    ID: {turingId}, Type: {nodeTypeName}
                  </p>
                )}
              </div>
            </div>
          </div>
        </div>
        {isExtended ? (
          <AppIconButton
            className="flex-shrink-0"
            label="Close"
            onClick={() => setIsExtended((v) => !v)}
          >
            <IconClose />
          </AppIconButton>
        ) : (
          <button
            className="flex flex-shrink-0 items-center gap-1 whitespace-nowrap text-xs tracking-[0.06em] text-primary-default"
            type="button"
            onClick={() => setIsExtended((v) => !v)}
          >
            <span>Details</span>
            <IconChevronRight />
          </button>
        )}
      </div>
      <div
        className={clsx(
          'custom-scrollbar flex flex-grow flex-col overflow-y-auto overflow-x-hidden border-r border-grey-600 transition-opacity duration-300',
          [visible ? 'opacity-100' : 'opacity-0']
        )}
      >
        {isExtended ? (
          <TheNodeSidebarExtended
            turingId={turingId}
            edges={edges}
            selectedNodeIds={selectedNodeIds}
            loading={areEdgesLoading}
            properties={properties}
          />
        ) : (
          <TheNodeSidebarCollapsed
            turingId={turingId}
            edges={edges}
            selectedNodeIds={selectedNodeIds}
            loading={areEdgesLoading}
            properties={properties}
          />
        )}
      </div>
    </div>
  )
}
