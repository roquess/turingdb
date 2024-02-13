import AppButton from '@/components/base/AppButton'

import AddNodeDialog from '@/visualizer/components/AddNodeDialog'

import AppDialog from '@/components/base/AppDialog'
import AppInputSearch from '@/components/base/AppInputSearch'
import AppSpinner from '@/components/base/AppSpinner'
import AppCardList from '@/components/base/AppCardList'
import AppCard from '@/components/base/AppCard'
import AppIconButton from '@/components/base/AppIconButton'
import { Icon } from '@blueprintjs/core'
import { SelectProperty } from '@/visualizer/components/AddNodeDialog/SelectProperty'

export default function TheVisualizerAddNodeAction(props: { highlight?: boolean }) {
  const { highlight } = props

  return (
    <AddNodeDialog
      trigger={({ open }) => (
        <AppButton highlight={highlight} icon="add" text="Add entity" onClick={open} />
      )}
      dialog={({
        isOpen,
        onClose,
        onSearch,
        loading,
        onSubmit,
        nodes,
        getNodePropValue,
        getNodeSecondaryPropValue,
        isNodeSelected,
        addNode,
        removeNode,
        setInspectedNode,
        currentPropName,
        properties,
      }) => (
        <AppDialog
          isOpen={isOpen}
          title="Add nodes from the database"
          bodyClassName="overflow-hidden flex flex-col px-0 h-[60vh]"
          onClose={onClose}
        >
          <form
            className="flex items-center gap-x-2 px-5"
            onSubmit={(e) => {
              e.preventDefault()
              onSubmit()
            }}
          >
            <div className="flex items-center gap-x-2">
              <p className="text-sm text-content-secondary">Find by property</p>
              <SelectProperty
                currentProp={currentPropName.value}
                setCurrentProp={currentPropName.set}
                properties={properties}
                popoverProps={{
                  popoverClassName: 'app-popover app-select-popover',
                }}
                className="app-select"
              />
            </div>
            <AppInputSearch
              className="flex-grow"
              placeholder="Search nodes"
              onValueChange={onSearch}
              onIconClick={onSubmit}
            />
          </form>
          {loading ? (
            <p className="flex flex-grow px-5 py-8 text-center">
              <AppSpinner className="m-auto" />
            </p>
          ) : (
            <AppCardList className="app-dialog-card-list">
              {nodes.map((node) => (
                <AppCard
                  interactive
                  clampSubtitle={true}
                  key={node.turing_id}
                  title={getNodePropValue(node)}
                  subtitle={getNodeSecondaryPropValue(node)}
                  badge={node.turing_id}
                  activeBadge={isNodeSelected(node)}
                  icon={
                    <AppIconButton
                      className="flex-shrink-0"
                      label={`${isNodeSelected(node) ? 'Remove' : 'Add'} node`}
                      ghost
                      tooltip={`${isNodeSelected(node) ? 'Remove' : 'Add'} node`}
                      tooltipProps={{
                        placement: 'top',
                      }}
                    >
                      <Icon icon={isNodeSelected(node) ? 'remove' : 'add'} />
                    </AppIconButton>
                  }
                  onClick={() => {
                    setInspectedNode(node)
                    isNodeSelected(node) ? removeNode(node) : addNode(node)
                  }}
                />
              ))}
            </AppCardList>
          )}
        </AppDialog>
      )}
    />
  )
}
