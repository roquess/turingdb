import { ShowInPathwayDialog } from '@/visualizer/components/ShowInPathwayDialog'
import ThePathwaysDialog from './ThePathwaysDialog'
import TheSearchNodesDialog from './TheSearchNodesDialog'

export default function TheVisualizerDialogs() {
  return (
    <div className="visualizer-dialogs">
      <ThePathwaysDialog />
      <TheSearchNodesDialog />
      <ShowInPathwayDialog />
    </div>
  )
}
