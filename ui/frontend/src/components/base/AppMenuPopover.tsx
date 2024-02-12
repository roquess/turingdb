import { Popover, PopoverProps } from '@blueprintjs/core'
import clsx from 'clsx'
import AppPopoverMenu from './AppPopoverMenu'

interface Props extends PopoverProps {
  trigger: React.ReactNode
  children: React.ReactNode
}

export default function AppMenuPopover(props: Props) {
  const { trigger, children, popoverClassName, ...rest } = props
  return (
    <Popover
      popoverClassName={clsx('app-popover', popoverClassName)}
      hasBackdrop={false}
      minimal
      content={<AppPopoverMenu>{children}</AppPopoverMenu>}
      modifiers={{
        offset: {
          enabled: true,
          options: {
            offset: [0, 5],
          },
        },
      }}
      {...rest}
    >
      {trigger}
    </Popover>
  )
}
