import { Card, CardProps } from '@blueprintjs/core'
import clsx from 'clsx'
import AppBadge from './AppBadge'

interface AppCardProps extends CardProps {
  title?: string
  subtitle?: string | number
  clampSubtitle?: boolean
  icon?: React.ReactNode
  iconOnlyOnHover?: boolean
  badge?: React.ReactNode
  activeBadge?: boolean
}

export default function AppCard(props: AppCardProps) {
  const isHeaderVisible = props.title || props.subtitle
  return (
    <Card
      {...props}
      className={clsx(
        'app-card',
        {
          'icon-only-on-hover': props.iconOnlyOnHover,
        },
        props.className
      )}
    >
      <div className="app-card-content flex-grow">
        {isHeaderVisible && (
          <div className="app-card-header flex w-full items-center justify-between gap-x-4">
            <div className="app-card-header-text">
              {props.title && (
                <h5 className="app-card-title text-sm font-semibold leading-[1.43] text-content-primary">
                  {props.title}
                </h5>
              )}
              <div className="mt-1 flex items-center gap-2">
                {props.badge && <AppBadge active={props.activeBadge}>{props.badge}</AppBadge>}
                {props.subtitle && (
                  <p
                    className={clsx(
                      'app-card-subtitle text-xs leading-[1.33] text-content-secondary',
                      {
                        'line-clamp-1': props.clampSubtitle,
                      }
                    )}
                  >
                    {props.subtitle}
                  </p>
                )}
              </div>
            </div>
            {props.icon && <div className="app-card-header-icon flex-shrink-0">{props.icon}</div>}
          </div>
        )}
        {props.children}
      </div>
    </Card>
  )
}
