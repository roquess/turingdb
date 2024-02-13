import { Popover, PopoverInteractionKind, PopoverProps } from '@blueprintjs/core'
import React from 'react'

export interface AppTooltipProps extends PopoverProps {
  label?: string | React.ReactNode
}

export default function AppPopover(props: AppTooltipProps) {
  const { label, children, interactionKind = PopoverInteractionKind.HOVER, ...rest } = props
  return (
    <Popover
      interactionKind={interactionKind}
      popoverClassName="app-tooltip bp5-dark"
      hoverCloseDelay={50}
      placement="top-start"
      content={
        props.content || (
          <div className="px-2 py-[0.4375rem]">
            <span>{label}</span>
          </div>
        )
      }
      {...rest}
    >
      {children}
    </Popover>
  )
}
