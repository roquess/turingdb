'use client'

import './page.css'
import dynamic from 'next/dynamic'
import { QueryClient, QueryClientProvider } from '@tanstack/react-query'
import clsx from 'clsx'

import PageTabs from '@/components/tabs/PageTabs'
import { VisualizerContextProvider } from '@/visualizer/components'
import { getDefaultCanvasTheme } from '@/visualizer/cytoscape/tools'
import { TabsContextProvider } from '@/context/TabsContext'
import { VisualizerPageContextProvider, useVisualizerPageContext } from './VisualizerPageContext'
import { useEffect } from 'react'
import { VisualizerTabsContextProvider, useVisualizerTabsContext } from './VisualizerTabsContext'
import AppSpinner from '@/components/base/AppSpinner'

const TheVisualizer = dynamic(() => import('@/components/visualizer/TheVisualizer'), {
  ssr: false,
  loading: () => (
    <div className="flex flex-grow ">
      <AppSpinner className="m-auto" />
    </div>
  ),
})

const queryClient = new QueryClient()

function HomeContent() {
  const { initialized, isLoading, init } = useVisualizerPageContext()

  const tabsContext = useVisualizerTabsContext()
  const { tabs, addTab, activeTabIndex } = tabsContext

  const canvasTheme = getDefaultCanvasTheme()

  canvasTheme.dark.edges = {
    connecting: {
      line: '#7FEEB7',
      text: '#7FEEB7',
    },
    neighbor: {
      line: '#7FEEB7',
      text: '#7FEEB7',
    },
  }

  canvasTheme.dark.nodes = {
    selected: {
      icon: '#C9CFDE',
      text: '#C9CFDE',
    },
    neighbor: {
      icon: '#C9CFDE',
      text: '#C9CFDE',
    },
  }
  // load initial tab once
  useEffect(() => {
    if (tabs.length > 0 || !initialized) return
    addTab()
  }, [tabs, addTab, initialized])

  useEffect(() => {
    if (initialized) return
    init()
  }, [initialized, init])

  if (isLoading) {
    return (
      <div className="flex flex-grow flex-col">
        <AppSpinner className="m-auto" />
      </div>
    )
  }

  return (
    <div className="flex flex-grow flex-col">
      <PageTabs {...tabsContext} />
      <div className="page-content relative flex flex-grow flex-col bg-grey-900">
        {tabs.map((tab, index) => (
          <div
            key={tab.id}
            className={clsx([
              'flex flex-grow flex-col',
              {
                hidden: index !== activeTabIndex,
              },
            ])}
          >
            <VisualizerContextProvider
              themeMode="dark"
              dbName="reactome"
              containerId={tab.id}
              canvasTheme={canvasTheme}
            >
              <TheVisualizer initialNode={tab.data.node} pathway={tab.data.pathway} />
            </VisualizerContextProvider>
          </div>
        ))}
      </div>
    </div>
  )
}

export default function Home() {
  return (
    <QueryClientProvider client={queryClient}>
      <TabsContextProvider>
        <VisualizerPageContextProvider>
          <VisualizerTabsContextProvider>
            <HomeContent />
          </VisualizerTabsContextProvider>
        </VisualizerPageContextProvider>
      </TabsContextProvider>
    </QueryClientProvider>
  )
}
