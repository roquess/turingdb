import { Tab } from '@/types/PageTabs'
import clsx from 'clsx'
import IconClose from '@/assets/icons/close.svg'

export default function PageTabsItem({
  tab,
  closable,
  isActive,
  onSelect,
  onDelete,
}: {
  tab: Tab
  closable?: boolean
  isActive?: boolean
  onSelect?: () => void
  onDelete?: () => void
}) {
  return (
    <li
      className={clsx(
        'flex cursor-pointer items-center gap-x-2 border text-sm font-medium shadow-dark transition-colors',
        {
          'pr-3': !closable,
          'bg-grey-900': isActive,
        },
        isActive
          ? 'border-transparent text-content-primary'
          : 'border-grey-700 text-content-tertiary'
      )}
    >
      <button
        className="whitespace-nowrap py-2 pl-3 focus:outline-none"
        type="button"
        onClick={onSelect}
      >
        {tab.name}
      </button>
      {closable && (
        <button
          className="py-2 pr-2 text-content-tertiary transition-colors hover:text-content-primary focus:outline-none"
          type="button"
          onClick={onDelete}
        >
          <IconClose />
          <span className="sr-only">Close tab</span>
        </button>
      )}
    </li>
  )
}
