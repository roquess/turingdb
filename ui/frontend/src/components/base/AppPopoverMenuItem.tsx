import { MenuItem, MenuItemProps } from '@blueprintjs/core'
import IconCheck from '@/assets/icons/check.svg'
import clsx from 'clsx'

export interface AppPopoverMenuItemProps extends MenuItemProps {
  isActive?: boolean
}

export default function AppPopoverMenuItem(props: AppPopoverMenuItemProps) {
  const { active, icon, children, className, popoverProps, ...rest } = props
  const iconComputed = icon ?? (active ? <IconCheck /> : null)

  return (
    <MenuItem
      className={clsx('app-popover-menu-item', className)}
      icon={iconComputed}
      active={active}
      popoverProps={{
        ...popoverProps,
        popoverClassName: clsx('app-popover', popoverProps?.popoverClassName),
      }}
      {...rest}
    >
      {children}
    </MenuItem>
  )
}
