import AppIconButton from '@/components/base/AppIconButton'
import IconSearchSmall from '@/assets/icons/search-small.svg'
import AppBadge from '@/components/base/AppBadge'
import clsx from 'clsx'
import AppLinkButton from '@/components/base/AppLinkButton'
import AppInputSearch from '@/components/base/AppInputSearch'
import { useState } from 'react'

export default function AppNodeSidebarCard({
  title,
  children,
  searchable,
  filters,
  canSelectAll,
  selectAllLabel = 'Select All',
  onSelectAll,
  onSearch,
  onSearchClear,
  counter,
  className,
  headerClassName,
}: {
  title: string
  children: React.ReactNode
  items?: any[]
  searchable?: boolean
  filters?: React.ReactNode
  canSelectAll?: boolean
  selectAllLabel?: string
  onSelectAll?: () => void
  onSearch?: (v: string) => void
  onSearchClear?: () => void
  counter?: {
    max: number
    current: number
  }
  className?: string
  headerClassName?: string
}) {
  const [isSearchActive, setIsSearchActive] = useState(false)

  const searchIcon = searchable && (
    <AppIconButton label="Search" ghost onClick={() => setIsSearchActive(!isSearchActive)}>
      <IconSearchSmall />
    </AppIconButton>
  )

  const handleSearchClear = () => {
    setIsSearchActive(false)
    onSearchClear && onSearchClear()
  }

  return (
    <div className={clsx('rounded-[0.25rem] bg-grey-900 px-4 pb-4 pt-2', className)}>
      <div
        className={clsx(
          'flex h-8 items-center justify-between gap-x-2 text-xs font-medium',
          headerClassName
        )}
      >
        <div className="flex items-center gap-x-2">
          <p className="whitespace-nowrap font-bold uppercase leading-[1.66] tracking-[0.06em] text-content-primary">
            {title}
          </p>
          {counter && (
            <AppBadge circle>
              {counter.current}/{counter.max}
            </AppBadge>
          )}
        </div>
        <div className="flex gap-x-2">
          {isSearchActive ? (
            <AppInputSearch
              autoFocus
              className="max-w-[10.3125rem]"
              onValueChange={(v) => onSearch && onSearch(v)}
              onClear={handleSearchClear}
              placeholder="Search edges..."
              clearAlwaysVisible
            />
          ) : (
            <>
              {searchIcon}
              {filters}
              {canSelectAll && (
                <AppLinkButton onClick={onSelectAll}>{selectAllLabel}</AppLinkButton>
              )}
            </>
          )}
        </div>
      </div>
      <div className="pt-3">{children}</div>
    </div>
  )
}
