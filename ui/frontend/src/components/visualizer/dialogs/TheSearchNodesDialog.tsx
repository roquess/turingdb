import AppDialog from '@/components/base/AppDialog'
import AppInputSearch from '@/components/base/AppInputSearch'
import { Tag } from '@blueprintjs/core'
import { SelectProperty } from '@/visualizer/components/SearchNodesDialog/QueryNodesBar'
import AppCard from '@/components/base/AppCard'
import AppCardList from '@/components/base/AppCardList'
import { SearchNodesDialog } from '@/visualizer/components/SearchNodesDialog'
import { useVisualizerContext } from '@/visualizer'

export default function TheSearchNodesDialog() {
  const vis = useVisualizerContext()
  if (!vis) {
    throw new Error('No vis context')
  }

  const handleNodeClick = (n: cytoscape.SingularElementArgument) => {
    vis.searchNodesDialog.close()
    vis.cy()!.elements().unselect()
    vis.cy()!.animate(
      {
        center: {
          eles: n,
        },
      },
      {
        duration: 600,
        easing: 'ease-in-out-sine',
      }
    )
    n.select()
  }

  return (
    <SearchNodesDialog
      dialog={({
        isOpen,
        onClose,
        title,
        setPropValue,
        propValue,
        nodeCount,
        propNames,
        propName,
        setPropName,
        nodes,
      }) => {
        return (
          <AppDialog
            bodyClassName="overflow-hidden flex flex-col px-0 h-[60vh]"
            isOpen={isOpen}
            onClose={onClose}
            title={title}
          >
            <div className="flex items-center gap-x-2 px-5">
              <div className="flex items-center gap-x-2">
                <p className="text-sm text-content-secondary">Find by property</p>
                <SelectProperty
                  items={propNames}
                  onItemSelect={setPropName}
                  value={propName}
                  popoverProps={{
                    popoverClassName: 'app-popover app-select-popover',
                  }}
                  className="app-select"
                />
              </div>
              <AppInputSearch
                className="flex-grow"
                placeholder="Find nodes"
                onValueChange={setPropValue}
                value={propValue}
                rightElement={<Tag minimal>{nodeCount}</Tag>}
              />
            </div>
            <AppCardList className="app-dialog-card-list">
              {nodes?.length ? (
                nodes.map((n) => (
                  <AppCard
                    title={
                      propName === 'Node Type'
                        ? n.data().node_type_name
                        : n.data().properties[propName] || 'Unnamed node'
                    }
                    key={'node-' + n.id()}
                    interactive
                    badge={`id [${n.data().turing_id}]`}
                    activeBadge={n.selected()}
                    onClick={() => handleNodeClick(n)}
                  />
                ))
              ) : (
                <p>No results</p>
              )}
            </AppCardList>
          </AppDialog>
        )
      }}
    />
  )
}
