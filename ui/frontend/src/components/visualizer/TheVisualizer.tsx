'use client'

import { useEffect, useState } from 'react'

import { Visualizer, Canvas } from '@/visualizer/components'
import { useVisualizerContext } from '@/visualizer'
import * as cyEvents from '@/visualizer/events'

import TheNodeSidebar from '@/components/visualizer/TheNodeSidebar'
import TheVisualizerTopActions from '@/components/visualizer/TheVisualizerTopActions'
import TheVisualizerBottomActions from '@/components/visualizer/TheVisualizerBottomActions'
import TheVisualizerContextMenu from '@/components/visualizer/TheVisualizerContextMenu'
import { useVisualizerPageContext } from '@/app/VisualizerPageContext'
import { GraphNode } from '@/visualizer/types'
import { getCyStyle } from './style'
import TheVisualizerDialogs from './dialogs'
import { useCanvasTrigger } from '@/visualizer/useCanvasTrigger'
import { useVisualizerTabsContext } from '@/app/VisualizerTabsContext'

interface TheVisualizerProps {
  initialNode?: GraphNode
  pathway?: GraphNode[]
}

export default function TheVisualizer(props: TheVisualizerProps) {
  const { initialNode } = props
  const vis = useVisualizerContext()
  const { activeNode, setActiveNode } = useVisualizerPageContext()
  const { updateActiveTab, clearActiveTab } = useVisualizerTabsContext()

  // handle tap on cytoscape canvas
  const setOneTap = () => {
    if (!vis.refs?.events.current) return
    vis.refs.events.current.onetap = (vis, e) => {
      if (e.target?.group && e.target.group() === 'nodes') {
        const data = e.target.data()
        if (data.turing_id) {
          data.turing_id = String(data.turing_id)
        }
        setActiveNode(data)
      } else {
        setActiveNode(null)
      }
      cyEvents.onetap(vis, e)
    }
  }
  setOneTap()

  const [isInitialized, setIsInitialized] = useState(false)

  // set initial selected node
  useEffect(() => {
    if (isInitialized || !initialNode) return

    const state = vis.state()
    const callbacks = vis.callbacks()
    if (!state || !callbacks) return
    const ids = state.selectedNodeIds
    if (ids.length) return

    const initialSelectedNodeId = initialNode.data.turing_id

    const defaultSelectedNodeIds = props.pathway
      ? props.pathway.map((n) => n.data.turing_id)
      : [initialSelectedNodeId]
    callbacks.setSelectedNodeIds(defaultSelectedNodeIds)

    vis.callbacks()?.setInspectedNode(initialNode.data)

    setActiveNode(initialNode.data)

    setIsInitialized(true)
  }, [vis, setActiveNode, initialNode, isInitialized, props.pathway])

  useCanvasTrigger({
    category: 'selectedNodeIds',
    name: 'TheVisualizer - handle initial node',
    callback() {
      const cy = vis?.cy()
      const state = vis?.state()
      if (!cy || !state) return

      const selectedNodeIds = state.selectedNodeIds || []
      if (!selectedNodeIds.length && initialNode) {
        // clear name and data
        clearActiveTab()
        return
      }

      if (initialNode) return

      // @ts-expect-error wrong cytoscape types
      cy.once('layoutstop', () => {
        const selected = cy.nodes()
        if (!selected.length) {
          throw new Error('Cannot get any selected nodes')
        }

        const node = selected[0]
        if (!node) {
          throw new Error('Error setting initial node')
        }

        const data = node.data()
        if (data.turing_id) {
          data.turing_id = String(data.turing_id)
        }

        const name = data.properties.displayName ?? data.turing_id ?? 'New tab'
        updateActiveTab({
          name,
          data: {
            node: {
              data,
              group: 'nodes',
            },
          },
        })
      })
    },
  })

  const style = getCyStyle(initialNode)

  // needed to set initial node styles when canvas trigger sets the node
  useEffect(() => {
    if (!initialNode) return

    const cy = vis.cy()
    if (!cy) return

    const initialNodeSelector = `node[id = "${initialNode.data.id}"]`
    // @ts-expect-error wrong cytoscape types
    const styleArr = cy.style().json() || []
    if (styleArr.some((v: any) => v.selector === initialNodeSelector)) {
      // styles already applied
      return
    }

    cy.style().resetToDefault().fromJson(getCyStyle(initialNode)).update()
  }, [initialNode, vis])

  return (
    <Visualizer canvas={<Canvas />} contextMenu={<TheVisualizerContextMenu />} cyStyle={style}>
      <TheVisualizerTopActions highlightAddEntity={!initialNode} />
      <TheVisualizerBottomActions initialNode={initialNode} />
      <TheNodeSidebar node={activeNode} visible={activeNode !== null} />
      <TheVisualizerDialogs />
    </Visualizer>
  )
}
