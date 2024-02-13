import AppButton from '@/components/base/AppButton'
import IconSearchSmall from '@/assets/icons/search-small.svg'
import AppButtonGroup from '@/components/base/AppButtonGroup'
import { useVisualizerContext } from '@/visualizer'
import AppTooltip from '@/components/base/AppTooltip'
import SelectNodesMenu from '@/visualizer/components/ActionsToolbar/SelectNodesMenu'
import { useMenuActions } from '@/visualizer/components/ContextMenu/hooks'
import HiddenNodesDialog from '@/visualizer/components/HiddenNodesDialog'
import React from 'react'
import ActionAddNode from './actions/ActionAddNode'
import ActionSettings from './actions/ActionSettings'
import ActionLabels from './actions/ActionLabels'

export default function TheVisualizerTopActions({
  highlightAddEntity,
}: {
  highlightAddEntity?: boolean
}) {
  const vis = useVisualizerContext()
  const actions = useMenuActions()

  return (
    <div className=" mx-4 mt-4 flex justify-end">
      <div className="pointer-events-auto flex flex-wrap items-center gap-2 ">
        <AppButtonGroup>
          <AppTooltip label="Clean up canvas" placement="bottom">
            <AppButton icon={'eraser'} onClick={() => vis.callbacks()?.requestLayoutRun(true)} />
          </AppTooltip>

          <HiddenNodesDialog
            trigger={({ open }) => (
              <AppTooltip label="Hidden nodes" placement="bottom">
                <AppButton className="w-8" icon="eye-open" onClick={open} />
              </AppTooltip>
            )}
          />

          <AppTooltip label="Expand all neighbors" placement="bottom">
            <AppButton icon={'expand-all'} onClick={() => actions.expandNeighbors()} />
          </AppTooltip>

          <AppTooltip label="Hides neighbors" placement="bottom">
            <AppButton icon={'collapse-all'} onClick={() => actions.collapseNeighbors()} />
          </AppTooltip>
        </AppButtonGroup>

        <ActionLabels />

        <ActionAddNode highlight={highlightAddEntity} />

        <ActionSettings />

        <AppButton
          text="Search view"
          icon={<IconSearchSmall />}
          onClick={vis.searchNodesDialog.toggle}
        />

        <AppTooltip
          interactionKind="click"
          content={
            <SelectNodesMenu
              className="app-popover-menu"
              selectClassName="app-select-popover"
              selectPopoverProps={{
                popoverClassName: 'app-popover app-select-popover',
              }}
              button={(props: any) => (
                <AppButton {...props} className="whitespace-nowrap" borderless />
              )}
            />
          }
          placement="bottom"
        >
          <AppButton icon={'label'} text="Select" />
        </AppTooltip>
      </div>
    </div>
  )
}
