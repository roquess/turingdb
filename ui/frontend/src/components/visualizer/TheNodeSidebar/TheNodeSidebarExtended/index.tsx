import { useState } from 'react'
import _groupBy from 'lodash.groupby'
import AppNodePropertiesList from './AppNodePropertiesList'
import AppNodeEdges from './AppNodeEdges'
import AppInputSearch from '@/components/base/AppInputSearch'
import AppSpinner from '@/components/base/AppSpinner'

export default function TheNodeSidebarExtended({
  turingId,
  properties,
  loading,
  edges = [],
  selectedNodeIds,
}: {
  turingId?: string
  properties?: Record<string, string>
  loading?: boolean
  edges: any[]
  selectedNodeIds: string[]
}) {
  const inEdges = edges.filter((v) => {
    return v.data.turing_target_id === turingId
  })
  const outEdges = edges.filter((v) => v.data.turing_source_id === turingId)
  const [search, setSearch] = useState('')

  return (
    <div className="grid gap-y-4">
      {loading ? (
        <AppSpinner className="px-4 py-12" />
      ) : (
        <div className="edges">
          <div className="p-4">
            <AppInputSearch
              value={search}
              onValueChange={(v) => setSearch(v)}
              onClear={() => setSearch('')}
              placeholder="Search in edges and out edges..."
            />
          </div>
          <ul className="grid gap-y-4 px-4">
            {inEdges.length > 0 && (
              <li>
                <AppNodeEdges
                  title="In edges"
                  edges={inEdges}
                  selectedNodeIds={selectedNodeIds}
                  search={search}
                />
              </li>
            )}
            {outEdges.length > 0 && (
              <li>
                <AppNodeEdges
                  title="Out edges"
                  edges={outEdges}
                  selectedNodeIds={selectedNodeIds}
                  search={search}
                />
              </li>
            )}
          </ul>
        </div>
      )}
      {properties && (
        <div className="px-4 pb-4">
          <AppNodePropertiesList properties={properties} />
        </div>
      )}
    </div>
  )
}
