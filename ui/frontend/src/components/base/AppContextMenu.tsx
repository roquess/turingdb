import { ContextMenu, ContextMenuProps } from '@blueprintjs/core'
import clsx from 'clsx'

interface Props extends ContextMenuProps {}

export default function AppContextMenu(props: Props) {
  return (
    <ContextMenu
      {...props}
      popoverProps={{
        ...props.popoverProps,
        popoverClassName: clsx('app-popover', props.popoverProps?.popoverClassName),
      }}
      className={clsx('app-context-menu', props.className)}
    >
      {props.children}
    </ContextMenu>
  )
}
