import { useTabsContext, type TabsContextType, TabsContext } from '@/context/TabsContext'
import useSafeContext from '@/hooks/useSafeContext'
import { endpoints } from '@/visualizer/queries'
import { GraphNode } from '@/visualizer/types'
import { Tab } from '@/types/PageTabs'
import { createContext, useCallback } from 'react'
import { useVisualizerPageContext } from './VisualizerPageContext'

interface NodeTabData {
  node?: GraphNode
  pathway?: GraphNode[]
}

interface VisualizerTab extends Tab {
  data: NodeTabData
}

interface AddTabOptions {
  title?: string
  pathway?: GraphNode[]
}

type VisualizerTabsContext = Omit<TabsContextType<VisualizerTab>, 'addTab'> & {
  addTab: (nodeId?: string | string[], options?: AddTabOptions) => Promise<void>
}

export const VisualizerTabsContext = createContext<VisualizerTabsContext | undefined>(undefined)

export const VisualizerTabsContextProvider = ({ children }: { children: React.ReactNode }) => {
  const { addTab, setActiveTabIndex, tabs, ...rest } =
    useTabsContext<TabsContextType<VisualizerTab>>()

  const { setActiveNode } = useVisualizerPageContext()

  const handleAddTab = useCallback(
    async (nodeId?: string | string[], options?: AddTabOptions) => {
      if (!nodeId) {
        return addTab('New tab')
      }

      const { title, pathway } = options ?? {}
      const nodes =
        pathway ||
        (await endpoints['get_nodes']({
          nodeIds: Array.isArray(nodeId) ? nodeId : [nodeId],
          yield_edges: false,
        }))

      const node = pathway ? pathway?.find((n) => n.data.turing_id === nodeId) : nodes[0]
      if (!node) {
        throw new Error('Node not found')
      }

      const nodeDisplayName = title ?? node.data.properties.displayName ?? node.data.turing_id
      addTab(nodeDisplayName, { node, pathway })
    },
    [addTab]
  )

  const handleSetActiveTabIndex = useCallback(
    (index: number) => {
      setActiveNode(null)
      setActiveTabIndex(index)
      setActiveNode(tabs[index].data.node?.data ?? null)
    },
    [setActiveNode, setActiveTabIndex, tabs]
  )

  return (
    <VisualizerTabsContext.Provider
      value={{
        ...rest,
        tabs,
        addTab: handleAddTab,
        setActiveTabIndex: handleSetActiveTabIndex,
      }}
    >
      {children}
    </VisualizerTabsContext.Provider>
  )
}

export const useVisualizerTabsContext = () => useSafeContext(VisualizerTabsContext)
