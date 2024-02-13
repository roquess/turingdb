import { Menu, MenuProps } from '@blueprintjs/core'
import clsx from 'clsx'

interface Props extends MenuProps {}

export default function AppPopoverMenu(props: Props) {
  return (
    <Menu {...props} className={clsx('app-popover-menu', props.className)}>
      {props.children}
    </Menu>
  )
}
