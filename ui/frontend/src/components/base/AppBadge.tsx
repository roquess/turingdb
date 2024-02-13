import clsx from 'clsx'
import { ComponentProps, ElementType } from 'react'

export type AppBadgeProps<T extends keyof JSX.IntrinsicElements> = ComponentProps<T> & {
  children: React.ReactNode
  as?: T
  circle?: boolean
  alt?: boolean
  active?: boolean
  className?: string
}

export default function AppBadge<T extends keyof JSX.IntrinsicElements>({
  children,
  as: Tag = 'p' as T,
  circle,
  alt,
  active,
  className = '',
  ...rest
}: AppBadgeProps<T>) {
  const defaultClasses = 'shadow-badge rounded-[0.125rem] bg-grey-600 p-1'
  const circleClasses =
    'rounded-full px-1.5 py-0.5 rounded-[4.375rem] bg-white bg-opacity-[0.08] border border-white border-opacity-[0.12]'
  const altClasses =
    'bg-white px-1 py-0.5 bg-opacity-[0.04] border border-white border-opacity-[0.12] rounded-[0.125rem]'

  let classes = ''
  if (circle) {
    classes = circleClasses
  } else if (alt) {
    classes = altClasses
  } else {
    classes = defaultClasses
  }

  const Component = Tag as ElementType

  return (
    <Component
      className={clsx(
        'inline-flex text-xs font-medium leading-[1.16] tracking-[0.06rem] text-content-primary',
        {
          'border border-primary-default': active,
        },
        {
          'focus:outline-none': Tag === 'button',
        },
        classes,
        className
      )}
      {...rest}
    >
      {children}
    </Component>
  )
}
