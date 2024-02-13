import AppButton from '@/components/base/AppButton'
import AppButtonGroup from '@/components/base/AppButtonGroup'
import { useVisualizerPageContext } from '@/app/VisualizerPageContext'

import IconMaximizeBox from '@/assets/icons/maximize-box.svg'
import IconPlus from '@/assets/icons/plus.svg'
import IconMinus from '@/assets/icons/minus.svg'

import { useVisualizerContext } from '@/visualizer'
import { GraphNode } from '@/visualizer/types'
import { useState } from 'react'

export default function TheVisualizerBottomActions({ initialNode }: { initialNode?: GraphNode }) {
  const vis = useVisualizerContext()
  const { setActiveNode } = useVisualizerPageContext()

  const animationDuration = 600

  const [isAnimating, setIsAnimating] = useState(false)

  const animate = (...params: Parameters<cytoscape.CoreAnimation['animate']>) => {
    const cy = vis.cy()
    if (!cy || isAnimating) return
    setIsAnimating(true)
    const options = params[1] ? { ...params[1] } : {}
    const newOptions = { ...options }
    newOptions.complete = () => {
      options.complete && options.complete()
      setIsAnimating(false)
    }

    cy.animate(params[0], {
      duration: animationDuration,
      easing: 'ease-in-out-sine',
      ...newOptions,
    })
  }

  const handleFitToScreen = () => {
    const cy = vis.cy()
    if (!cy) return
    setIsAnimating(true)
    animate({
      fit: {
        eles: cy.nodes(),
        padding: 100,
      },
    })
  }

  const handleReturnToOrigin = () => {
    if (!initialNode) return

    const state = vis.state()
    const callbacks = vis.callbacks()
    if (!state || !callbacks) return
    const defaultSelectedNodeIds = [initialNode.data.turing_id]
    callbacks.setSelectedNodeIds(defaultSelectedNodeIds)
    vis.callbacks()?.setInspectedNode(initialNode.data)
    setActiveNode(initialNode.data)
  }

  const zoomFactor = 1

  const handleZoomIn = () => {
    const cy = vis.cy()
    if (!cy) return

    animate({
      zoom: cy.zoom() + zoomFactor,
    })
  }

  const handleZoomOut = () => {
    const cy = vis.cy()
    if (!cy) return

    animate({
      zoom: cy.zoom() - zoomFactor,
    })
  }

  return (
    <div className="absolute bottom-0 left-0 right-0 mx-4 mb-4 flex justify-between gap-x-4">
      <div className="pointer-events-auto flex gap-x-2">
        {initialNode && <AppButton text="Return to origin" onClick={handleReturnToOrigin} />}
      </div>
      <div className="pointer-events-auto flex gap-x-2">
        <AppButton text="Fit to screen" icon={<IconMaximizeBox />} onClick={handleFitToScreen} />
        <AppButtonGroup>
          <AppButton className="w-[2.0625rem]" icon={<IconPlus />} onClick={handleZoomIn} />
          <AppButton className="w-[2rem]" icon={<IconMinus />} onClick={handleZoomOut} />
        </AppButtonGroup>
      </div>
    </div>
  )
}
