import PageTabsItem from '@/components/tabs/PageTabsItem'
import IconPlus from '@/assets/icons/plus.svg'
import { Tab } from '@/types/PageTabs'

export default function PageTabs<
  T extends {
    tabs: Tab[]
    activeTabIndex: number
    deleteTab: (index: number) => void
    addTab: () => void
    setActiveTabIndex: (index: number) => void
  },
>(props: T) {
  const { tabs, deleteTab, addTab, setActiveTabIndex, activeTabIndex } = props

  return (
    <div className="flex items-center gap-x-[0.0625rem] overflow-auto bg-grey-800 ">
      <ul className="custom-scrollbar flex gap-x-[0.0625rem] overflow-auto p-[0.0625rem] pb-0">
        {tabs.map((tab, index) => {
          return (
            <PageTabsItem
              key={index}
              tab={tab}
              closable={tabs.length > 1}
              isActive={index === activeTabIndex}
              onSelect={() => setActiveTabIndex(index)}
              onDelete={() => deleteTab(index)}
            />
          )
        })}
      </ul>
      <button
        type="button"
        className="flex h-9 w-10 flex-shrink-0 items-center justify-center text-content-secondary transition-colors hover:text-content-primary focus:outline-none"
        onClick={() => addTab()}
      >
        <IconPlus />
        <span className="sr-only">Add tab</span>
      </button>
    </div>
  )
}
