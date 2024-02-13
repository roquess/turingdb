import { GraphEdge, GraphNode, GraphNodeData } from '@/visualizer/types'
import { createContext } from 'react'
import { useState, Dispatch, SetStateAction } from 'react'
import useSafeContext from '@/hooks/useSafeContext'

export interface VisualizerPageContextType {
  activeNode: GraphNodeData | null
  setActiveNode: (node: GraphNodeData | null) => void
  activeNodeProperties?: GraphNodeData['properties'] | undefined
  activeNodeTuringId?: GraphNodeData['turing_id'] | undefined
  isInEdge: (edge: GraphEdge) => boolean
  isOutEdge: (edge: GraphEdge) => boolean
  getEdgeReferenceNodeId: (edge: EdgeWithNodes) => string
  getEdgeReferenceNode: (edge: EdgeWithNodes) => GraphNode | undefined
  pathwaysDialogNodeData: GraphNodeData | null
  setPathwaysDialogNodeData: Dispatch<SetStateAction<GraphNodeData | null>>
  isOnboardingActive: boolean
  setIsOnboardingActive: Dispatch<SetStateAction<boolean>>
  isLoading: boolean
  init: () => Promise<void>
  initialized: boolean
}

export interface EdgeWithNodes extends GraphEdge {
  source: GraphNode
  target: GraphNode
}

export const VisualizerPageContext = createContext<VisualizerPageContextType | undefined>(undefined)

export const VisualizerPageContextProvider = ({ children }: { children: React.ReactNode }) => {
  const [activeNode, setActiveNode] = useState<GraphNodeData | null>(null)
  const activeNodeProperties = activeNode?.properties
  const activeNodeTuringId = activeNode?.turing_id

  const isInEdge = (edge: GraphEdge) => edge.data.turing_target_id === activeNodeTuringId
  const isOutEdge = (edge: GraphEdge) => edge.data.turing_source_id === activeNodeTuringId
  const getEdgeReferenceNode = (edge: EdgeWithNodes) => (isInEdge(edge) ? edge.source : edge.target)
  const getEdgeReferenceNodeId = (edge: EdgeWithNodes) => getEdgeReferenceNode(edge)?.data.turing_id

  const [pathwaysDialogNodeData, setPathwaysDialogNodeData] = useState<GraphNodeData | null>(null)

  const [isOnboardingActive, setIsOnboardingActive] = useState(true)

  const [isLoading, setIsLoading] = useState(true)
  const [initialized, setInitialized] = useState(false)

  const init = async () => {
    setIsLoading(true)
    try {
      setInitialized(true)
    } finally {
      setIsLoading(false)
    }
  }

  return (
    <VisualizerPageContext.Provider
      value={{
        activeNode,
        setActiveNode,
        activeNodeProperties,
        activeNodeTuringId,
        isInEdge,
        isOutEdge,
        getEdgeReferenceNode,
        getEdgeReferenceNodeId,
        pathwaysDialogNodeData,
        setPathwaysDialogNodeData,
        isOnboardingActive,
        setIsOnboardingActive,
        isLoading,
        init,
        initialized,
      }}
    >
      {children}
    </VisualizerPageContext.Provider>
  )
}

export const useVisualizerPageContext = () => useSafeContext(VisualizerPageContext)
