import AppPopoverMenu from '@/components/base/AppPopoverMenu'
import AppPopoverMenuItem from '@/components/base/AppPopoverMenuItem'
import AppButton from '@/components/base/AppButton'

import { useMenuActions } from '@/visualizer/components/ContextMenu/hooks'
import {
  ItemSelectAllBySameNodeTypeNoData,
  ItemSelectAllBySamePropertyNoData,
} from '@/visualizer/components/ContextMenu/items'

const SelectButton = (props: any) => (
  <AppButton {...props} className="whitespace-nowrap" borderless />
)

export default function NodeContextMenu() {
  const menuActions = useMenuActions()

  return (
    <AppPopoverMenu>
      <AppPopoverMenuItem text="Select all...">
        <ItemSelectAllBySameNodeTypeNoData
          actions={menuActions}
          className="app-select-popover"
          popoverProps={{
            popoverClassName: 'app-popover app-select-popover',
          }}
          button={SelectButton}
        />
        <ItemSelectAllBySamePropertyNoData
          actions={menuActions}
          className="app-select-popover"
          popoverProps={{
            popoverClassName: 'app-popover app-select-popover',
          }}
          button={SelectButton}
        />
      </AppPopoverMenuItem>
    </AppPopoverMenu>
  )
}
