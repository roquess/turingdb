import AppDialog, { AppDialogProps } from '@/components/base/AppDialog'
import { useVisualizerPageContext } from '@/app/VisualizerPageContext'
import AppInputSearch from '@/components/base/AppInputSearch'
import React, { ReactNode, useState } from 'react'
import AppCardList from '@/components/base/AppCardList'
import AppCard from '@/components/base/AppCard'
import AppIconButton from '@/components/base/AppIconButton'
import IconOpen from '@/assets/icons/open.svg'
import { useQuery } from '@tanstack/react-query'
import { endpoints } from '@/visualizer/queries'
import { GraphNode } from '@/visualizer/types'
import AppSpinner from '@/components/base/AppSpinner'
import { useVisualizerTabsContext } from '@/app/VisualizerTabsContext'

export interface ThePathwaysDialogProps extends AppDialogProps {}

const NoResultsInfo = ({ children }: { children: ReactNode }) => (
  <p className="py-4 font-medium text-content-secondary">{children}</p>
)

export default function ThePathwaysDialog() {
  const { setPathwaysDialogNodeData, pathwaysDialogNodeData } = useVisualizerPageContext()
  const { addTab } = useVisualizerTabsContext()

  const turingId = pathwaysDialogNodeData?.turing_id

  const [search, setSearch] = useState('')

  const { data: pathways = [], isFetching: arePathwaysLoading } = useQuery({
    queryKey: ['list_pathways', turingId],
    enabled: !!turingId,
    queryFn: async () => {
      if (!turingId) {
        throw new Error('No turingId')
      }
      const data = await endpoints['list_pathways']({
        nodeId: turingId, // List the edges of the node
      })

      return data
    },
  })

  const getPathwayTitle = (pathway: GraphNode) => {
    const displayName = pathway.data.properties.displayName
    return displayName
  }

  const getPathwaySubtitle = (pathway: GraphNode) => {
    const turingId = pathway.data.turing_id
    return turingId
  }

  const [newPathwayLoading, setNewPathwayLoading] = useState(false)
  const [newPathwayError, setNewPathwayError] = useState<string | null>(null)

  const handlePathwayClick = async (node: GraphNode) => {
    const turingId = node.data.turing_id

    setNewPathwayLoading(true)
    setNewPathwayError(null)

    const defaultError = 'Cannot get pathway, please try again'
    try {
      const response = await endpoints['get_pathway']({
        nodeId: turingId, // List the edges of the node
        // filters: {
        //   showOnlyHomoSapiens: true,
        //   hidePublications: true,
        //   hideCompartments: true,
        //   hideSpecies: true,
        //   hideDatabaseReferences: true,
        // },
        // additionalPropFilterIn: [],
        // additionalPropFilterOut: [],
      })

      if (response.failed) {
        setNewPathwayError(response.error || defaultError)
        return
      }

      await addTab(turingId, {
        title: `Pathway - ${getPathwayTitle(node)}`,
        pathway: response.nodes,
      })
      setPathwaysDialogNodeData(null)
    } catch {
      setNewPathwayError(defaultError)
    } finally {
      setNewPathwayLoading(false)
    }
  }

  const filteredPathways = pathways.filter(
    (p) => !search || getPathwayTitle(p)?.toLowerCase().includes(search.toLowerCase())
  )

  const loading = arePathwaysLoading || newPathwayLoading

  const pathwayItemIconButton = (
    <AppIconButton
      className="flex-shrink-0"
      label="Open in new tab"
      ghost
      tooltip="Open in new tab"
      tooltipProps={{
        placement: 'top',
      }}
    >
      <IconOpen />
    </AppIconButton>
  )

  const AppCardListContent = () => {
    if (!pathways.length) {
      return <NoResultsInfo>No pathways found.</NoResultsInfo>
    }

    if (!filteredPathways.length) {
      return <NoResultsInfo>No results found. Try different phrase.</NoResultsInfo>
    }

    return filteredPathways.map((p) => (
      <AppCard
        key={p.data.turing_id}
        title={getPathwayTitle(p)}
        subtitle={getPathwaySubtitle(p)}
        onClick={() => handlePathwayClick(p)}
        interactive
        clampSubtitle={true}
        icon={pathwayItemIconButton}
        iconOnlyOnHover
      />
    ))
  }

  return (
    <AppDialog
      isOpen={pathwaysDialogNodeData !== null}
      title="Show in Pathways"
      bodyClassName="overflow-hidden flex flex-col px-0 h-[60vh]"
      onClose={() => setPathwaysDialogNodeData(null)}
    >
      <div className="px-5">
        <AppInputSearch placeholder="Show in Pathways" onValueChange={(v) => setSearch(v)} />
      </div>
      {loading ? (
        <div className="flex flex-grow px-5 py-8 text-center">
          <AppSpinner className="m-auto" />
        </div>
      ) : (
        <AppCardList className="app-dialog-card-list">
          <p>{newPathwayError}</p>
          <AppCardListContent />
        </AppCardList>
      )}
    </AppDialog>
  )
}
