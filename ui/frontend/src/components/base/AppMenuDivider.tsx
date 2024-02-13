import { MenuDivider, MenuDividerProps } from '@blueprintjs/core'
import clsx from 'clsx'

interface Props extends MenuDividerProps {}

export default function AppButtonGroup(props: Props) {
  return <MenuDivider {...props} className={clsx('app-menu-divider', props.className)} />
}
