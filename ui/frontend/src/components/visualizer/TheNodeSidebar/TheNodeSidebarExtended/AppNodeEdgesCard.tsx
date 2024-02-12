import AppNodeSidebarCard from './AppNodeSidebarCard'
import AppNodeEdgesGroup from './AppNodeEdgesGroup'

export default function AppNodeEdgesCard({
  title,
  groupedEdges,
  onEdgeClick = () => undefined,
  selectedEdges = [],
  counter,
  onSelectAll,
  onSearch,
  onSearchClear,
  selectAllLabel = 'Select all',
  filters,
}: {
  title: string
  groupedEdges: Record<string, any[]>
  onEdgeClick?: (edge: any) => void
  selectedEdges?: any[]
  counter?: {
    max: number
    current: number
  }
  onSelectAll: () => void
  onSearch?: (v: string) => void
  onSearchClear?: () => void
  selectAllLabel?: string
  filters?: React.ReactNode
}) {
  const empty = !Object.keys(groupedEdges).length

  return (
    <AppNodeSidebarCard
      title={title}
      searchable
      filters={filters}
      canSelectAll
      counter={counter}
      onSelectAll={onSelectAll}
      selectAllLabel={selectAllLabel}
      onSearch={onSearch}
      onSearchClear={onSearchClear}
    >
      <div className="grid gap-y-4">
        {empty ? (
          <p className="py-2 text-center font-medium leading-[1.5] tracking-[0.01em] text-content-primary">
            No results found. Try different words or remove filters.
          </p>
        ) : (
          Object.entries(groupedEdges).map(([edgeType, edges]) => (
            <AppNodeEdgesGroup
              key={edgeType}
              edges={edges}
              type={edgeType}
              onEdgeClick={onEdgeClick}
              selectedEdges={selectedEdges}
            />
          ))
        )}
      </div>
    </AppNodeSidebarCard>
  )
}
