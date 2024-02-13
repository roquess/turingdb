import { Dispatch, SetStateAction, createContext, useCallback } from 'react'
import { useState, Context } from 'react'
import useSafeContext from '@/hooks/useSafeContext'
import { Tab, TabData } from '@/types/PageTabs'

export interface TabsContextType<GTab extends Tab> {
  tabs: GTab[]
  addTab: (name?: string, data?: GTab['data']) => void
  deleteTab: (index: number) => void
  setTabs: Dispatch<SetStateAction<GTab[]>>
  activeTabIndex: number
  activeTab: GTab | undefined
  setActiveTabIndex: (index: number) => void
  getTab: (name: string, data: GTab['data']) => GTab
  updateTab: (index: number, tab: Partial<GTab>) => void
  clearTab: (index: number) => void
  updateActiveTab: (tab: Partial<GTab>) => void
  clearActiveTab: () => void
}

export const TabsContext = createContext<TabsContextType<Tab> | undefined>(undefined)

function getTab(name: string, data?: TabData): Tab {
  return {
    id: crypto.randomUUID(),
    name,
    data: data ?? {},
  }
}

export const TabsContextProvider = ({ children }: { children: React.ReactNode }) => {
  const [tabs, setTabs] = useState<Tab[]>([])
  const [activeTabIndex, setActiveTabIndex] = useState(0)
  const activeTab = tabs[activeTabIndex]

  const addTab = useCallback((name?: string, data?: TabData) => {
    setTabs((prevTabs) => {
      const computedName = name ?? `New tab`
      const newTabs = [...prevTabs, getTab(computedName, data)]
      setActiveTabIndex(newTabs.length - 1)
      return newTabs
    })
  }, [])

  const updateTab = useCallback((index: number, tab: Partial<Tab>) => {
    setTabs((prevTabs) => {
      const newTabs = [...prevTabs]
      const oldTab = newTabs[index]
      newTabs.splice(index, 1, {
        ...oldTab,
        ...tab,
      })
      return newTabs
    })
  }, [])

  const updateActiveTab = useCallback(
    (tab: Partial<Tab>) => updateTab(activeTabIndex, tab),
    [activeTabIndex, updateTab]
  )

  const clearTab = useCallback(
    (index: number) => updateTab(index, { name: 'New tab', data: {} }),
    [updateTab]
  )

  const clearActiveTab = useCallback(() => clearTab(activeTabIndex), [activeTabIndex, clearTab])

  const deleteTab = useCallback(
    (index: number) => {
      setTabs((prevTabs) => {
        if (index <= activeTabIndex) {
          setActiveTabIndex(Math.max(0, activeTabIndex - 1))
        }

        const newTabs = [...prevTabs]
        newTabs.splice(index, 1)
        return newTabs
      })
    },
    [activeTabIndex]
  )

  return (
    <TabsContext.Provider
      value={{
        tabs,
        activeTabIndex,
        setActiveTabIndex,
        addTab,
        deleteTab,
        setTabs,
        getTab,
        activeTab,
        updateTab,
        clearTab,
        updateActiveTab,
        clearActiveTab,
      }}
    >
      {children}
    </TabsContext.Provider>
  )
}

export function useTabsContext<T = TabsContextType<Tab>>() {
  return useSafeContext(TabsContext as unknown as Context<T | undefined>)
}
