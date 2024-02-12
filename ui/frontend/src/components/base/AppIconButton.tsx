import { ComponentProps } from 'react'
import clsx from 'clsx'
import AppTooltip from '@/components/base/AppTooltip'
import { AppTooltipProps } from '@/components/base/AppTooltip'

export interface AppIconButtonProps extends ComponentProps<'button'> {
  label: string
  ghost?: boolean
  tooltip?: string
  tooltipProps?: Omit<AppTooltipProps, 'label'>
}

export default function AppIconButton(props: AppIconButtonProps) {
  const { type, label, children, className, ghost, tooltip, tooltipProps = {} } = props
  0
  const ghostClasses = 'bg-transparent'

  const defaultClasses = 'bg-grey-800 icon-button-border icon-button-bg-gradient icon-button-shadow'

  let classes = ''
  if (ghost) {
    classes = ghostClasses
  } else {
    classes = defaultClasses
  }

  const wrapWithTooltip = (element: React.ReactElement) => {
    if (!tooltip) return element
    return (
      <AppTooltip label={tooltip} {...tooltipProps}>
        {element}
      </AppTooltip>
    )
  }

  return wrapWithTooltip(
    <button
      className={clsx(
        'app-icon-button',
        'relative flex h-8 w-8 items-center justify-center rounded-[0.125rem]',
        'text-content-secondary transition-colors',
        'focus:icon-button-focus-shadow  hover:bg-grey-700 focus:outline-none active:bg-grey-600',
        classes,
        className
      )}
      type={type}
      onClick={props.onClick}
    >
      <span className="sr-only">{label}</span>
      {children}
    </button>
  )
}
